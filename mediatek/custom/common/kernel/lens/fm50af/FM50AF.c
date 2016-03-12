/*
 * MD218A voice coil motor driver
 *
 *
 */

#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include "FM50AF.h"
#include "../camera/kd_camera_hw.h"

#define LENS_I2C_BUSNUM 1
static struct i2c_board_info __initdata kd_lens_dev={ I2C_BOARD_INFO("FM50AF", 0x18)};


#define FM50AF_DRVNAME "FM50AF"
#define FM50AF_VCM_WRITE_ID           0x30

//#define FM50AF_DEBUG   // josephhsu- 20121225 to avoid from printk() spending too much time which over 1/30 sec.
#ifdef FM50AF_DEBUG
#define FM50AFDB printk
#else
#define FM50AFDB(x,...)
#endif

static spinlock_t g_FM50AF_SpinLock;

static struct i2c_client * g_pstFM50AF_I2Cclient = NULL;

static dev_t g_FM50AF_devno;
static struct cdev * g_pFM50AF_CharDrv = NULL;
static struct class *actuator_class = NULL;

static int  g_s4FM50AF_Opened = 0;
static long g_i4MotorStatus = 0;
static long g_i4Dir = 0;
static unsigned long g_u4FM50AF_INF = 0;
static unsigned long g_u4FM50AF_MACRO = 1023;
static unsigned long g_u4TargetPosition = 0;
static unsigned long g_u4CurrPosition   = 0;
static long g_i4Position = 0;

static int g_sr = 15;

#if 0
extern s32 mt_set_gpio_mode(u32 u4Pin, u32 u4Mode);
extern s32 mt_set_gpio_out(u32 u4Pin, u32 u4PinOut);
extern s32 mt_set_gpio_dir(u32 u4Pin, u32 u4Dir);
#endif

static int s4FM50AF_ReadReg(unsigned short * a_pu2Result)
{
    int  i4RetValue = 0;
    char pSetBuff[2];
	char pGetBuff[2];

	pSetBuff[0] = 3;
	i4RetValue = i2c_master_send(g_pstFM50AF_I2Cclient, pSetBuff, 1);
	if (i4RetValue < 0) 
	{
		FM50AFDB("[DRV201] I2C send vcm current reg failed!! \n");
		return -1;
	}

    i4RetValue = i2c_master_recv(g_pstFM50AF_I2Cclient, &pGetBuff[0] , 1);

    if (i4RetValue < 0) 
    {
        FM50AFDB("[FM50AF] I2C read failed!! \n");
        return -1;
    }
	pSetBuff[0] = 4;
	i4RetValue = i2c_master_send(g_pstFM50AF_I2Cclient, pSetBuff, 1);
	if (i4RetValue < 0) 
	{
		FM50AFDB("[DRV201] I2C send vcm current reg failed!! \n");
		return -1;
	}

    i4RetValue = i2c_master_recv(g_pstFM50AF_I2Cclient, &pGetBuff[1] , 1);

    if (i4RetValue < 0) 
    {
        FM50AFDB("[FM50AF] I2C read failed!! \n");
        return -1;
    }

    *a_pu2Result = ((((u16)pGetBuff[0]) << 8) + (pGetBuff[1]))&0x3ff;

    return 0;
}

static int s4FM50AF_WriteReg(u16 a_u2Data)
{
    int  i4RetValue = 0;
    //char puSendCmd[2];

	a_u2Data = a_u2Data&0x3FF;
	
	char puSendCmd1[2]= {(char)(0x03) , (char)(a_u2Data>>8)};
    //FM50AFDB("[FM50AF] g_sr %d, write %d \n", g_sr, a_u2Data);
    i4RetValue = i2c_master_send(g_pstFM50AF_I2Cclient, puSendCmd1, 2);
	
    if (i4RetValue < 0) 
    {
        FM50AFDB("[FM50AF] I2C send failed!! \n");
        return -1;
    }
	
   char puSendCmd2[2] = {(char)(0x04) , (char)(a_u2Data&0xff)};
		
    //FM50AFDB("[FM50AF] g_sr %d, write %d \n", g_sr, a_u2Data);
    i4RetValue = i2c_master_send(g_pstFM50AF_I2Cclient, puSendCmd2, 2);
	
    if (i4RetValue < 0) 
    {
        FM50AFDB("[FM50AF] I2C send failed!! \n");
        return -1;
    }

    return 0;
}

inline static int getFM50AFInfo(__user stFM50AF_MotorInfo * pstMotorInfo)
{
    stFM50AF_MotorInfo stMotorInfo;
    stMotorInfo.u4MacroPosition   = g_u4FM50AF_MACRO;
    stMotorInfo.u4InfPosition     = g_u4FM50AF_INF;
    stMotorInfo.u4CurrentPosition = g_u4CurrPosition;
    stMotorInfo.bIsSupportSR      = TRUE;

	if (g_i4MotorStatus == 1)	{stMotorInfo.bIsMotorMoving = 1;}
	else						{stMotorInfo.bIsMotorMoving = 0;}

	if (g_s4FM50AF_Opened >= 1)	{stMotorInfo.bIsMotorOpen = 1;}
	else						{stMotorInfo.bIsMotorOpen = 0;}

    if(copy_to_user(pstMotorInfo , &stMotorInfo , sizeof(stFM50AF_MotorInfo)))
    {
        FM50AFDB("[FM50AF] copy to user failed when getting motor information \n");
    }

    return 0;
}

//Main jobs:
// 1. Break down move step for eliminating noise
// Protect Macro to Inf. only
// MTK
static int breakDownSteps(int max_step, int interval, int CurrentPos, int TargetPos)
{
    unsigned short tmpTargetPos;

    FM50AFDB("[FM50AF]MTK: break down steps from=%d, to=%d \n", CurrentPos, TargetPos);

    if(TargetPos >= CurrentPos){
        return 0;
    }
	    if( CurrentPos>350)
   {
         s4FM50AF_WriteReg(350);
		 CurrentPos= 350;
    }
		
    while( CurrentPos - TargetPos > max_step ){
        if(TargetPos >= CurrentPos){
            FM50AFDB("[FM50AF]MTK: Inverse!!\n");
            return 0;
        }
    	tmpTargetPos = CurrentPos - max_step;
        FM50AFDB("[FM50AF] :MTK:tmpTargetPos=%d\n", tmpTargetPos);
        if(s4FM50AF_WriteReg(tmpTargetPos) == 0)
        {
            CurrentPos = tmpTargetPos;
            FM50AFDB("[FM50AF] :MTK:CurrentPos=%d\n", CurrentPos);
        }
        else
        {
            FM50AFDB("[FM50AF] set I2C failed when moving the motor \n");
            g_i4MotorStatus = -1;
        }
        //Delay for few milli-second
        FM50AFDB("[FM50AF]MTK: start delay \n");
        msleep(interval);
        FM50AFDB("[FM50AF]MTK: after delay \n");
    }

    FM50AFDB("[FM50AF]MTK: Last step from=%d, to=%d \n", CurrentPos, TargetPos);
    if(s4FM50AF_WriteReg(TargetPos) == 0)
    {
     	  CurrentPos = TargetPos;
    }
    else
    {
     	  FM50AFDB("[FM50AF] set I2C failed when moving the motor \n");
     	  g_i4MotorStatus = -1;
    }
    g_u4CurrPosition = CurrentPos;
    FM50AFDB("[FM50AF]MTK: Leave breakdown\n");
    return 0;
}

//MTK
#define BREAK_DOWN

inline static int moveFM50AF(unsigned long a_u4Position)
{
    int ret = 0;
    
    if((a_u4Position > g_u4FM50AF_MACRO) || (a_u4Position < g_u4FM50AF_INF))
    {
        FM50AFDB("[FM50AF] out of range \n");
        return -EINVAL;
    }

    if (g_s4FM50AF_Opened == 1)
    {
        unsigned short InitPos;
		ret = s4FM50AF_ReadReg(&InitPos); // MTK Reference Design
	    
        spin_lock(&g_FM50AF_SpinLock);
        if(ret == 0)
        {
            FM50AFDB("[FM50AF] Init Pos %6d \n", InitPos);
            g_u4CurrPosition = (unsigned long)InitPos;
        }
        else
        {		
            g_u4CurrPosition = 0;
        }
        g_s4FM50AF_Opened = 2;
        spin_unlock(&g_FM50AF_SpinLock);
    }

    if (g_u4CurrPosition < a_u4Position)
    {
        spin_lock(&g_FM50AF_SpinLock);	
        g_i4Dir = 1;
        spin_unlock(&g_FM50AF_SpinLock);	
    }
    else if (g_u4CurrPosition > a_u4Position)
    {
        spin_lock(&g_FM50AF_SpinLock);	
        g_i4Dir = -1;
        spin_unlock(&g_FM50AF_SpinLock);			
    }
    else									

	{return 0;}
	if (1)
		{
		g_i4Position = (long)g_u4CurrPosition;
			g_u4TargetPosition = a_u4Position;
	
			if (g_i4Dir == 1)
			{
				//if ((g_u4TargetPosition - g_u4CurrPosition)<60)
				{		
					g_i4MotorStatus = 0;
					if(s4FM50AF_WriteReg((unsigned short)g_u4TargetPosition) == 0)
					{
						g_u4CurrPosition = (unsigned long)g_u4TargetPosition;
					}
					else
					{
						FM50AFDB("[FM50AF] set I2C failed when moving the motor \n");
						g_i4MotorStatus = -1;
					}
				}
				//else
				//{
				//	g_i4MotorStatus = 1;
				//}
			}
			else if (g_i4Dir == -1)
			{
				//if ((g_u4CurrPosition - g_u4TargetPosition)<60)
				{
					g_i4MotorStatus = 0;		
					
#ifdef BREAK_DOWN

	
		if((g_u4CurrPosition!=g_u4TargetPosition) && (g_u4TargetPosition==180))
			{
				FM50AFDB("[FM50AF]MTK: in small stepLeave breakdown \n");
				breakDownSteps(50, 10, g_u4CurrPosition, g_u4TargetPosition);
			} 
		else 
			{
			   if(s4FM50AF_WriteReg((unsigned short)g_u4TargetPosition) == 0)
				{
					spin_lock(&g_FM50AF_SpinLock);		
					g_u4CurrPosition = (unsigned long)g_u4TargetPosition;
					spin_unlock(&g_FM50AF_SpinLock);				
				}
			 else
				{
					FM50AFDB("[FM50AF] set I2C failed when moving the motor \n"); 		
					spin_lock(&g_FM50AF_SpinLock);
					g_i4MotorStatus = -1;
					spin_unlock(&g_FM50AF_SpinLock);				
				}
			}
#else				
					if(s4FM50AF_WriteReg((unsigned short)g_u4TargetPosition) == 0)
					{
					    spin_lock(&g_FM50AF_SpinLock);		
                g_u4CurrPosition = (unsigned long)g_u4TargetPosition;
                spin_unlock(&g_FM50AF_SpinLock);	
					}
					else
					{
						FM50AFDB("[FM50AF] set I2C failed when moving the motor \n");
						   		
                spin_lock(&g_FM50AF_SpinLock);
                g_i4MotorStatus = -1;
                spin_unlock(&g_FM50AF_SpinLock);				
					}
#endif
				}
				//else
				//{
				//	g_i4MotorStatus = 1;		
				//}
			}
		}
		else
	{
	g_i4Position = (long)g_u4CurrPosition;

 spin_lock(&g_FM50AF_SpinLock);    
    g_u4TargetPosition = a_u4Position;
    spin_unlock(&g_FM50AF_SpinLock);	
	g_i4MotorStatus = 1;

    //FM50AFDB("[FM50AF] move [curr] %d [target] %d\n", g_u4CurrPosition, g_u4TargetPosition);
			}
            

    return 0;
}

inline static int setFM50AFInf(unsigned long a_u4Position)
{
    spin_lock(&g_FM50AF_SpinLock);
    g_u4FM50AF_INF = a_u4Position;
    spin_unlock(&g_FM50AF_SpinLock);	
    return 0;
}

inline static int setFM50AFMacro(unsigned long a_u4Position)
{
    spin_lock(&g_FM50AF_SpinLock);
    g_u4FM50AF_MACRO = a_u4Position;
    spin_unlock(&g_FM50AF_SpinLock);	
    return 0;	
}

////////////////////////////////////////////////////////////////
static long FM50AF_Ioctl(
struct file * a_pstFile,
unsigned int a_u4Command,
unsigned long a_u4Param)
{
    long i4RetValue = 0;

    switch(a_u4Command)
    {
        case FM50AFIOC_G_MOTORINFO :
            i4RetValue = getFM50AFInfo((__user stFM50AF_MotorInfo *)(a_u4Param));
        break;

        case FM50AFIOC_T_MOVETO :
            i4RetValue = moveFM50AF(a_u4Param);
        break;
 
        case FM50AFIOC_T_SETINFPOS :
            i4RetValue = setFM50AFInf(a_u4Param);
        break;

        case FM50AFIOC_T_SETMACROPOS :
            i4RetValue = setFM50AFMacro(a_u4Param);
        break;
		
        default :
      	    FM50AFDB("[FM50AF] No CMD \n");
            i4RetValue = -EPERM;
        break;
    }

    return i4RetValue;
}

//Main jobs:
// 1.check for device-specified errors, device not ready.
// 2.Initialize the device if it is opened for the first time.
// 3.Update f_op pointer.
// 4.Fill data structures into private_data
//CAM_RESET
static int FM50AF_Open(struct inode * a_pstInode, struct file * a_pstFile)
{
    //char puSendCmd[2];
	
    spin_lock(&g_FM50AF_SpinLock);

    if(g_s4FM50AF_Opened)
    {
        spin_unlock(&g_FM50AF_SpinLock);
        FM50AFDB("[FM50AF] the device is opened \n");
        return -EBUSY;
    }

    g_s4FM50AF_Opened = 1;
		
    spin_unlock(&g_FM50AF_SpinLock);
	
	//<2012/11/01-kylechang, Init DW9719 SAC mode/SAC operation time by MTK Meimei
	//char puSendCmd1[2]= {(char)(0x06) ,(char)(0x80)};   // SRC mode 4
	char puSendCmd1[2]= {(char)(0x06) ,(char)(0xc0)};   // SRC mode 5
    i2c_master_send(g_pstFM50AF_I2Cclient, puSendCmd1, 2);
    char puSendCmd2[2] = {(char)(0x07) ,(char)(0x60)};
    i2c_master_send(g_pstFM50AF_I2Cclient, puSendCmd2, 2);
    //>2012/11/01-kylechang
	char puSendCmd3[2] = {(char)(0x02) ,(char)(0x02)};   // 20121115 josephhsu: select Slew Rate Control mode.
	i2c_master_send(g_pstFM50AF_I2Cclient, puSendCmd3, 2);
	
    return 0;
}


//Main jobs:
// 1.Deallocate anything that "open" allocated in private_data.
// 2.Shut down the device on last close.
// 3.Only called once on last time.
// Q1 : Try release multiple times.
static int FM50AF_Release(struct inode * a_pstInode, struct file * a_pstFile)
{
    if (g_s4FM50AF_Opened)
    {
        FM50AFDB("[FM50AF] feee \n");
  		breakDownSteps(100, 10, g_u4CurrPosition, 50);  	    
        spin_lock(&g_FM50AF_SpinLock);
        g_s4FM50AF_Opened = 0;
        spin_unlock(&g_FM50AF_SpinLock);

    }

    return 0;
}

static const struct file_operations g_stFM50AF_fops = 
{
    .owner = THIS_MODULE,
    .open = FM50AF_Open,
    .release = FM50AF_Release,
    .unlocked_ioctl = FM50AF_Ioctl
};

inline static int Register_FM50AF_CharDrv(void)
{
    struct device* vcm_device = NULL;

    //Allocate char driver no.
    if( alloc_chrdev_region(&g_FM50AF_devno, 0, 1,FM50AF_DRVNAME) )
    {
        FM50AFDB("[FM50AF] Allocate device no failed\n");

        return -EAGAIN;
    }

    //Allocate driver
    g_pFM50AF_CharDrv = cdev_alloc();

    if(NULL == g_pFM50AF_CharDrv)
    {
        unregister_chrdev_region(g_FM50AF_devno, 1);

        FM50AFDB("[FM50AF] Allocate mem for kobject failed\n");

        return -ENOMEM;
    }

    //Attatch file operation.
    cdev_init(g_pFM50AF_CharDrv, &g_stFM50AF_fops);

    g_pFM50AF_CharDrv->owner = THIS_MODULE;

    //Add to system
    if(cdev_add(g_pFM50AF_CharDrv, g_FM50AF_devno, 1))
    {
        FM50AFDB("[FM50AF] Attatch file operation failed\n");

        unregister_chrdev_region(g_FM50AF_devno, 1);

        return -EAGAIN;
    }

    actuator_class = class_create(THIS_MODULE, "actuatordrv");
    if (IS_ERR(actuator_class)) {
        int ret = PTR_ERR(actuator_class);
        FM50AFDB("Unable to create class, err = %d\n", ret);
        return ret;            
    }

    vcm_device = device_create(actuator_class, NULL, g_FM50AF_devno, NULL, FM50AF_DRVNAME);

    if(NULL == vcm_device)
    {
        return -EIO;
    }
    
    return 0;
}

inline static void Unregister_FM50AF_CharDrv(void)
{
    //Release char driver
    cdev_del(g_pFM50AF_CharDrv);

    unregister_chrdev_region(g_FM50AF_devno, 1);
    
    device_destroy(actuator_class, g_FM50AF_devno);

    class_destroy(actuator_class);
}

//////////////////////////////////////////////////////////////////////

static int FM50AF_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int FM50AF_i2c_remove(struct i2c_client *client);
static const struct i2c_device_id FM50AF_i2c_id[] = {{FM50AF_DRVNAME,0},{}};   
struct i2c_driver FM50AF_i2c_driver = {                       
    .probe = FM50AF_i2c_probe,                                   
    .remove = FM50AF_i2c_remove,                           
    .driver.name = FM50AF_DRVNAME,                 
    .id_table = FM50AF_i2c_id,                             
};  

#if 0 
static int FM50AF_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info) {         
    strcpy(info->type, FM50AF_DRVNAME);                                                         
    return 0;                                                                                       
}      
#endif 
static int FM50AF_i2c_remove(struct i2c_client *client) {
    return 0;
}

/* Kirby: add new-style driver {*/
static int FM50AF_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int i4RetValue = 0;

    FM50AFDB("[FM50AF] Attach I2C \n");

    /* Kirby: add new-style driver { */
    g_pstFM50AF_I2Cclient = client;
    
    g_pstFM50AF_I2Cclient->addr = g_pstFM50AF_I2Cclient->addr >> 1;
    
    //Register char driver
    i4RetValue = Register_FM50AF_CharDrv();

    if(i4RetValue){

        FM50AFDB("[FM50AF] register char device failed!\n");

        return i4RetValue;
    }

    spin_lock_init(&g_FM50AF_SpinLock);

    FM50AFDB("[FM50AF] Attached!! \n");

    return 0;
}

static int FM50AF_probe(struct platform_device *pdev)
{
    return i2c_add_driver(&FM50AF_i2c_driver);
}

static int FM50AF_remove(struct platform_device *pdev)
{
    i2c_del_driver(&FM50AF_i2c_driver);
    return 0;
}

static int FM50AF_suspend(struct platform_device *pdev, pm_message_t mesg)
{
    return 0;
}

static int FM50AF_resume(struct platform_device *pdev)
{
    return 0;
}

// platform structure
static struct platform_driver g_stFM50AF_Driver = {
    .probe		= FM50AF_probe,
    .remove	= FM50AF_remove,
    .suspend	= FM50AF_suspend,
    .resume	= FM50AF_resume,
    .driver		= {
        .name	= "lens_actuator",
        .owner	= THIS_MODULE,
    }
};

static int __init FM50AF_i2C_init(void)
{
    i2c_register_board_info(LENS_I2C_BUSNUM, &kd_lens_dev, 1);
	
    if(platform_driver_register(&g_stFM50AF_Driver)){
        FM50AFDB("failed to register FM50AF driver\n");
        return -ENODEV;
    }

    return 0;
}

static void __exit FM50AF_i2C_exit(void)
{
	platform_driver_unregister(&g_stFM50AF_Driver);
}

module_init(FM50AF_i2C_init);
module_exit(FM50AF_i2C_exit);

MODULE_DESCRIPTION("FM50AF lens module driver");
MODULE_AUTHOR("KY Chen <ky.chen@Mediatek.com>");
MODULE_LICENSE("GPL");


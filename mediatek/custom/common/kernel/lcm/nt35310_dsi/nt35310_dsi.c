/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
#ifndef _NT35310_DSI_C_
#define _NT35310_DSI_C_
/*****************************************************************************
** LCM model
**  AUO H347QVN01.0, 3.47" 320RGBx480 TFT-LCD
**  NovaTek NT35310 driver IC.
**  Fosc : 13MHz or 20MHz
**  Bit rate: 80~500Mbps.
**  Frame rate: 60Hz
******************************************************************************/
#if defined( BUILD_LK )
    #include  <platform/mt_typedefs.h>
    #include  <platform/mt_gpio.h>
  //#include  <platform/mt_pmic.h>
#else
  #if defined( BUILD_UBOOT )
  //#include  <asm/arch/mt65xx.h>
    #include  <asm/arch/mt65xx_typedefs.h>
    #include  <asm/arch/mt6577_gpio.h>
  //#include  <asm/arch/mt6577_pmic6329.h>
  #else
    #include  <linux/kernel.h>
    #include  <mach/mt_typedefs.h>
    #include  <mach/mt_gpio.h>
  //#include  <mach/mt_pm_ldo.h>
  #endif
    #include  <linux/string.h>
#endif /** End.. (BUILD_LK) **/
    #include  "lcm_drv.h"

/*****************************************************************************
** Local Macro-defined Variables
******************************************************************************/
  /*============================================================
  **  For information and debug 
  **============================================================*/
  #if defined( LCM_MSG_ENABLE )
    #undef    LCM_MSG_ENABLE
  #endif
  //#define   LCM_MSG_ENABLE

  #if defined( LCM_DEBUG_ENABLE )
    #undef    LCM_DEBUG_ENABLE
  #endif
  //#define   LCM_DEBUG_ENABLE

#if defined( BUILD_LK )
  //#define   LCM_MSG(srt,arg...)     printf(str,##arg)
  //#define   LCM_MSG(srt,arg...)
    #define   LCM_MSG                 printf
#else
  #if defined( BUILD_UBOOT )
  //#define   LCM_MSG(srt,arg...)     printf(str,##arg)
  //#define   LCM_MSG(srt,arg...)
    #define   LCM_MSG                 printf
  #else
  //#define   LCM_MSG(srt,arg...)     printk(str,##arg)
  //#define   LCM_MSG(srt,arg...)
    #define   LCM_MSG                 printk
  #endif
#endif /** End.. (BUILD_LK) **/

  /*============================================================
  **
  **============================================================*/
  //#define   LCM_PANEL_HW_P0
  //#define   LCM_PANEL_HW_P1
    #define   LCM_PANEL_HW_P2

  //#define   LCM_GAMMA_SAMPLE_1
  //#define   LCM_GAMMA_SAMPLE_2
  //#define   LCM_GAMMA_SAMPLE_3
    #define   LCM_GAMMA_SAMPLE_4

	#define CE_ON	//EricHsieh,2013/2/6,CE enable setting.

  #if defined( LCM_DSI_CMD_MODE )
    #undef    LCM_DSI_CMD_MODE
  #endif
    #define   LCM_DSI_CMD_MODE

    #define   FRAME_WIDTH               (320)
    #define   FRAME_HEIGHT              (480)

    #define   REGFLAG_DELAY             0xFFFE
    #define   REGFLAG_END_OF_TABLE      0xFFFF  /* END OF REGISTERS MARKER */

    #define   FACTORY_INIT_CODE   /* Factory initialization code */

  #if defined( LCM_PIXEL_24B_888 )
    #undef    LCM_PIXEL_24B_888
  #endif
  #if defined( LCM_PIXEL_RGB )
    #undef    LCM_PIXEL_RGB
  #endif

//<2012/11/14-Yuting Shih-17197. Masked for color.
  //#define   LCM_PIXEL_24B_888
    #define   LCM_PIXEL_RGB
//>2012/11/14-Yuting Shih-17197.

//<2012/12/07-Yuting Shih, Add for HW P2.
  #if defined( LCM_PANEL_HW_P2 )
    #define   LCM_MEM_MY                0   /* 1: Decrease in vertical, 0: Increase in vertical */
    #define   LCM_MEM_MX                0   /* 1: Decrease in horizon, 0: Increase in horizon */
    #define   LCM_MEM_MV                0   //(0x1<<5)  /* */
    #define   LCM_MEM_ML                0
  #else
//>2012/12/07-Yuting Shih.
    #define   LCM_MEM_MY                (0x1<<7)  /* 1: Decrease in vertical, 0: Increase in vertical */
    #define   LCM_MEM_MX                (0x1<<6)  /* 1: Decrease in horizon, 0: Increase in horizon */
    #define   LCM_MEM_MV                0   //(0x1<<5)  /* */
//<2012/11/28-Yuting Shih-17923-[BU2SC00139391]. Sync with MY for memory scan.
    #define   LCM_MEM_ML                (0x1<<4)
//>2012/11/28-Yuting Shih-17923-[BU2SC00139391].
  #endif
  #if defined( LCM_PIXEL_RGB )
    #define   LCM_MEM_RGB               0         /* RGB:RGB color filter panel */
  #else
    #define   LCM_MEM_RGB               (0x1<<3)  /* RGB:BGR color filter panel */
  #endif
    #define   LCM_MEM_MH                0   //(0x1<<2)

    #define   LCM_MEM_OPERATE           (LCM_MEM_MY|LCM_MEM_MX|LCM_MEM_MV|LCM_MEM_ML|LCM_MEM_MH)
    #define   LCM_MEMORY_ACCESS         (LCM_MEM_OPERATE|LCM_MEM_RGB)

  //<2012/11/14-Yuting Shih-[BU2SC00139391].Enable tearing function.
  #if defined( LCM_TE_VSYNC_MODE )
    #undef    LCM_TE_VSYNC_MODE
  #endif
    #define   LCM_TE_VSYNC_MODE
  //>2012/11/14-Yuting shih-[BU2SC00139391].

  /*============================================================
  ** Note: MIPI Video mode please keep HBP at 2us, HFP at 600UI.
  **------------------------------------------------------------
  ** VBP>BP,
  **============================================================*/
    #define   LCM_BPAC_NUM              (28)  // 0x1C
    #define   LCM_BPB_NUM               (28)  // 0x1C
    #define   LCM_FP_NUM                (4)   // 0x04
    #define   LCM_BP_ACB                (((LCM_BPAC_NUM>>2)&0x40)|((LCM_BPB_NUM>>4)&0x10))

    #define   LCM_HSYNC_NUM             (4)   /** Shall be larger than 3 **/
    #define   LCM_HBP_NUM               (40)  // 0x28
    #define   LCM_HFP_NUM               (50)  // 0x32
    #define   LCM_VSYNC_NUM             (4)   /** Shall be larger than 3 **/
    #define   LCM_VBP_NUM               (12)  // 0x0C
    #define   LCM_VFP_NUM               (4)   // 0x04
    #define   LCM_LINE_BYTE             ((FRAME_WIDTH+LCM_HSYNC_NUM+LCM_HBP_NUM+LCM_HFP_NUM)*3)

  /**********************************************
  ** 0x0077
  ** 0x00B3
  ** 0x00EF: 240Mbps
  ** 0x012B: 300Mbps
  ** 0x0167: 360Mbps
  ** 0x01A3: 403Mbps
  **********************************************/
    #define   LCM_TE_SCAN_LINE          0
    #define   LCM_TE_SCAN_LINE_MSB      (((LCM_TE_SCAN_LINE)>>8)&0x01)
    #define   LCM_TE_SCAN_LINE_LSB      ((LCM_TE_SCAN_LINE)&0xFF)
   
/*======================================================================
** ref freq = 13 or 15MHz, B0h setting 0x80, so 80.6% * freq is pwm_clk;
** pwm_clk / 255 / 2(lcm_setpwm() 6th params) = pwm_duration = 23706
**======================================================================*/
    #define   LCM_PWM_CLK               23706

/*======================================================================
** READID4 (D4h): Read ID4
**----------------------------------------------------------------------
** 1st parameter: Vender ID code. "01" means Novatek.
** 2nd and 3rd parameter: Chip ID code. "5310" means NT35310.
** 4th parameter: ID43-ID40: Chip version code.
**======================================================================*/
    #define   LCM_ID0                   (0x01)
    #define   LCM_ID1                   (0x53)
    #define   LCM_ID2                   (0x10)
    #define   LCM_ID3                   (0x01)

  typedef struct LCM_setting_table
  {
    unsigned        cmd;
    unsigned char   count;
    unsigned char   para_list[64];
  } LCM_SET_TAB;

#if 0 //!defined( GPIO_LCM_ID_PIN )
  #if defined( HAWK35_EP0 )
    #define   GPIO_LCM_ID_PIN           GPIO47
  #else
    #define   GPIO_LCM_ID_PIN           GPIO101
  #endif
#endif

    #define   LCM_SECOND_SOURCE         0   /** 0: For second source panel **/
    #define   LCM_DSIN                  (0x00)  /* DSIN=1, Power of SRAM is ON when sleep in */
  #if defined( LCM_PANEL_HW_P2 ) || defined( LCM_PANEL_HW_P0 )
    #define   LCM_SRGB_ORDER            (0x1<<4)  /* SRGB=1:RGB order is B-G-R */
  #else /* HW panel P1 */
    #define   LCM_SRGB_ORDER            (0x00)    /* SRGB=0:RGB order is R-G-B */
  #endif
/*****************************************************************************
** Local Variables
******************************************************************************/
static LCM_UTIL_FUNCS   lcm_util = { 0 };

/*****************************************************************************
** Local Functions
******************************************************************************/
    #define   SET_RESET_PIN(v)        (lcm_util.set_reset_pin((v)))
    #define   UDELAY(n)               (lcm_util.udelay(n))
    #define   MDELAY(n)               (lcm_util.mdelay(n))

    #define   dsi_set_cmdq(pdata, queue_size, force_update) \
                                      lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
    #define   dsi_set_cmdq_V2(cmd, count, ppara, force_update)  \
                                      lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
    #define   wrtie_cmd(cmd)          lcm_util.dsi_write_cmd(cmd)
    #define   write_regs(addr, pdata, byte_nums)  \
                                      lcm_util.dsi_write_regs(addr, pdata, byte_nums)
    #define   read_reg                lcm_util.dsi_read_reg()
    #define   read_reg_v2(cmd, buffer, buffer_size) \
                                      lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

    #define   SET_GPIO_MODE(v,m)      lcm_util.set_gpio_mode(v,m)
    #define   SET_GPIO_DIR_OUT(v)     lcm_util.set_gpio_dir(v,GPIO_DIR_OUT)
    #define   SET_GPIO_DIR_IN(v)      lcm_util.set_gpio_dir(v,GPIO_DIR_IN)
    #define   SET_GPIO_PULL_ENABLE(v) lcm_util.set_gpio_pull_enable(v,GPIO_PULL_ENABLE)

/*****************************************************************************
** External Functions
******************************************************************************/
//<2012/10/17-Yuting Shih, Midified reading LCM id is unstable.
#if 0 //defined( BUILD_LK ) || defined( BUILD_UBOOT )
    extern void   DSI_clk_HS_mode(BOOL enter);
#endif
//>2012/10/17-Yuting Shih.

/*****************************************************************************
**  Note :
******************************************************************************
** Data ID will depends on the following rule.
**----------------------------------------------------------------------------
** count of parameters > 1  => Data ID = 0x39
** count of parameters = 1  => Data ID = 0x15
** count of parameters = 0  => Data ID = 0x05
**============================================================================
** Structure Format :
**----------------------------------------------------------------------------
** {DCS command, count of parameters, {parameter list}}
** {REGFLAG_DELAY, milliseconds of time, {}},
** ...
**
**============================================================================
** Setting ending by predefined flag
**----------------------------------------------------------------------------
** {REGFLAG_END_OF_TABLE, 0x00, {}}
******************************************************************************/
static LCM_SET_TAB lcm_initial_set[] =
{
//<2012/12/07-Yuting Shih, Add for HW P2.
#if defined( LCM_PANEL_HW_P2 )
  /* Display OFF */
    { 0x28, 0, { 0x00 }},
    { REGFLAG_DELAY, 5, {}},
  /* Sleep IN */
    { 0x10, 0, { 0x00 }},
    { REGFLAG_DELAY, 120, {}},

  /* SET_PIXEL_FORMAT: Set the Interface Pixel Format */
  #if defined( LCM_PIXEL_24B_888 )
    { 0x3A,  1, { 0x67 }},  /* 24-bits/pixel(MIPI only) */
  #else
    { 0x3A,  1, { 0x66 }},  /* 18-bits/pixel */
  #endif
  /* SET_ADDRESS_MODE: Memory Data Access Control */
    { 0x36,  1, { LCM_MEMORY_ACCESS }},

  /*******************************************
  ** CMD2 Page0
  ********************************************/
  /* PAGE CTRL: Unlock CMD2 */
    { 0xED,  2, { 0x01, 0xFE }},  /* Enter CMD2 Page 0 */
  /*  MTPPWR: MTP Write function enable */
    { 0xDF,  1, { 0x10 }},  /* nROM: Do not reloae MTP after SLPOUT. MTP_W: MTP write function disable. */
  /* PWR_CTRL3: VGH charge pump circuit selection. */
    { 0xC2,  3, { 0x24, 0x24, 0x24 }},
  /* PWR_CTRL5: VCOM. */
    { 0xC4,  1, { 0x53 }},
  /* PWR_CTRL7: VGL clamp voltage selection. */
    { 0xC6,  4, { 0x00, 0xE4, 0xE4, 0xE4 }},
  /*  */
    { 0xDA,  1, { 0x00 }},
  /*  */
    { 0xDB,  1, { 0x80 }},
  /*  */
    { 0xDC,  1, { 0x00 }},
  /* INVCTRL: Inversion Control */
    { 0xB4,  1, { 0x15 }},
  /*******************************************
  ** DCS mode, HS mode
  ********************************************/
  /*  */
    { 0xB7,  2, { 0x50, 0x02 }},
  /*  */
    { 0xBD,  2, { 0x00, 0x00 }},
  /*  */
    { 0xBC,  2, { 0x01, 0x00 }},

  /* DISPLAY_CTRL2: Set the States for LED Control */
    { 0xB7,  1, { (LCM_SRGB_ORDER|0x20) }}, /* REV:NB, SRGB:RGB/BGR, CTS:FULL Command Set */

  /*******************************************
  ** CMD2 Page1
  ********************************************/
  /* PAGE LOCK, Set the Register to command2 Page 1  */
    { 0xBF,  1, { 0xAA }},
#if defined( LCM_GAMMA_SAMPLE_1 )
  /* 3GAMMAR_CTRL_RED_P: Red Positive Gamma */
    { 0xE0, 36, { 0x03, 0x00, 0x10, 0x00, 0x1C, 0x00, 0x27, 0x00, 0x30, 0x00, 0x38, 0x00, 0x46, 0x00, 0x58, 0x00, 0x66, 0x00,
                  0x74, 0x00, 0x7E, 0x00, 0x86, 0x00, 0x8F, 0x00, 0x91, 0x00, 0x9F, 0x00, 0xA7, 0x00, 0xBB, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_RED_N: Red Negative Gamma */
    { 0xE1, 36, { 0x03, 0x00, 0x10, 0x00, 0x1C, 0x00, 0x27, 0x00, 0x30, 0x00, 0x38, 0x00, 0x46, 0x00, 0x58, 0x00, 0x66, 0x00,
                  0x74, 0x00, 0x7E, 0x00, 0x86, 0x00, 0x8F, 0x00, 0x91, 0x00, 0x9F, 0x00, 0xA7, 0x00, 0xBB, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_GREEN_P: Green Positive Gamma */
    { 0xE2, 36, { 0x03, 0x00, 0x10, 0x00, 0x1C, 0x00, 0x27, 0x00, 0x30, 0x00, 0x38, 0x00, 0x46, 0x00, 0x58, 0x00, 0x66, 0x00,
                  0x74, 0x00, 0x7E, 0x00, 0x86, 0x00, 0x8F, 0x00, 0x91, 0x00, 0x9F, 0x00, 0xA7, 0x00, 0xBB, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_GREEN_N: Green Negative Gamma */
    { 0xE3, 36, { 0x03, 0x00, 0x10, 0x00, 0x1C, 0x00, 0x27, 0x00, 0x30, 0x00, 0x38, 0x00, 0x46, 0x00, 0x58, 0x00, 0x66, 0x00,
                  0x74, 0x00, 0x7E, 0x00, 0x86, 0x00, 0x8F, 0x00, 0x91, 0x00, 0x9F, 0x00, 0xA7, 0x00, 0xBB, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_BLUE_P: Blue Positive Gamma */
    { 0xE4, 36, { 0x03, 0x00, 0x10, 0x00, 0x1C, 0x00, 0x27, 0x00, 0x30, 0x00, 0x38, 0x00, 0x46, 0x00, 0x58, 0x00, 0x66, 0x00,
                  0x74, 0x00, 0x7E, 0x00, 0x86, 0x00, 0x8F, 0x00, 0x91, 0x00, 0x9F, 0x00, 0xA7, 0x00, 0xBB, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_BLUE_N: Blue Negative Gamma */
    { 0xE5, 36, { 0x03, 0x00, 0x10, 0x00, 0x1C, 0x00, 0x27, 0x00, 0x30, 0x00, 0x38, 0x00, 0x46, 0x00, 0x58, 0x00, 0x66, 0x00,
                  0x74, 0x00, 0x7E, 0x00, 0x86, 0x00, 0x8F, 0x00, 0x91, 0x00, 0x9F, 0x00, 0xA7, 0x00, 0xBB, 0x00, 0xFA, 0x00 }},
#elif defined( LCM_GAMMA_SAMPLE_2 )
  /* 3GAMMAR_CTRL_RED_P: Red Positive Gamma */
    { 0xE0, 36, { 0x09, 0x00, 0x13, 0x00, 0x1C, 0x00, 0x28, 0x00, 0x30, 0x00, 0x39, 0x00, 0x46, 0x00, 0x59, 0x00, 0x66, 0x00,
                  0x74, 0x00, 0x7C, 0x00, 0x83, 0x00, 0x89, 0x00, 0x91, 0x00, 0xA3, 0x00, 0xB0, 0x00, 0xDA, 0x00, 0xFF, 0x00 }},
  /* 3GAMMAR_CTRL_RED_N: Red Negative Gamma */
    { 0xE1, 36, { 0x09, 0x00, 0x13, 0x00, 0x1C, 0x00, 0x28, 0x00, 0x30, 0x00, 0x39, 0x00, 0x46, 0x00, 0x59, 0x00, 0x66, 0x00,
                  0x74, 0x00, 0x7C, 0x00, 0x83, 0x00, 0x89, 0x00, 0x91, 0x00, 0xA3, 0x00, 0xB0, 0x00, 0xDA, 0x00, 0xFF, 0x00 }},
  /* 3GAMMAR_CTRL_GREEN_P: Green Positive Gamma */
    { 0xE2, 36, { 0x09, 0x00, 0x13, 0x00, 0x1C, 0x00, 0x28, 0x00, 0x30, 0x00, 0x39, 0x00, 0x46, 0x00, 0x59, 0x00, 0x66, 0x00,
                  0x74, 0x00, 0x7C, 0x00, 0x83, 0x00, 0x89, 0x00, 0x91, 0x00, 0xA3, 0x00, 0xB0, 0x00, 0xDA, 0x00, 0xFF, 0x00 }},
  /* 3GAMMAR_CTRL_GREEN_N: Green Negative Gamma */
    { 0xE3, 36, { 0x09, 0x00, 0x13, 0x00, 0x1C, 0x00, 0x28, 0x00, 0x30, 0x00, 0x39, 0x00, 0x46, 0x00, 0x59, 0x00, 0x66, 0x00,
                  0x74, 0x00, 0x7C, 0x00, 0x83, 0x00, 0x89, 0x00, 0x91, 0x00, 0xA3, 0x00, 0xB0, 0x00, 0xDA, 0x00, 0xFF, 0x00 }},
  /* 3GAMMAR_CTRL_BLUE_P: Blue Positive Gamma */
    { 0xE4, 36, { 0x09, 0x00, 0x13, 0x00, 0x1C, 0x00, 0x28, 0x00, 0x30, 0x00, 0x39, 0x00, 0x46, 0x00, 0x59, 0x00, 0x66, 0x00,
                  0x74, 0x00, 0x7C, 0x00, 0x83, 0x00, 0x89, 0x00, 0x91, 0x00, 0xA3, 0x00, 0xB0, 0x00, 0xDA, 0x00, 0xFF, 0x00 }},
  /* 3GAMMAR_CTRL_BLUE_N: Blue Negative Gamma */
    { 0xE5, 36, { 0x09, 0x00, 0x13, 0x00, 0x1C, 0x00, 0x28, 0x00, 0x30, 0x00, 0x39, 0x00, 0x46, 0x00, 0x59, 0x00, 0x66, 0x00,
                  0x74, 0x00, 0x7C, 0x00, 0x83, 0x00, 0x89, 0x00, 0x91, 0x00, 0xA3, 0x00, 0xB0, 0x00, 0xDA, 0x00, 0xFF, 0x00 }},
#elif defined( LCM_GAMMA_SAMPLE_3 )
  /* 3GAMMAR_CTRL_RED_P: Red Positive Gamma */
    { 0xE0, 36, { 0x04, 0x00, 0x0E, 0x00, 0x1A, 0x00, 0x26, 0x00, 0x2F, 0x00, 0x37, 0x00, 0x43, 0x00, 0x54, 0x00, 0x61, 0x00,
                  0x6C, 0x00, 0x76, 0x00, 0x80, 0x00, 0x86, 0x00, 0x91, 0x00, 0x96, 0x00, 0xAE, 0x00, 0xCE, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_RED_N: Red Negative Gamma */
    { 0xE1, 36, { 0x04, 0x00, 0x0E, 0x00, 0x1A, 0x00, 0x26, 0x00, 0x2F, 0x00, 0x37, 0x00, 0x43, 0x00, 0x54, 0x00, 0x61, 0x00,
                  0x6C, 0x00, 0x76, 0x00, 0x80, 0x00, 0x86, 0x00, 0x91, 0x00, 0x96, 0x00, 0xAE, 0x00, 0xCE, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_GREEN_P: Green Positive Gamma */
    { 0xE2, 36, { 0x04, 0x00, 0x0E, 0x00, 0x1A, 0x00, 0x26, 0x00, 0x2F, 0x00, 0x37, 0x00, 0x43, 0x00, 0x54, 0x00, 0x61, 0x00,
                  0x6C, 0x00, 0x76, 0x00, 0x80, 0x00, 0x86, 0x00, 0x91, 0x00, 0x96, 0x00, 0xAE, 0x00, 0xCE, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_GREEN_N: Green Negative Gamma */
    { 0xE3, 36, { 0x04, 0x00, 0x0E, 0x00, 0x1A, 0x00, 0x26, 0x00, 0x2F, 0x00, 0x37, 0x00, 0x43, 0x00, 0x54, 0x00, 0x61, 0x00,
                  0x6C, 0x00, 0x76, 0x00, 0x80, 0x00, 0x86, 0x00, 0x91, 0x00, 0x96, 0x00, 0xAE, 0x00, 0xCE, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_BLUE_P: Blue Positive Gamma */
    { 0xE4, 36, { 0x04, 0x00, 0x0E, 0x00, 0x1A, 0x00, 0x26, 0x00, 0x2F, 0x00, 0x37, 0x00, 0x43, 0x00, 0x54, 0x00, 0x61, 0x00,
                  0x6C, 0x00, 0x76, 0x00, 0x80, 0x00, 0x86, 0x00, 0x91, 0x00, 0x96, 0x00, 0xAE, 0x00, 0xCE, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_BLUE_N: Blue Negative Gamma */
    { 0xE5, 36, { 0x04, 0x00, 0x0E, 0x00, 0x1A, 0x00, 0x26, 0x00, 0x2F, 0x00, 0x37, 0x00, 0x43, 0x00, 0x54, 0x00, 0x61, 0x00,
                  0x6C, 0x00, 0x76, 0x00, 0x80, 0x00, 0x86, 0x00, 0x91, 0x00, 0x96, 0x00, 0xAE, 0x00, 0xCE, 0x00, 0xFA, 0x00 }},
#elif defined( LCM_GAMMA_SAMPLE_4 )
  /* 3GAMMAR_CTRL_RED_P: Red Positive Gamma */
    { 0xE0, 36, { 0x04, 0x00, 0x0E, 0x00, 0x1A, 0x00, 0x26, 0x00, 0x30, 0x00, 0x38, 0x00, 0x43, 0x00, 0x55, 0x00, 0x61, 0x00,
                  0x6D, 0x00, 0x77, 0x00, 0x80, 0x00, 0x86, 0x00, 0x8C, 0x00, 0x96, 0x00, 0xA3, 0x00, 0xBC, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_RED_N: Red Negative Gamma */
    { 0xE1, 36, { 0x04, 0x00, 0x0E, 0x00, 0x1A, 0x00, 0x26, 0x00, 0x30, 0x00, 0x38, 0x00, 0x43, 0x00, 0x55, 0x00, 0x61, 0x00,
                  0x6D, 0x00, 0x77, 0x00, 0x80, 0x00, 0x86, 0x00, 0x8C, 0x00, 0x96, 0x00, 0xA3, 0x00, 0xBC, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_GREEN_P: Green Positive Gamma */
    { 0xE2, 36, { 0x04, 0x00, 0x0E, 0x00, 0x1A, 0x00, 0x26, 0x00, 0x30, 0x00, 0x38, 0x00, 0x43, 0x00, 0x55, 0x00, 0x61, 0x00,
                  0x6D, 0x00, 0x77, 0x00, 0x80, 0x00, 0x86, 0x00, 0x8C, 0x00, 0x96, 0x00, 0xA3, 0x00, 0xBC, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_GREEN_N: Green Negative Gamma */
    { 0xE3, 36, { 0x04, 0x00, 0x0E, 0x00, 0x1A, 0x00, 0x26, 0x00, 0x30, 0x00, 0x38, 0x00, 0x43, 0x00, 0x55, 0x00, 0x61, 0x00,
                  0x6D, 0x00, 0x77, 0x00, 0x80, 0x00, 0x86, 0x00, 0x8C, 0x00, 0x96, 0x00, 0xA3, 0x00, 0xBC, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_BLUE_P: Blue Positive Gamma */
    { 0xE4, 36, { 0x04, 0x00, 0x0E, 0x00, 0x1A, 0x00, 0x26, 0x00, 0x30, 0x00, 0x38, 0x00, 0x43, 0x00, 0x55, 0x00, 0x61, 0x00,
                  0x6D, 0x00, 0x77, 0x00, 0x80, 0x00, 0x86, 0x00, 0x8C, 0x00, 0x96, 0x00, 0xA3, 0x00, 0xBC, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_BLUE_N: Blue Negative Gamma */
    { 0xE5, 36, { 0x04, 0x00, 0x0E, 0x00, 0x1A, 0x00, 0x26, 0x00, 0x30, 0x00, 0x38, 0x00, 0x43, 0x00, 0x55, 0x00, 0x61, 0x00,
                  0x6D, 0x00, 0x77, 0x00, 0x80, 0x00, 0x86, 0x00, 0x8C, 0x00, 0x96, 0x00, 0xA3, 0x00, 0xBC, 0x00, 0xFA, 0x00 }},
#else /* default */
  /* 3GAMMAR_CTRL_RED_P: Red Positive Gamma */
    { 0xE0, 36, { 0x05, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x2F, 0x00, 0x37, 0x00, 0x42, 0x00, 0x4E, 0x00, 0x60, 0x00, 0x6D, 0x00,
                  0x78, 0x00, 0x82, 0x00, 0x8B, 0x00, 0x91, 0x00, 0x95, 0x00, 0x9C, 0x00, 0xA5, 0x00, 0xB2, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_RED_N: Red Negative Gamma */
    { 0xE1, 36, { 0x05, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x2F, 0x00, 0x37, 0x00, 0x42, 0x00, 0x4E, 0x00, 0x60, 0x00, 0x6D, 0x00,
                  0x78, 0x00, 0x82, 0x00, 0x8B, 0x00, 0x91, 0x00, 0x95, 0x00, 0x9C, 0x00, 0xA5, 0x00, 0xB2, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_GREEN_P: Green Positive Gamma */
    { 0xE2, 36, { 0x05, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x2F, 0x00, 0x37, 0x00, 0x42, 0x00, 0x4E, 0x00, 0x60, 0x00, 0x6D, 0x00,
                  0x78, 0x00, 0x82, 0x00, 0x8B, 0x00, 0x91, 0x00, 0x95, 0x00, 0x9C, 0x00, 0xA5, 0x00, 0xB2, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_GREEN_N: Green Negative Gamma */
    { 0xE3, 36, { 0x05, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x2F, 0x00, 0x37, 0x00, 0x42, 0x00, 0x4E, 0x00, 0x60, 0x00, 0x6D, 0x00,
                  0x78, 0x00, 0x82, 0x00, 0x8B, 0x00, 0x91, 0x00, 0x95, 0x00, 0x9C, 0x00, 0xA5, 0x00, 0xB2, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_BLUE_P: Blue Positive Gamma */
    { 0xE4, 36, { 0x05, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x2F, 0x00, 0x37, 0x00, 0x42, 0x00, 0x4E, 0x00, 0x60, 0x00, 0x6D, 0x00,
                  0x78, 0x00, 0x82, 0x00, 0x8B, 0x00, 0x91, 0x00, 0x95, 0x00, 0x9C, 0x00, 0xA5, 0x00, 0xB2, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_BLUE_N: Blue Negative Gamma */
    { 0xE5, 36, { 0x05, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x2F, 0x00, 0x37, 0x00, 0x42, 0x00, 0x4E, 0x00, 0x60, 0x00, 0x6D, 0x00,
                  0x78, 0x00, 0x82, 0x00, 0x8B, 0x00, 0x91, 0x00, 0x95, 0x00, 0x9C, 0x00, 0xA5, 0x00, 0xB2, 0x00, 0xFA, 0x00 }},
#endif
  /* WRCTRLD: Write CTRL Display */
  //{ 0x53,  1, { 0x24 }},
  /* WRCTRLD: Write CTRL Display, Image enhancement */
//<<EricHsieh,2013/2/6,CE enable setting.
#ifdef CE_ON
  { 0xB0,  18, { 0x01,0x00,0x09,0x00,0x11,0x00,0x19,0x00,0x21,0x00,0x2D,0x00,0x21,0x00,0x19,0x00,0x11,0x00 }},
  { 0xB1,  6, { 0x8B,0x00,0xA1,0x00,0x90,0x00 }},
  { 0xB2,  6, { 0x00,0x00,0x00,0x00,0x00,0x00 }},
  { 0xB3,  24, { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }},
  { 0xB6,  2, { 0x00,0x00 }},
  { 0xB7,  22, { 0x3E,0x00,0x5E,0x00,0x9E,0x00,0x04,0x00,0x8C,0x00,0xAc,0x00,0xDC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }},
  { 0xB4,  6, { 0x98,0x00,0xA0,0x00,0xA8,0x00 }},
  { 0xB5,  6, { 0x02,0x00,0x03,0x00,0x04,0x00 }},
#endif
  //>>EricHsieh,2013/2/6,CE enable setting.

  /* Gamma Line and non-line OPEN */
    { 0xE9,  4, { 0xAA, 0x00, 0x00, 0x00 }},

  /*******************************************
  ** CMD2 Page0
  ********************************************/
  /* PAGE_LOCK: Set the Register to command2 Page 0 */
    { 0x00,  1, { 0xAA }}, /* Exit CMD2 Page1 to CMD2 Page0. */

//<<EricHsieh,2013/2/6,CE enable setting.
#ifdef CE_ON
  { 0x55,  1, { 0xB0 }},
#endif
 //>>EricHsieh,2013/2/6,CE enable setting.


  /* */
  //{ 0x56,  1, { 0x90 }},
  /* */
  //{ 0x0C,  1, { 0x66 }},
  /* SET_TEAR_ON: Tearing Effect Line ON TE ON */
    { 0x35,  1, { 0x00 }},
  /* Sleep Mode Off */
    { 0x11,  0, { 0x00 }},
    { REGFLAG_DELAY, 15, {}},
  /* Display On */
    { 0x29,  0, {}},
    { REGFLAG_DELAY, 5, {}},

#else /** (LCM_PANEL_HW_P2) **/
//>2012/12/07-Yuting Shih, Add for HW P2.
/*******************************************
** SW Reset
********************************************/
#if defined( BUILD_LK ) || defined( BUILD_UBOOT )
    { 0x01,  0, { }},
    { REGFLAG_DELAY, 150, {}},
#endif
//<2012/11/26-Yuting Shih-17685-[BU2SC00138012], Adjust the timing of resume.
  /*******************************************
  **
  ********************************************/
#if 0
  /*  */
    { 0xB8,  2, { 0x00, 0x00 }},
  /*  */
    { 0xB9,  2, { 0x00, 0x00 }},
  /*  */
  //{ 0xBA,  2, { 0x0B, 0x00 }},  /* OSC=20MHz / n=11 ==>(11+1)x20M= 240Mbps ( high speed 20: 500 Mbps (n+1) x TX_CLK) */
    { 0xBA,  2, { 0x10, 0x00 }},  /* OSC=13MHz / n=16 ==>(16+1)x13M= 221Mbps ( high speed 13: 500 Mbps (n+1) x TX_CLK) */
  /*  */
  //{ 0xBB,  2, { 0x03, 0x00 }},  /* LP Mode setting : LP Clock = byte clock/4 : 0x03 => 240M/8/4=7.5M */
    { 0xBB,  2, { 0x03, 0x00 }},  /* LP Mode setting : LP Clock = byte clock/4 : 0x03 => 221M/8/4=6.9M */
  /*  */
    { 0xB9,  2, { 0x01, 0x00 }},
    { REGFLAG_DELAY, 10, {}},
#endif
  /*******************************************
  ** DCS mode, HS mode
  ********************************************/
  /*  */
    { 0xB7,  2, { 0x50, 0x02 }},
  /*  */
    { 0xBD,  2, { 0x00, 0x00 }},
  /*  */
    { 0xBC,  2, { 0x00, 0x00 }},
//>2012/11/26-Yuting Shih-17685-[BU2SC00138012].
  /*******************************************
  ** CMD1
  ********************************************/
  /* Sleep Out */
    { 0x11,  0, { 0x00 }},
    { REGFLAG_DELAY, 120, {}},
  /* SET_PIXEL_FORMAT: Set the Interface Pixel Format */
  #if defined( LCM_PIXEL_24B_888 )
    { 0x3A,  1, { 0x67 }},  /* 24-bits/pixel(MIPI only) */
  #else
    { 0x3A,  1, { 0x66 }},  /* 18-bits/pixel */
  #endif
  /* SET_ADDRESS_MODE: Memory Data Access Control */
    { 0x36,  1, { LCM_MEMORY_ACCESS }},
 /* RGBCTRL: RGB Interface Signal Control */
  #if defined( LCM_DSI_CMD_MODE )
  /* RGB_MODE1: CRCM = 0;HBP, HFP, VBP, VFP are not used. */
  //{ 0x3B,  5, { 0x03, LCM_VBP_NUM, LCM_VFP_NUM, LCM_HBP_NUM, LCM_HFP_NUM }},
  #else
  /* RGB_MODE2: CRCM = 1; DE is not used. */
  //{ 0x3B,  5, { 0x43, LCM_VBP_NUM, LCM_VFP_NUM, LCM_HBP_NUM, LCM_HFP_NUM }},
  #endif
  /* WRCTRLD: Write CTRL Display */
  //{ 0x53,  1, { 0x24 }},
  /* WRCTRLD: Write CTRL Display, Image enhancement */
  //{ 0x55,  1, { 0x00 }},

  /*******************************************
  ** CMD2 Page0
  ********************************************/
  /* PAGE CTRL: Unlock CMD2 */
    { 0xED,  2, { 0x01, 0xFE }},  /* Enter CMD2 Page 0 */
//<2012/11/06-Yuting Shih-17685-[BU2SC00138012], Adjust the timing of resume.
#if defined( BUILD_LK ) || defined( BUILD_UBOOT )
    { REGFLAG_DELAY, 5, {}},     /* Wait 20ms */
#endif
//>2012/11/06-Yuting Shih-17685-[BU2SC00138012].
//<2012/11/26-Yuting Shih-17685-[BU2SC00138012], Adjust the timing of resume.
  /*  MTPPWR: MTP Write function enable */
    { 0xDF,  1, { 0x10 }},  /* nROM: Do not reloae MTP after SLPOUT. MTP_W: MTP write function disable. */
//>2012/11/26-Yuting Shih.
  /*  DISPLAY_CTRL: Display Control */
    { 0xB0,  1, { (LCM_DSIN|0x0C) }}, /* DSIN:0, PT1:V63+/V63-, CRL:Source S0~S959, CTB:G479~G0, PT:V63+/V63- */
  /* PORCH_CTRL: Front and Back porch setting */
  //{ 0xB1,  3, { LCM_BPAC_NUM, LCM_BPB_NUM, (LCM_BP_ACB|LCM_FP_NUM) }},
  /* FRAMERATE_CTRL: Front and Back porch setting */
  //{ 0xB2,  3, { 0x8D, 0xD3, 0x8D }}, /* RTNA, RTNB, RTNC: 60Hz */
  /* SPI&RGB IF SETTING */
  //{ 0xB3,  1, { 0x20 }}, /* */
  /* INVCTRL: Inversion Control */
    { 0xB4,  1, { 0x1D }}, /* NLC:2dot inversion, NLB:column inversion, NLA:2dot inversion */
  /* DISPLAY_CTRL2: Set the States for LED Control */
    { 0xB7,  1, { (LCM_SRGB_ORDER|0x20) }}, /* REV:NB, SRGB:RGB/BGR, CTS:FULL Command Set */
  /* PWR_CTRL1: Set the GVDD regulator output voltage. */
    { 0xC0,  4, { 0x4C, 0x4C, 0x10, 0x10 }},
  /* PWR_CTRL3: VGH charge pump circuit selection. */
  //{ 0xC2,  3, { 0x24, 0x24, 0x24 }},
  /* PWR_CTRL7: VGL clamp voltage selection. */
    { 0xC6,  4, { 0x00, 0xE4, 0xE4, 0xE4 }},

  /*******************************************
  ** CMD2 Page1
  ********************************************/
  /* PAGE LOCK, Set the Register to command2 Page 1  */
    { 0xBF,  1, { 0xAA }},
//<2012/11/06-Yuting Shih-17685-[BU2SC00138012], Adjust the timing of resume.
#if defined( BUILD_LK ) || defined( BUILD_UBOOT )
    { REGFLAG_DELAY, 5, {}},     /* Wait 20ms */
#endif
//>2012/11/06-Yuting Shih-17685-[BU2SC00138012].
  /* 3GAMMAR_CTRL_RED_P: Red Positive Gamma */
    { 0xE0, 36, { 0x05, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x2F, 0x00, 0x37, 0x00,
                  0x42, 0x00, 0x4E, 0x00, 0x60, 0x00, 0x6D, 0x00, 0x78, 0x00,
                  0x82, 0x00, 0x8B, 0x00, 0x91, 0x00, 0x95, 0x00, 0x9C, 0x00,
                  0xA5, 0x00, 0xB2, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_RED_N: Red Negative Gamma */
    { 0xE1, 36, { 0x05, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x2F, 0x00, 0x37, 0x00,
                  0x42, 0x00, 0x4E, 0x00, 0x60, 0x00, 0x6D, 0x00, 0x78, 0x00,
                  0x82, 0x00, 0x8B, 0x00, 0x91, 0x00, 0x95, 0x00, 0x9C, 0x00,
                  0xA5, 0x00, 0xB2, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_GREEN_P: Green Positive Gamma */
    { 0xE2, 36, { 0x05, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x2F, 0x00, 0x37, 0x00,
                  0x42, 0x00, 0x4E, 0x00, 0x60, 0x00, 0x6D, 0x00, 0x78, 0x00,
                  0x82, 0x00, 0x8B, 0x00, 0x91, 0x00, 0x95, 0x00, 0x9C, 0x00,
                  0xA5, 0x00, 0xB2, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_GREEN_N: Green Negative Gamma */
    { 0xE3, 36, { 0x05, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x2F, 0x00, 0x37, 0x00,
                  0x42, 0x00, 0x4E, 0x00, 0x60, 0x00, 0x6D, 0x00, 0x78, 0x00,
                  0x82, 0x00, 0x8B, 0x00, 0x91, 0x00, 0x95, 0x00, 0x9C, 0x00,
                  0xA5, 0x00, 0xB2, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_BLUE_P: Blue Positive Gamma */
    { 0xE4, 36, { 0x05, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x2F, 0x00, 0x37, 0x00,
                  0x42, 0x00, 0x4E, 0x00, 0x60, 0x00, 0x6D, 0x00, 0x78, 0x00,
                  0x82, 0x00, 0x8B, 0x00, 0x91, 0x00, 0x95, 0x00, 0x9C, 0x00,
                  0xA5, 0x00, 0xB2, 0x00, 0xFA, 0x00 }},
  /* 3GAMMAR_CTRL_BLUE_N: Blue Negative Gamma */
    { 0xE5, 36, { 0x05, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x2F, 0x00, 0x37, 0x00,
                  0x42, 0x00, 0x4E, 0x00, 0x60, 0x00, 0x6D, 0x00, 0x78, 0x00,
                  0x82, 0x00, 0x8B, 0x00, 0x91, 0x00, 0x95, 0x00, 0x9C, 0x00,
                  0xA5, 0x00, 0xB2, 0x00, 0xFA, 0x00 }},
  /*******************************************
  ** CMD2 Page0
  ********************************************/
  /* PAGE_LOCK: Set the Register to command2 Page 0 */
    { 0x00,  1, { 0xAA }}, /* Exit CMD2 Page1 to CMD2 Page0. */
//<2012/11/06-Yuting Shih-17685-[BU2SC00138012], Adjust the timing of resume.
#if defined( BUILD_LK ) || defined( BUILD_UBOOT )
    { REGFLAG_DELAY, 5, {}},     /* Wait 20ms */
#endif
//>2012/11/06-Yuting Shih-17685-[BU2SC00138012].

  /*******************************************
  ** CMD1
  ********************************************/
  /* PAGE_LOCK: Set the Register to command1 */
//    { 0xEF,  1, { 0xAA }}, /* Exit CMD2 Page0 to CMD1. */
////<2012/11/06-Yuting Shih-17685-[BU2SC00138012], Adjust the timing of resume.
//#if defined( BUILD_LK ) || defined( BUILD_UBOOT )
//  //{ REGFLAG_DELAY, 5, {}},     /* Wait 20ms */
//#endif
////>2012/11/06-Yuting Shih-17685-[BU2SC00138012].

  /* SET_TEAR_LINE: Set tearing line */
  //{ 0x44,  2, { LCM_TE_SCAN_LINE_MSB, LCM_TE_SCAN_LINE_LSB }},
  /* SET_TEAR_ON: Tearing Effect Line ON TE ON */
    { 0x35,  1, { 0x00 }},
  /* Display On */
    { 0x29,  0, {}},
    { REGFLAG_DELAY, 5, {}},

#endif /** End.. (LCM_PANEL_HW_P2) **/
/*******************************************
** End
********************************************/
    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

static LCM_SET_TAB  lcm_suspend_set[] =
{
//<2012/11/26-Yuting Shih-17685-[BU2SC00138012], Adjust the timing of resume.
  /*******************************************
  ** DCS mode, HS mode
  ********************************************/
  /*  */
  //{ 0xB7,  2, { 0x50, 0x02 }},
  /*  */
  //{ 0xBD,  2, { 0x00, 0x00 }},
  /*  */
  //{ 0xBC,  2, { 0x00, 0x00 }},
//>2012/11/26-Yuting Shih-17685-[BU2SC00138012].
  /* Display off sequence */
    { 0x28, 0, { 0x00 }},
    { REGFLAG_DELAY, 5, {}},
  /* Sleep Mode On */
    { 0x10, 0, { 0x00 }},
    { REGFLAG_DELAY, 120, {}},

    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

static LCM_SET_TAB  lcm_resume_set[] =
{
//<2012/11/26-Yuting Shih-17685-[BU2SC00138012], Adjust the timing of resume.
  /*******************************************
  ** DCS mode, HS mode
  ********************************************/
  /*  */
  //{ 0xB7,  2, { 0x50, 0x02 }},
  /*  */
  //{ 0xBD,  2, { 0x00, 0x00 }},
  /*  */
  //{ 0xBC,  2, { 0x00, 0x00 }},
//>2012/11/26-Yuting Shih-17685-[BU2SC00138012].
//<2012/11/06-Yuting Shih-17685-[BU2SC00138012], Adjust the timing of resume.
  /* Sleep Mode Off */
    { 0x11,  0, { 0x00 }},
  //{ REGFLAG_DELAY, 120, {}},
    { REGFLAG_DELAY, 15, {}},
//>2012/11/06-Yuting Shih-17685-[BU2SC00138012].
  /* Display On */
    { 0x29,  0, {}},
    { REGFLAG_DELAY, 5, {}},

    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

#if 0
static LCM_SET_TAB  lcm_deep_standby_set[] =
{
  /*******************************************
  ** DCS mode, HS mode
  ********************************************/
  /*  */
  //{ 0xB7,  2, { 0x50, 0x02 }},
  /*  */
  //{ 0xBD,  2, { 0x00, 0x00 }},
  /*  */
  //{ 0xBC,  2, { 0x00, 0x00 }},
  /* Display off sequence */
    { 0x28, 0, { 0x00 }},
    { REGFLAG_DELAY, 5, {}},
  /* Sleep Mode On */
    { 0x10, 0, { 0x00 }},
    { REGFLAG_DELAY, 120, {}}, /* Delay time > 4 Frames */
  /* ENTER_DSTB_MODE */
    { 0x4F, 1, { 0x01 }},

    { REGFLAG_END_OF_TABLE, 0x00, {}}
};
#endif

#if defined( LCM_DSI_CMD_MODE )
static LCM_SET_TAB  lcm_backlight_set[] =
{
    { 0x51,  1, { 0x00 }},
    { REGFLAG_END_OF_TABLE, 0x00, {}}
};
#endif

/******************************************************************************
** LCM Driver Implementations
** Local Functions
*******************************************************************************/
/***********************************************************************
** push_table
************************************************************************/
static void push_table(LCM_SET_TAB *table,unsigned int count,unsigned char force_update)
{
unsigned int  i = 0;
unsigned      cmd = 0;

  for( i = 0; i < count; i++ )
  {
    cmd = table[i].cmd;
    switch( cmd )
    {
      case REGFLAG_DELAY:
        MDELAY( table[i].count );
        break;

      case REGFLAG_END_OF_TABLE:
        break;

      default:
        dsi_set_cmdq_V2( cmd, table[i].count, table[i].para_list, force_update );
    }
  }
} /** End.. push_table() **/

/***********************************************************************
** init_lcm_registers
************************************************************************/
static void init_lcm_registers(void)
{
  push_table((LCM_SET_TAB*)&lcm_initial_set, sizeof( lcm_initial_set )/sizeof( LCM_SET_TAB ), 1 );
} /* End.. init_lcm_registers() */

#if defined( LCM_DSI_CMD_MODE )
/***********************************************************************
** lcm_set_backlight
************************************************************************/
static void lcm_set_backlight(unsigned int level)
{
  if( level > 255 )
    level = 255;

  lcm_backlight_set[0].para_list[0] = (unsigned char)level;
  push_table((LCM_SET_TAB*)&lcm_backlight_set, sizeof( lcm_backlight_set )/sizeof( LCM_SET_TAB ), 1 );
} /* End.. lcm_set_backlight() */

/***********************************************************************
** lcm_update
************************************************************************/
static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
unsigned int  data_array[16] = { 0 };
unsigned int  x0 = x;
unsigned int  y0 = y;
unsigned int  x1 = x0 + width - 1;
unsigned int  y1 = y0 + height - 1;

unsigned char x0_MSB = ((x0>>8)&0xFF);
unsigned char x0_LSB = (x0&0xFF);
unsigned char x1_MSB = ((x1>>8)&0xFF);
unsigned char x1_LSB = (x1&0xFF);
unsigned char y0_MSB = ((y0>>8)&0xFF);
unsigned char y0_LSB = (y0&0xFF);
unsigned char y1_MSB = ((y1>>8)&0xFF);
unsigned char y1_LSB = (y1&0xFF);

#if defined( LCM_DEBUG_ENABLE )
  LCM_MSG( "[NT35310_DSI]%s enter\n", __func__ );
#endif

  data_array[0] = 0x00053902;
  data_array[1] = (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2A;
  data_array[2] = (x1_LSB);
  data_array[3] = 0x00053902;
  data_array[4] = (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2B;
  data_array[5] = (y1_LSB);
  data_array[6] = 0x002C3909;

  dsi_set_cmdq( &data_array, 7, 0 );
} /* End.. lcm_update() */

/***********************************************************************
** lcm_setpwm
************************************************************************/
static void lcm_setpwm(unsigned int divider)
{
  // TBD
} /* End.. lcm_setpwm() */

/***********************************************************************
** lcm_getpwm
************************************************************************/
static unsigned int lcm_getpwm(unsigned int divider)
{
unsigned int  pwm_clk = 0;

  pwm_clk = LCM_PWM_CLK / ( 1 << divider );

  return  pwm_clk;
} /* End.. lcm_getpwm() */
#endif /* End.. (LCM_DSI_CMD_MODE) */

/***********************************************************************
** lcm_set_util_funcs
************************************************************************/
static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
  memcpy( &lcm_util, util, sizeof( LCM_UTIL_FUNCS ));
} /* End.. lcm_set_util_funcs() */

/***********************************************************************
** lcm_init
************************************************************************/
static void lcm_init(void)
{
//<2012/11/06-Yuting Shih-[BU2SC00138012], Adjust the timing of resume.
#if defined( BUILD_LK ) || defined( BUILD_UBOOT )
  SET_RESET_PIN( 1 );
  MDELAY( 2 );
  SET_RESET_PIN( 0 );
  MDELAY( 10 );
  SET_RESET_PIN( 1 );
  MDELAY( 30 );
#else
  SET_RESET_PIN( 1 );
  MDELAY( 2 );
  SET_RESET_PIN( 0 );
  MDELAY( 7 );
  SET_RESET_PIN( 1 );
  MDELAY( 12 );
#endif
//>2012/11/06-Yuting Shih-[BU2SC00138012].
  init_lcm_registers();
} /* End.. lcm_init() */

/***********************************************************************
** lcm_suspend
************************************************************************/
static void lcm_suspend(void)
{
#if defined( LCM_MSG_ENABLE )
  LCM_MSG( "[NT35310_DSI] %s...\n", __func__ );
#endif
  push_table((LCM_SET_TAB*)&lcm_suspend_set, sizeof( lcm_suspend_set )/sizeof( LCM_SET_TAB ), 1 );
} /* End.. lcm_suspend() */

/***********************************************************************
** lcm_resume
************************************************************************/
static void lcm_resume(void)
{
#if defined( LCM_MSG_ENABLE )
  LCM_MSG( "[NT35310_DSI] %s...\n", __func__ );
#endif
//<2012/11/06-Yuting Shih-17685-[BU2SC00138012], Adjust the timing of resume.
#if defined( BUILD_LK ) || defined( BUILD_UBOOT )
    lcm_init();
#else
  #if 0
    lcm_init();
  #else
    push_table((LCM_SET_TAB*)&lcm_resume_set, sizeof( lcm_resume_set )/sizeof( LCM_SET_TAB ), 1 );
  #endif
#endif
//>2012/11/06-Yuting Shih-17685-[BU2SC00138012].
} /* End.. lcm_resume() */

/***********************************************************************
** lcm_deep_standby
************************************************************************/
#if 0
static void lcm_deep_standby(void)
{
  push_table((LCM_SET_TAB*)&lcm_deep_standby_set, sizeof( lcm_deep_standby_set )/sizeof( LCM_SET_TAB ), 1 );
} /* End.. lcm_deep_standby() */
#endif

/***********************************************************************
** lcm_compare_id
************************************************************************/
static unsigned int lcm_compare_id(void)
{
unsigned int  data_array[16] = { 0 };
unsigned int  id[4] = { 0 }, vRet = 0;
unsigned char buffer[4] = { 0 };

#if defined( LCM_MSG_ENABLE )
  LCM_MSG( "[NT35310_DSI] %s...\n", __func__ );
#endif

//<2012/10/17-Yuting Shih, Midified reading LCM id is unstable.
#if 0 //defined( BUILD_LK ) || defined( BUILD_UBOOT )
  DSI_clk_HS_mode( 1 );
  MDELAY( 10 );
  DSI_clk_HS_mode( 0 );
#endif
//>2012/10/17-Yuting Shih.
#if defined( BUILD_LK ) || defined( BUILD_UBOOT )
  SET_GPIO_MODE( GPIO_LCM_ID_PIN, GPIO_LCM_ID_PIN_M_GPIO );
  SET_GPIO_DIR_IN( GPIO_LCM_ID_PIN );
#endif
/* NOTE:should reset LCM firstly */
  SET_RESET_PIN( 1 );
  MDELAY( 3 );
  SET_RESET_PIN( 0 );
  MDELAY( 10 );
  SET_RESET_PIN( 1 );
  MDELAY( 15 );

/** Sleep out **/
#if 0
  data_array[0] = 0x00110500;
  dsi_set_cmdq( &data_array, 1, 1 );
  MDELAY( 120 );
#endif

/************* Enable CMD2 Page0 *******************/
  data_array[0] = 0x00033902;
  data_array[1] = 0x00FE01ED;
  dsi_set_cmdq( &data_array, 2, 1 );
  MDELAY( 10 );

/* Set Maximum Return Packet Size: Read ID return 4 bytes, vender and version id */
  data_array[0] = 0x00043700;
  dsi_set_cmdq( &data_array, 1, 1 );
  MDELAY( 10 );

/***************************************************************
** READID4 (D4h): Read ID4
****************************************************************/
  read_reg_v2( 0xD4, (unsigned char*)&buffer[0], 4 );
  MDELAY( 10 );
  id[0] = buffer[0];  /* Vender ID code */
  id[1] = buffer[1];  /* Chip ID code. */
  id[2] = buffer[2];  /* Chip ID code. */
  id[3] = buffer[3];  /* Chip version code. */

#if 1 //defined( LCM_MSG_ENABLE )
  LCM_MSG( "[NT35310_DSI] %s -- ID is 0x%02X,0x%02X%02X,0x%02X\n", __func__,
         id[0], id[1], id[2], id[3]  );
#endif

//if(( LCM_ID0 == id[0] ) && ( LCM_ID1 == id[1] ) && ( LCM_ID2 == id[2] ))

  if( LCM_ID1 == id[1] )
  {
  #if 1//defined( LCM_MSG_ENABLE )
    LCM_MSG( "[NT35310_DSI] %s -- Check ID OK. id[1]=%x, LCM_ID1=%x\n", __func__,id[1],LCM_ID1 );
  #endif
    vRet = 1;
  }
  else
  {
    if( LCM_SECOND_SOURCE != mt_get_gpio_in( GPIO_LCM_ID_PIN ))
    {
    #if 1//defined( LCM_MSG_ENABLE )
      LCM_MSG( "[NT35310_DSI] %s -- LCM_ID GPIO OK.\n", __func__ );
    #endif
      vRet = 1;
    }
  }

/************* Enable CMD1 *******************/
//data_array[0] = 0xAAEF1502;
//dsi_set_cmdq( &data_array, 1, 1 ); /* Exit CMD2 Page0 to CMD1. */
//MDELAY( 20 );

  return  vRet;   //( LCM_ID1 == id[1] )? 1 : 0;
} /* End.. lcm_compare_id() */

/***********************************************************************
** lcm_get_params
************************************************************************/
static void lcm_get_params(LCM_PARAMS *params)
{
    memset( params, 0x00, sizeof( LCM_PARAMS ));

    params->type    = LCM_TYPE_DSI;

    params->width   = FRAME_WIDTH;
    params->height  = FRAME_HEIGHT;

  /* Enable tearing-free */
#if defined( LCM_TE_VSYNC_MODE )
    params->dbi.te_mode           = LCM_DBI_TE_MODE_VSYNC_ONLY;
#else
    params->dbi.te_mode           = LCM_DBI_TE_MODE_DISABLED;
#endif
    params->dbi.te_edge_polarity  = LCM_POLARITY_RISING;  //LCM_POLARITY_FALLING

#if defined( LCM_DSI_CMD_MODE )
    params->dsi.mode  = CMD_MODE;
#else
  //params->dsi.mode  = SYNC_PULSE_VDO_MODE;
    params->dsi.mode  = SYNC_EVENT_VDO_MODE;
  //params->dsi.mode  = BURST_VDO_MODE;
#endif

  /** DSI **/
  /* Command mode setting */
    params->dsi.LANE_NUM                = LCM_ONE_LANE;   //LCM_TWO_LANE;

  /* The following defined the fomat for data coming from LCD engine. */
#if 1 //defined( LCM_PIXEL_RGB )
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
#else
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_BGR;
#endif
    params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
#if defined( LCM_PIXEL_24B_888 )
    params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;
#else
    params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB666;
#endif
  /* Highly depends on LCD driver capability. */
    params->dsi.packet_size = 256;

  /* Video mode setting */
    params->dsi.intermediat_buffer_num  = 2;
#if defined( LCM_PIXEL_24B_888 )
    params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
#else
    params->dsi.PS = LCM_PACKED_PS_18BIT_RGB666;
#endif

  /* Should be not equal 0 */
    params->dsi.word_count = FRAME_WIDTH * 3;
    params->dsi.line_byte  = LCM_LINE_BYTE;
    params->dsi.rgb_byte   = FRAME_WIDTH * 3 + 6;

#if !defined( LCM_DSI_CMD_MODE )
    params->dsi.vertical_sync_active    = LCM_VSYNC_NUM;
    params->dsi.vertical_backporch      = LCM_VBP_NUM;
    params->dsi.vertical_frontporch     = LCM_VFP_NUM;
    params->dsi.vertical_active_line    = FRAME_HEIGHT;

    params->dsi.horizontal_sync_active  = LCM_HSYNC_NUM;
    params->dsi.horizontal_backporch    = LCM_HBP_NUM;
    params->dsi.horizontal_frontporch   = LCM_HFP_NUM;
    params->dsi.horizontal_active_pixel = FRAME_WIDTH;

    params->dsi.horizontal_sync_active_byte = LCM_HSYNC_NUM * 3 + 6;
    params->dsi.horizontal_backporch_byte   = LCM_HBP_NUM * 3 + 6;
    params->dsi.horizontal_frontporch_byte  = LCM_HFP_NUM * 3 + 6;

    params->dsi.horizontal_sync_active_word_count = LCM_HSYNC_NUM * 3;
    params->dsi.horizontal_backporch_word_count   = LCM_HBP_NUM * 3;
    params->dsi.horizontal_frontporch_word_count  = LCM_HFP_NUM * 3;
#endif

  /*===========================================================================
  ** Bit rate calculation
  **---------------------------------------------------------------------------
  ** fref=26MHz, fvco=fref*(div1+1) (div1=0~63, fvco=500MHZ~1GHz)
  ** div2=0~15: fout=fvco/(2*div2) (Mbps)
  **---------------------------------------------------------------------------
  ** cycle_time = (8*1000*(div2*2))/(26*(div1+1))
  ** ui = (1000*(div2*2))/(26*(div1+1)) + 1
  **===========================================================================
  ** 24/1 for 325.0Mbps, 26/1 for 351.0Mbps, 27/1 for 364.0Mbps,
  ** 30/1 for 403.0Mbps, 36/2 for 240.5Mbps, 34/2 for 221.0Mbps
  **===========================================================================*/
#if defined( LCM_PIXEL_24B_888 )
  /*=======================================================
  ** Min rate: 320*(480+32)*24*60=235.9296Mbps
  ** Min: 240.5 Mbps
  **=======================================================*/
  #if defined( LCM_DSI_CMD_MODE )
    params->dsi.pll_div1 = 27;
    params->dsi.pll_div2 = 1;
  #else
    params->dsi.pll_div1 = 30;
    params->dsi.pll_div2 = 1;
  #endif
#else
  /*=======================================================
  ** Min rate: 320*(480+32)*18*60=176.9472Mbps
  ** Min: 182.0 Mbps
  **=======================================================*/
  #if defined( LCM_DSI_CMD_MODE )
    params->dsi.pll_div1 = 27;  //24
    params->dsi.pll_div2 = 1;   //1
  #else
    params->dsi.pll_div1 = 30;  //26
    params->dsi.pll_div2 = 1;   //1
  #endif
#endif

//<<EricHsieh,2012/1/8,For read LCD ID will cause system reset
//<<EricHsieh,2013/1/21, improve the LCD tearing issue
  /*=========================================================
  ** DSI frame interval time
  **---------------------------------------------------------
  ** THS_TRAIL  = (HS_TRAIL - 6) * cycle_time, 
  ** THS_ZERO   = (HS_ZERO + 6) * cycle_time
  ** THS_PRPR   = (HS_PRPR) * cycle_time
  ** TLPX       = (LPX) * cycle_time
  **=========================================================*/
  #if 0
    params->dsi.HS_TRAIL  = 10; //8;  //Min:max(n*8*UI,60ns+n*4UI)=60+4UI
    params->dsi.HS_ZERO   = 2;  //6;  //Min:105ns+6*UI
    params->dsi.HS_PRPR   = 4;  //3;  //Min:40ns+4*UI; Max:85ns+6UI
    params->dsi.LPX       = 4;  //4;  //Min:50ns

    params->dsi.TA_SACK   = 1;
    params->dsi.TA_GET    = 20;
    params->dsi.TA_SURE   = 6;  //18;
    params->dsi.TA_GO     = 16; //4*LPX

    params->dsi.CLK_TRAIL = 4;  //3;  //min 60ns
    params->dsi.CLK_ZERO  = 12; //8;  //min 300ns-38ns
    params->dsi.LPX_WAIT  = 8;  //10;
    params->dsi.CONT_DET  = 0;

    params->dsi.CLK_HS_PRPR = 3;  //2;  //min 38ns; max 95ns
  #else
    params->dsi.HS_TRAIL  = 12; //8;  //min max(n*8*UI, 60ns+n*4UI)
    params->dsi.HS_ZERO   = 8;  //6;  //min 105ns+6*UI
    params->dsi.HS_PRPR   = 4;  //3;  //min 40ns+4*UI; max 85ns+6UI
    params->dsi.LPX       = 12; //4;  //min 50ns

    params->dsi.TA_SACK   = 1;  //1;
    params->dsi.TA_GET    = params->dsi.LPX * 5;          //60; //5*LPX
    params->dsi.TA_SURE   = ( params->dsi.LPX * 3 ) / 2;  //18; //Min: LPX, MAX:2*LPX
    params->dsi.TA_GO     = params->dsi.LPX * 4;          //16; //4*LPX

    params->dsi.CLK_TRAIL = 5;  //3;  //min 60ns
    params->dsi.CLK_ZERO  = 15; //8;  //min 300ns-38ns
    params->dsi.LPX_WAIT  = 10; //10;
    params->dsi.CONT_DET  = 0;  //0;

    params->dsi.CLK_HS_PRPR = 4;  //2;  //min 38ns; max 95ns
  #endif
//>>EricHsieh,2013/1/21, improve the LCD tearing issue
//>>EricHsieh,2012/1/8,For read LCD ID will cause system reset

  /**************************************************************************
  ** ESD or noise interference recovery For video mode LCM only.
  ***************************************************************************/
#if !defined( LCM_DSI_CMD_MODE )
  /* Send TE packet to LCM in a period of n frames and check the response. */
    params->dsi.lcm_int_te_monitor = FALSE;
    params->dsi.lcm_int_te_period = 1;  /* Unit : frames */
  /* Need longer FP for more opportunity to do int. TE monitor applicably. */
    if( params->dsi.lcm_int_te_monitor )
      params->dsi.vertical_frontporch *= 2;

  /* Monitor external TE (or named VSYNC) from LCM once per 2 sec.
  ** (LCM VSYNC must be wired to baseband TE pin.) */
    params->dsi.lcm_ext_te_monitor = FALSE;

  /* Non-continuous clock */
    params->dsi.noncont_clock = FALSE;
    params->dsi.noncont_clock_period = 2; /* Unit : frames */
#endif

} /* End.. lcm_get_params() */

/***********************************************************************
** Get LCM Driver Hooks
************************************************************************/
LCM_DRIVER nt35310_dsi_lcm_drv =
{
    .name           = "nt35310_dsi",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
  #if defined( LCM_DSI_CMD_MODE )
    .set_backlight  = lcm_set_backlight,
    .update         = lcm_update,
  //.set_pwm        = NULL, //lcm_setpwm,
  //.get_pwm        = NULL, //lcm_getpwm,
  #endif /* End.. (LCM_DSI_CMD_MODE) */
};

/*****************************************************************************
** End
******************************************************************************/
#undef _NT35310_DSI_C_
#endif /** End.. !(_NT35310_DSI_C_) **/

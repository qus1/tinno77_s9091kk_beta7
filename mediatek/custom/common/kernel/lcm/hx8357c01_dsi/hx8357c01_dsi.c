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
#ifndef _HX8357C01_DSI_C_
#define _HX8357C01_DSI_C_
/*****************************************************************************
** LCM model
**  Himax HX8357-C01/C02 320RGB x 480 dot, 16M color, with internal GRAM,
**  TFT Mobile Single Chip Driver.
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
  #if defined( LCM_DSI_CMD_MODE )
    #undef    LCM_DSI_CMD_MODE
  #endif
    #define   LCM_DSI_CMD_MODE
  //#define   LCM_DBI_USAGE
  //#define   LCM_DPI_USAGE

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

//<2012/11/26-Yuting Shih-17682. Masked for color format.
  //#define   LCM_PIXEL_24B_888
  //#define   LCM_PIXEL_RGB
//<2012/11/26-Yuting Shih-17682.

    #define   LCM_MEM_MY                0 //(0x1<<7)  /* 1: Decrease in vertical, 0: Increase in vertical */
    #define   LCM_MEM_MX                0 //(0x1<<6)  /* 1: Decrease in horizon, 0: Increase in horizon */
    #define   LCM_MEM_MV                0 //(0x1<<5)  /* */
    #define   LCM_MEM_ML                0 //(0x1<<4)
  #if defined( LCM_PIXEL_RGB )
    #define   LCM_MEM_RGB               0         /* RGB:RGB color filter panel */
  #else
    #define   LCM_MEM_RGB               (0x1<<3)  /* RGB:BGR color filter panel */
  #endif
    #define   LCM_MEM_MH                0 //(0x1<<2)

    #define   LCM_MEM_OPERATE           (LCM_MEM_MY|LCM_MEM_MX|LCM_MEM_MV|LCM_MEM_ML|LCM_MEM_MH)
    #define   LCM_MEMORY_ACCESS         (LCM_MEM_OPERATE|LCM_MEM_RGB)

  //<2012/11/14-Yuting shih-[BU2SC00139391].Enable tearing function.
  #if defined( LCM_TE_VSYNC_MODE )
    #undef    LCM_TE_VSYNC_MODE
  #endif
    #define   LCM_TE_VSYNC_MODE
  //>2012/11/14-Yuting shih-[BU2SC00139391].

  /*============================================================
  **
  **============================================================*/
    #define   LCM_HSYNC_NUM             4   /** Shall be larger than 3 **/
    #define   LCM_HBP_NUM               48  // 0x28
    #define   LCM_HFP_NUM               48  // 0x32
    #define   LCM_VSYNC_NUM             4   /** Shall be larger than 3 **/
    #define   LCM_VBP_NUM               6
    #define   LCM_VFP_NUM               4
    #define   LCM_LINE_BYTE             ((FRAME_WIDTH+LCM_HSYNC_NUM+LCM_HBP_NUM+LCM_HFP_NUM)*3)

  /*======================================================================
  ** RDID1(D0h): Read ID1
  **----------------------------------------------------------------------
  ** 1st parameter: Dummy byte.
  ** 2nd parameter: Read HX8357-C ID = 0x90.
  **======================================================================*/
    #define   LCM_IC_ID0                (0x03)  //(0xBF)
    #define   LCM_IC_ID1                (0x90)  //(0xCD)

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
  /* SETEXTC: Enable external Command */
    { 0xB9, 3, { 0xFF, 0x83, 0x57 }}, /* Enable SETEXTC command */
    { REGFLAG_DELAY, 10, {}},

#if defined( FACTORY_INIT_CODE )
  /* SETPanel: Set panel characteristic */
    { 0xCC, 1, { 0x0A }}, /* Panel V inveserse */
  /* SETRGB: set RGB interface */
  #if defined( LCM_DSI_CMD_MODE )
    { 0xB3, 4, { 0x40, 0x00, 0x06, 0x06 }},
  #else
    { 0xB3, 4, { 0x43, 0x00, 0x06, 0x06 }},
  #endif
  /* Interface pixel format */
  #if defined( LCM_PIXEL_24B_888 )
    { 0x3A, 1, { 0x77 }}, /* Set pixel format, 24BPP */
  #else
    { 0x3A, 1, { 0x66 }}, /* Set pixel format, 18BPP */
  #endif
  /* SETCOM: set VCOM voltage related register */
    //<2013/2/28-22340-Cokychen,  [8317][DRV]Fix truly lcm hx8357c01_dsi background flicker and gamma issue
    //{ 0xB6, 1, { 0x3C }},
    { 0xB6, 1, { 0x55 }},
    //>2013/2/28-22340-Cokychen
  /* SETOSC: Set internal oscillator */
  //{ 0xB0, 1, { 0x66 }}, /* 100% x 10MHz, 60Hz */
  //{ REGFLAG_DELAY, 100, {}},
  /* SETPOWER: Set power control */
    { 0xB1, 6, { 0x00, 0x15, 0x1C, 0x1C, 0x83, 0xAA }}, /* */
  /* SETSTBA: Set source circuit option */
    { 0xC0, 6, { 0x33, 0x50, 0x01, 0x3C, 0xC8, 0x08 }},
  /* SETCYC: Set display cycle register */
    //<2013/2/28-22340-Cokychen,  [8317][DRV]Fix truly lcm hx8357c01_dsi background flicker and gamma issue
    //{ 0xB4, 7, { 0x02, 0x40, 0x00, 0x2A, 0x2A, 0x1D, 0x46 }},
    { 0xB4, 7, { 0x02, 0x40, 0x00, 0x2A, 0x2A, 0x1D, 0x60 }},
    //>2013/2/28-22340-Cokychen
  /* */
  #if defined( LCM_DSI_CMD_MODE )
    { 0xBA,16, { 0x00, 0x56, 0xD4, 0x00, 0x0A, 0x00, 0x10, 0x32, 0x6E, 0x04,
                 0x05, 0x9A, 0x14, 0x19, 0x10, 0x40 }}, /* SET MIPI */
  #else
    { 0xBA,16, { 0x00, 0x56, 0xD4, 0x00, 0x0A, 0x00, 0x10, 0x32, 0x6E, 0x04,
                 0x05, 0x9A, 0x14, 0x19, 0x10, 0x40 }}, /* SET MIPI */
  #endif
  /* Memory access control */
    { 0x36, 1, { LCM_MEMORY_ACCESS }},
  /* */
  //{ 0xC6, 2, { 0x00, 0xF8 }},
  /* */
  //{ 0xE3, 2, { 0x27, 0x27 }},
  /* SETGamma: Set gamma curve. Must be SLEEP IN mode when use MIPI DSI */
  //<2013/2/20-21961-Cokychen, [8317][DRV] Change truly LCM hx8357c01_dsi gamma setting
  //<2013/2/28-22340-Cokychen,  [8317][DRV]Fix truly lcm hx8357c01_dsi background flicker and gamma issue
    { 0xE0, 67, { 0x00, 0x00,
                  0x03, 0x00, 0x0F, 0x00, 0x14, 0x00, 0x1C, 0x00, 0x21, 0x00,
                  0x31, 0x00, 0x3C, 0x00, 0x47, 0x00, 0x50, 0x00, 0x46, 0x00,
                  0x3E, 0x00, 0x2A, 0x00, 0x1E, 0x00, 0x0A, 0x00, 0x0B, 0x00,
                  0x05, 0x01/*0x00*/, /* VRP15, GMA_RELOAD=1 */
                  0x03, 0x00, 0x0F, 0x00, 0x14, 0x00, 0x1C, 0x00, 0x21, 0x00,
                  0x31, 0x00, 0x3C, 0x00, 0x47, 0x00, 0x50, 0x00, 0x46, 0x00,
                  0x3E, 0x00, 0x2A, 0x00, 0x1E, 0x00, 0x0A, 0x00, 0x0B, 0x00,
                  0x05, 0x00, 0x00 /* VRN15, 00, CGP/CGPN */ }},	
  //>2013/2/28-22340-Cokychen
  //>2013/2/20-21961-Cokychen
  /* Tearing effect line on: TE on */
    { 0x35, 1, { 0x00 }},
#else
  /* SETPanel: Set panel characteristic */
  //{ 0xCC, 1, { 0x05 }}, /* Panel V inveserse */
    { 0xCC, 1, { 0x0A }},
  /* SETRGB: set RGB interface */
  #if defined( LCM_DSI_CMD_MODE )
    { 0xB3, 4, { 0x40, 0x00, 0x06, 0x06 }},
  #else
    { 0xB3, 4, { 0x43, 0x00, 0x06, 0x06 }},
  #endif
  /* Interface pixel format */
  #if defined( LCM_PIXEL_24B_888 )
    { 0x3A, 1, { 0x77 }}, /* Set pixel format, 24BPP */
  #else
    { 0x3A, 1, { 0x66 }}, /* Set pixel format, 18BPP */
  #endif
  /* SETCOM: set VCOM voltage related register */
  //{ 0xB6, 1, { 0x53 }},
    { 0xB6, 1, { 0x3C }},
  /* SETOSC: Set internal oscillator */
    { 0xB0, 1, { 0x66 }}, /* 100% x 10MHz, 60Hz */
    { REGFLAG_DELAY, 100, {}},
  /* SETPOWER: Set power control */
  //{ 0xB1, 6, { 0x00, 0x15, 0x1C, 0x1C, 0x83, 0x44 }},
    { 0xB1, 6, { 0x00, 0x15, 0x1C, 0x1C, 0x83, 0xAA }},
  /* SETSTBA: Set source circuit option */
  //{ 0xC0, 6, { 0x50, 0x50, 0x01, 0x3C, 0xC8, 0x08 }}, /* Panel Driving Setting */
    { 0xC0, 6, { 0x33, 0x50, 0x01, 0x3C, 0xC8, 0x08 }},
  /* SETCYC: Set display cycle register */
  //{ 0xB4, 7, { 0x02, 0x40, 0x00, 0x2A, 0x2A, 0x0D, 0x78 }},
    { 0xB4, 7, { 0x02, 0x40, 0x00, 0x2A, 0x2A, 0x1D, 0x46 }},
  /* */
  #if defined( LCM_DSI_CMD_MODE )
    { 0xBA,16, { 0x00, 0x56, 0xD4, 0x00, 0x0A, 0x00, 0x10, 0x32, 0x6E, 0x04,
                 0x05, 0x9A, 0x14, 0x19, 0x10, 0x40 }}, /* SET MIPI */
  #else
    { 0xBA,16, { 0x00, 0x56, 0xD4, 0x00, 0x0A, 0x00, 0x10, 0x32, 0x6E, 0x04,
                 0x05, 0x9A, 0x14, 0x19, 0x10, 0x40 }}, /* SET MIPI */
  #endif
  /* Memory access control */
    { 0x36, 1, { LCM_MEMORY_ACCESS }},
  /* */
  //{ 0xC6, 2, { 0x00, 0xF8 }},
  /* */
  //{ 0xE3, 2, { 0x27, 0x27 }},
  /* SETGamma: Set gamma curve. Must be SLEEP IN mode when use MIPI DSI */
  #if 0
    { 0xE0, 67, { 0x00, 0x00,
                  0x30, 0x00, 0x37, 0x00, 0x3B, 0x00, 0x3F, 0x00, 0x44, 0x00,
                  0x52, 0x00, 0x59, 0x00, 0x60, 0x00, 0x3A, 0x00, 0x35, 0x00,
                  0x2F, 0x00, 0x29, 0x00, 0x22, 0x00, 0x1C, 0x00, 0x19, 0x00,
                  0x03, 0x00, /* VRP15, GMA_RELOAD=0 */
                  0x30, 0x00, 0x37, 0x00, 0x3B, 0x00, 0x3F, 0x00, 0x44, 0x00,
                  0x52, 0x00, 0x59, 0x00, 0x60, 0x00, 0x3A, 0x00, 0x35, 0x00,
                  0x2F, 0x00, 0x29, 0x00, 0x22, 0x00, 0x1C, 0x00, 0x19, 0x00,
                  0x03, 0x00, 0x01 /* VRN15, 00, CGP/CGPN */ }},
    { REGFLAG_DELAY, 20, {}},
  #else
    { 0xE0, 67, { 0x00, 0x00,
                  0x02, 0x00, 0x0A, 0x00, 0x10, 0x00, 0x18, 0x00, 0x1F, 0x00,
                  0x32, 0x00, 0x3E, 0x00, 0x48, 0x00, 0x4F, 0x00, 0x43, 0x00,
                  0x3E, 0x00, 0x27, 0x00, 0x1B, 0x00, 0x08, 0x00, 0x09, 0x00,
                  0x03, 0x01/*0x00*/, /* VRP15, GMA_RELOAD=1 */
                  0x02, 0x00, 0x0A, 0x00, 0x10, 0x00, 0x18, 0x00, 0x1F, 0x00,
                  0x32, 0x00, 0x3E, 0x00, 0x48, 0x00, 0x4F, 0x00, 0x43, 0x00,
                  0x3E, 0x00, 0x27, 0x00, 0x1B, 0x00, 0x08, 0x00, 0x09, 0x00,
                  0x03, 0x00, 0x01 /* VRN15, 00, CGP/CGPN */ }},
  #endif
  /* Tearing effect line on: TE on */
    { 0x35, 1, { 0x00 }},
  /* Display inversion on */
  //{ 0x21, 0, { 0x00 }}, /* Enter inversion mode */
  /* SETEXTC: Enable external Command */
  //{ 0xB9, 3, { 0xFF, 0x83, 0x47 }}, /* Disable SETEXTC command */
  //{ REGFLAG_DELAY, 5, {}},
#endif /* End.. (FACTORY_INIT_CODE) */

//<2012/11/06-Yuting Shih-[BU2SC00138012], Adjust the timing of resume.
#if defined( BUILD_LK ) || defined( BUILD_UBOOT )
  /* Sleep Mode Off: Sleep out */
    { 0x11, 0, { 0x00 }},
    { REGFLAG_DELAY, 125, {}},
  /* Display On */
    { 0x29,  0, {}},
    { REGFLAG_DELAY, 20, {}},
#endif
//>2012/11/06-Yuting Shih-[BU2SC00138012].
    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

static LCM_SET_TAB  lcm_suspend_set[] =
{
//<2012/11/06-Yuting Shih-[BU2SC00138012], Adjust the timing of resume.
  /* Display off sequence */
    { 0x28, 0, { 0x00 }},
    { REGFLAG_DELAY, 20, {}},
  /* Sleep Mode On */
    { 0x10, 0, { 0x00 }},
    { REGFLAG_DELAY, 125, {}},

//>2012/11/06-Yuting Shih-[BU2SC00138012].
    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

static LCM_SET_TAB  lcm_resume_set[] =
{
//<2012/11/06-Yuting Shih-[BU2SC00138012], Adjust the timing of resume.
  /* Sleep Mode Off */
    { 0x11, 0, { 0x00 }},
    { REGFLAG_DELAY, 125, {}},
  /* Display On */
    { 0x29,  0, {}},
    { REGFLAG_DELAY, 20, {}},
//>2012/11/06-Yuting Shih-[BU2SC00138012].
    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

#if defined( LCM_DSI_CMD_MODE )
static LCM_SET_TAB  lcm_backlight_set[] =
{
  /* Write display brightness value */
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

#if 0
/***********************************************************************
** init_lcm_gamma
**======================================================================
** cmd type 2 only support 16 * 4 bytes.
**  - Header: 4 bytes.
**  - DCS   : 1 byte.
**  - Paremeter: 59 bytes(max).
************************************************************************/
static void init_lcm_gamma( void )
{
unsigned int data_array[18];  // Max: 16

  data_array[0]  = 0x00443902;
  data_array[1]  = 0x020000E0;
  data_array[2]  = 0x10000A00;
  data_array[3]  = 0x1F001800;
  data_array[4]  = 0x3E003200;
  data_array[5]  = 0x4F004800;
  data_array[6]  = 0x3E004300;
  data_array[7]  = 0x1B002700;
  data_array[8]  = 0x09000800;
  data_array[9]  = 0x02010300;
  data_array[10] = 0x10000A00;
  data_array[11] = 0x1F001800;
  data_array[12] = 0x3E003200;
  data_array[13] = 0x4F004800;
  data_array[14] = 0x3E004300;
  data_array[15] = 0x1B002700;
  data_array[16] = 0x09000800;
  data_array[17] = 0x01000300;

  dsi_set_cmdq( data_array, 18, 1 );  
} /* End.. init_lcm_gamma() */
#endif

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
  LCM_MSG( "[HX8357C01] %s...\n", __func__ );
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
  MDELAY( 10 );   // 30ms
  SET_RESET_PIN( 0 );
  MDELAY( 10 );   // 100ms
  SET_RESET_PIN( 1 );
  MDELAY( 30 );   // 120ms
#else
  SET_RESET_PIN( 1 );
  MDELAY( 5 );   // 30ms
  SET_RESET_PIN( 0 );
  MDELAY( 10 );  // 100ms
  SET_RESET_PIN( 1 );
  MDELAY( 30 );  // 120ms
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
  LCM_MSG( "[HX8357C01] %s...\n", __func__ );
#endif
  push_table((LCM_SET_TAB*)&lcm_suspend_set, sizeof( lcm_suspend_set )/sizeof( LCM_SET_TAB ), 1 );
} /* End.. lcm_suspend() */

/***********************************************************************
** lcm_resume
************************************************************************/
static void lcm_resume(void)
{
#if defined( LCM_MSG_ENABLE )
  LCM_MSG( "[HX8357C01] %s...\n", __func__ );
#endif
//<2012/11/06-Yuting Shih-[BU2SC00138012], Adjust the timing of resume.
#if defined( BUILD_LK ) || defined( BUILD_UBOOT )
    lcm_init();
#else
  #if 0
    lcm_init();
  #endif
    push_table((LCM_SET_TAB*)&lcm_resume_set, sizeof( lcm_resume_set )/sizeof( LCM_SET_TAB ), 1 );
#endif
//<2012/11/06-Yuting Shih-[BU2SC00138012].
} /* End.. lcm_resume() */

//**********************************************************************
//* lcm_compare_id
//**********************************************************************
static unsigned int lcm_compare_id(void)
{
unsigned int  data_array[16] = { 0 };
unsigned int  id[4] = { 0 }, vRet = 0;
unsigned char buffer[4] = { 0 };

#if defined( LCM_MSG_ENABLE )
  LCM_MSG( "[HX8357C01] %s...\n", __func__ );
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
  MDELAY( 10 );    // 30ms
  SET_RESET_PIN( 0 );
  MDELAY( 10 );   // 100ms
  SET_RESET_PIN( 1 );
  MDELAY( 30 );   // 120ms

/** Sleep out **/
#if 0
  data_array[0] = 0x00110500;
  dsi_set_cmdq( &data_array, 1, 1 );
  MDELAY( 120 );
#endif

/* SETEXTC: Enable external Command */
  data_array[0]=0x00043902;
  data_array[1]=0x5783FFB9;
  dsi_set_cmdq( &data_array, 2, 1 );
  MDELAY( 10 );

/* Set Maximum Return Packet Size: Read ID return 2 bytes */
  data_array[0] = 0x00043700;
  dsi_set_cmdq( &data_array, 1, 1 );
  MDELAY( 10 );

/***************************************************************
** GETICID: IC ID Read Command Data (D0h)
****************************************************************/
  read_reg_v2( 0xD0, (unsigned char*)&buffer[0], 2 );
  MDELAY( 10 );
  id[0] = buffer[0];  /** Dummy byte **/
  id[1] = buffer[1];

#if 1 //defined( LCM_MSG_ENABLE )
  LCM_MSG( "[HX8357C01] %s -- IC ID is 0x%02X, 0x%02X.\n", __func__, id[0], id[1] );
#endif

  if( LCM_IC_ID1 == id[1] )
  {
  #if defined( LCM_MSG_ENABLE )
    LCM_MSG( "[HX8357C01] %s -- Check IC ID OK.\n", __func__ );
  #endif
    vRet = 1;
  }
  else
  {
    if( LCM_SECOND_SOURCE == mt_get_gpio_in( GPIO_LCM_ID_PIN ))
    {
    #if defined( LCM_MSG_ENABLE )
      LCM_MSG( "[HX8357C01] %s -- LCM_ID GPIO OK.\n", __func__ );
    #endif
      vRet = 1;
    }
  }

  return  vRet;
} /* End.. lcm_compare_id() */

/***********************************************************************
** lcm_get_params
************************************************************************/
static void lcm_get_params(LCM_PARAMS *params)
{
    memset( params, 0x00, sizeof( LCM_PARAMS ));

    params->type    = LCM_TYPE_DSI;
  //params->ctrl    = LCM_CTRL_PARALLEL_DBI;  //LCM_CTRL_NONE;
    params->width   = FRAME_WIDTH;
    params->height  = FRAME_HEIGHT;

  /*====================================================
  ** DBI
  **====================================================*/
#if defined( LCM_DBI_USAGE )
  //params->dbi.port                = 0;
    params->dbi.clock_freq          = LCM_DBI_CLOCK_FREQ_104M;
    params->dbi.data_width          = LCM_DBI_DATA_WIDTH_16BITS;
  #if 1 //defined( LCM_PIXEL_RGB )
    params->dbi.data_format.color_order = LCM_COLOR_ORDER_RGB;
  #else
    params->dbi.data_format.color_order = LCM_COLOR_ORDER_BGR;
  #endif
    params->dbi.data_format.trans_seq   = LCM_DBI_TRANS_SEQ_MSB_FIRST;
    params->dbi.data_format.padding = LCM_DBI_PADDING_ON_LSB;
  #if defined( LCM_PIXEL_24B_888 )
    params->dbi.data_format.format  = LCM_DBI_FORMAT_RGB888;
    params->dbi.data_format.width   = LCM_DBI_DATA_WIDTH_24BITS;
  #else
    params->dbi.data_format.format  = LCM_DBI_FORMAT_RGB666;
    params->dbi.data_format.width   = LCM_DBI_DATA_WIDTH_18BITS
  #endif
    params->dbi.cpu_write_bits      = LCM_DBI_CPU_WRITE_16_BITS;
    params->dbi.io_driving_current  = 0;
#endif /* End.. (LCM_DBI_USAGE) */

  /* Enable tearing-free */
#if defined( LCM_TE_VSYNC_MODE )
    params->dbi.te_mode             = LCM_DBI_TE_MODE_VSYNC_ONLY;
#else
    params->dbi.te_mode             = LCM_DBI_TE_MODE_DISABLED;
#endif
    params->dbi.te_edge_polarity    = LCM_POLARITY_RISING;

  /*====================================================
  ** DPI
  **====================================================*/
#if defined( LCM_DPI_USAGE )
  #if defined( LCM_PIXEL_24B_888 )
    params->dpi.format      = LCM_DPI_FORMAT_RGB888;
  #else
    params->dpi.format      = LCM_DPI_FORMAT_RGB666;
  #endif
  #if 1 //defined( LCM_PIXEL_RGB )
    params->dpi.rgb_order   = LCM_COLOR_ORDER_RGB;
  #else
    params->dpi.rgb_order   = LCM_COLOR_ORDER_BGR;
  #endif
    params->dpi.intermediat_buffer_num  = 2;
#endif /* End.. (LCM_DPI_USAGE) */

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
    params->dsi.PS                      = LCM_PACKED_PS_24BIT_RGB888;
#else
    params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB666;
    params->dsi.PS                      = LCM_PACKED_PS_18BIT_RGB666;
#endif

  /* Highly depends on LCD driver capability. */
    params->dsi.packet_size = 256;

  /* Video mode setting */
    params->dsi.intermediat_buffer_num  = 2;

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
    params->dsi.pll_div1 = 24;
    params->dsi.pll_div2 = 1;
  #else
    params->dsi.pll_div1 = 26;
    params->dsi.pll_div2 = 1;
  #endif
#else
  /*=======================================================
  ** Min rate: 320*(480+32)*18*60=176.9472Mbps
  ** Min: 182.0 Mbps
  **=======================================================*/
  #if defined( LCM_DSI_CMD_MODE )
    params->dsi.pll_div1 = 24;
    params->dsi.pll_div2 = 1;
  #else
    params->dsi.pll_div1 = 26;
    params->dsi.pll_div2 = 1;
  #endif
#endif

  /* DSI frame interval time */
    params->dsi.HS_TRAIL  = 15; //min max(n*8*UI, 60ns+n*4UI)
    params->dsi.HS_ZERO   = 6;  //min 105ns+6*UI
    params->dsi.HS_PRPR   = 4;  //min 40ns+4*UI; max 85ns+6UI
    params->dsi.LPX       = 12; //min 50ns

    params->dsi.TA_SACK   = 1;
    params->dsi.TA_GET    = 60;
    params->dsi.TA_SURE   = 18;
    params->dsi.TA_GO     = 48; //4*LPX

    params->dsi.CLK_TRAIL = 5;  //min 60ns
    params->dsi.CLK_ZERO  = 15; //min 300ns-38ns
    params->dsi.LPX_WAIT  = 10;
    params->dsi.CONT_DET  = 0;

    params->dsi.CLK_HS_PRPR = 3;  //min 38ns; max 95ns

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
LCM_DRIVER hx8357c01_dsi_lcm_drv =
{
    .name           = "hx8357c01_dsi",
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
#undef _HX8357C01_DSI_C_
#endif /** End.. !(_HX8357C01_DSI_C_) **/

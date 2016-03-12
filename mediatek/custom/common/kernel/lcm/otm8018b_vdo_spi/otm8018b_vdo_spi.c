/*****************************************************************************
** TRULY LCD module TFT3P2514-M-E
**============================================================================
** LCD driver IC: Orise Tech. OTM8018B
**----------------------------------------------------------------------------
** 1440-channel 8-bit source driver and 864 gate driver with system-on-chip
** for color amorphous TFT-LCDs.
*****************************************************************************/
#ifndef _OTM8018_DSI_VDO_C_
#define _OTM8018_DSI_VDO_C_
/*****************************************************************************
** Include head files
******************************************************************************/
#if defined( BUILD_LK )
    #include  <platform/mt_typedefs.h>
    #include  <platform/mt_gpio.h>
#else
#if defined( BUILD_UBOOT )
  //#include  <asm/arch/mt65xx.h>
    #include  <asm/arch/mt65xx_typedefs.h>
    #include  <asm/arch/mt6577_gpio.h>
#else
    #include  <linux/kernel.h>
    #include  <mach/mt_typedefs.h>
    #include  <mach/mt_gpio.h>
  //#include  <mach/mt_pm_ldo.h>
#endif
    #include  <linux/string.h>
#endif /** End.. (BUILD_LK) **/
    #include  "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
    #define   FRAME_WIDTH             (480)
    #define   FRAME_HEIGHT            (854)

    #define   REGFLAG_DELAY           0xFD  //0xFE
    #define   REGFLAG_END_OF_TABLE    0xFE  //0x00  /* END OF REGISTERS MARKER */

  #if defined( LCM_DSI_CMD_MODE )
    #undef    LCM_DSI_CMD_MODE
  #endif
  //#define   LCM_DSI_CMD_MODE        0

//<2012/09/19-14141-stevenchen, [Hawk4.0] Implement LCM auto detection.
    #define   LCM_ID1_OTM8018B         0x40
    #define   LCM_ID2_OTM8018B         0x00
    #define   LCM_ID3_OTM8018B         0x00
//>2012/09/19-14141-stevenchen

  #if 1
  /* Source of MTK */
    #define   LCM_HSYNC_NUM           6
    #define   LCM_HBP_NUM             60
    #define   LCM_HFP_NUM             60

    #define   LCM_VSYNC_NUM           4
    #define   LCM_VBP_NUM             8
    #define   LCM_VFP_NUM             8
  #else
    #define   LCM_HSYNC_NUM           4    /** Shall be larger than 3 **/
    #define   LCM_HBP_NUM             44
    #define   LCM_HFP_NUM             46

    #define   LCM_VSYNC_NUM           1   /** Shall be larger than 3 ? **/
    #define   LCM_VBP_NUM             16
    #define   LCM_VFP_NUM             15
  #endif

    #define   LCM_LINE_BYTE           ((FRAME_WIDTH+LCM_HSYNC_NUM+LCM_HBP_NUM+LCM_HFP_NUM)*3)  //2048
    #define   LCM_OSC_FREQ            28  //30

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------
    static    LCM_UTIL_FUNCS          lcm_util = { 0 };

  #if 0 //!defined( BUILD_LK ) && !defined( BUILD_UBOOT )
    static    BOOL                    lcm_run_first  = TRUE;
  #endif
// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
    #define   SET_RESET_PIN(v)        (lcm_util.set_reset_pin((v)))

    #define   UDELAY(n)               (lcm_util.udelay(n))
    #define   MDELAY(n)               (lcm_util.mdelay(n))

    #define   dsi_set_cmdq_V2(cmd, count, ppara, force_update)  \
                                      lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
    #define   dsi_set_cmdq(pdata, queue_size, force_update) \
                                      lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
    #define   wrtie_cmd(cmd)          lcm_util.dsi_write_cmd(cmd)
    #define   write_regs(addr, pdata, byte_nums)  \
                                      lcm_util.dsi_write_regs(addr, pdata, byte_nums)
    #define   read_reg                lcm_util.dsi_read_reg()
    #define   read_reg_v2(cmd, buffer, buffer_size) \
                                      lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)


    typedef
    struct LCM_setting_table
    {
        unsigned        cmd;
        unsigned char   count;
        unsigned char   para_list[64];
    } LCM_SET_TAB;


/*****************************************************************************
** Note :
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
static LCM_SET_TAB lcm_initialization_setting[] =
{
/***** MIPI IF Enable access command 2 register. *****/
    { 0xFF, 3, { 0x80, 0x09, 0x01 }}, /* Enable EXTC (CMD2 Enable) */
  /** ORISE CMD Enable **/
    { 0x00, 1, { 0x80 }},       /* Shift address */
    { 0xFF, 2, { 0x80, 0x09 }}, /* Enable Orise mode */

  /* CMD 0xD8, Parameter 0x00: 0xD800 */
    { 0x00, 1, { 0x00 }},
    { 0xD8, 1, { 0x87 }}, /* GVDD 4.8V */

    { 0x00, 1, { 0x01 }},
    { 0xD8, 1, { 0x87 }}, /* NGVDD -4.8V */

    { 0x00, 1, { 0xB1 }},
    { 0xC5, 1, { 0xA9 }}, /* [0]GVDD output Enable : 0xA9,VDD_18V = LVDSVDD = 1.8V */

    { 0x00, 1, { 0x91 }},
    { 0xC5, 1, { 0x79 }}, /* [7:4]VGH_level = 15V,[3:0]VGL_level = -12V */

    { 0x00, 1, { 0x00 }},
    { 0xD9, 1, { 0x45 }}, /* VCOM = -1.15V */

    { 0x00, 1, { 0x92 }},
    { 0xC5, 1, { 0x01 }}, /* pump45 */

  /* ? */
    { 0x00, 1, { 0xA1 }},
    { 0xC1, 1, { 0x08 }}, /* reg_oscref_rgb_vs_video */

  /* OSC frequency in Normal mode */
    { 0x00, 1, { 0x81 }},
    { 0xC1, 1, { 0x66 }}, /* OSC Adj = 65Hz, 31.15MHz */
  //{ 0xC1, 1, { 0x55 }}, /* OSC Adj = 60Hz, 28.75MHz */

  //<2012/06/08 Yuting Shih, VFP and VBP setting
    //{ 0x00, 1, { 0x83 }},
    //{ 0xC0, 2, { LCM_VFP_NUM, LCM_VBP_NUM }}, /* VFP, VBP */

    //{ 0x00, 1, { 0x87 }},
    //{ 0xC0, 2, { LCM_VFP_NUM, LCM_VBP_NUM }}, /* Powersq_vfp, Powersq_vbp */
  //>2012/06/08 Yuting Shih, VFP and VBP setting

  //<2012/06/08 Yuting Shih, HSYNC setting
  /* source driver pull low region(pl_width): 12+1 mclk */
    //{ 0x00, 1, { 0xA2 }},
    //{ 0xC0, 1, { LCM_HSYNC_NUM - 1 }},
  //>2012/06/08 Yuting Shih, HSYNC setting

  /* Pre-charge region: pcg_dri 28(27+1) mclk */
    { 0x00, 1, { 0xA3 }},
    { 0xC0, 1, { 0x1B }}, /* source pch */

  /* Power Control Setting 1:
  ** pump1_ss_width: 29 lines, pump1_clamp: 6.00V, en_c24: Enable, en_c32: Enable */
    { 0x00, 1, { 0x82 }},
    { 0xC5, 1, { 0x83 }}, /* REG-Pump23 AVEE VCL */

    { 0x00, 1, { 0x81 }},
    { 0xC4, 1, { 0x83 }}, /* source bias */

    { 0x00, 1, { 0xA0 }},
    { 0xC1, 1, { 0xEA }}, /* Follow External CLK */

    { 0x00, 1, { 0x90 }},
    { 0xB3, 1, { 0x02 }}, /* SW_GM 480X854 */

    { 0x00, 1, { 0x92 }},
    { 0xB3, 1, { 0x45 }}, /* Enable SW_GM */

///////////////////////////////////////////////////////////////////////////////
  /* ? */
    { 0x00, 1, { 0xA7 }},
    { 0xB3, 1, { 0x01 }}, /* panel_set[0] = 1 */

  /* Pseudo-Dot inversion Driving Setting: */
    { 0x00, 1, { 0xA6 }},
    { 0xB3, 1, { 0x2B }}, /* reg_panel_zinv, reg_panel_zinv_pixel, reg_panel_zinv_odd,
                          ** reg_panel_zigzag, reg_panel_zigzag_blue, reg_panel_zigzag_shift_r,
                          ** reg_panel_zigzag_odd */

  /* 0xC09x : mck_shift1/mck_shift2/mck_shift3 */
    { 0x00, 1, { 0x90 }},
    { 0xC0, 6, { 0x00, 0x4E, 0x00, 0x00, 0x00, 0x03 }},

//<2012/07/10 Yuting Shih, Modified byte0 from 0x00 to 0x01 for the screen image shaking sometimes.
  /* 0xC0Ax : hs_shift/vs_shift */
    { 0x00, 1, { 0xA6 }},
    { 0xC1, 3, { 0x01, 0x00, 0x00 }}, /* 16'hc1a7 [7:0] : oscref_vedio_hs_shift[7:0] */
//>2012/07/10 Yuting Shih, Modified for the screen image shaking sometimes.

  #if 0
  /**** Gamma2.2 +/- ****/
    { 0x00, 1, { 0x00 }},
    { 0xE1,16, { 0x02, 0x0A, 0x0F, 0x0F, 0x08, 0x0C, 0x0C, 0x0C,
                 0x01, 0x05, 0x1A, 0x15, 0x1E, 0x1D, 0x0E, 0x05 }},
              /* V255  V251  V247  V239  V231  V203  V175  V147
              ** V108  V80   V52   V24   V16   V8    V4    V0 */
    { 0x00, 1, { 0x00)},
    { 0xE2,16, { 0x02, 0x0A, 0x0F, 0x0F, 0x08, 0x0C, 0x0C, 0x0C,
                 0x01, 0x05, 0x1A, 0x15, 0x1E, 0x1D, 0x0E, 0x05 }},
              /* V255  V251  V247  V239  V231  V203  V175  V147
              ** V108  V80   V52   V24   V16   V8    V4    V0 */
  #else
  //<2012/07/10 Yuting Shih, Add delay for voltage stability
  /**** Gamma2.2 +/- ****/
    { 0x00, 1, { 0x00 }},
    { 0xE1,16, { 0x05, 0x0B, 0x0F, 0x0F, 0x08, 0x0D, 0x0C, 0x0B,
                 0x02, 0x06, 0x16, 0x12, 0x18, 0x24, 0x17, 0x00 }},
              /* V255  V251  V247  V239  V231  V203  V175  V147
              ** V108  V80   V52   V24   V16   V8    V4    V0 */
    { REGFLAG_DELAY, 3, {}},

    { 0x00, 1, { 0x00 }},
    { 0xE2,16, { 0x05, 0x0B, 0x0F, 0x0F, 0x08, 0x0D, 0x0C, 0x0B,
                 0x02, 0x06, 0x16, 0x12, 0x18, 0x24, 0x17, 0x00 }},
              /* V255  V251  V247  V239  V231  V203  V175  V147
              ** V108  V80   V52   V24   V16   V8    V4    V0 */
    { REGFLAG_DELAY, 3, {}},
  //>2012/07/10 Yuting Shih, Add delay for voltage stability
  #endif
//--------------------------------------------------------------------------------
// initial setting 2 < tcon_goa_wave >
//--------------------------------------------------------------------------------
    { 0x00, 1, { 0x91 }}, /* zigzag reverse scan */
    { 0xB3, 1, { 0x00 }},
  /* u2d GOA Setting */
  /* 0xCE8x : vst1, vst2, vst3, vst4 */
    { 0x00, 1, { 0x80 }},
    { 0xCE,12, { 0x85, 0x01, 0x18, 0x84, 0x01, 0x18, 0x00, 0x00,
                 0x00, 0x00, 0x00, 0x00 }},

  /* 0xCE9x : vend1, vend2, vend3, vend4 */
    { 0x00, 1, { 0x90 }},
    { 0xCE,14, { 0x13, 0x56, 0x18, 0x13, 0x57, 0x18, 0x00, 0x00,
                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }},

  /* 0xCEAx : clka1, clka2 */
    { 0x00, 1, { 0xA0 }},
    { 0xCE,14, { 0x18, 0x0B, 0x03, 0x5E, 0x00, 0x18, 0x00, 0x18,
                 0x0A, 0x03, 0x5F, 0x00, 0x18, 0x00 }},

  /* 0xCEBx : clka3, clka4 */
    { 0x00, 1, { 0xB0 }},
    { 0xCE,14, { 0x18, 0x0D, 0x03, 0x5C, 0x00, 0x18, 0x00, 0x18,
                 0x0C, 0x03, 0x5D, 0x00, 0x18, 0x00 }},

  /* 0xCECx : clkb1, clkb2 */
    { 0x00, 1, { 0xC0 }},
    { 0xCE,14, { 0x38, 0x0D, 0x03, 0x5E, 0x00, 0x10, 0x07, 0x38,
                 0x0C, 0x03, 0x5F, 0x00, 0x10, 0x07 }},

  /* 0xCEDx : clkb3, clkb4 */
    { 0x00, 1,{ 0xD0 }},
    { 0xCE,14,{ 0x38, 0x09, 0x03, 0x5A, 0x00, 0x10, 0x07, 0x38,
                0x08, 0x03, 0x5B, 0x00, 0x10, 0x07 }},

  /* 0xCFCx : */
    { 0x00, 1, { 0xC7 }},
    { 0xCF, 1, { 0x04 }},

    { 0x00, 1, { 0xC9 }},
    { 0xCF, 1, { 0x00 }},

//--------------------------------------------------------------------------------
// initial setting 3 < Panel setting >
//--------------------------------------------------------------------------------
  /* 0xCBCx */
    { 0x00, 1, { 0xC0 }},
    { 0xCB, 1, { 0x14 }},

    { 0x00, 1, { 0xC2 }},
    { 0xCB, 5, { 0x14, 0x14, 0x14, 0x14, 0x14 }},

  /* 0xCBDx */
    { 0x00, 1, { 0xD5 }},
    { 0xCB, 1, { 0x14 }},

    { 0x00, 1,{ 0xD7 }},
    { 0xCB, 5,{ 0x14, 0x14, 0x14, 0x14, 0x14 }},

  /* 0xCC8x */
    { 0x00, 1, { 0x80 }},
    { 0xCC, 1, { 0x01 }},

    { 0x00, 1, { 0x82 }},
    { 0xCC, 5, { 0x0F, 0x0D, 0x0B, 0x09, 0x05 }},

  /* 0xCC9x */
    { 0x00, 1,{ 0x9A }},
    { 0xCC, 1,{ 0x02 }},

    { 0x00, 1,{ 0x9C }},
    { 0xCC, 3,{ 0x10, 0x0E, 0x0C }},

  /* 0xCCAx */
    { 0x00, 1, { 0xA0 }},
    { 0xCC, 1, { 0x0A }},

    { 0x00, 1, { 0xA1 }},
    { 0xCC, 1, { 0x06 }},

  /* 0xCCBx */
    { 0x00, 1, { 0xB0 }},
    { 0xCC, 1, { 0x01 }},

    { 0x00, 1, { 0xB2 }},
    { 0xCC, 5, { 0x0F, 0x0D, 0x0B, 0x09, 0x05 }},

  /* 0xCCCx */
    { 0x00, 1, { 0xCA }},
    { 0xCC, 1, { 0x02 }},

    { 0x00, 1, { 0xCC }},
    { 0xCC, 3, { 0x10, 0x0E, 0x0C }},

  /* 0xCCDx */
    { 0x00, 1, { 0xD0 }},
    { 0xCC, 1, { 0x0A }},

    { 0x00, 1, { 0xD1 }},
    { 0xCC, 1, { 0x06 }},

  /* Tearing on */
    { 0x00, 1, { 0x00 }},
    { 0x35, 1, { 0x00 }},

  #if 0
/**** CABC start ****/
  /* Write display brightness */
    { 0x00, 1, { 0x00 }},
    { 0x51, 1, { 0xFF }},
  /* Write CTRL display */
    { 0x00, 1, { 0x00 }},
    { 0x53, 1, { 0x2C }},
  /* Write content adaptive brightness control */
    { 0x00, 1, { 0x00 }},
    { 0x55, 1, { 0x01 }},
  /* Write CABC minimum brightness */
    { 0x00, 1, { 0x00 }},
    { 0x5E, 1, { 0x3F }},
/**** CABC  end ****/
  #endif
  #if 0 //defined( BUILD_UBOOT )
  /* Sleep out */
    { 0x00, 1, { 0x00 }},
    { 0x11, 0, {}},
    { REGFLAG_DELAY, 200, {}},
  /* Display on */
    { 0x00, 1, { 0x00 }},
    { 0x29, 0, {}},
    { REGFLAG_DELAY, 10, {}},
  #endif

  /* Note :
  ** Strongly recommend not to set Sleep out / Display On here.
  ** That will cause messed frame to be shown as later the backlight is on. */

  /* Setting ending by predefined flag */
    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

static LCM_SET_TAB
lcm_set_window[] =
{
    { 0x2A, 4, { 0x00, 0x00, ( FRAME_WIDTH >> 8), ( FRAME_WIDTH & 0xFF )}},
    { 0x2B, 4, { 0x00, 0x00, ( FRAME_HEIGHT >> 8), ( FRAME_HEIGHT & 0xFF )}},
    { REGFLAG_END_OF_TABLE, 0x00, {}}
};
 
static LCM_SET_TAB
lcm_sleep_out_setting[] =
{
  /* Sleep Out */
    { 0x11, 1, { 0x00 }},
    { REGFLAG_DELAY, 120, {}},

  /* Display ON */
    { 0x29, 1, { 0x00 }},
    { REGFLAG_DELAY, 10, {}},

    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

static LCM_SET_TAB
lcm_sleep_in_setting[] =
{
  /* Display off sequence */
    { 0x28, 0, { 0x00 }},
    { REGFLAG_DELAY, 40, {}},

  /* Sleep Mode On */
    { 0x10, 0, { 0x00 }},
    { REGFLAG_DELAY, 120, {}},

    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

static LCM_SET_TAB
lcm_compare_id_setting[] =
{
    { 0xF0,  5,  { 0x55, 0xAA, 0x52, 0x08, 0x00 }},
    { REGFLAG_DELAY, 10, {}},

    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

/*****************************************************************************
** push_table
******************************************************************************/
static void
push_table(
LCM_SET_TAB   * table,
unsigned int    count,
unsigned char   force_update
)
{
unsigned int  i = 0;
unsigned      cmd = REGFLAG_END_OF_TABLE;

  for( i = 0; i < count; i++ )
  {
    cmd = table[i].cmd;
    switch( cmd )
    {
      case REGFLAG_DELAY :
        MDELAY( table[i].count );
        break;

      case REGFLAG_END_OF_TABLE :
        break;

      default:
      {
        dsi_set_cmdq_V2( cmd, table[i].count, table[i].para_list, force_update );
        if( 0x00 != cmd )
          MDELAY( 1 );
      } break;
    }
  }
} /** End.. push_table() **/

/*----------------------------------------------------------------------------
**  LCM Driver Implementations
**----------------------------------------------------------------------------*/

/*****************************************************************************
** lcm_set_util_funcs
******************************************************************************/
static void
lcm_set_util_funcs(
const LCM_UTIL_FUNCS  * util
)
{
    memcpy( &lcm_util, util, sizeof( LCM_UTIL_FUNCS ));
} /** End.. lcm_set_util_funcs() **/

/*****************************************************************************
** lcm_get_params
******************************************************************************/
static void
lcm_get_params(
LCM_PARAMS  * params
)
{
    memset( params, 0x00, sizeof( LCM_PARAMS ));

    params->type    = LCM_TYPE_DSI;

    params->width   = FRAME_WIDTH;
    params->height  = FRAME_HEIGHT;

// enable tearing-free
//<2012/09/21-14190-stevenchen, [Hawk4.0] Turn on tearing effect mode.
    params->dbi.te_mode           = LCM_DBI_TE_MODE_VSYNC_ONLY;  //LCM_DBI_TE_MODE_DISABLED; 
//>2012/09/21-14190-stevenchen
    params->dbi.te_edge_polarity  = LCM_POLARITY_RISING;

#if defined( LCM_DSI_CMD_MODE )
    params->dsi.mode  = CMD_MODE;
#else
    params->dsi.mode  = SYNC_PULSE_VDO_MODE;
  //params->dsi.mode  = SYNC_EVENT_VDO_MODE;
  //params->dsi.mode  = BURST_VDO_MODE;
#endif

// DSI
  /* Command mode setting */
    params->dsi.LANE_NUM  = LCM_TWO_LANE;
  /* The following defined the fomat for data coming from LCD engine. */
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

  // Highly depends on LCD driver capability.
  // Not support in MT6573
    params->dsi.packet_size   = 128;  //256;

  // Video mode setting
    params->dsi.intermediat_buffer_num  = 2;

    params->dsi.PS  = LCM_PACKED_PS_24BIT_RGB888;

    params->dsi.word_count = FRAME_WIDTH * 3;
    params->dsi.line_byte  = LCM_LINE_BYTE; //2048;
    params->dsi.rgb_byte   = ( FRAME_WIDTH * 3 + 6 );

    params->dsi.vertical_sync_active  = LCM_VSYNC_NUM;
    params->dsi.vertical_backporch    = LCM_VBP_NUM;
    params->dsi.vertical_frontporch   = LCM_VFP_NUM;
    params->dsi.vertical_active_line  = FRAME_HEIGHT;

    params->dsi.horizontal_sync_active  = LCM_HSYNC_NUM;
    params->dsi.horizontal_backporch    = LCM_HBP_NUM;
    params->dsi.horizontal_frontporch   = LCM_HFP_NUM;
    params->dsi.horizontal_active_pixel = FRAME_WIDTH;

  // Bit rate calculation
    params->dsi.pll_div1  = LCM_OSC_FREQ; // fref=26MHz, fvco=fref*(div1+1) (div1=0~63, fvco=500MHZ~1GHz)
    params->dsi.pll_div2  = 1;  // div2=0~15: fout=fvo/(2*div2)

  /**************************************************************************
  ** ESD or noise interference recovery For video mode LCM only.
  ***************************************************************************/
  // Send TE packet to LCM in a period of n frames and check the response.
    params->dsi.lcm_int_te_monitor = FALSE;
    params->dsi.lcm_int_te_period = 1;  // Unit : frames
  // Need longer FP for more opportunity to do int. TE monitor applicably.
    if( params->dsi.lcm_int_te_monitor )
      params->dsi.vertical_frontporch *= 2;

  // Monitor external TE (or named VSYNC) from LCM once per 2 sec. (LCM VSYNC must be wired to baseband TE pin.)
    params->dsi.lcm_ext_te_monitor = FALSE;

  // Non-continuous clock
    params->dsi.noncont_clock = FALSE;
    params->dsi.noncont_clock_period = 2; // Unit : frames

} /** End.. lcm_get_params() **/

//<2012/10/17-15290-stevenchen, [HAWK40] Fix reading LCM id is unstable.
extern void DSI_clk_HS_mode();
//>2012/10/17-15290-stevenchen
/*****************************************************************************
** lcm_compare_id
******************************************************************************/
//<2012/10/11-14948-stevenchen, [Hawk40] Modify the infrastructure of LCM auto detection.
static unsigned int
lcm_compare_id( void )
{
//<2012/09/19-14141-stevenchen, [Hawk4.0] Implement LCM auto detection.
#ifdef LCM_DETECT_BY_PIN
	#if defined( BUILD_UBOOT ) || defined(BUILD_LK)
	//<2012/10/26-15740-stevenchen, [Hawk40] Init GPIO before reading LCM ID pin.
  		SET_GPIO_MODE( GPIO_LCM_ID_PIN, GPIO_LCM_ID_PIN_M_GPIO );
  		SET_GPIO_DIR_IN( GPIO_LCM_ID_PIN );
	//>2012/10/26-15740-stevenchen
		printf("[uboot/lk][Steven] LCM auto detect by ID pin \n");
		printf("[uboot/lk][Steven] ID pin = %d \n", mt_get_gpio_in(GPIO_LCM_ID_PIN));
	#endif
	//<2012/10/17-15296-stevenchen, [Hawk40] Modify HIGH/LOW definitions of LCM.
	/* Truly LCM ID pin = HIGH */
	return (mt_get_gpio_in(GPIO_LCM_ID_PIN)) ? 1 : 0;
	//>2012/10/17-15296-stevenchen
#else
int   array[4] = { 0 };
char  buffer[3] = { 0 };
char  id1 = 0;
char  id2 = 0;
char  id3 = 0;

    //<2012/10/17-15290-stevenchen, [HAWK40] Fix reading LCM id is unstable.
    #if defined( BUILD_UBOOT ) || defined(BUILD_LK)
    DSI_clk_HS_mode(1);
    MDELAY(10);
    DSI_clk_HS_mode(0);
    #endif
    //>2012/10/17-15290-stevenchen

  /* NOTE: Should reset LCM firstly */
    SET_RESET_PIN( 1 );
    SET_RESET_PIN( 0 );
    MDELAY( 10 );
    SET_RESET_PIN( 1 );
    MDELAY( 10 );

  /* read id return two byte,version and id */
    array[0] = 0x00033700;
    dsi_set_cmdq( &array, 1, 1 );
    read_reg_v2( 0xDA, (unsigned char*)&buffer[0], 1 );

  /* read id return two byte,version and id */
    array[0] = 0x00033700;
    dsi_set_cmdq( &array, 1, 1 );
    read_reg_v2( 0xDB, (unsigned char*)&buffer[1], 1 );

  /* read id return two byte,version and id */
    array[0] = 0x00033700;
    dsi_set_cmdq( &array, 1, 1);
    read_reg_v2( 0xDC, (unsigned char*)&buffer[2], 1 );

    id1 = buffer[0]; /* should be 0x40 */
    id2 = buffer[1]; /* should be 0x00 */
    id3 = buffer[2]; /* should be 0x00 */

    #if defined( BUILD_UBOOT ) || defined(BUILD_LK)
	printf("[uboot/lk][Steven] %s, id1 = 0x%08X, id2 = 0x%08X, id3 = 0x%08X\n", __func__, id1, id2, id3 );
    #else
	printk("[kernel][Steven] %s, id1 = 0x%08X, id2 = 0x%08X, id3 = 0x%08X\n", __func__, id1, id2, id3 );
    #endif
    
    #ifdef LCM_DETECT_BY_BOTH
	#if defined( BUILD_UBOOT ) || defined(BUILD_LK)
	//<2012/10/26-15740-stevenchen, [Hawk40] Init GPIO before reading LCM ID pin.
  		SET_GPIO_MODE( GPIO_LCM_ID_PIN, GPIO_LCM_ID_PIN_M_GPIO );
  		SET_GPIO_DIR_IN( GPIO_LCM_ID_PIN );
	//>2012/10/26-15740-stevenchen
		printf( "[uboot/lk][Steven] LCM auto detect by ID & ID pin \n");
	#endif
    	if(LCM_ID1_OTM8018B == id1)
	{
		return 1;
	}
	else
	{
		/* Truly LCM ID pin = LOW */
		return (mt_get_gpio_in(GPIO_LCM_ID_PIN)) ? 0 : 1;
	}
    #else
	#if defined( BUILD_UBOOT ) || defined(BUILD_LK)
		printf( "[uboot/lk][Steven] LCM auto detect by ID \n");
	#endif
    	return  ( LCM_ID1_OTM8018B == id1 ) ? 1 : 0;
    #endif /* LCM_DETECT_BY_BOTH */
#endif /* LCM_DETECT_BY_PIN */
//>2012/09/19-14141-stevenchen
} /** End.. lcm_compare_id() **/
//>2012/10/11-14948-stevenchen

/*****************************************************************************
** lcm_init
******************************************************************************/
static void
lcm_init( void )
{
    SET_RESET_PIN( 1 );
    SET_RESET_PIN( 0 );
    MDELAY( 10 );
    SET_RESET_PIN( 1 );
    MDELAY( 10 );

  //lcm_compare_id();
    push_table( lcm_initialization_setting, sizeof( lcm_initialization_setting ) / sizeof( LCM_SET_TAB ), 1 );
} /** End.. lcm_init() **/

/*****************************************************************************
** lcm_suspend
******************************************************************************/
static void
lcm_suspend( void )
{
#if defined( BUILD_UBOOT ) || defined(BUILD_LK)
    printf( "xxh suspend uboot %s\n", __func__);
#else
    printk( "xxh suspend kernel %s\n", __func__);
#endif

    SET_RESET_PIN( 0 );
    MDELAY( 1 );
    SET_RESET_PIN( 1 );

    push_table( lcm_sleep_in_setting, sizeof( lcm_sleep_in_setting ) / sizeof( LCM_SET_TAB ), 1 );
} /** End.. lcm_suspend() **/

/*****************************************************************************
** lcm_resume
******************************************************************************/
static void
lcm_resume( void )
{
#if defined( BUILD_UBOOT ) || defined(BUILD_LK)
    printf("xxh resume uboot %s\n", __func__);
#else
    printk("xxh resume kernel %s\n", __func__);
#endif

    lcm_init();
    push_table( lcm_sleep_out_setting, sizeof( lcm_sleep_out_setting ) / sizeof( LCM_SET_TAB ), 1 );
} /** End.. lcm_resume() **/

#if defined( LCM_DSI_CMD_MODE )
/*****************************************************************************
** lcm_update
******************************************************************************/
static void
lcm_update(
unsigned int  x,
unsigned int  y,
unsigned int  width,
unsigned int  height
)
{
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

unsigned int data_array[16] = { 0 };

#if defined( BUILD_UBOOT ) || defined(BUILD_LK)
    printf( "uboot %s\n", __func__ );
#else
    printk( "kernel %s\n", __func__ );
#endif

    data_array[0]= 0x00053902;
    data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2A;
    data_array[2]= (x1_LSB);
    data_array[3]= 0x00053902;
    data_array[4]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2B;
    data_array[5]= (y1_LSB);
    data_array[6]= 0x002C3909;

    dsi_set_cmdq( &data_array, 7, 0 );
} /** End.. lcm_update() **/
#endif /* End.. (LCM_DSI_CMD_MODE) */

/*****************************************************************************
** lcm_update
******************************************************************************/
LCM_DRIVER  otm8018b_vdo_spi_lcm_drv =
{
    .name           = "otm8018b_vdo_spi",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
  #if defined( LCM_DSI_CMD_MODE )
    .set_backlight  = NULL, //lcm_setbacklight,
    .update         = lcm_update,
  #endif
};

/*****************************************************************************
** End
*****************************************************************************/
#undef _OTM8018_DSI_VDO_C_
#endif

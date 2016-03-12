#include <linux/types.h>
#include <mach/mt_pm_ldo.h>
#include <cust_alsps.h>
static struct alsps_hw cust_alsps_hw = {
    .i2c_num    = 0,
	.polling_mode_ps = 0,
	.polling_mode_als =1,
    .power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
    .power_vol  = VOL_DEFAULT,          /*LDO is not used*/
    .i2c_addr   = {0x0C, 0x48, 0x78, 0x00}, 
     .als_level  = { 0,  8,  13,  16,   19,   22,   25,   28,  31,  34,   37,   40,  43,  46, 49,  55}, 
    .als_value  = {1, 12, 24,  32, 56, 96,  167,  289,  499,  864,  1459,  2586,  4475, 7743,  13396, 23178},
    .ps_threshold = 0x1a,
    .als_window_loss = 0x05,
};
struct alsps_hw *get_cust_alsps_hw(void) {
    return &cust_alsps_hw;
}

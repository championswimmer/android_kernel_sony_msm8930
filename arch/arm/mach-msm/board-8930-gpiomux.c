/* Copyright (c) 2011-2012, Code Aurora Forum. All rights reserved.
 * Copyright (C) 2012 Sony Mobile Communications AB.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/gpio.h>
#include <asm/mach-types.h>
#include <mach/gpiomux.h>
#include <mach/socinfo.h>
#include "devices.h"
#include "board-8930.h"

#if 1
static struct gpiomux_setting volume_key_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting volume_key_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_LOW,
};
static struct gpiomux_setting camera_key_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting camera_key_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
//	.pull = GPIOMUX_PULL_NONE,
//	.dir = GPIOMUX_OUT_LOW,
	.pull = GPIOMUX_PULL_UP,
};
#endif

//S:LO
static struct gpiomux_setting sim_det_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};
static struct gpiomux_setting sim_det_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};
//E:LO

/* The SPI configurations apply to GSBI 1*/
#if 0   // MK, org
static struct gpiomux_setting spi_active = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting spi_suspended_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#endif

/*S: andy, P\L sensor porting*/
#ifdef ORG_VER
#else
static struct gpiomux_setting plsensor_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting plsensor_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
        // Luke, GPIO tuning, p-sensor
	// .pull = GPIOMUX_PULL_DOWN,
	.pull = GPIOMUX_PULL_UP,
};
#endif
/*E: andy, P\L sensor porting*/

static struct gpiomux_setting gsbi3_suspended_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_KEEPER,
};

static struct gpiomux_setting gsbi3_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi5 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gsbi9 = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gsbi10 = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

#if 0    // MK
static struct gpiomux_setting gsbi12 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif

//
// , 20120728, [ ] Correct GPIOs when suspend/resume..
//
#if 1   // add
#if 0
static struct gpiomux_setting gsbi1_sus_ecompass = {   //: not run this, only for ref
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_KEEPER,
};
#endif
static struct gpiomux_setting gsbi1_init_ecompass = {  //== gsbi1_act_ecompass
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

#if 0
static struct gpiomux_setting gsbi1_act_ecompass = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif

static struct gpiomux_setting gsbi12_init_g_sensor = {  //== gsbi12_act_g_sensor
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

#if 0
static struct gpiomux_setting gsbi12_act_g_sensor = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif
#endif
// 


static struct gpiomux_setting cdc_mclk = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

#if 0  
static struct gpiomux_setting audio_auxpcm[] = {
	/* Suspended state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},
	/* Active state */
	{
		.func = GPIOMUX_FUNC_1,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},
};
#endif 

static struct gpiomux_setting audio_mbhc = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting audio_spkr_boost = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
static struct gpiomux_setting gpio_eth_suspend_1_cfg = {
	.pull = GPIOMUX_PULL_DOWN,
	.drv = GPIOMUX_DRV_2MA,
	.func = GPIOMUX_FUNC_GPIO,
};

static struct gpiomux_setting gpio_eth_suspend_2_cfg = {
	.pull = GPIOMUX_PULL_NONE,
	.drv = GPIOMUX_DRV_2MA,
	.func = GPIOMUX_FUNC_GPIO,
};
#endif

static struct gpiomux_setting slimbus = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_KEEPER,
};

static struct gpiomux_setting wcnss_5wire_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting wcnss_5wire_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv  = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#if 0 
static struct gpiomux_setting atmel_resout_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting atmel_resout_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting atmel_ldo_en_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting atmel_ldo_en_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting atmel_int_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting atmel_int_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#endif

static struct gpiomux_setting cyttsp_resout_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	//.pull = GPIOMUX_PULL_UP,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting cyttsp_resout_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting cyttsp_int_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting cyttsp_int_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

#ifdef MSM8930_PHASE_2
static struct gpiomux_setting hsusb_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_LOW,
};

// Luke, GPIO tuning, HW ID, APT
static struct gpiomux_setting hwid_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};
// Luke, GPIO tuning, SIM_DETEC
static struct gpiomux_setting sim_detec_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};
static struct gpiomux_setting apt_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_LOW,
};
// Luke, GPIO tuning, ANT_SEL
static struct gpiomux_setting ant_sel_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};


static struct msm_gpiomux_config msm8930_hsusb_configs[] = {
	// Luke, GPIO tuning, HW ID -->
	{
		.gpio = 63,     /* HSUSB_EXTERNAL_5V_LDO_EN */
		.settings = {
                        // Luke, GPIO tuning, HW ID
			[GPIOMUX_SUSPENDED] = &hwid_sus_cfg,//&hsusb_sus_cfg,
		},
	},
	{
		.gpio = 54, 
		.settings = {
			[GPIOMUX_SUSPENDED] = &hwid_sus_cfg,
		},
	},
	{
		.gpio = 55, 
		.settings = {
			[GPIOMUX_SUSPENDED] = &hwid_sus_cfg,
		},
	},
	{
		.gpio = 50, 
		.settings = {
			[GPIOMUX_SUSPENDED] = &hwid_sus_cfg,
		},
	},
	{
		.gpio = 51, 
		.settings = {
			[GPIOMUX_SUSPENDED] = &hwid_sus_cfg,
		},
	},
	{
		.gpio = 53, 
		.settings = {
			[GPIOMUX_SUSPENDED] = &hwid_sus_cfg,
		},
	},
	// Luke, GPIO tuning, HW ID <--
	
	{
		.gpio = 97,     /* HSUSB_5V_EN */
		.settings = {
			[GPIOMUX_SUSPENDED] = &hsusb_sus_cfg,
		},
	},
	// Luke, GPIO tuning, CODEC_RST_N
	{
		.gpio = 42, 
		.settings = {
			[GPIOMUX_SUSPENDED] = &ant_sel_cfg,
		},
	},
	// Luke, GPIO tuning, SIM_DETEC
	{
		.gpio = 33, 
		.settings = {
			[GPIOMUX_SUSPENDED] = &sim_detec_cfg,
		},
	},
	// Luke, GPIO tuning, APT
	{
		.gpio = 57, 
		.settings = {
			[GPIOMUX_SUSPENDED] = &apt_sus_cfg,
		},
	},
	{
		.gpio = 127,  
		.settings = {
			[GPIOMUX_SUSPENDED] = &apt_sus_cfg,
		},
	},
	{
		.gpio = 138,   
		.settings = {
			[GPIOMUX_SUSPENDED] = &apt_sus_cfg,
		},
	},
        // Luke, GPIO tuning, ANT_SEL 
	{
		.gpio = 112,   
		.settings = {
			[GPIOMUX_SUSPENDED] = &ant_sel_cfg,
		},
	},
	{
		.gpio = 118,   
		.settings = {
			[GPIOMUX_SUSPENDED] = &ant_sel_cfg,
		},
	},

};
#endif

#if 0
static struct gpiomux_setting hap_lvl_shft_suspended_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting hap_lvl_shft_active_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};
#endif


static struct gpiomux_setting ap2mdm_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mdm2ap_status_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

//
// , 20120728, [ ] Correct GPIOs when suspend/resume..
//
#if 0
static struct gpiomux_setting mdm2ap_errfatal_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#endif

static struct gpiomux_setting ap2mdm_kpdpwr_n_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting mdp_vsync_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mdp_vsync_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
static struct gpiomux_setting hdmi_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting hdmi_active_1_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting hdmi_active_2_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting hdmi_active_3_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting hdmi_active_4_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting hdmi_active_5_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH,
};

#endif

// Bato add for CCI unused gpio control
static struct gpiomux_setting cci_empty_susp_cfg = {
        .func = GPIOMUX_FUNC_GPIO,
        .drv = GPIOMUX_DRV_2MA,
        .pull = GPIOMUX_PULL_DOWN,
        .dir = GPIOMUX_IN,
};

static struct msm_gpiomux_config msm8x60_cci_empty_configs[] __initdata = {
	{
                .gpio = 1,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 3,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 7,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 10,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 14,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 15,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 18,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 24,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 25,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
// Luke, GPIO tuning, SIM_DETEC
/*        
	{
                .gpio = 33,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
*/        
	{
                .gpio = 38,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 41,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 43,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 56,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 64,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 66,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 71,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 72,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 73,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 74,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 75,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 77,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 79,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 80,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 81,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 82,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 89,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 90,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 91,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 92,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 97,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 99,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 100,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 101,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 102,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 110,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 120,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 121,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 124,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 128,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 131,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 132,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 135,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 139,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 140,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 141,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 144,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 145,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 149,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 150,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
	{
                .gpio = 151,
                .settings = {
                        [GPIOMUX_SUSPENDED] = &cci_empty_susp_cfg,
                },
        },
};
// end


#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
static struct msm_gpiomux_config msm8960_ethernet_configs[] = {
	{
		.gpio = 89,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_eth_suspend_1_cfg,
		}
	},
	{
		.gpio = 90,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_eth_suspend_2_cfg,
		}
	},
};
#endif

static struct msm_gpiomux_config msm8960_gsbi_configs[] __initdata = {
#if 0   // MK, org
	{
		.gpio      = 6,		/* GSBI1 QUP SPI_DATA_MOSI */
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
	{
		.gpio      = 7,		/* GSBI1 QUP SPI_DATA_MISO */
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
#endif
	{
		.gpio      = 8,		/* GSBI1 QUP SPI_CS_N */
		.settings = {
//
// , 20120708, [ ] Porting sensor.
//
#if 0   // MK, org
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
#else
			[GPIOMUX_SUSPENDED] = &gsbi1_init_ecompass,    //: when system boot will use this config
#endif
// 
		},
	},
	{
		.gpio      = 9,		/* GSBI1 QUP SPI_CLK */
		.settings = {
//
// , 20120708, [ ] Porting sensor.
//
#if 0   // MK, org
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
#else
			[GPIOMUX_SUSPENDED] = &gsbi1_init_ecompass,    //: when system boot will use this config
#endif
// 
		},
	},
	{
		.gpio      = 16,	/* GSBI3 I2C QUP SDA */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi3_suspended_cfg,
			[GPIOMUX_ACTIVE] = &gsbi3_active_cfg,
		},
	},
	{
		.gpio      = 17,	/* GSBI3 I2C QUP SCL */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi3_suspended_cfg,
			[GPIOMUX_ACTIVE] = &gsbi3_active_cfg,
		},
	},
	{
		.gpio      = 22,	/* GSBI5 UART2 */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi5,
		},
	},
	{
		.gpio      = 23,	/* GSBI5 UART2 */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi5,
		},
	},
	{
		.gpio      = 44,	/* GSBI12 I2C QUP SDA */
		.settings = {
//
// , 20120708, [ ] Porting sensor.
//
			//[GPIOMUX_SUSPENDED] = &gsbi12,
			[GPIOMUX_SUSPENDED] = &gsbi12_init_g_sensor,     //: when system boot will use this config
#if 0
			[GPIOMUX_ACTIVE] = &gsbi3_active_cfg,
#endif
		},
	},
	{
		.gpio      = 95,	/* GSBI9 I2C QUP SDA */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi9,
		},
	},
	{
		.gpio      = 96,	/* GSBI12 I2C QUP SCL */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi9,
		},
	},
	{
		.gpio      = 45,	/* GSBI12 I2C QUP SCL */
		.settings = {
//
// , 20120708, [ ] Porting sensor.
//
			//[GPIOMUX_SUSPENDED] = &gsbi12,
			[GPIOMUX_SUSPENDED] = &gsbi12_init_g_sensor,     //: when system boot will use this config
#if 0
			[GPIOMUX_ACTIVE] = &gsbi3_active_cfg,
#endif
		},
	},
	{
		.gpio      = 73,	/* GSBI10 I2C QUP SDA */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi10,
		},
	},
	{
		.gpio      = 74,	/* GSBI10 I2C QUP SCL */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi10,
		},
	},
};

static struct msm_gpiomux_config msm8960_slimbus_config[] __initdata = {
	{
		.gpio	= 60,		/* slimbus data */
		.settings = {
			[GPIOMUX_SUSPENDED] = &slimbus,
		},
	},
	{
		.gpio	= 61,		/* slimbus clk */
		.settings = {
			[GPIOMUX_SUSPENDED] = &slimbus,
		},
	},
};

static struct msm_gpiomux_config msm8960_audio_codec_configs[] __initdata = {
	{
		.gpio = 59,
		.settings = {
			[GPIOMUX_SUSPENDED] = &cdc_mclk,
		},
	},
};

static struct msm_gpiomux_config msm8960_audio_mbhc_configs[] __initdata = {
	{
		.gpio = 37,
		.settings = {
			[GPIOMUX_SUSPENDED] = &audio_mbhc,
		},
	},
};

static struct msm_gpiomux_config msm8960_audio_spkr_configs[] __initdata = {
	{
		.gpio = 15,
		.settings = {
			[GPIOMUX_SUSPENDED] = &audio_spkr_boost,
		},
	},
};

#if 0 
static struct msm_gpiomux_config msm8960_audio_auxpcm_configs[] __initdata = {
	{
		.gpio = 63,
		.settings = {
			[GPIOMUX_SUSPENDED] = &audio_auxpcm[0],
			[GPIOMUX_ACTIVE] = &audio_auxpcm[1],
		},
	},
	{
		.gpio = 64,
		.settings = {
			[GPIOMUX_SUSPENDED] = &audio_auxpcm[0],
			[GPIOMUX_ACTIVE] = &audio_auxpcm[1],
		},
	},
	{
		.gpio = 65,
		.settings = {
			[GPIOMUX_SUSPENDED] = &audio_auxpcm[0],
			[GPIOMUX_ACTIVE] = &audio_auxpcm[1],
		},
	},
	{
		.gpio = 66,
		.settings = {
			[GPIOMUX_SUSPENDED] = &audio_auxpcm[0],
			[GPIOMUX_ACTIVE] = &audio_auxpcm[1],
		},
	},
};
#endif 

static struct msm_gpiomux_config wcnss_5wire_interface[] = {
	{
		.gpio = 84,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 85,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 86,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 87,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 88,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
};
#if 0 
static struct msm_gpiomux_config msm8960_atmel_configs[] __initdata = {
	{	/* TS INTERRUPT */
		.gpio = 11,
		.settings = {
			[GPIOMUX_ACTIVE]    = &atmel_int_act_cfg,
			[GPIOMUX_SUSPENDED] = &atmel_int_sus_cfg,
		},
	},
	{	/* TS LDO ENABLE */
		.gpio = 50,
		.settings = {
			[GPIOMUX_ACTIVE]    = &atmel_ldo_en_act_cfg,
			[GPIOMUX_SUSPENDED] = &atmel_ldo_en_sus_cfg,
		},
	},
	{	/* TS RESOUT */
		.gpio = 52,
		.settings = {
			[GPIOMUX_ACTIVE]    = &atmel_resout_act_cfg,
			[GPIOMUX_SUSPENDED] = &atmel_resout_sus_cfg,
		},
	},
};
#endif

static struct msm_gpiomux_config msm8960_cyttsp_configs[] __initdata = {
	{	/* TS INTERRUPT */
		.gpio = 11,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cyttsp_int_act_cfg,
			[GPIOMUX_SUSPENDED] = &cyttsp_int_sus_cfg,
		},
	},	
	{	/* TS RESOUT */
		.gpio = 52,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cyttsp_resout_act_cfg,
			[GPIOMUX_SUSPENDED] = &cyttsp_resout_sus_cfg,
		},
	},
};


#if 0
static struct msm_gpiomux_config hap_lvl_shft_config[] __initdata = {
	{
		.gpio = 47,
		.settings = {
			[GPIOMUX_SUSPENDED] = &hap_lvl_shft_suspended_config,
			[GPIOMUX_ACTIVE] = &hap_lvl_shft_active_config,
		},
	},
};
#endif


static struct msm_gpiomux_config mdm_configs[] __initdata = {
	/* AP2MDM_STATUS */
#if 0   //: used by e-compass
	{
		.gpio = 94,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_cfg,
		}
	},
#endif
	/* MDM2AP_STATUS */
	{
		.gpio = 69,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mdm2ap_status_cfg,
		}
	},
#if 0   //: used by e-compass
	/* MDM2AP_ERRFATAL */
	{
		.gpio = 70,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mdm2ap_errfatal_cfg,
		}
	},
#endif
	/* AP2MDM_ERRFATAL */
	{
		.gpio = 95,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_cfg,
		}
	},
	/* AP2MDM_KPDPWR_N */
	{
		.gpio = 81,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_kpdpwr_n_cfg,
		}
	},
	/* AP2MDM_PMIC_RESET_N */
	{
		.gpio = 80,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_kpdpwr_n_cfg,
		}
	}
};


#if 1
static struct msm_gpiomux_config msm8930_keypad_configs[] __initdata = {
	{
		.gpio      = 47,
		.settings = {
			[GPIOMUX_ACTIVE]    = &volume_key_actv_cfg,
			[GPIOMUX_SUSPENDED] = &volume_key_susp_cfg,
		},
	},
	{
		.gpio      = 48,
		.settings = {
			[GPIOMUX_ACTIVE]    = &volume_key_actv_cfg,
			[GPIOMUX_SUSPENDED] = &volume_key_susp_cfg,
		},
	},
	{
		.gpio      = 68,
		.settings = {
			[GPIOMUX_ACTIVE]    = &camera_key_actv_cfg,
			[GPIOMUX_SUSPENDED] = &camera_key_susp_cfg,
		},
	},
	{
		.gpio      = 69,
		.settings = {
			[GPIOMUX_ACTIVE]    = &camera_key_actv_cfg,
			[GPIOMUX_SUSPENDED] = &camera_key_susp_cfg,
		},
	},
};
#endif

//S:LO
static struct msm_gpiomux_config msm8930_sim_det_configs[] __initdata = {
	{
		.gpio      = 33,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sim_det_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sim_det_susp_cfg,
		},
	},
};
//E:LO

static struct msm_gpiomux_config msm8960_mdp_vsync_configs[] __initdata = {
	{
		.gpio = 0,
		.settings = {
			[GPIOMUX_ACTIVE]    = &mdp_vsync_active_cfg,
			[GPIOMUX_SUSPENDED] = &mdp_vsync_suspend_cfg,
		},
	}
};
/*S: andy, P/L sensor porting*/
#ifdef ORG_VER
#else
static struct msm_gpiomux_config msm8930_plsensor_configs[] __initdata = {
	{
		.gpio = 49,
		.settings = {
			[GPIOMUX_ACTIVE]    = &plsensor_active_cfg,
			[GPIOMUX_SUSPENDED] = &plsensor_suspend_cfg,
		},
	},
};
#endif
/*E: andy, P/L sensor porting*/

#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
static struct msm_gpiomux_config msm8960_hdmi_configs[] __initdata = {
	{
		.gpio = 99,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 100,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 101,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 102,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_2_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},

};

static struct msm_gpiomux_config msm8930_mhl_configs[] __initdata = {
	{
		.gpio = 72,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_3_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 71,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_4_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 73,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_5_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},

};
#endif

#if 0  
static struct gpiomux_setting haptics_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting haptics_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct msm_gpiomux_config msm8930_haptics_configs[] __initdata = {
	{
		.gpio = 77,
		.settings = {
			[GPIOMUX_ACTIVE] = &haptics_active_cfg,
			[GPIOMUX_SUSPENDED] = &haptics_suspend_cfg,
		},
	},
	{
		.gpio = 78,
		.settings = {
			[GPIOMUX_ACTIVE] = &haptics_active_cfg,
			[GPIOMUX_SUSPENDED] = &haptics_suspend_cfg,
		},
	},
};
#endif 

#if 0	// MK
static struct gpiomux_setting sd_det_line = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct msm_gpiomux_config msm8930_sd_det_config[] __initdata = {
	{
		.gpio = 94,	/* SD Card Detect Line */
		.settings = {
			[GPIOMUX_SUSPENDED] = &sd_det_line,
			[GPIOMUX_ACTIVE] = &sd_det_line,
		},
	},
};
#endif

int __init msm8930_init_gpiomux(void)
{
	int rc = msm_gpiomux_init(NR_GPIO_IRQS);
	if (rc) {
		pr_err(KERN_ERR "msm_gpiomux_init failed %d\n", rc);
		return rc;
	}

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
	msm_gpiomux_install(msm8960_ethernet_configs,
			ARRAY_SIZE(msm8960_ethernet_configs));
#endif

	msm_gpiomux_install(msm8960_gsbi_configs,
			ARRAY_SIZE(msm8960_gsbi_configs));

#if 0 
	msm_gpiomux_install(msm8960_atmel_configs,
			ARRAY_SIZE(msm8960_atmel_configs));
#endif

#if 1
    msm_gpiomux_install(msm8930_keypad_configs,
    	ARRAY_SIZE(msm8930_keypad_configs));
#endif



#if defined(CONFIG_TOUCHSCREEN_CYTTSP3_I2C) && \
		defined(CONFIG_TOUCHSCREEN_CYTTSP3_CORE)		
	msm_gpiomux_install(msm8960_cyttsp_configs,
			ARRAY_SIZE(msm8960_cyttsp_configs));
#endif


//S:LO
	msm_gpiomux_install(msm8930_sim_det_configs,
			ARRAY_SIZE(msm8930_sim_det_configs));
//E:LO
	msm_gpiomux_install(msm8960_slimbus_config,
			ARRAY_SIZE(msm8960_slimbus_config));

	msm_gpiomux_install(msm8960_audio_codec_configs,
			ARRAY_SIZE(msm8960_audio_codec_configs));

	msm_gpiomux_install(msm8960_audio_mbhc_configs,
			ARRAY_SIZE(msm8960_audio_mbhc_configs));

	msm_gpiomux_install(msm8960_audio_spkr_configs,
			ARRAY_SIZE(msm8960_audio_spkr_configs));

	#if 0  
	msm_gpiomux_install(msm8960_audio_auxpcm_configs,
			ARRAY_SIZE(msm8960_audio_auxpcm_configs));
	#endif 

	msm_gpiomux_install(wcnss_5wire_interface,
			ARRAY_SIZE(wcnss_5wire_interface));

	if (machine_is_msm8930_mtp() || machine_is_msm8930_fluid() ||
		machine_is_msm8930_cdp()) {
		#if 0
		msm_gpiomux_install(hap_lvl_shft_config,
			ARRAY_SIZE(hap_lvl_shft_config));
		#endif

#ifdef MSM8930_PHASE_2
		msm_gpiomux_install(msm8930_hsusb_configs,
			ARRAY_SIZE(msm8930_hsusb_configs));
#endif
	}

	if (PLATFORM_IS_CHARM25())
		msm_gpiomux_install(mdm_configs,
			ARRAY_SIZE(mdm_configs));

    #if 0  
	if (machine_is_msm8930_cdp() || machine_is_msm8930_mtp()
		|| machine_is_msm8930_fluid())
		msm_gpiomux_install(msm8930_haptics_configs,
			ARRAY_SIZE(msm8930_haptics_configs));
    #endif 

#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
	msm_gpiomux_install(msm8960_hdmi_configs,
			ARRAY_SIZE(msm8960_hdmi_configs));
	if (machine_is_msm8930_fluid())
		msm_gpiomux_install(msm8930_mhl_configs,
				ARRAY_SIZE(msm8930_mhl_configs));
#endif

	msm_gpiomux_install(msm8960_mdp_vsync_configs,
			ARRAY_SIZE(msm8960_mdp_vsync_configs));

#if 0	// MK
	msm_gpiomux_install(msm8930_sd_det_config,
			ARRAY_SIZE(msm8930_sd_det_config));
#endif

/*S:andy, P/L sensor*/
#ifdef ORG_VER
#else
	msm_gpiomux_install(msm8930_plsensor_configs,
			ARRAY_SIZE(msm8930_plsensor_configs));
#endif
/*E:andy, P/L sensor*/

#if 1 //Bato add for empty pin config
	msm_gpiomux_install(msm8x60_cci_empty_configs,
			ARRAY_SIZE(msm8x60_cci_empty_configs));
#endif
	return 0;
}

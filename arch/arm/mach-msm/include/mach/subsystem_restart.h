/* Copyright (c) 2011, Code Aurora Forum. All rights reserved.
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
 */

#ifndef __SUBSYS_RESTART_H
#define __SUBSYS_RESTART_H

#include <linux/spinlock.h>
#include <mach/msm_iomap.h>
#define SUBSYS_NAME_MAX_LENGTH 40

enum {
	RESET_SOC = 1,
	RESET_SUBSYS_COUPLED,
	RESET_SUBSYS_INDEPENDENT,
	RESET_LEVEL_MAX
};
#ifdef CONFIG_CCI_KLOG
#define POWER_OFF_SPECIAL_ADDR	IOMEM(0xFB9FFFFC)
#define CRASH_SPECIAL_ADDR	IOMEM(0xFB9FFFF8)
#define UNKONW_CRASH_SPECIAL_ADDR	IOMEM(0xFB9FFFF0)
#endif
#define POWERONOFFRECORD	0xBBCCDD00
#define IMEM_CERT_RECORD	0x11223344
enum poweronvalue
{
	pmic_inactive = 0,
	pmic_keypad,
	pmic_rtc,
	pmic_cable,
	pmic_smpl,
	pmic_wdog,
	pmic_usbchg,
	pmic_wallchg,
	pmic_hotkeyreset,		
	pmic_unknown,
	pmic_max,
};

enum pwroffvalue
{
	inactive = 0,
	hotwarereset,
	rpmdogbite,
	acpudogbite,	
	kernelpanic,	
	modemfatal,
	lpassfatal,
	rivafatal,
	q6fatal,
	gssfatal,
	exmodemfatal,
	dspsfatal,
	normalreboot,	
	normalreboot_bootloader,	
	normalreboot_oem,	
	normalreboot_recovery,	
	normalreboot_fastboot,	
	adloadmode,	
	mdloadmode,	
	pmicwdog,	
	coldboot,
	lowbattery,
	overheat,
	poweroff,
	unknown,
	pwroffvaluemax,
};

typedef struct
{
	unsigned char	pwronindex;
	unsigned char	pwronrecord[0xA];
	unsigned short	pwronrecordcount[pmic_max];
	unsigned char	pwroffindex;
	unsigned char	pwroffrecord[0xA];
	unsigned short	pwroffrecordcount[pwroffvaluemax];
	bool	PM_WDT;
	unsigned int	RPM_WDT;
	unsigned int	MSMRST_STAT;
	unsigned int	ACPU0_WDT;
	unsigned int	ACPU1_WDT;
	unsigned int	PM_PWRON;
	unsigned int	FIQ_ADDR;
	unsigned int	REST_REASON;
	unsigned int	CUR_DDRVALUE;
	unsigned int	CUR_IMEMVALUE;
}cci_pwrrecord;

typedef struct
{
	cci_pwrrecord szpwrd;
	unsigned char	reserved[0x200-sizeof(cci_pwrrecord)-sizeof(unsigned int)];
	unsigned int	SIGN;
}cci_rsvrecord;

struct subsys_data {
	const char *name;
	int (*shutdown) (const struct subsys_data *);
	int (*powerup) (const struct subsys_data *);
	void (*crash_shutdown) (const struct subsys_data *);
	int (*ramdump) (int, const struct subsys_data *);

	/* Internal use only */
	struct list_head list;
	void *notif_handle;

	struct mutex shutdown_lock;
	struct mutex powerup_lock;

	void *restart_order;
	struct subsys_data *single_restart_list[1];
};

#if defined(CONFIG_MSM_SUBSYSTEM_RESTART)

int get_restart_level(void);
int subsystem_restart(const char *subsys_name);
int ssr_register_subsystem(struct subsys_data *subsys);

#else

static inline int get_restart_level(void)
{
	return 0;
}

static inline int subsystem_restart(const char *subsystem_name)
{
	return 0;
}

static inline int ssr_register_subsystem(struct subsys_data *subsys)
{
	return 0;
}

#endif /* CONFIG_MSM_SUBSYSTEM_RESTART */

#endif

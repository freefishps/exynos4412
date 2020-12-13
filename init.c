//DDR3
static void phy_control_reset(int ctrl_no, struct exynos4_dmc *dmc)
{
	if (ctrl_no) {
		writel((mem.control1 | (1 << mem.dll_resync)),
		&dmc->phycontrol1);
		writel((mem.control1 | (0 << mem.dll_resync)),
		&dmc->phycontrol1);
	}
	else {
		writel((mem.control0 | (0 << mem.dll_on)),
		&dmc->phycontrol0);
		writel((mem.control0 | (1 << mem.dll_on)),
		&dmc->phycontrol0);
	}
}

/*
 * ZQ Calibration
 * Termination: Disable
 * Auto Calibration Start: Enable
 */
writel(0xE3855503, &dmc->phyzqcontrol);

/* Set the PhyControl0 */
writel(0x71101008, &dmc->phycontrol0);

/* Set the PhyControl0.ctrl_dll_on */
writel(0x7110100A, &dmc->phycontrol0);

/* Phase Delay for DQS Cleaning: 180' Shift */
writel(0x20000086, &dmc->phycontrol1);

/* Set the PhyControl0.ctrl_start */
writel(0x7110100B, &dmc->phycontrol0);

/* Set the ConControl */
writel(0x0FFF300A, &dmc->concontrol);

/*
 * Memor Burst length: 8
 * Number of chips: 1
 * Memory Bus width: 32 bit
 * Memory Type: DDR3
 */
writel(0x00302600, &dmc->memcontrol);

writel(0x40801333, &dmc->memconfig0);

writel(0x80000007, &dmc->ivcontrol);


/*
 * TimingAref, TimingRow, TimingData, TimingPower Setting:
 * Values as per Memory AC Parameters
 */
writel(0x000000BB, &dmc->timingref);
writel(0x7846654F, &dmc->timingrow);
writel(0x46400506, &dmc->timingdata);
writel(0x52000A3C, &dmc->timingpower);

/* Set the PhyControl1.fp_resync to update DLL information */
writel(0x2000008E, &dmc->phycontrol1);

/* Set the PhyControl1.fp_resync to 0 */
writel(0x20000086, &dmc->phycontrol1);

//wait phy state

/* Issue a NOP command */
writel(0x07000000, &dmc->directcmd);

/* Issue an EMRS2 command */
writel(0x00020000, &dmc->directcmd);

/* Issue an EMRS3 command */
writel(0x00030000, &dmc->directcmd);

/*Issue an EMRS command */
writel(0x00010000, &dmc->directcmd);

/* Issue a MRS command, reset DLL */
writel(0x00000100, &dmc->directcmd);

/* Issue a MRS command */
writel(0x00000420, &dmc->directcmd);

/* Issues a ZQINIT command */
writel(0x0A000000, &dmc->directcmd);

/* Set the ConControl to turn on an auto refresh counter */
writel(mem.concontrol | (1 << 5), &dmc->directcmd);

/* Set the MemControl to turn on power down mode */
writel(mem.memcontrol | ((1 << 4) + (1 << 1) + (1 << 0)), &dmc->directcmd);

//power
static void exynos4_set_ps_hold_ctrl(void)
{
	struct exynos4_power *power =
		(struct exynos4_power *)samsung_get_base_power();

	/* Set PS-Hold high */
	setbits_le32(&power->ps_hold_control, 0x00005300);
}

void set_ps_hold_ctrl(void)
{
	if (cpu_is_exynos5())
		exynos5_set_ps_hold_ctrl();
	else if(cpu_is_exynos4())
		exynos4_set_ps_hold_ctrl();
}

//LED
writel(1 << 4, 0x11000060);//GPK1CON
writel(1 << 1, 0x11000064);//GPK1DAT
writel(1 << 0, 0x11000100);//GPL2CON
writel(1 << 0, 0x11000104);//GPL2DAT

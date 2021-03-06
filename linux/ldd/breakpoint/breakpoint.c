#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/kallsyms.h>

#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <linux/time.h>
#include <linux/delay.h>

#define TRUE  1
#define FALSE 0

static bool bRegisterFlag = FALSE;
struct perf_event * __percpu *sample_hbp;

static char ksym_name[KSYM_NAME_LEN] = "pid_max";
module_param_string(ksym, ksym_name, KSYM_NAME_LEN, S_IRUGO);
MODULE_PARM_DESC(ksym, "Kernel symbol to monitor; this module will report any"
			" write operations on the kernel symbol");

static int __init hw_break_module_init(void);
static void sample_hbp_handler(struct perf_event *bp,
				   struct perf_sample_data *data,
				   struct pt_regs *regs)
{
	printk(KERN_INFO "%s value is changed\n", ksym_name);
	dump_stack();
	printk(KERN_INFO "Dump stack from sample_hbp_handler\n");

	unregister_wide_hw_breakpoint(sample_hbp);
	bRegisterFlag = FALSE;
}

static int __init hw_break_module_init(void)
{
	int ret;
	struct perf_event_attr attr;

	hw_breakpoint_init(&attr);
	attr.bp_addr = kallsyms_lookup_name(ksym_name);
	printk("ksym_name :%s, addr:0x%lx\n", ksym_name, (unsigned long)attr.bp_addr);
	attr.bp_len = HW_BREAKPOINT_LEN_4;
	attr.bp_type = HW_BREAKPOINT_W | HW_BREAKPOINT_R;

	sample_hbp = register_wide_hw_breakpoint(&attr, sample_hbp_handler, NULL);
	if (IS_ERR((void __force *)sample_hbp)) {
		ret = PTR_ERR((void __force *)sample_hbp);
		goto fail;
	}

	bRegisterFlag = TRUE;
	printk(KERN_INFO "HW Breakpoint for %s write installed\n", ksym_name);

	return 0;

fail:
	printk(KERN_INFO "Breakpoint registration failed\n");

	return ret;
}

static void __exit hw_break_module_exit(void)
{
	if (TRUE == bRegisterFlag)
		unregister_wide_hw_breakpoint(sample_hbp);
	printk(KERN_INFO "HW Breakpoint for %s write uninstalled\n", ksym_name);
}

module_init(hw_break_module_init);
module_exit(hw_break_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("K.Prasad");
MODULE_DESCRIPTION("ksym breakpoint");

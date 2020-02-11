#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <asm/pgtable.h>
MODULE_LICENSE("GPL");
static int id;
struct pid *pid;
struct task_struct *task;
struct mm_struct *mm_struct;
static pgd_t *pgd;
static p4d_t *p4d;
static pud_t *pud;
static pmd_t *pmd;
static pte_t *ptep,pte;
static unsigned long pfn;
struct vm_area_struct *vma;
long vaddr;
struct pid *x =NULL;
module_param(id, int, 0);
MODULE_PARM_DESC(id, "Process ID of the background process");
// called when module is installed
int __init init_module()
{
pid = find_get_pid(id);
if(x == pid)
{
	printk(KERN_ALERT "Invalid PID");
	return 0;
}
//get the task descriptor
task = pid_task(pid,PIDTYPE_PID);
//get the memory descriptor of the task
mm_struct = task->mm;

for(vma = mm_struct->mmap;vma;vma = vma->vm_next)
{
	printk("Virtual Start Address: %lx\t Virtual End Address:%lx",vma->vm_start,vma->vm_end);
	for(vaddr = vma->vm_start; vaddr < vma->vm_end; vaddr++)
	{
		//Directing the 5 level Page table
		pgd = pgd_offset(mm_struct,vaddr);
		p4d = p4d_offset(pgd,vaddr);
		pud = pud_offset(p4d,vaddr);
		pmd = pmd_offset(pud,vaddr);
		ptep = pte_offset_map(pmd,vaddr);
		pte = *ptep;
		if(pte_present(pte))
		{
			pfn = pte_pfn(pte);						
			printk("Physical Address: %lx",pfn);
		}
							
	}
}
    return 0;
}



// called when module is removed
void __exit cleanup_module()
{
	printk(KERN_ALERT "mymodule: Goodbye, cruel world!!\n");
}

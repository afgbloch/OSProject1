#include<linux/linkage.h>
#include<linux/sched.h>
#include<linux/kernel.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
#include<linux/gfp.h>


asmlinkage long sys_get_child_pids(pid_t* list, size_t limit, size_t* num_children)
{
    pid_t* tmpList = NULL;
    pid_t* tmpListHead = NULL;
    struct list_head *position = NULL;
	struct task_struct *child = NULL;
	int nbP = 0;
    int returnVal = 0;
    
	if(list == NULL && limit != 0) {
        return -EFAULT;
	}
	
    tmpListHead = kmalloc(sizeof(pid_t) * limit, GFP_KERNEL);
    if(tmpListHead == NULL && limit != 0) {
        return -EFAULT;
    }
    
    tmpList = tmpListHead;
    read_lock(&tasklist_lock);
	list_for_each(position, &current->children) {
	    child = list_entry(position, struct task_struct, sibling);
	    nbP++;
	    if(nbP <= limit) {
            *tmpList = child->pid;
            tmpList++;            
        } else {
            returnVal = -ENOBUFS;
        }
    }
    read_unlock(&tasklist_lock);
    
    if(put_user(nbP, num_children)){
        return -EFAULT;
    }
   
    tmpList = tmpListHead;
    while(limit > 0) {
        if(put_user(*tmpList, list)){
            return -EFAULT;
        }
        tmpList++;
        list++;
        limit--;
    }    
	
    kfree(tmpListHead);
    return returnVal;
}
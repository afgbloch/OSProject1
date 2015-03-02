#include<linux/linkage.h>
#include<linux/sched.h>
#include<linux/kernel.h>
#include<linux/uaccess.h>


asmlinkage long sys_get_child_pids(pid_t* list, size_t limit, size_t* num_children)
{
    struct list_head *position = NULL;
	struct task_struct *child = NULL;
	int nbP = 0;
    int returnVal = 0;
    
	if(list == NULL && limit != 0) {
        return -EFAULT;
	}
	
    read_lock(&tasklist_lock);
	list_for_each(position, &current->children) {
	    child = list_entry(position, struct task_struct, sibling);
	    nbP++;
	    if(nbP <= limit) {
            returnVal = put_user(child->pid, list);
            if(0 != returnVal) {
                return returnVal;
            }
            list++;            
        } else {
            returnVal = -ENOBUFS;
        }
    }
    read_unlock(&tasklist_lock);
    returnVal = -put_user(nbP, num_children);
	
    return returnVal;
}
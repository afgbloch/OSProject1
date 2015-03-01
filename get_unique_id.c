#include<linux/linkage.h>
#include<linux/uaccess.h>
#include<linux/types.h>

atomic_t counter = ATOMIC_INIT(0);

asmlinkage long sys_get_unique_id(int *uuid)
{
  long result;
  result = put_user(atomic_read(&counter), uuid);
    
  // If the put_user succeeded, we increment the counter.
  if (result == 0) {
    atomic_inc(&counter);
  }
  
  return result;
}
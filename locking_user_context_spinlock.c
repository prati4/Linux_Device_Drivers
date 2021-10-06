#include <linux/module.h>
#include <linux/kernel.h>


#include <linux/kthread.h> // therad 
#include <linux/delay.h>   // sleep 

#include <linux/spinlock.h>  //for spin


MODULE_AUTHOR("pratiksha");
MODULE_LICENSE("GPL");

static struct task_struct *char_thread;
static struct task_struct *char_thread1;

int mydata =0;

int my_thread_fun(void *p);
int my_thread_fun1(void *p);

spinlock_t char_spinlock;

int my_thread_fun(void *p)
{
	
	while(!kthread_should_stop())
	{
		if(spin_is_locked(&char_spinlock)==0)
		{
			printk(KERN_INFO"Spinlock is not locked in my_thread_fun");
		}
		spin_lock(&char_spinlock);
		if(spin_is_locked(&char_spinlock))
		{
			printk(KERN_INFO"Spinlock is locked in my_thread_fun");
			
		
		printk(KERN_INFO"my_thread_fun increment value %d\n",mydata++);
		}
		
		spin_unlock(&char_spinlock);
		msleep(1000);
	}
	return 0;
}

int my_thread_fun1(void *p)
{
	//int i = 0;
	while(!kthread_should_stop())
	{
		if(spin_is_locked(&char_spinlock)==0)  // This is used to check wheather the lock is available or not 
		{
			printk(KERN_INFO"Spinlock is not locked in my_thread_fun1");
		}
		spin_lock(&char_spinlock);
		if(spin_is_locked(&char_spinlock))
		{
			printk(KERN_INFO"Spinlock is locked in my_thread_fun1");
		
		printk(KERN_INFO"my_thread_fun1 increment value %d\n",mydata++);
		}
		
		spin_unlock(&char_spinlock);
		msleep(1000);
	}
	return 0;
}




static int __init my_simpledriver_init(void)
{
	printk(KERN_INFO"My sample driver start");

	spin_lock_init(&char_spinlock);  //  spinlock init 

	char_thread = kthread_run(my_thread_fun,NULL,"my char thread");
	if(char_thread)
	{
		printk(KERN_INFO" create the thread");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread");
		
	}
	char_thread1 = kthread_run(my_thread_fun1,NULL,"my char thread1");
	if(char_thread1)
	{
		printk(KERN_INFO" create the thread1");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread");
		
	}
	return 0;

}


static void __exit my_simpledriver_exit(void)
{
	printk(KERN_INFO"My samle driver exit\n");

	kthread_stop(char_thread);
	kthread_stop(char_thread1);

}
module_init(my_simpledriver_init);
module_exit(my_simpledriver_exit);



#include <linux/module.h>
#include <linux/kernel.h>


#include <linux/kthread.h> // therad 
#include <linux/delay.h>   // sleep 
#include <linux/sched.h>   //task_struct
//#include <linux/mutex.h>   //mutex_init




MODULE_AUTHOR ("pratiksha");
MODULE_LICENSE ("GPL");

static struct task_struct *char_thread;
static struct task_struct *char_thread1;

int mydata =0; //1

int my_thread_fun(void *p);
int my_thread_fun1(void *p);
struct mutex my_mutex;  //mutex to be initialize


int my_thread_fun(void *p)
{
	while(!kthread_should_stop())
	{		
		//mutex_lock(&my_mutex);   // used to lock
		mydata++;
		printk(KERN_INFO "my_thread_fun  %lu\n", mydata);
		//mutex_unlock(&my_mutex);   // used to unlock
		msleep(1000);    
		
	}
	return 0;	
}

int my_thread_fun1(void *p)
{
	while(!kthread_should_stop())
	{
		//mutex_lock(&my_mutex);
		mydata++;
		printk(KERN_INFO "my_thread_fun2  %lu\n", mydata);
		//mutex_unlock(&my_mutex);
		msleep(500);
	}
	return 0;
}


static int __init my_simpledriver_init(void)
{
	printk(KERN_INFO"My sample driver start");

	//mutex_init(&my_mutex); // to initialzed mutex dynamically

	char_thread = kthread_run(my_thread_fun,NULL,"my char thread");
	if(char_thread)

	{
	  	
		printk(KERN_INFO"pid is:%d",char_thread->pid);
		printk(KERN_INFO"flag is %u",char_thread->thread_info.flags);
		printk(KERN_INFO"status is %u",char_thread->thread_info.status);
		printk(KERN_INFO"state is %ld",char_thread->state);   //0 runnable
		/* -1 unrunnable, 0 runnable, >0 stopped: */
	
		printk(KERN_INFO" create the thread ");
		
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread");
		
	}
	char_thread1 = kthread_run(my_thread_fun1,NULL,"my char thread1");
	if(char_thread1)
	{
	          
	          
		
		printk(KERN_INFO"pid is:%d",char_thread1->pid);
		printk(KERN_INFO"flag is %u",char_thread1->thread_info.flags);
		printk(KERN_INFO"status is %u",char_thread->thread_info.status);
		printk(KERN_INFO"state is %ld",char_thread1->state); 
		printk(KERN_INFO"create the thread 1");
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




#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>           //线程的取消动作发生在加锁和解锁过程中时，当发生线程2取消后而没有进行解锁时，就会出现线程1将一直阻塞
 
pthread_mutex_t mutex;
 
void* odd(void* arg)
{
  int i = 1;
  for(; ; i+=2)
  {
    pthread_mutex_lock(&mutex);
    printf("odd:%d\n", i);
    pthread_mutex_unlock(&mutex);
  }
}
 
void* even(void* arg)
{
  int i = 1;
  for(; ; i+=1)
  {
    pthread_mutex_lock(&mutex);
    printf("even:%d\n", i);
	if(!(i%50))
		while(1)
			i=i;
    pthread_mutex_unlock(&mutex);
  }
}
 
 
int main()
{
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&t1, NULL, even, NULL);
    pthread_create(&t2, NULL, odd, NULL);
    //pthread_create(&t3, NULL, even, NULL);
    
    //sleep(3);
    //pthread_cancel(t2);             //取消线程2，这个动作可能发生在线程2加锁之后和解锁之前
 
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);
 
    return 0;
}

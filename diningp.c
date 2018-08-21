#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>

#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define left (num+4)%5
#define right (num+1)%5

int state[5];
int phil[5]={0,1,2,3,4};
sem_t mutex,S[5];

void *test(int num)
{
	if(state[num]==HUNGRY && state[left]!=EATING && state[right]!=EATING)
	{
		state[num]=EATING;
		sleep(2);
		printf("Philosopher %d is Eating \n",num+1);
		printf("Philosopher %d takes fork %d and fork %d \n",num+1,left+1,num+1);
		sem_post(&S[num]);
	}
}

void *putfork(int num)
{
	sem_wait(&mutex);
	
	state[num]=THINKING;
	
	printf("Philosopher %d put down the fork %d and %d \n",num+1,left+1,num+1);
	printf("Philosopher %d is THINKING \n",num+1);
	
	test(left);
	test(right);
	
	sem_post(&mutex);
	
}

void *takefork(int num)
{
	sem_wait(&mutex);
	
	state[num]=HUNGRY;
	
	printf("Philosopher %d is Hungry\n",num+1);
	
	test(num);
	
	sem_post(&mutex);
	sem_wait(&S[num]);
}
void *philosopher(int num)
{
	while(1)
	{
		sleep(1);
		takefork(num);
		sleep(0);
		putfork(num);
	}
}

int main()
{
	int i;
	pthread_t tid[5];
	
	sem_init(&mutex,0,1);
	for(i=0;i<5;i++)
	{
		sem_init(&S[i],0,0);
	}
	
	for(i=0;i<5;i++)
	{
		pthread_create(&tid[i],NULL,philosopher,i);
		printf("Philosopher %d is thinking\n",i+1);
	}
	
	for(i=0;i<5;i++)
	{
		pthread_join(tid[i],NULL);
	}
}

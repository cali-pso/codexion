#include <pthread.h>
#include <stdio.h>

typedef struct s_locker
{
	int value;
	pthread_mutex_t lock;
}	t_locker;

void *tryingshitout(void *arg)
{
	t_locker *actual_arg = arg;
	pthread_mutex_lock(&(actual_arg->lock));
	printf("thread %lu with value %i\n\n", pthread_self(), actual_arg->value);
	actual_arg->value++;
	pthread_mutex_unlock(&(actual_arg->lock));
	return NULL;
}

int main()
{
	t_locker locker1;
	locker1.value = 12;
	pthread_mutex_init(&locker1.lock, NULL);

	pthread_t thread1;
	pthread_t thread2;
	pthread_create(&thread1, NULL, tryingshitout, &locker1);
	pthread_create(&thread2, NULL, tryingshitout, &locker1);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_mutex_destroy(&locker1.lock);
	return 0;
}

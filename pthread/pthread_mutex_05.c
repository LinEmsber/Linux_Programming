/* An example of pthread mutex */

#include <stdio.h>
#include <pthread.h>

int use_mutex;
pthread_mutex_t mutex;
volatile int global = 0;

void *compute(void *arg)
{
        int i;

        for (i = 0; i < 100 * 100 * 100; i++) {
                if (use_mutex) {
                        pthread_mutex_lock(&mutex);
                        global++;
                        pthread_mutex_unlock(&mutex);
                } else
                        global++;
        }
        return NULL;
}

void multi_thread_test()
{
        int i;
        pthread_t tids[3];

        global = 0;
        pthread_mutex_init(&mutex, NULL);

        for (i = 0; i < 3; i++)
                pthread_create(&tids[i], NULL, compute, NULL);

        for (i = 0; i < 3; i++)
                pthread_join(tids[i], NULL);

        printf("global = %d\n", global);
        pthread_mutex_destroy(&mutex);
}

int main()
{
        // without using mutex
        use_mutex = 0;
        multi_thread_test();

        // using mutex
        use_mutex = 1;
        multi_thread_test();
        return 0;
}

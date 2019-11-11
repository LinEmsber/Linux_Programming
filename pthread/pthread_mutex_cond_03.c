/* An example of pthread condition variable.
 *
 * producer(): produce a char item which starts from a, and save this char into buffer.
 * put_item(): save this char into buffer, and make the global index variable, in, point to next index.
 * consumer(): get the char according to the global index variable, in.
 */

#include <stdio.h>
#include <unistd.h>

#include <pthread.h>

#define CAPACITY 160
#define ITEM_COUNT (CAPACITY * 2) 


/* global variable */

int buffer[CAPACITY];
int in = 0;
int out = 0;

/* pthread variable */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t wait_empty_buffer;
pthread_cond_t wait_full_buffer;


/* The functions of item operantion */
int buffer_is_empty()
{
        return in == out;
}

int buffer_is_full()
{
        return (in + 1) % CAPACITY == out;
}

int get_item()
{
        int item;

        item = buffer[out];
        out = (out + 1) % CAPACITY;
        return item;
}

void put_item(int item)
{
        /* save item into buffer at index, in. */
        buffer[in] = item;

        /* increase variable in each time. */
        in = (in + 1) % CAPACITY;
}

/* consumer() and producer() */
void *consumer(void *arg)
{
        int i;
        int item;

        for (i = 0; i < ITEM_COUNT ; i++) {

		pthread_mutex_lock(&mutex);

                // wait the pthread condition variable, wait_full_buffer, from producer
                while (buffer_is_empty())
                        pthread_cond_wait(&wait_full_buffer, &mutex);

                // get the item from global array
                item = get_item();

                /*
                 * The pthread_cond_signal() function unblocks one thread waiting for the condition variable cond.
                 */

                // After get item from buffer, wake up the pthread condition variable, wait_full_buffer, in the conumer function
                pthread_cond_signal(&wait_empty_buffer);

                pthread_mutex_unlock(&mutex);

                printf("    consume : %d\n", item);
                usleep(50000);
        }
        return NULL;
}

void producer()
{
        int i;
        int item;

        for (i = 0; i < ITEM_COUNT; i++) {


		// assign and print a char to item, and wait 0.5 secs
                item = i + 'a';

		printf("produce : %d\n", item);
                usleep(50000);

                pthread_mutex_lock(&mutex);

                // wait the pthread condition variable, wait_empty_buffer, from conumer
                while (buffer_is_full())
                        pthread_cond_wait(&wait_empty_buffer, &mutex);

                // save the item into global array, buffer, and update the index of this array
                put_item(item);

                // After put_item(), wake up the pthread condition variable, wait_full_buffer, in the conumer function
                pthread_cond_signal(&wait_full_buffer);

                pthread_mutex_unlock(&mutex);
        }
}

int main()
{
	int i;

        pthread_t consumer_tid;

        // initialize in
        in = 0;

        // initialize pthread mutex
        pthread_mutex_init(&mutex, NULL);

        // condition for producer and comsumer
        pthread_cond_init(&wait_empty_buffer, NULL);
        pthread_cond_init(&wait_full_buffer, NULL);

	for ( i = 0; i < 1000; i++){

		// create a thread for consumer(), but this consumer() will wait for producer().
		pthread_create(&consumer_tid, NULL, consumer, NULL);

		// producer() output char and send the condition variable, wait_full_buffer, for consumer() who is waiting.
		producer();

	}
        return 0;
}

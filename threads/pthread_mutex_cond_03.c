/* An example of pthread condition variable */

// The producer() produce a char item which starts from a, and save this char into buffer.
// The put_item() save this char into buffer, and make the global index variable, in, point
// to next index.
//
// The consumer() get the char according to the global index variable, in.

#include <stdio.h>
#include <unistd.h>

#include <pthread.h>

#define CAPACITY 6
int buffer[CAPACITY];
int in;
int out;

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
        buffer[in] = item;

        // in++;
        // in = in % CAPACITY;
        // increase variable in each time, but discrete it in range from 0 to 3.
        in = (in + 1) % CAPACITY;
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t wait_empty_buffer;
pthread_cond_t wait_full_buffer;

#define ITEM_COUNT (CAPACITY * 2)

void *consumer(void *arg)
{
        int i;
        int item;

        for (i = 0; i < ITEM_COUNT; i++) {
                pthread_mutex_lock(&mutex);

                // wait wait_full_buffer from producer
                while (buffer_is_empty())
                        pthread_cond_wait(&wait_full_buffer, &mutex);

                item = get_item();

                // The pthread_cond_signal() function unblocks one thread waiting for the
                // condition variable cond.

                // wake up wait_full_buffer in the conumer function
                pthread_cond_signal(&wait_empty_buffer);

                pthread_mutex_unlock(&mutex);

                printf("    consume : %c\n", item);
                usleep(500000);
        }
        return NULL;
}

void producer()
{
        int i;
        int item;

        for (i = 0; i < ITEM_COUNT; i++) {

                // print char starts from a, and wait 0.5 secs
                item = i + 'a';
                printf("produce : %c\n", item);
                usleep(500000);

                pthread_mutex_lock(&mutex);

                // wait wait_empty_buffer from conumer
                while (buffer_is_full())
                        pthread_cond_wait(&wait_empty_buffer, &mutex);

                put_item(item);

                // wake up wait_full_buffer in the conumer function
                pthread_cond_signal(&wait_full_buffer);

                pthread_mutex_unlock(&mutex);
        }
}

int main()
{
        pthread_t consumer_tid;

        // initialize in
        in = 0;

        // initialize pthread mutex
        pthread_mutex_init(&mutex, NULL);

        // condition for producer and comsumer
        pthread_cond_init(&wait_empty_buffer, NULL);
        pthread_cond_init(&wait_full_buffer, NULL);

        // create a thread for consumer, but this consumer wait for producer
        pthread_create(&consumer_tid, NULL, consumer, NULL);

        // producer output char and make condition variable for consumer who is waiting.
        producer();

        return 0;
}

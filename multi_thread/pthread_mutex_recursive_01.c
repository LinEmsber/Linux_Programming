/* An example of pthread mutex attr using PTHREAD_MUTEX_RECURSIVE */


// case 1, use PTHREAD_MUTEX_RECURSIVE:
//
// > gcc pthread_mutex_recursive_01.c -lpthread -D_RECURSIVE_LOCK
// > ./a.out

// case 2, not use PTHREAD_MUTEX_RECURSIVE, and deadlock happen:
//
// > gcc pthread_mutex_recursive_01.c -lpthread -D_NO_RECURSIVE_LOCK
// > ./a.out


#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


struct object
{
        pthread_mutex_t mutex;
        int data;
};


void object_init(struct object *object)
{

#ifdef _RECURSIVE_LOCK

        pthread_mutexattr_t attr;

        // initialize pthread mutex attribute
        pthread_mutexattr_init(&attr);

        // As the NP suffix indicates, this is a non-portable extension to the POSIX standard and
        // should not be employed in portable programs.
        // pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

        // use PTHREAD_MUTEX_RECURSIVE
        pthread_mutex_init(&object->mutex, &attr);

#elif _NOT_RECURSIVE_LOCK

        // not use PTHREAD_MUTEX_RECURSIVE
        pthread_mutex_init(&object->mutex, NULL);
#endif
        object->data = 0;
}

// object provides two synchronized methods to protect the shared data.
// object_method_0 and object_method_1 must acquire the lock before access the shared data.
void object_method_0(struct object *object)
{
        pthread_mutex_lock(&object->mutex);
        object->data++;
        pthread_mutex_unlock(&object->mutex);
}

void object_method_1(struct object *object)
{
        // lock the mutex
        pthread_mutex_lock(&object->mutex);

        // object_method_0 attempts to lock the mutex again.
        // If the mutex type is PTHREAD_MUTEX_RECURSIVE, attempting to recursively lock the mutex is allowed.
        object_method_0(object);

        // access the shared data
        object->data++;

        pthread_mutex_unlock(&object->mutex);
}

void *entry_0(void *arg)
{
        int i;
        struct object *object = (struct object *)arg;

        for (i = 0; i < 3; i++) {
                puts("Thread 0 is executing ...");
                object_method_1(object);
                sleep(1);
        }
        return NULL;
}

void *entry_1(void *arg)
{
        int i;
        struct object *object = (struct object *)arg;

        for (i = 0; i < 3; i++) {
                puts("Thread 1 is executing ...");
                object_method_1(object);
                sleep(1);
        }
        return NULL;
}

int main()
{
        struct object object;
        pthread_t thread_0;
        pthread_t thread_1;

        // init mutex, and determine to set attribute or not
        object_init(&object);

        pthread_create(&thread_0, NULL, entry_0, &object);
        pthread_create(&thread_1, NULL, entry_1, &object);

        pthread_join(thread_0, NULL);
        pthread_join(thread_1, NULL);

        return 0;
}

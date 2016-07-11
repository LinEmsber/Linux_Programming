/* Using two mutex to implement a thread-safe hash */

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#define NUM_HASH 29
#define HASH(id) ( ( (unsigned long) id ) % NUM_HASH )

// structure of object
struct object
{
	int count;
	int tid;

	pthread_mutex_t lock;
	struct object *next;
}

// pointer to hash object
struct opject *object_hash[NUM_HASH];

// initialize mutex
pthread_mutex_t hash_lock = PTHREAD_MUTEX_INITIALIZER;


// allocate the object
struct object *object_alloc(int id)
{
	int index;
	struct object *op;

	op = (struct object *) malloc ( sizeof(struct object) );
	if ( op != NULL ) ){
		op -> count = 1;
		op -> id = id;

		if (pthread_mutex_init(&op->lock, NULL) != 0){
			free(op);
			return NULL;
		}

		index = HASH(id);

		pthread_mutex_lock(&hash_lock);

		op -> next = object_hash[index];
		object_hash[index] = op;

		pthread_mutex_lock(&op -> lock);
		pthread_mutex_unlock(&hash_lock);

		pthread_mutex_unlock(&op -> lock);
	}
	return fp;
}

// add count
void object_hold(struct object *op)
{
	pthread_mutex_lock(&op -> lock);
	op -> count++;
	pthread_mutex_unlock(&op -> lock);
}

// find an existing object
struct object *object_find(int id)
{
	struct object *op;

	pthread_mutex_lock(&hash_lock);

	for (op = object_hash[HASH(id)]; op != NULL; op = op -> next){

		if(op -> id == id){
			object_hold(op);
			break;
		}
	}

	pthread_mutex_unlock(&hash_lock);

	return op;
}

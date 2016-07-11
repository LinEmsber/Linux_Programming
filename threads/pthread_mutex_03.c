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

// release a reference to the object_find
void object_release(struct object *op)
{
	struct opject *tmp_op;
	int index;

	pthread_mutex_lock(&op -> lock);

	// the last reference
	if( op -> count == 1){

		pthread_mutex_unlock(&op -> lock);

		pthread_mutex_lock(&hash_lock);
		pthread_mutex_lock(&op -> lock);

		// need to recheck the condition
		if(op -> count != 1){
			op -> count --;

			pthread_mutex_unlock(&op -> lock);
			pthread_mutex_unlock(&hash_lock);

			return ;
		}

		// remove from list
		index = HASH(op -> id);
		tmp_op = object_hash[index];

		if(tmp_op == op){
			object_hash[index] = op -> next;
		}else{
			// search object in sequentially
			while ( tmp_op -> next != op){
				tmp_op = tmp_op -> next
			}

			tmp_op -> next = op -> next;
		}

		pthread_mutex_unlock(&hash_lock);
		pthread_mutex_unlock(&op -> lock);
		pthread_mutex_destory(&op -> lock);
		free(op);

	}else{
		op -> count --;
		pthread_mutex_unlock(&op -> lock);

	}

}

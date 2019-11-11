/* Using two mutex to implement a thread-safe hash */

// This program is compared to the pthread_mutex_03.c.
//
// The lock-ordering issures surrounding the hash list and the reference count go away when we
// use the same lock for both purposes.
// If your locking granularity is too coarse, you end up with too many threads blocking behind
// the same locks, with little improvement possible from concurrency.
// If your locking granularity is too fine, then you suffer bad performance from excess locking
// overhead, and you end up with complex code.


#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#define NUM_HASH 29
#define HASH(id) ( ( (unsigned long) id ) % NUM_HASH )

// structure of object
struct object
{
	int count;
	int id;

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

			// The different part between pthread_mutex_03.c
			// We do not use function object_hold to lock op's mutex.
			// Only lock the whole object_hash array.
			//
			// object_hold(op);
			op -> count ++;

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


	// We only lock object_hash array to try to improve possible from conconcurrency.
	
	pthread_mutex_lock(&hack_lock);

	// the last reference
	if (--(op->count) == 0){

		index = HASH(op->id);
		tmp_op = object_hash[index];

		if (tmp_op == op){

			object_hash[index] == op -> next;

		}else{

			while (tmp_op -> next != op){
				tmp_op = tmp_op -> next;
			}

			tmp_op -> next= tmp_op -> next;

		}

		pthread_mutex_unlock(&hash_lock);
		pthread_mutex_destory(&op->lock);

		free(op);
	}else{
		pthread_mutex_unlock(&hash_lock);
	}


	// pthread_mutex_lock(&op -> lock);
	//
	// // the last reference
	// if( op -> count == 1){
	//
	// 	pthread_mutex_unlock(&op -> lock);
	//
	// 	pthread_mutex_lock(&hash_lock);
	// 	pthread_mutex_lock(&op -> lock);
	//
	// 	// need to recheck the condition
	// 	if(op -> count != 1){
	// 		op -> count --;
	//
	// 		pthread_mutex_unlock(&op -> lock);
	// 		pthread_mutex_unlock(&hash_lock);
	//
	// 		return ;
	// 	}
	//
	// 	// remove from list
	// 	index = HASH(op -> id);
	// 	tmp_op = object_hash[index];
	//
	// 	if(tmp_op == op){
	// 		object_hash[index] = op -> next;
	// 	}else{
	// 		// search object in sequentially
	// 		while ( tmp_op -> next != op){
	// 			tmp_op = tmp_op -> next
	// 		}
	//
	// 		tmp_op -> next = op -> next;
	// 	}
	//
	// 	pthread_mutex_unlock(&hash_lock);
	// 	pthread_mutex_unlock(&op -> lock);
	// 	pthread_mutex_destory(&op -> lock);
	// 	free(op);
	//
	// }else{
	// 	op -> count --;
	// 	pthread_mutex_unlock(&op -> lock);
	//
	// }

}

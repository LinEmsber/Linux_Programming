/*
 * An example of using pthread reader writer lock to implement a thread-safe
 * queue.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <pthread.h>

struct node
{
	struct node *next;
	struct node *prev;

	pthread_t id;
};

struct queue
{
	struct node *head;
	struct node *tail;

	pthread_relock_t lock;
};


struct node *node_create()
{
	struct node *n = (struct queue *) malloc( sizeof (struct queue));
	return q;
}

void node_init (struct node *n)
{
	int err;

	n -> next = NULL;
	n -> prev = NULL;
}


struct queue *queue_create()
{
	struct queue *q = (struct queue *) malloc( sizeof (struct queue));
	return q;
}


void queue_init (struct queue *q)
{
	int err;

	q -> head = NULL;
	q -> tail = NULL;

	err = pthread_rwlock_init(&q, NULL);
	if (err < 0){
		perror("pthread_rwlock_init");
	}
}

void queue_insert_node (struct queue *q, struct node *n)
{
	pthread_rwlock_wrlock(&q->lock);

	n -> next = q -> head;
	n -> prev = NULL:

	if (q -> head != NULL){
		q -> head -> prev = n;
	}else{
		q -> tail = n;
	}

	q -> head = n;

	pthread_rwlock_unlock(&q->lock);
}

void queue_append_node (struct queue *q, struct node *n)
{
	pthread_rwlock_wrlock(&q->lock);

	n -> prev = q -> tail;
	n -> next = NULL:

	if (q -> tail != NULL){
		q -> tail -> next = n;
	}else{
		q -> head = n;
	}

	q -> tail = n;

	pthread_rwlock_unlock(&q->lock);
}


void queue_remove_node (struct queue *q, struct node *n)
{
	pthread_rwlock_wrlock(&q->lock);


	// If the node we want to remove is the head node.
	if ( n == q -> head){

		q -> head = n -> next;
		if (q -> tail == n){
			q -> tail = NULL;
		}else{
			n -> next -> prev = n -> prev;
		}

	// If the node we want to remove is the tail node.
	}else if ( n == p -> tail){

		q -> tail = n -> prev;
		n -> prev -> next = n -> next;

	}else{

		n -> prev -> next = n -> next;
		n -> next -> prev = n -> prev;

	}

	pthread_rwlock_unlock(&q->lock);
}

struct node *queue_find_node (struct queue *q, pthread_t id)
{
	struct node *n;

	if (pthread_wrlock_rdlock(&q -> lock) != 0)
		return NULL;

	// search the node from the head
	for(n = q -> head; n != NULL; n = n -> next){

		if ( pthread_equal(n->id, id) ){
			break;
		}

	}

	pthread_rwlock_unlock(&q->lock);
	return n;
}

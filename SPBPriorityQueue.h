#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include <stdbool.h>


/**
 * SP Bounded Priority Queue summary
 *
 * A bounded priority queue with element type of BPQueueElement
 *
 * pop the element with the lowest value in O(1) complexity
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t SPBPQueue;

typedef struct sp_bpq_element_t {
	int index;
	double value;
} BPQueueElement;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Allocates a new SPBPQueue in the memory.
 * Given maxSize the bound of the queue
 *
 * Initialize every queue element to EMPTY_BPQUEUE_ELEMENT_INDEX & EMPTY_BPQUEUE_ELEMENT_VALUE
 *
 * O(maxSize) - complexity
 *
 * @param maxSize is the maximum Capacity of the queue
 * @assert(maxSize>0)
 * @return
 * NULL in case allocation failure occurred
 * Otherwise, the new point is returned
 */
SPBPQueue* spBPQueueCreate(int maxSize);

/**
 * Allocates a copy of the given Queue.
 *
 * Given the SPBQueue source, the functions returns a
 * new SPBQueue such that:
 *
 * - copy_maxSize = source_maxSize (The maximum capacity is the same)
 * - size(copy) = size(source) (copy and source have the same current size)
 * - for every BPQueueElement:
 *		- index(copy) = index(source) (copy and source have the same index)
 *		- value(copy) = value(source) (copy and source have the same value)
 *
 * @param source - The source queue
 * @assert (source != NULL && source->queue !=NULL)
 *
 * @return
 * NULL in case memory allocation occurs
 * Others a copy of source is returned.
 */
SPBPQueue* spBPQueueCopy(SPBPQueue* source);

/**
 * Free all memory allocation associated with point,
 * if source is NULL nothing happens.
 */
void spBPQueueDestroy(SPBPQueue* source);

/**
 * Given the SPBQueue source,the function initialize the queue
 * the functions returns source such that:
 *
 * - source_maxSize (no change)
 * - size(source)=0 (initialized the size to empty queue)
 * Initialize every queue element to EMPTY_BPQUEUE_ELEMENT_VALUE & EMPTY_BPQUEUE_ELEMENT_VALUE
 *
 * @param source - The source queue
 * @assert (source != NUlL)
 */
void spBPQueueClear(SPBPQueue* source);

/**
 * returns the number of elements in the queue source.
 *
 * Given the SPBQueue source

 * @param source - The source queue
 * @assert(NULL != source)
 *
 * @return
 * The numbers of elements in the queue
 */
int spBPQueueSize(SPBPQueue* source);

/**
 * returns the maximum capacity of elements in the queue source.
 *
 * Given the SPBQueue source

 * @param source - The source queue
 * @assert (source != NUlL)
 *
 * @return
 * The maximum capacity of elements in the queue
 */
int spBPQueueGetMaxSize(SPBPQueue* source);

/**
 * Inserts an element to the queue
 *
 * @param source pointer to the queue that we will insert the element to
 * @param index is the index of the element that we insert
 * @param value is the value of the element that we insert
 *
 * case1 : the queue isn't full
 * 		- inserts the value and the index after all the values
 * 		  that with higher value than the new value
 *
 * case2 : the queue is full & the value is higher than every value in the queue
 * 		- nothing happened
 * case3 : the queue is full & the value is not higher than every value in the queue
 * 		- follow case1 and remove the element with the highest value
 *
 *  * O(size) - complexity
 *
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT -for NULL pointer or value is negative
 * SP_BPQUEUE_FULL - the insertion was successful and the queue is full capacity
 * SP_BPQUEUE_SUCCESS - the insertion was successful
 *
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value);

/**
 * removes the element with the lowest value
 *
 * @param source pointer to the queue
 *
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT -for NULL pointer
 * SP_BPQUEUE_EMPTY - for am empty queue and do nothing
 * SP_BPQUEUE_SUCCESS - switch the lowest element with empty value and index
 * and decrement the size of the queue
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source);

/**
 * Returns a copy of the element with the lowest value
 *
 * @param source pointer to the queue
 * @param res pointer to BPQueueElement that a copy to the last element will insert to
 *
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT -for NULL pointer
 * SP_BPQUEUE_EMPTY - for am empty queue  and return empty index and value in res
 * SP_BPQUEUE_SUCCESS - the copy of the lowest element was successful
 *
 */
SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res);

/**
 * Returns a copy of the element with the highest value
 *
 * @param source pointer to the queue
 * @param res pointer to BPQueueElement that a copy to the last element will insert to
 *
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT -for NULL pointer
 * SP_BPQUEUE_EMPTY - for am empty queue  and return NULL in res
 * SP_BPQUEUE_SUCCESS - the copy of the  highest element was successful
 */
SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res);

/**
 * returns the minimum value in the queue
 *
 * Given the SPBQueue source
 *
 * @param source - The source queue
 * @assert(NULL != source)
 * @assert(source->queue != NULL);
 *
 * @return
 * double - the minimum value in the queue or  -1 if the queue is empty
 *
 */
double spBPQueueMinValue(SPBPQueue* source);

/**
 * returns the maximum value in the queue
 *
 * Given the SPBQueue source
 *
 * @param source - The source queue
 * @assert(NULL != source)
 * @assert(source->queue != NULL);
 *
 * @return
 * double - the maximum value in the queue or -1 if the queue is empty
 *
 */
double spBPQueueMaxValue(SPBPQueue* source);

/**
 * checks if the queue is empty
 *
 * Given the SPBQueue source
 *
 * @param source - The source queue
 * @assert(NULL != source)
 *
 * @return
 * TRUE - the queue is empty
 * FALSE - the queue is not empty
 *
 */
bool spBPQueueIsEmpty(SPBPQueue* source);

/**
 * checks if the queue is in full capacity
 *
 * Given the SPBQueue source
 *
 * @param source - The source queue
 * @assert(NULL != source)
 *
 * @return
 * TRUE - the queue is in full capacity
 * FALSE - the queue is not full capacity
 *
 */
bool spBPQueueIsFull(SPBPQueue* source);

#endif

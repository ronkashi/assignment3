#include <stdbool.h>
#include "SPBPriorityQueue.h"
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>

#define EMPTY_BPQUEUE_ELEMENT_INDEX -1
#define EMPTY_BPQUEUE_ELEMENT_VALUE -1

/** type used to define Bounded priority queue
 *
 * size - the numbers of elements in the queue
 * maxSize - the bound on the number of the elements in the queue
 * queue - pointer to BPQueueElement with the highest value in the queue
 *
 */
struct sp_bp_queue_t {
	int size; //the numbers of elements in the queue
	int maxSize;
	BPQueueElement* queue; //pointer to the first element of the queue
};

SPBPQueue* spBPQueueCreate(int maxSize) {
	assert(0 < maxSize);
	SPBPQueue* res = NULL;
	res = (SPBPQueue*) malloc(sizeof(*res));
	if (NULL == res) {
		return NULL;
	}
	res->queue = (BPQueueElement*) malloc(sizeof(*(res->queue)) * maxSize);
	if (NULL == res->queue) {
		return NULL;
	}
	res->size = maxSize; //only for init the queue
	res->maxSize = maxSize;
	spBPQueueClear(res);
	return res;
}

SPBPQueue* spBPQueueCopy(SPBPQueue* source) {
	assert(NULL != source && NULL !=source->queue);
	int i = 0;
	SPBPQueue* copy;
	copy = spBPQueueCreate(source->maxSize);
	copy->maxSize = source->maxSize;
	copy->size = source->size;
	for (i = 0; i < source->maxSize; i++) {
		copy->queue[i].value = source->queue[i].value;
		copy->queue[i].index = source->queue[i].index;
	}
	return copy;
}

void spBPQueueDestroy(SPBPQueue* source) {
	if (NULL == source)
		return;
	free(&(source->queue));
	free(source);
}

void spBPQueueClear(SPBPQueue* source) {
	assert(NULL != source);
	int i = 0;
	BPQueueElement emptyElement;
	emptyElement.index = EMPTY_BPQUEUE_ELEMENT_INDEX;
	emptyElement.value = EMPTY_BPQUEUE_ELEMENT_VALUE;

	for (i = 0; i < source->size; i++) {
		source->queue[i] = emptyElement;
	}
	source->size = 0;
	return;
}

int spBPQueueSize(SPBPQueue* source) {
	assert(NULL != source);
	return source->size;
}

int spBPQueueGetMaxSize(SPBPQueue* source) {
	assert(source != NULL);
	return source->maxSize;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value) {
	int i = 0;
	int j = 0;
	if (NULL == source || 0 > value || NULL == source->queue) {	//Values should be non negative
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	for (i = 0; i < (source->size); i++) {
		if (value >= source->queue[i].value) {
			break;
		}
	}
	for (; (i < (source->size)) && (value == source->queue[i].value); i++) {
		if (index >= source->queue[i].index) {
			break;
		}
	}
	if (spBPQueueIsFull(source)) {
		for (j = 0; j < i - 1; j++) {
			source->queue[j] = source->queue[j + 1];
		}
		if (i == 0) {//the queue is full and we trying to insert higher value than all the elements
			return SP_BPQUEUE_FULL;
		}
		source->queue[i - 1].index = index;
		source->queue[i - 1].value = value;
		return SP_BPQUEUE_FULL;
	} else {
		for (j = source->size - 1; j >= i; j--) {
			source->queue[j + 1] = source->queue[j];
		}
		source->queue[i].index = index;
		source->queue[i].value = value;
		source->size++;
		return SP_BPQUEUE_SUCCESS;
	}
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source) {
	BPQueueElement emptyElement;
	emptyElement.index = EMPTY_BPQUEUE_ELEMENT_INDEX;
	emptyElement.value = EMPTY_BPQUEUE_ELEMENT_VALUE;
	if (source == NULL) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if (spBPQueueIsEmpty(source)) {
		return SP_BPQUEUE_EMPTY;
	}
	source->queue[(source->size) - 1] = emptyElement;
	source->size--;
	return SP_BPQUEUE_SUCCESS;

}

SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res) {
	if (NULL == source || NULL == source->queue || NULL == res) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if (spBPQueueIsEmpty(source)) {
		res->index = EMPTY_BPQUEUE_ELEMENT_INDEX;
		res->value = EMPTY_BPQUEUE_ELEMENT_VALUE;
		//because we aren't able to peek from an empty queue
		return SP_BPQUEUE_EMPTY;
	}
	res->index = source->queue[source->size - 1].index;
	res->value = source->queue[source->size - 1].value;
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res) {
	if (NULL == source || NULL == source->queue || NULL == res) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if (spBPQueueIsEmpty(source)) {
		res->index = EMPTY_BPQUEUE_ELEMENT_INDEX;
		res->value = EMPTY_BPQUEUE_ELEMENT_VALUE;
		//because we aren't able to peek from an empty queue
		return SP_BPQUEUE_EMPTY;
	}
	res->index = source->queue[0].index;
	res->value = source->queue[0].value;
	return SP_BPQUEUE_SUCCESS;
}

double spBPQueueMinValue(SPBPQueue* source) {
	assert(source != NULL);
	assert(source->queue != NULL);
	if (spBPQueueIsEmpty(source)) {
		return -1;
	}
	return source->queue[source->size - 1].value;
}

double spBPQueueMaxValue(SPBPQueue* source) {
	assert(NULL != source);
	assert(source->queue != NULL);
	if (spBPQueueIsEmpty(source)) {
		return -1;
	}
	return source->queue[0].value;
}

bool spBPQueueIsEmpty(SPBPQueue* source) {
	assert(source!=NULL);
	if (source->size == 0) {
		return true;
	} else {
		return false;
	}
}

bool spBPQueueIsFull(SPBPQueue* source) {
	assert(source!=NULL);
	if (source->size < source->maxSize) {
		return false;
	} else {
		return true;
	}
}

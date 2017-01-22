/*
 * main_aux.cpp
 *
 *  Created on: 17 1 2017
 *      Author: ron
 */

#include "main_aux.h"

extern "C" {
//Use this syntax in-order to include C-header files
//HINT : You don't need to include other C header files here -> Maybe in sp_image_proc_util.c ? <-
#include "SPPoint.h"
#include "SPBPriorityQueue.h"
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
}

#define ENTER_IMG_DIR_MSG "Enter images directory path:\n"
#define IMG_PREFIX_MSG "Enter images prefix:\n"
#define IMG_SUFFIX_MSG "Enter images suffix:\n"
#define IMG_NUM_OF_MSG "Enter number of %s:\n"
#define IMG_ERR_NUM_OF_MSG "An error occurred - invalid number of %s\n"
#define EXIT_MSG "Exiting...\n"
#define QUERY_IMG_MSG "Enter a query image or # to terminate:\n"
#define ALLOC_FAIL "An error occurred - allocation failure\n"
#define MIN(a,b) ((a) < (b) ? a : b)
#define kClosest 5
#define flush fflush(NULL);
#define MAX_STR_LEN 1024

MAIN_MSG spEnterImgsPath(char* path) {
	if (NULL == path) {
		return SP_INVALID_ARGUMENT;
	}
	printf(ENTER_IMG_DIR_MSG);
	flush
	scanf("%s*c", path);
	flush
	return SP_SUCCESS;
}

MAIN_MSG spEnterImgPrefix(char* prefix) {
	if (NULL == prefix) {
		return SP_INVALID_ARGUMENT;
	}
	printf(IMG_PREFIX_MSG);
	flush
	scanf("%s*c", prefix);
	flush
	return SP_SUCCESS;
}

MAIN_MSG spEnterImgSuffix(char* suffix) {
	if (NULL == suffix) {
		return SP_INVALID_ARGUMENT;
	}
	printf(IMG_SUFFIX_MSG);
	flush
	scanf("%s*c", suffix);
	flush
	return SP_SUCCESS;
}

MAIN_MSG spEnterImgNum(int* imgNum) {
	if (NULL == imgNum) {
		return SP_INVALID_ARGUMENT;
	}
	char* imgNumStr;
	imgNumStr = (char*) calloc(MAX_STR_LEN, sizeof(*imgNumStr));

	if (NULL == imgNumStr) {
		printf(ALLOC_FAIL);
		flush
		//TODO free all
		return SP_OUT_OF_MEMORY;
	}
	printf(IMG_NUM_OF_MSG, "images");
	flush
	scanf("%s*c", imgNumStr);
	flush
	*imgNum = atoi(imgNumStr);
	free(imgNumStr);
	if (1 > *imgNum) {
		//TODO free all mem allocations
		printf(IMG_ERR_NUM_OF_MSG, "images");
		return SP_INVALID_ARGUMENT;
	}
	return SP_SUCCESS;

}

MAIN_MSG spEnterBinNum(int* BinNum) {
	if (NULL == BinNum) {
		return SP_INVALID_ARGUMENT;
	}
	char* imgBinNumStr;
	imgBinNumStr = (char*) calloc(MAX_STR_LEN, sizeof(*imgBinNumStr));

	if (NULL == imgBinNumStr) {
		return SP_OUT_OF_MEMORY;
	}
	printf(IMG_NUM_OF_MSG, "bins");
	flush
	scanf("%s*c", imgBinNumStr);
	flush
	*BinNum = atoi(imgBinNumStr);
	free(imgBinNumStr);
	if (1 > *BinNum || 255 < *BinNum) {
		//TODO free all mem allocations
		printf(IMG_ERR_NUM_OF_MSG, "bins");
		return SP_INVALID_ARGUMENT;
	}
	return SP_SUCCESS;
}

MAIN_MSG spEnterFeaturesNum(int* FeaturesNum) {
	if (NULL == FeaturesNum) {
		return SP_INVALID_ARGUMENT;
	}
	char* imgFeaNumStr;
	imgFeaNumStr = (char*) calloc(MAX_STR_LEN, sizeof(*imgFeaNumStr));

	if (NULL == imgFeaNumStr) {
		return SP_OUT_OF_MEMORY;
	}
	printf(IMG_NUM_OF_MSG, "features");
	flush
	scanf("%s*c", imgFeaNumStr);
	flush
	*FeaturesNum = atoi(imgFeaNumStr);
	free(imgFeaNumStr);
	if (1 > *FeaturesNum) {
		//TODO free all mem allocations
		printf(IMG_ERR_NUM_OF_MSG, "features");
		return SP_INVALID_ARGUMENT;
	}
	return SP_SUCCESS;
}

MAIN_MSG spMakeFullPath(char* fullPath, char* path, char* prefix, char* suffix,
		int imgCurrNum) {
	if (NULL == fullPath || NULL == path || NULL == prefix || NULL == suffix) {
		return SP_INVALID_ARGUMENT;
	}
	char buffer[MAX_STR_LEN];
	sprintf(buffer, "%d", imgCurrNum);
	strcat(fullPath, path);
	strcat(fullPath, prefix);
	strcat(fullPath, buffer);
	strcat(fullPath, suffix);
	return SP_SUCCESS;
}

MAIN_MSG spGetUserInput(char* path, char* prefix, char* suffix, int* imgNum,
		int* binNum, int* featuresNum) {
	MAIN_MSG spEnterImgsPath(char* path);
	MAIN_MSG spEnterImgPrefix(char* prefix);
	MAIN_MSG spEnterImgNum(int* imgNum);
	MAIN_MSG spEnterImgSuffix(char* suffix);
	MAIN_MSG spEnterBinNum(int* BinNum);
	MAIN_MSG spEnterFeaturesNum(int* FeaturesNum);
	return SP_SUCCESS;

}

MAIN_MSG spCalcHistAndSIFT(int numOfBins, int numOfImgs,
		int numOfFeaturesToExtract, char* fullPath) {

}

MAIN_MSG spCalcHist(int numOfBins, SPPoint*** dataBaseHist,
		const char* fullPath, int numOfImgs) {
	int i;
	char* path = (char*) malloc(MAX_STR_LEN * sizeof(char));
	for (i = 0; i < numOfImgs; i++) {
		sprintf(path, fullPath, i);
		dataBaseHist[i] = spGetRGBHist(path, i, numOfBins);
		if (dataBaseHist[i] == NULL) {
			//free resources
			return SP_OUT_OF_MEMORY;
		}
	}
	free(path);
	return SP_SUCCESS;
}

MAIN_MSG spCalcSift(int numOfFeaturesToExtract, SPPoint*** dataBaseFeatures,
		const char* fullPath, int numOfImgs, int *nFeaturesPerImage) {
	int i, nExtracted;
	char* path = (char*) malloc(MAX_STR_LEN * sizeof(char));
	for (i = 0; i < numOfImgs; i++) {
		sprintf(path, fullPath, i);
		dataBaseFeatures[i] = spGetSiftDescriptors(path, i,
				numOfFeaturesToExtract, &nExtracted);
		if (dataBaseFeatures[i] == NULL) {
			//free resources
			return SP_OUT_OF_MEMORY;
		}
		nFeaturesPerImage[i] = nExtracted;
	}
	free(path);
	return SP_SUCCESS;
}

//will get a relative path
//before this func check if not # aka exit and free all
MAIN_MSG spEnterQueryImg(char* queryPath) {
	if (NULL == queryPath) {
		return SP_INVALID_ARGUMENT;
	}
	printf(QUERY_IMG_MSG);
	flush
	scanf("%s*c", queryPath);
	flush
	if (queryPath[0] == '#') {
		//TODO free all allocations
		//TODO exit the program
		//TODO boof trrach shikshikshik the program will die
		printf(EXIT_MSG);
		return SP_EXIT;
	}
	return SP_SUCCESS;
}

MAIN_MSG spQueryImg(char* queryPath, SPPoint** dataBaseHist,
		SPPoint*** dataBaseFeatures) {

}

MAIN_MSG spReturnGlobalSearch(char* queryPath, int* imgNum,
		SPPoint*** dataBaseHist, int nBins) {
	if (NULL == queryPath || NULL == imgNum || NULL == dataBaseHist) {
		return SP_INVALID_ARGUMENT;
	}
	SPPoint** queryHist;
	int j = 0;
	SPBPQueue* globalQueue;
	globalQueue = spBPQueueCreate(5);
	if (NULL == globalQueue) {
		printf(ALLOC_FAIL);
		//TODO free mem alloc
		return SP_OUT_OF_MEMORY;
	}
	BPQueueElement* res;

	//TODO maybe convert query path to const char
	queryHist = spGetRGBHist(queryPath, MAX_STR_LEN, nBins);//TODO max its is nonsense

	for (j = 0; j < (*imgNum); j++) {
		spBPQueueEnqueue(globalQueue, j,
				spRGBHistL2Distance(queryHist, dataBaseHist[j]));
	}
	printf("Nearest images using global descriptors:\n");
	for (j = 0; j < 5; j++) {
		spBPQueuePeek(globalQueue, res);
		printf("%d, ", res->index);
		spBPQueueDequeue(globalQueue);
	}
	printf("\n");
	flush

	for (j = 0; j < NumOfChannels; j++) {		//free the query Hist DB
		spPointDestroy(queryHist[j]);
	}
	return SP_SUCCESS;
}

MAIN_MSG spReturnLocalSearch(char* queryPath, int* FeaturesNumToExtract,
		SPPoint*** dataBaseFeatures, int numOfImgs, int* nFeaturesPerImage) {
	if (NULL == queryPath || NULL == FeaturesNumToExtract
			|| NULL == dataBaseFeatures) {
		return SP_INVALID_ARGUMENT;
	}
	SPPoint** queryFea;
	int j = 0;
	int i = 0;

	int* numQeuryFeatures;
	numQeuryFeatures = (int*) malloc(sizeof(*numQeuryFeatures));
	if (NULL == numQeuryFeatures) {
		printf(ALLOC_FAIL);
		//TODO free mem alloc
		return SP_OUT_OF_MEMORY;
	}
	*numQeuryFeatures = 0;

	//TODO maybe convert query path to const char
	queryFea = spGetSiftDescriptors(queryPath, MAX_STR_LEN,
			*FeaturesNumToExtract, numQeuryFeatures);//TODO max its is nonsense
	if (NULL == queryFea) {
		printf(ALLOC_FAIL);
		//TODO free mem alloc
		return SP_OUT_OF_MEMORY;
	}

	int hits[numOfImgs] = { 0 };
	int* arr;

	for (j = 0; j < (*numQeuryFeatures); j++) {
		arr = spBestSIFTL2SquaredDistance(kClosest, queryFea[j],
				dataBaseFeatures, numOfImgs, nFeaturesPerImage);
		for (i = 0; i < kClosest; i++) {
			hits[arr[j]]++;
		}
	}
	SPPoint** pointHits;
	pointHits = (SPPoint**) malloc(numOfImgs * sizeof(*pointHits));
	for (j = 0; j < numOfImgs; j++) {
		pointHits[j] = spPointCreate((double*) (&hits[j]), 1, j);
	}
	qsort(pointHits[0], numOfImgs, sizeof(pointHits[0]), cmpfunc);

	printf("Nearest images using local descriptors:\n");
	for (j = 0; j < 5; j++) {
		printf("%d, ", spPointGetIndex(pointHits[j]));
	}
	printf("\n");
	flush

	for (j = 0; j < *numQeuryFeatures; j++) {		//free the query SIFT DB
		spPointDestroy (queryFea[j]);
	}
	free(queryFea);
	free(numQeuryFeatures);

	return SP_SUCCESS;
}

int cmpfunc(const void * a, const void * b) {
	//sort from high to low
	return (spPointGetAxisCoor((SPPoint*) b, 1)
			- spPointGetAxisCoor((SPPoint*) a, 1));
}

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
#define MIN(a,b) ((a) < (b) ? a : b)

MAIN_MSG spEnterImgsPath(char* path) {
	if (NULL == path) {
		return SP_INVALID_ARGUMENT;
	}
	printf(ENTER_IMG_DIR_MSG);
	scanf("%s*c", path);
	return SP_SUCCESS;
}

MAIN_MSG spEnterImgPrefix(char* prefix) {
	if (NULL == prefix) {
		return SP_INVALID_ARGUMENT;
	}
	printf(IMG_PREFIX_MSG);
	scanf("%s*c", prefix);
	return SP_SUCCESS;
}

MAIN_MSG spEnterImgSuffix(char* suffix) {
	if (NULL == suffix) {
		return SP_INVALID_ARGUMENT;
	}
	printf(IMG_SUFFIX_MSG);
	scanf("%s*c", suffix);
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
		return SP_OUT_OF_MEMORY;
	}
	printf(IMG_NUM_OF_MSG, "images");
	scanf("%s*c", imgNumStr);
	*imgNum = atoi(imgNumStr);
	free(imgNumStr);
	if (1 > *imgNum) {
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
		printf(ALLOC_FAIL);
		free(imgBinNumStr);
		return SP_OUT_OF_MEMORY;
	}
	printf(IMG_NUM_OF_MSG, "bins");
	scanf("%s*c", imgBinNumStr);
	*BinNum = atoi(imgBinNumStr);
	free(imgBinNumStr);
	if (1 > *BinNum || 255 < *BinNum) {
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
		printf(ALLOC_FAIL);
		return SP_OUT_OF_MEMORY;
	}
	printf(IMG_NUM_OF_MSG, "features");
	scanf("%s*c", imgFeaNumStr);
	*FeaturesNum = atoi(imgFeaNumStr);
	free(imgFeaNumStr);
	if (1 > *FeaturesNum) {
		free(imgFeaNumStr);
		printf(IMG_ERR_NUM_OF_MSG, "features");
		return SP_INVALID_ARGUMENT;
	}
	return SP_SUCCESS;
}

MAIN_MSG spMakeFullPath(char* fullPath, char* path, char* prefix, char* suffix) {
	if (NULL == fullPath || NULL == path || NULL == prefix || NULL == suffix) {
		return SP_INVALID_ARGUMENT;
	}
	memset(fullPath,0, sizeof (*fullPath));
	strcat(fullPath, path);
	strcat(fullPath, prefix);
	strcat(fullPath, "%d");
	strcat(fullPath, suffix);
	return SP_SUCCESS;
}

MAIN_MSG spGetUserInput(char* path, char* prefix, char* suffix, int* imgNum,
		int* binNum, int* featuresNum) {

	if (SP_SUCCESS != spEnterImgsPath(path)) {
		return SP_EXIT;
	}
	if(SP_SUCCESS!= spEnterImgPrefix(prefix)){
		return SP_EXIT;
	}
	if(SP_SUCCESS != spEnterImgNum(imgNum)){
		return SP_EXIT;
	}
	if(SP_SUCCESS!= spEnterImgSuffix(suffix)){
		return SP_EXIT;
	}
	if(SP_SUCCESS!= spEnterBinNum(binNum)){
		return SP_EXIT;
	}
	if(SP_SUCCESS!= spEnterFeaturesNum(featuresNum)){
		return SP_EXIT;
	}
	return SP_SUCCESS;
}

MAIN_MSG spCalcHist(int numOfBins, SPPoint*** dataBaseHist,
		const char* fullPath, int numOfImgs) {
	int i;
	char* path = (char*) malloc(MAX_STR_LEN * sizeof(char));
	if (NULL == path) {
		free(dataBaseHist);
		printf(ALLOC_FAIL);
		return SP_OUT_OF_MEMORY;
	}
	for (i = 0; i < numOfImgs; i++) {
		sprintf(path, fullPath, i);
		dataBaseHist[i] = spGetRGBHist(path, i, numOfBins);
		if (dataBaseHist[i] == NULL) {
			spDestroyDBhist(dataBaseHist,i);
			free(path);
			printf(ALLOC_FAIL);
			return SP_OUT_OF_MEMORY;
		}
	}
	free(path);
	return SP_SUCCESS;
}

MAIN_MSG spCalcSift(int numOfFeaturesToExtract, SPPoint*** dataBaseFeatures,
		const char* fullPath, int numOfImgs, int* nFeaturesPerImage) {
	int i, nExtracted;
	char* path = (char*) malloc(MAX_STR_LEN * sizeof(char));
	if (NULL == path) {
		free(dataBaseFeatures);
		printf(ALLOC_FAIL);
		return SP_OUT_OF_MEMORY;
	}
	for (i = 0; i < numOfImgs; i++) {
		sprintf(path, fullPath, i);
		dataBaseFeatures[i] = spGetSiftDescriptors(path, i,
				numOfFeaturesToExtract, &nExtracted);
		if (dataBaseFeatures[i] == NULL) {
			spDestroyDBsift(dataBaseFeatures,i,nFeaturesPerImage);
			free(path);
			printf(ALLOC_FAIL);
			return SP_OUT_OF_MEMORY;
		}
		nFeaturesPerImage[i] = nExtracted;
	}
	free(path);
	return SP_SUCCESS;
}

MAIN_MSG spEnterQueryImg(char* queryPath) {
	if (NULL == queryPath) {
		return SP_INVALID_ARGUMENT;
	}
	printf(QUERY_IMG_MSG);
	scanf("%s*c", queryPath);
	if (queryPath[0] == '#') {
		printf(EXIT_MSG);
		return SP_EXIT;
	}
	return SP_SUCCESS;
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
		return SP_OUT_OF_MEMORY;
	}
	BPQueueElement* res;
	res = (BPQueueElement*) malloc(sizeof(*res));
	if (NULL == res) {
		spBPQueueDestroy(globalQueue);
		printf(ALLOC_FAIL);
		return SP_OUT_OF_MEMORY;
	}
	queryHist = spGetRGBHist(queryPath, 0, nBins);// the query image index is not important so zero is used
	if (NULL == queryHist) {
		spBPQueueDestroy(globalQueue);
		free(res);
		printf(ALLOC_FAIL);
		return SP_OUT_OF_MEMORY;
	}
	for (j = 0; j < (*imgNum); j++) {
		spBPQueueEnqueue(globalQueue, j,
				spRGBHistL2Distance(queryHist, dataBaseHist[j]));
	}
	printf("Nearest images using global descriptors:\n");
	for (j = 0; j < kClosest; j++) {
		spBPQueuePeek(globalQueue, res);
		if(j== (kClosest-1)){
			printf("%d", res->index);
			break;
		}
		printf("%d, ", res->index);
		spBPQueueDequeue(globalQueue);
	}
	printf("\n");

	for (j = 0; j < NumOfChannels; j++) {		//free the query Hist DB
		spPointDestroy(queryHist[j]);
	}
	spBPQueueDestroy(globalQueue);
	free(res);
	return SP_SUCCESS;
}

MAIN_MSG spReturnLocalSearch(char* queryPath, int* FeaturesNumToExtract,
		SPPoint*** dataBaseFeatures, int numOfImgs, int* nFeaturesPerImage) {
	if (NULL == queryPath || NULL == FeaturesNumToExtract
			|| NULL == dataBaseFeatures) {
		return SP_INVALID_ARGUMENT;
	}
	SPPoint** queryFea;
	int j = 0, i = 0;
	int numQeuryFeatures = 0;
	int *hits;
	int* arr;
	SPBPQueue* localQueue;
	BPQueueElement* res;
	res = (BPQueueElement*) malloc(sizeof(*res));
	if (NULL == res) {
		printf(ALLOC_FAIL);
		return SP_OUT_OF_MEMORY;
	}

	queryFea = spGetSiftDescriptors(queryPath, 0,
			*FeaturesNumToExtract, &numQeuryFeatures);// the query image index is not important so zero is used
	if (NULL == queryFea) {
		free(res);
		printf(ALLOC_FAIL);
		return SP_OUT_OF_MEMORY;
	}

	hits = (int*) calloc(numOfImgs, sizeof (int));
	if (NULL == hits) {
		free(res);
		for(i=0;i<numQeuryFeatures;i++){
			spPointDestroy(queryFea[i]);
		}
		free(queryFea);
		printf(ALLOC_FAIL);
		return SP_OUT_OF_MEMORY;
	}
	for (j = 0; j < numQeuryFeatures; j++) {
		arr = spBestSIFTL2SquaredDistance(kClosest, queryFea[j],
				dataBaseFeatures, numOfImgs, nFeaturesPerImage);
		if(NULL == arr){
			free(hits);
			free(res);
			for(i=0;i<numQeuryFeatures;i++){
				spPointDestroy(queryFea[i]);
			}
			free(queryFea);
			printf(ALLOC_FAIL);
			return SP_OUT_OF_MEMORY;
		}
		for (i = 0; i < kClosest; i++) {
			hits[arr[i]]++;
		}
		free(arr);
	}

	localQueue = spBPQueueCreate(kClosest);
	if(NULL == localQueue){
		free(hits);
		free(res);
		for(i=0;i<numQeuryFeatures;i++){
			spPointDestroy(queryFea[i]);
		}
		free(queryFea);
		printf(ALLOC_FAIL);
		return SP_OUT_OF_MEMORY;

	}
	for (j = 0; j < numOfImgs; j++) {
		spBPQueueEnqueue(localQueue, j, (double) ((kClosest * numQeuryFeatures) - hits[j]));
	}
	free(hits);

	printf("Nearest images using local descriptors:\n");
	for (j = 0; j < kClosest; j++) {
		spBPQueuePeek(localQueue, res);
		if(j== (kClosest-1)){
			printf("%d", res->index);
			break;
		}
		printf("%d, ", res->index);
		spBPQueueDequeue(localQueue);
	}
	printf("\n");

	for (j = 0; j < numQeuryFeatures; j++) {		//free the query SIFT DB
		spPointDestroy(queryFea[j]);
	}
	free(queryFea);
	spBPQueueDestroy(localQueue);
	free(res);

	return SP_SUCCESS;
}

MAIN_MSG spDestroyDBsift(SPPoint*** DB,int imgNum,int* nFeaturesPerImage){
	int i=0;
	int j=0;
	if(NULL == DB || NULL==nFeaturesPerImage){
		return SP_INVALID_ARGUMENT;
	}
	for(i=0;i<imgNum;i++){
		for(j=0;j<nFeaturesPerImage[i];j++){
			spPointDestroy(DB[i][j]);
		}
		free(DB[i]);
	}
	free(DB);
	return SP_SUCCESS;
}

MAIN_MSG spDestroyDBhist(SPPoint*** DB,int imgNum){
	int i=0;
	int j=0;
	if(NULL == DB){
		return SP_INVALID_ARGUMENT;
	}
	for(i=0;i<imgNum;i++){
		for(j=0;j<NumOfChannels;j++){
			spPointDestroy(DB[i][j]);
		}
		free(DB[i]);
	}
	free(DB);
	return SP_SUCCESS;
}

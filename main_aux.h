/*
 * main_aux.h
 *
 *  Created on: 11 1 2017
 *      Author: ronkashi
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

#include "sp_image_proc_util.h"

typedef enum sp_main_aux_msg_t {
	SP_SUCCESS, SP_OUT_OF_MEMORY, SP_INVALID_ARGUMENT, SP_EXIT
} MAIN_MSG;

extern "C" {
//Use this syntax in-order to include C-header files
#include "SPPoint.h"
}
#define kClosest 5
#define flush fflush(NULL);
#define MAX_STR_LEN 1024
#define ALLOC_FAIL "An error occurred - allocation failure\n"


/*
 * Ask the user to enter a string representing the path of the images directory
 * (more information will be given shortly): "Enter images directory path:\n"
 * The directory contains the images that we will be searching from.
 * All images names must be the same except for their indexes.
 * For examples, the directory: "./images/"
 * may contain the following images: "img0.jpg","img1.jpg"...
 */
MAIN_MSG spEnterImgsPath(char* path);

MAIN_MSG spEnterImgPrefix(char* prefix);

MAIN_MSG spEnterImgSuffix(char* suffix);

MAIN_MSG spEnterImgNum(int* imgNum);

MAIN_MSG spEnterBinNum(int* BinNum);

MAIN_MSG spEnterFeaturesNum(int* FeaturesNum);

MAIN_MSG spMakeFullPath(char* fullPath, char* path, char* prefix, char* suffix);

MAIN_MSG spGetUserInput(char* path, char* prefix, char* suffix, int* imgNum,
		int* binNum, int* featuresNum);

MAIN_MSG spCalcHist(int numOfBins, SPPoint*** dataBaseHist,
		const char* fullPath, int numOfImgs);

MAIN_MSG spCalcSift(int numOfFeaturesToExtract, SPPoint*** dataBaseFeatures,
		const char* fullPath, int numOfImgs, int *nFeaturesPerImage);

//before this func check if not # aka exit and free all
MAIN_MSG spEnterQueryImg(char* queryPath); //will get a relative path

MAIN_MSG spQueryImg(char* queryPath, SPPoint*** dataBaseHist,
		SPPoint*** dataBaseFeatures);

MAIN_MSG spReturnGlobalSearch(char* queryPath, int* imgNum,
		SPPoint*** dataBaseHist, int nBins);

MAIN_MSG spReturnLocalSearch(char* queryPath, int* FeaturesNumToExtract,
		SPPoint*** dataBaseFeatures, int numOfImgs, int* nFeaturesPerImage);

int cmpfunc(const void * a, const void * b);

MAIN_MSG freeMemInput (char* path, char* prefix, char* suffix, int* imgNum,
		int* binNum, int* featuresNum);
MAIN_MSG spDestroyDB(SPPoint*** DB,int imgNum);

#endif /* MAIN_AUX_H_ */

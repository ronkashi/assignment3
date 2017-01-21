/*
 * main_aux.cpp
 *
 *  Created on: 17 1 2017
 *      Author: ron
 */


#include "main_aux.h"
#include <stdio.h>


extern "C" {
//Use this syntax in-order to include C-header files
//HINT : You don't need to include other C header files here -> Maybe in sp_image_proc_util.c ? <-
	#include "SPPoint.h"
	#include <assert.h>
	#include <stdlib.h>
	#include <malloc.h>
	#include <stdlib.h>
}
#define ENTER_IMG_DIR_MSG "Enter images directory path:\n"
#define IMG_PREFIX_MSG "Enter images prefix:\n"
#define IMG_SUFFIX_MSG "Enter images suffix:\n"
#define IMG_NUM_OF_MSG "Enter number of %s:\n"
#define IMG_ERR_NUM_OF_MSG "An error occurred - invalid number of %s\n"
#define flush fflush(NULL);



MAIN_MSG spEnterImgsPath(char* path){
	if(NULL==path){
		return SP_INVALID_ARGUMENT;
	}
	printf(ENTER_IMG_DIR_MSG);
	flush
	scanf("%s",path);
	flush
	return SP_SUCCESS;
}

MAIN_MSG spEnterImgPrefix(char* prefix){
	if(NULL==prefix){
		return SP_INVALID_ARGUMENT;
	}
	printf(IMG_PREFIX_MSG);
	flush
	scanf("%s",prefix);
	flush
	return SP_SUCCESS;
}

MAIN_MSG spEnterImgSuffix(char* suffix){
	if(NULL==suffix){
		return SP_INVALID_ARGUMENT;
	}
	printf(IMG_SUFFIX_MSG);
	flush
	scanf("%s",suffix);
	flush
	return SP_SUCCESS;
}

MAIN_MSG spEnterImgNum(int* imgNum){
	char* imgNumStr;
	imgNumStr=(char*)calloc(1024,sizeof(*imgNumStr));

	if(NULL==imgNumStr){
		return SP_OUT_OF_MEMORY;
	}
	printf(IMG_NUM_OF_MSG,"images");
	flush
	scanf("%s",imgNumStr);
	flush
	*imgNum=atoi(imgNumStr);
	free(imgNumStr);
	if(1>*imgNum){
		//TODO free all mem allocations
		printf(IMG_ERR_NUM_OF_MSG,"images");
		return SP_INVALID_ARGUMENT;
	}
	return SP_SUCCESS;

}

MAIN_MSG spEnterBinNum(int* BinNum){
	char* imgBinNumStr;
	imgBinNumStr=(char*)calloc(1024,sizeof(*imgBinNumStr));

	if(NULL==imgBinNumStr){
		return SP_OUT_OF_MEMORY;
	}
	printf(IMG_NUM_OF_MSG,"bins");
	flush
	scanf("%s",imgBinNumStr);
	flush
	*BinNum=atoi(imgBinNumStr);
	free(imgBinNumStr);
	if(1>*BinNum || 255<*BinNum){
		//TODO free all mem allocations
		printf(IMG_ERR_NUM_OF_MSG,"bins");
		return SP_INVALID_ARGUMENT;
	}
	return SP_SUCCESS;
}

MAIN_MSG spEnterFeaturesNum(int* FeaturesNum){
	char* imgFeaNumStr;
	imgFeaNumStr=(char*)calloc(1024,sizeof(*imgFeaNumStr));

	if(NULL==imgFeaNumStr){
		return SP_OUT_OF_MEMORY;
	}
	printf(IMG_NUM_OF_MSG,"features");
	flush
	scanf("%s",imgFeaNumStr);
	flush
	*FeaturesNum=atoi(imgFeaNumStr);
	free(imgFeaNumStr);
	if(1>*FeaturesNum){
		//TODO free all mem allocations
		printf(IMG_ERR_NUM_OF_MSG,"features");
		return SP_INVALID_ARGUMENT;
	}
	return SP_SUCCESS;
}

MAIN_MSG spMakeFullPath(char* fullPath, char* path, char* prefix, char* suffix,int imgCurrNum){
	if(NULL== fullPath || NULL==path || NULL== prefix || NULL== suffix){
		return SP_INVALID_ARGUMENT;
	}
	char buffer[1024];
	printf(buffer,"%d",imgCurrNum);
//	fullPath=strcat(strcat(path,prefix),strcat(buffer,suffix);

	//TODO comlete with atoi
	free(buffer);
	return SP_SUCCESS;
}

MAIN_MSG spGetUserInput(char* path, char* prefix, char* suffix, int* imgNum,
		int* binNum, int* featuresNum){

}

MAIN_MSG spCalcHistAndSIFT(int numOfBins, int numOfImgs,
		int numOfFeaturesToExtract, char* fullPath){

}

MAIN_MSG spCalcHist(int numOfBins, SPPoint** dataBaseHist, char* fullPath){

}

MAIN_MSG spCalcSift(int numOfFeaturesToExtract, SPPoint*** dataBaseFeatures,
		char* fullPath){

}
//will get a relative path
//before this func check if not # aka exit and free all
MAIN_MSG spEnterQueryImg(char* queryPath){

}

MAIN_MSG spQueryImg(char* queryPath, SPPoint** dataBaseHist,
		SPPoint*** dataBaseFeatures){

}

MAIN_MSG spReturnGlobalSearch(){

}

MAIN_MSG spReturnLocalSearch(){

}


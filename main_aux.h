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
#include "SPPoint.h"
}
#define kClosest 5
#define MAX_STR_LEN 1024
#define ALLOC_FAIL "An error occurred - allocation failure\n"
#define NumOfChannels 3


/*
 * Ask the user to enter a string representing the path of the images directory by printing 
 * "Enter images directory path:\n" to stdout. The input string in stdin will be stored in path
 *
 * @param path - The string where the user input string will be stored
 * @return SP_INVALID_ARGUMENT if path is NULL, SP_SUCCESS otherwise
 */
MAIN_MSG spEnterImgsPath(char* path);

/*
 * Ask the user to enter a string representing the prefix of the image file name by printing 
 * "Enter images prefix:\n" to stdout. The input string in stdin will be stored in prefix
 *
 * @param prefix - The string where the user input string will be stored
 * @return SP_INVALID_ARGUMENT if prefix is NULL, SP_SUCCESS otherwise
 */
MAIN_MSG spEnterImgPrefix(char* prefix);

/*
 * Ask the user to enter a string representing the suffix of the image file name by printing 
 * "Enter images suffix:\n" to stdout. The input string in stdin will be stored in suffix
 *
 * @param suffix - The string where the user input string will be stored
 * @return SP_INVALID_ARGUMENT if suffix is NULL, SP_SUCCESS otherwise
 */
MAIN_MSG spEnterImgSuffix(char* suffix);

/*
 * Ask the user to enter the number of images in the database that the search will be 
 * performed on by printing "Enter number of images:\n" to stdout. The input number in 
 * stdin will be stored in imgNum
 *
 * @param imgNum - The pointer to the int where the user input number will be stored
 * @return SP_INVALID_ARGUMENT if imgNum is NULL or if the user input is less than 1, SP_SUCCESS otherwise
 */
MAIN_MSG spEnterImgNum(int* imgNum);

/*
 * Ask the user to enter the number of bins that will be used when calculating the histograms 
 * for the images by printing "Enter number of bins:\n" to stdout. The input number in 
 * stdin will be stored in BinNum
 *
 * @param BinNum - The pointer to the int where the user input number will be stored
 * @return SP_INVALID_ARGUMENT if BinNum is NULL or if the user input is less than 1 or greater than 255,
 * SP_SUCCESS otherwise
 */
MAIN_MSG spEnterBinNum(int* BinNum);

/*
 * Ask the user to enter the number of features that will be extracted when calculating the SIFT descriptors 
 * for the images by printing "Enter number of features:\n" to stdout. The input number in 
 * stdin will be stored in FeaturesNum
 *
 * @param FeaturesNum - The pointer to the int where the user input number will be stored
 * @return SP_INVALID_ARGUMENT if FeaturesNum is NULL or if the user input is less than 1,
 * SP_SUCCESS otherwise
 */
MAIN_MSG spEnterFeaturesNum(int* FeaturesNum);

/*
 * Gets the different parts of the path and creates from them the full image path, including "%d"
 *
 * @param fullPath - The pointer to the string where the full path will be stored
 * @param path - The pointer to the string where the user input path is stored
 * @param prefix - The pointer to the string where the user input image prefix is stored
 * @param suffix - The pointer to the string where the user input image suffix is stored
 * @return SP_INVALID_ARGUMENT if one of the parameters is NULL,
 * SP_SUCCESS otherwise
 */
MAIN_MSG spMakeFullPath(char* fullPath, char* path, char* prefix, char* suffix);

/*
 * Gets the all of the user input by calling on each of the previous functions in turn
 *
 * @param path - The pointer to the string where the user input path will be stored
 * @param prefix - The pointer to the string where the user input image prefix will be stored
 * @param suffix - The pointer to the string where the user input image suffix will be stored
 * @param imgNum - The pointer to the int where the user input number of images will be stored
 * @param binNum - The pointer to the int where the user input number of bins will be stored
 * @param featuresNum - The pointer to the int where the user input number of features will be stored
 * @return SP_EXIT if one of the inputs is invalid, SP_SUCCESS otherwise
 */
MAIN_MSG spGetUserInput(char* path, char* prefix, char* suffix, int* imgNum,
		int* binNum, int* featuresNum);

/*
 * Calculates the RGB histogram for each of the images in the search directory
 *
 * @param numOfBins - The number of bins that each histogram will contain
 * @param dataBaseHist - The array where each RGB hist will be stored
 * @param fullPath - The pointer to the string where the user input full image path is stored
 * @param numOfImgs - The number of images for which the image search will be performed
 * @return SP_OUT_OF_MEMORY if there is an allocation failure, SP_SUCCESS otherwise
 */
MAIN_MSG spCalcHist(int numOfBins, SPPoint*** dataBaseHist,
		const char* fullPath, int numOfImgs);

/*
 * Calculates the SIFT descriptors for each of the images in the search directory
 *
 * @param numOfFeaturesToExtract - The number of features that we will try to extract for each image
 * @param dataBaseFeatures - The array where each images feture array will be stored
 * @param fullPath - The pointer to the string where the user input full image path is stored
 * @param numOfImgs - The number of images for which the image search will be performed
 * @param nFeaturesPerImage - The array where the actual number of features that were extracted for each image is stored
 * @return SP_OUT_OF_MEMORY if there is an allocation failure, SP_SUCCESS otherwise
 */
MAIN_MSG spCalcSift(int numOfFeaturesToExtract, SPPoint*** dataBaseFeatures,
		const char* fullPath, int numOfImgs, int *nFeaturesPerImage);

/*
 * Ask the user to enter a string representing the full path of the query image by printing
 * "Enter a query image or # to terminate:\n" to stdout. The input string in stdin will be stored in queryPath
 *
 * @param queryPath - The string where the user input string will be stored
 * @return SP_EXIT if the user inputs "#", SP_SUCCESS otherwise
 */
MAIN_MSG spEnterQueryImg(char* queryPath);

/*
 * Calculates the RGB histogram of the query image, finds the 5 closest search images based on the 
 * L2 squred distances between the histograms and prints their indexes to stdout in order
 *
 * @param queryPath - The pointer to the string where the path of the query image is stored
 * @param imgNum - A pointer to an int where the number of search images is stored
 * @param dataBaseHist - The array where the RGB histograms for each of the images is stored
 * @param nBins - The number of bins that are in each histogram
 * @return SP_OUT_OF_MEMORY if there is an allocation failure, SP_SUCCESS otherwise
 */
MAIN_MSG spReturnGlobalSearch(char* queryPath, int* imgNum,
		SPPoint*** dataBaseHist, int nBins);

/*
 * Calculates the SIFT descriptors of the query image, finds the 5 closest search images based on the 
 * L2 squred distances between the features and prints their indexes to stdout in order
 *
 * @param queryPath - The pointer to the string where the path of the query image is stored
 * @param FeaturesNumToExtract - A pointer to an int where the number features that we will try to extract is stored
 * @param dataBaseFeatures - The array where the SIFT features for each of the images is stored
 * @param numOfImgs - The number of images that we are searching through
 * @param nFeaturesPerImage - The array where the actual number of features that were extracted for each image is stored
 * @return SP_OUT_OF_MEMORY if there is an allocation failure, SP_SUCCESS otherwise
 */
MAIN_MSG spReturnLocalSearch(char* queryPath, int* FeaturesNumToExtract,
		SPPoint*** dataBaseFeatures, int numOfImgs, int* nFeaturesPerImage);

/*
 * Frees all of the resources that were allocated for the SIFT features database
 *
 * @param DB - The array of the sift features of each image that needs to be freed
 * @param imgNum - The number of images that we are searching through
 * @param nFeaturesPerImage - The array where the actual number of features that were extracted for each image is stored
 * @return SP_INVALID_ARGUMENT if DB or nFeaturesPerImage is equal NULL, SP_SUCCESS otherwise
 */
MAIN_MSG spDestroyDBsift(SPPoint*** DB,int imgNum,int* nFeaturesPerImage);

/*
 * Frees all of the resources that were allocated for the image histogram database
 *
 * @param DB - The array of the histograms of each image that needs to be freed
 * @param imgNum - The number of images that we are searching through
 * @return SP_INVALID_ARGUMENT if DB is equal NULL, SP_SUCCESS otherwise
 */
MAIN_MSG spDestroyDBhist(SPPoint*** DB,int imgNum);

#endif /* MAIN_AUX_H_ */

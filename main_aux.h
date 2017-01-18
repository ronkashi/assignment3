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
	SP_OUT_OF_MEMORY,
	SP_INVALID_ARGUMENT,
	SP_SUCCESS
} MAIN_MSG;

extern "C" {
//Use this syntax in-order to include C-header files
}
/*
 * Ask the user to enter a string representing the path of the images directory
 * (more information will be given shortly): "Enter images directory path:\n"
 * The directory contains the images that we will be searching from.
 * All images names must be the same except for their indexes.
 * For examples, the directory: “./images/” may contain
 * the following images: “img0.jpg”, “img1.jpg”, “img2.jpg”…
 */
MAIN_MSG spEnterImgsPath(char* path);

MAIN_MSG spEnterImgPreffix(char* preffix);

MAIN_MSG spEnterImgNum(char* preffix);

MAIN_MSG spEnterImgSuffix(char* suffix);

MAIN_MSG spEnterBinNum(int* BinNum);

MAIN_MSG spEnterFeaturesNum(int* FeaturesNum);

MAIN_MSG spCalcHistAndSIFT(int numOfBins, int numOfImgs, int numOfFeaturesToExtract ,char* fullPath);

MAIN_MSG spCalcHist(int numOfBins ,char* fullPath);//per image

MAIN_MSG spCalcSift(int numOfFeaturesToExtract ,char* fullPath);//per image

MAIN_MSG spQueryImg(char* fullPathQuery); //another input is our mega DB with the hist & SIFTs





#endif /* MAIN_AUX_H_ */

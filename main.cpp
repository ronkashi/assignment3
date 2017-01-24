/*
 * main.cpp
 *
 *  Created on: 8 áéðå 2017
 *      Author: Arnit
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "sp_image_proc_util.h"
#include "main_aux.h"

int main() {

	char path[MAX_STR_LEN];
	char prefix[MAX_STR_LEN];
	char suffix[MAX_STR_LEN];
	char fullPath[MAX_STR_LEN];
	char queryPath[MAX_STR_LEN];
	bool flag = true;
	int imgNum = 1, binNum = 0, featuresNum = 0, *nFeaturesPerImage;
	SPPoint*** dataBaseHist;
	SPPoint*** dataBaseFeatures;

	if (SP_EXIT
			== spGetUserInput(path, prefix, suffix, &imgNum, &binNum,
					&featuresNum)) {
		return 0;
	}

	nFeaturesPerImage = (int*) malloc(imgNum * sizeof(int));
	if (NULL == nFeaturesPerImage) {
		printf(ALLOC_FAIL);
		return 0;
	}

	dataBaseHist = (SPPoint***) calloc(imgNum, sizeof(*dataBaseHist));
	if (NULL == dataBaseHist) {
		free(nFeaturesPerImage);
		printf(ALLOC_FAIL);
		return 0;
	}
	dataBaseFeatures = (SPPoint***) calloc(imgNum, sizeof(*dataBaseFeatures));
	if (NULL == dataBaseFeatures) {
		free(dataBaseHist);
		free(nFeaturesPerImage);
		printf(ALLOC_FAIL);
		return 0;
	}

	spMakeFullPath(fullPath, path, prefix, suffix);

	if (SP_SUCCESS != spCalcHist(binNum, dataBaseHist, fullPath, imgNum)) {
		free(dataBaseFeatures);
		free(nFeaturesPerImage);
	}

	if (SP_SUCCESS
			!= spCalcSift(featuresNum, dataBaseFeatures, fullPath, imgNum,
					nFeaturesPerImage)) {
		spDestroyDBhist(dataBaseHist, imgNum);
		free(nFeaturesPerImage);

	}

	while ((SP_EXIT != spEnterQueryImg(queryPath)) && flag) {
		if (SP_SUCCESS
				!= spReturnGlobalSearch(queryPath, &imgNum, dataBaseHist,
						binNum)) {
			flag = false;
		}
		if (flag) {
			if (SP_SUCCESS
					!= spReturnLocalSearch(queryPath, &featuresNum,
							dataBaseFeatures, imgNum, nFeaturesPerImage)) {
				flag=false;
			}
		}

	}
	spDestroyDBsift(dataBaseFeatures, imgNum, nFeaturesPerImage);
	spDestroyDBhist(dataBaseHist, imgNum);
	free(nFeaturesPerImage);

	return 0;

}

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
using namespace cv;
using namespace std;

int showPictureColor(const char* str);

int main() {
	SPPoint** rgbHistA;
	SPPoint** rgbHistB;
	SPPoint*** siftArray;
	SPPoint*** histArray;
	int* nFeatures;
	int n = 17, k = 5, i, j;
	MAIN_MSG msg;
	const char* path = "images\\img%d.png";
	rgbHistA=spGetRGBHist("images\\img3.png",1, 256);
	rgbHistB=spGetRGBHist("images\\img4.png",2, 256);
	printf("%d\n",spPointGetDimension(rgbHistA[0]));
	printf("%f\n",spPointGetAxisCoor(rgbHistA[0],0));

	printf("the dis between %f\n",spRGBHistL2Distance(rgbHistA, rgbHistB));
	free(rgbHistA);
	free(rgbHistB);

	histArray = (SPPoint***) malloc(n * sizeof (SPPoint**));
	msg = spCalcHist(256, histArray, path, n);
	if(msg == SP_SUCCESS) {
		printf("success!\n");
	}

	siftArray = (SPPoint***) malloc(n * sizeof(SPPoint**));
	nFeatures = (int*) malloc(n * sizeof(int));
	msg = spCalcSift(k, siftArray, path, n, nFeatures);
	if(msg == SP_SUCCESS) {
		printf("success!\n");
	}
	for(i=0; i<n; i++) {
		for(j=0; j<3; j++) {
			spPointDestroy(histArray[i][j]);
		}
		free(histArray[i]);
	}
	free(histArray);

	for(i=0; i<n; i++) {
		for(j=0; j<nFeatures[i]; j++) {
			spPointDestroy(siftArray[i][j]);
		}
		free(siftArray[i]);
	}
	free(siftArray);
	free(nFeatures);
	return showPictureColor("images\\img3.png");
}

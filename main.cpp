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
using namespace cv;
using namespace std;

int showPictureColor(const char* str);

int main() {
	SPPoint** rgbHistA;
	SPPoint** rgbHistB;
	SPPoint*** siftArray;
	int* nFeatures;
	int* res;
	int feat, n = 2, k = 5;
	rgbHistA=spGetRGBHist("images\\img3.png",1, 256);
	rgbHistB=spGetRGBHist("images\\img4.png",2, 256);
	printf("%d\n",spPointGetDimension(rgbHistA[0]));
	printf("%f\n",spPointGetAxisCoor(rgbHistA[0],0));

	printf("the dis between %f\n",spRGBHistL2Distance(rgbHistA, rgbHistB));
	free(rgbHistA);
	free(rgbHistB);


	siftArray = (SPPoint***) malloc(n * sizeof(SPPoint**));
	nFeatures = (int*) malloc(n * sizeof(int));
	siftArray[0]=spGetSiftDescriptors("images\\img3.png",0, 256, &feat);
	nFeatures[0] = feat;
	siftArray[1]=spGetSiftDescriptors("images\\img4.png",1, 256, &feat);
	nFeatures[1] = feat;
	res = spBestSIFTL2SquaredDistance(k, siftArray[1][110], siftArray, n, nFeatures);
	printf("%d %d %d %d %d", res[0], res[1], res[2], res[3], res[4]);
	free(siftArray);
	free(nFeatures);
	free(res);
	return showPictureColor("images\\img3.png");
}

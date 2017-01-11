/*
 * main.cpp
 *
 *  Created on: 8 αιπε 2017
 *      Author: Arnit
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "sp_image_proc_util.h"
using namespace cv;
using namespace std;



int main() {
	SPPoint** rgbHistA;
	SPPoint** rgbHistB;
	rgbHistA=spGetRGBHist("images\\img3.png",1, 256);
	rgbHistB=spGetRGBHist("images\\img4.png",2, 256);
	printf("%d\n",spPointGetDimension(rgbHistA[0]));
	printf("%f\n",spPointGetAxisCoor(rgbHistA[0],0));

	printf("the dis between %f\n",spRGBHistL2Distance(rgbHistA, rgbHistB));
	free(rgbHistA);
	free(rgbHistB);
	return showPictureColor("images\\img3.png");
}

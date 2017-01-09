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
	rgbHistA=spGetRGBHist("queryA.png",1, 256);
	rgbHistB=spGetRGBHist("queryB.png",2, 256);
	printf("%d\n",spPointGetDimension(rgbHistA[0]));
	printf("%f\n",spPointGetAxisCoor(rgbHistA[0],0));

	printf("the dis between %f\n",spRGBHistL2Distance(rgbHistA, rgbHistB));
	return showPictureColor("queryA.png");
}

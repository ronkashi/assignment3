/*
 * sp_image_proc_util.c
 *
 *  Created on: 4 1 2017
 *      Author: ron kashi
 */

#include "sp_image_proc_util.h"

#include <opencv2/imgproc.hpp>//calcHist
#include <vector>
#include <iostream>
using namespace cv;

extern "C" {
//Use this syntax in-order to include C-header files
//HINT : You don't need to include other C header files here -> Maybe in sp_image_proc_util.c ? <-
#include "SPPoint.h"
#include <assert.h>
}

SPPoint** spGetRGBHist(const char* str, int imageIndex, int nBins) {
	Mat src;
	SPPoint** res;
	int i=0;
	int j=0;
	int N=3;
	double arr[nBins] = {0};
	src = imread(str, CV_LOAD_IMAGE_COLOR);
	if (src.empty())
		return NULL;	//a problem with loading image
	res = (SPPoint**) malloc(3 * sizeof(*res));
	if (NULL == res) {
		return NULL;
	}
//	res[0]=spPointCreate(arr,nBins,imageIndex);//red
//	res[1]=spPointCreate(arr,nBins,imageIndex);//green
//	res[2]=spPointCreate(arr,nBins,imageIndex);//blue

	/// Separate the image in 3 places ( B, G and R )
	std::vector<Mat> bgr_planes;
	split(src, bgr_planes);

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	/// Set the other parameters:
	int nImages = 1;

	//Output
	Mat hist[N];

	/// Compute the histograms:
	/// The results will be store in b_hist,g_hist,r_hist.
	/// The output type of the matrices is CV_32F (float)
	calcHist(&bgr_planes[0], nImages, 0, Mat(), hist[0], 1, &nBins, &histRange);
	calcHist(&bgr_planes[1], nImages, 0, Mat(), hist[1], 1, &nBins, &histRange);
	calcHist(&bgr_planes[2], nImages, 0, Mat(), hist[2], 1, &nBins, &histRange);
	//broken not working
//	printf("the first hist %f\n",r_hist.at<float>(3,3));
	for (j=0;j<N;j++){
		for (i=0;i<nBins;i++){
			arr[i]=hist[j].at<float>(i);
		}
		res[N-1-j]= spPointCreate(arr, nBins, imageIndex);
	}
//
//	res[2] = spPointCreate((double*) hist[0].data, nBins, imageIndex);	//blue
//	res[1] = spPointCreate((double*) hist[1].data, nBins, imageIndex);	//green
//	res[0] = spPointCreate((double*) hist[2].data, nBins, imageIndex);	//red
	return res;
}
double spRGBHistL2Distance(SPPoint** rgbHistA, SPPoint** rgbHistB) {
	int i = 0;
	double sum = 0;
	if (NULL == rgbHistA || NULL == rgbHistB || NULL == rgbHistA[0]
			|| NULL == rgbHistB[0]) {
		return -1;
	}
	for (i = 0; i < 3; i++) {
		sum += spPointL2SquaredDistance(rgbHistA[i], rgbHistB[i]);
		printf("the %d dis between %f\n",i,sum);
	}
	sum = sum / 3;
	return sum;
}
SPPoint** spGetSiftDescriptors(const char* str, int imageIndex,
		int nFeaturesToExtract, int *nFeatures) {

}

int* spBestSIFTL2SquaredDistance(int kClosest, SPPoint* queryFeature,
		SPPoint*** databaseFeatures, int numberOfImages,
		int* nFeaturesPerImage) {

}

int showPictureColor(const char* str) {
	Mat image;
	image = imread(str, CV_LOAD_IMAGE_COLOR);   // Read the file

	if (!image.data)   // Check for invalid input
	{
		printf("Could not open or find the image\n");
		return -1;
	}

	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", image); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}

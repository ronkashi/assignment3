/*
 * sp_image_proc_util.c
 *
 *  Created on: 4 1 2017
 *      Author: ron kashi
 */

#include "sp_image_proc_util.h"

#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>

#include <opencv2/imgproc.hpp>//calcHist
#include <opencv2/core.hpp>//Mat
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
using namespace cv;
using namespace std;

#define NumOfChannels 3
#define IMG_NOT_LOADED "Image cannot be loaded - %s:\n"
extern "C" {
//Use this syntax in-order to include C-header files
//HINT : You don't need to include other C header files here -> Maybe in sp_image_proc_util.c ? <-
#include "SPPoint.h"
#include "SPBPriorityQueue.h"
#include <assert.h>
}

SPPoint** spGetRGBHist(const char* str, int imageIndex, int nBins) {
	Mat src;
	SPPoint** res;
	int i = 0, j = 0;
	double *arr;
	src = imread(str, CV_LOAD_IMAGE_COLOR);
	if (src.empty()){
		printf(IMG_NOT_LOADED,str); //TODO free all mallocs and exit
		return NULL;	//a problem with loading image
	}
	res = (SPPoint**) malloc(3 * sizeof(*res));
	if (NULL == res) {
		return NULL;
	}

	/// Separate the image in 3 places ( B, G and R )
	std::vector<Mat> bgr_planes;
	split(src, bgr_planes);

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	/// Set the other parameters:
	int nImages = 1;

	//Output
	Mat hist[NumOfChannels];

	/// Compute the histograms:
	/// The results will be store in b_hist,g_hist,r_hist.
	/// The output type of the matrices is CV_32F (float)
	calcHist(&bgr_planes[0], nImages, 0, Mat(), hist[0], 1, &nBins, &histRange);
	calcHist(&bgr_planes[1], nImages, 0, Mat(), hist[1], 1, &nBins, &histRange);
	calcHist(&bgr_planes[2], nImages, 0, Mat(), hist[2], 1, &nBins, &histRange);
	arr = (double*) malloc(nBins * sizeof (double));
	for (j = 0; j < NumOfChannels; j++) {
		for (i = 0; i < nBins; i++) {
			arr[i] = hist[j].at<float>(i);
		}
		res[NumOfChannels - 1 - j] = spPointCreate(arr, nBins, imageIndex);
	}
	free(arr);
	for (i=0;i<NumOfChannels;i++){
		hist[i].release();
	}
	bgr_planes.clear();
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
		//printf("the %d dis between %f\n",i,sum);
	}
	sum = sum / 3;
	return sum;
}

SPPoint** spGetSiftDescriptors(const char* str, int imageIndex,
		int nFeaturesToExtract, int *nFeatures) {
	int i, j;
	double* cord_array;
	cv::Mat src;
	src = cv::imread(str, CV_LOAD_IMAGE_GRAYSCALE);
	if (src.empty()) {
		printf(IMG_NOT_LOADED,str); //TODO free all mallocs and exit
		return NULL;
	}
	//Key points will be stored in kp1;
	std::vector<cv::KeyPoint> kp1;
	//Feature values will be stored in ds1;
	cv::Mat ds1;
	//Creating  a Sift Descriptor extractor
	cv::Ptr<cv::xfeatures2d::SiftDescriptorExtractor> detect =
			cv::xfeatures2d::SIFT::create(nFeaturesToExtract);
	//Extracting features
	//The features will be stored in ds1
	//The output type of ds1 is CV_32F (float)
	detect->detect(src, kp1, cv::Mat());
	detect->compute(src, kp1, ds1);
	SPPoint** pointArray;
	pointArray = (SPPoint**) malloc((ds1.rows) * sizeof(SPPoint*)); // n points and one comparison point
	if (pointArray == NULL) {
		return NULL;
	}
	cord_array = (double*) malloc(ds1.cols * sizeof(double));
	for (i = 0; i < ds1.rows; i++) {
		for (j = 0; j < ds1.cols; j++) {
			cord_array[j] = (double) ds1.at<float>(i, j);
		}
		pointArray[i] = spPointCreate(cord_array, ds1.cols, imageIndex);
	}
	*nFeatures = ds1.rows;
	free(cord_array);
	return pointArray;
}

int* spBestSIFTL2SquaredDistance(int kClosest, SPPoint* queryFeature,
		SPPoint*** databaseFeatures, int numberOfImages,
		int* nFeaturesPerImage) {
	SPBPQueue* queue;
	BPQueueElement* element;
	int i, j;
	int* array;
	queue = spBPQueueCreate(kClosest); // creating the queue that will hold teh index-L2distance elements
	if (queue == NULL) {
		return NULL;
	}
	for (i = 0; i < numberOfImages; i++) {
		for (j = 0; j < nFeaturesPerImage[i]; j++) {
			spBPQueueEnqueue(queue, i,
					spPointL2SquaredDistance(databaseFeatures[i][j],
							queryFeature));
		}
	}
	element = (BPQueueElement*) malloc(sizeof(BPQueueElement)); // will store the index and value when peeking
	array = (int*) malloc(kClosest * sizeof(int));
	for (i = 0; i < kClosest; i++) {
		spBPQueuePeek(queue, element); // gets the index and value of the smallest valued element
		array[i] = element->index; // prints the index
		spBPQueueDequeue(queue);  // removes the smallest element from the queue
	}
	spBPQueueDestroy(queue); // free queue
	free(element);
	return array;
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


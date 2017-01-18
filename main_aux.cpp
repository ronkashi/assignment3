/*
 * main_aux.cpp
 *
 *  Created on: 17 1 2017
 *      Author: ron
 */


#include "main_aux.h"



extern "C" {
//Use this syntax in-order to include C-header files
//HINT : You don't need to include other C header files here -> Maybe in sp_image_proc_util.c ? <-
	#include "SPPoint.h"
	#include <assert.h>
	#include <stdlib.h>
}
#define ENTER_IMG_DIR_MSG "Enter images directory path:\n"
#define IMG_PREFIX_MSG "Enter images prefix:\n"
#define IMG_SUFFIX_MSG "Enter images suffix:\n"
#define IMG_NUM_MSG "Enter images suffix :\n"

/*================================================================
*   Copyright (C) 2018 * Ltd. All rights reserved.
*   
*   File name   : image_util.h
*   Author      : FanmingL
*   Created date: 2018-11-10 13:42:24
*   Description : 
*
*===============================================================*/


#ifndef _IMAGE_UTIL_H
#define _IMAGE_UTIL_H
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <vector>
#include <iostream>

#include "alvision/alvisiondefinitions.h"

/*
 * get certain channel of a multil-channel img
 */
void getOneChannel(const cv::Mat &src, cv::Mat &dst, const int channel = 0);

/*
 * transform p_src to p_dst using homography matrix
 * homography matrix is a double type matrix
 */
void calculateTransform(const cv::Point2f &src_p, const cv::Mat &homography_matrix, cv::Point2f &p_dst);


#endif //IMAGE_UTIL_H

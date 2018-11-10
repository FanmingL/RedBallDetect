//
//  Detect.hpp
//  RedBallDetect
//
//  Created by Fanming Luo on 2017/11/19.
//  Copyright © 2017年 Fanming Luo. All rights reserved.
//

#ifndef _Detect_hpp_
#define _Detect_hpp_
#include <vector>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
//std::vector<float> DetectRedBall(cv::Mat &originalImage);
std::vector<float> DetectRedBall(cv::Mat &originalImage, int redhmin, int redhmax, int redsmin, int redsmax, int redvmin, int redvmax, int greenhmin, int greenhmax, int greensmin, int greensmax, int greenvmin, int greenvmax);
#endif /* Detect_hpp */


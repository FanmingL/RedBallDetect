//
//  pole.hpp
//  RedBallDetect
//
//  Created by Fanming Luo on 2017/11/23.
//  Copyright © 2017年 Fanming Luo. All rights reserved.
//

#ifndef pole_hpp
#define pole_hpp
#include <vector>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
//std::vector<float> DetectPole(cv::Mat &originalImage);
std::vector<float> DetectPole(cv::Mat &originalImage, int yellowhmin, int yellowhmax, int yellowsmin, int yellowsmax, int yellowvmin, int yellowvmax);
#endif /* pole_hpp */

//
//  Detect.hpp
//  RedBallDetect
//
//  Created by Fanming Luo on 2017/11/19.
//  Copyright © 2017年 Fanming Luo. All rights reserved.
//

#ifndef Detect_hpp
#define Detect_hpp
#include <vector>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
std::vector<double> DetectRedBall(cv::Mat &originalImage);
#endif /* Detect_hpp */

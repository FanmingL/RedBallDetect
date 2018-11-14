/*================================================================
*   Copyright (C) 2018 * Ltd. All rights reserved.
*   
*   File name   : detection_base.h
*   Author      : FanmingL
*   Created date: 2018-11-14 12:21:33
*   Description : 
*
*===============================================================*/


#ifndef _DETECTION_BASE_H
#define _DETECTION_BASE_H
#include "algorithm_base.h"
#include "parameter.h"
#include "opencv2/core/core.hpp"

class DetectionMessage{
public:
    enum DetectStatus{
        LOST_BALL_AND_POLE = 1,
        LOST_BALL,
        LOST_POLE
    };
    DetectStatus status;
    float pole_x, pole_y, ball_x, ball_y;
};


class DetectionBase : public AlgorithmBase{
public:
    DetectionBase(const std::string &name) : AlgorithmBase(name){}

    virtual ~DetectionBase(){}

    virtual void init(){}

    virtual void getParameters(const AL::ParameterGolf &parameter) {}

    virtual void executeDetection(const cv::Mat &src, DetectionMessage &detect) {}
};

#endif //DETECTION_BASE_H

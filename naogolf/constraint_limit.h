/*================================================================
*   Copyright (C) 2018 * Ltd. All rights reserved.
*   
*   File name   : constraint_limit.h
*   Author      : FanmingL
*   Created date: 2018-11-14 13:01:08
*   Description : 
*
*===============================================================*/


#ifndef _CONSTRAINT_LIMIT_H
#define _CONSTRAINT_LIMIT_H
#include "image_util.h"
#include "detection_base.h"

class ConstraintSetBall : public DetectionBase{
public:
    ConstraintSetBall(const std::string &name = std::string("constraint set ball"));

    virtual ~ConstraintSetBall();

    virtual void init();

    virtual void getParameter(const AL::ParameterGolf &parameter);

    virtual void executeDetection(const cv::Mat &src, DetectionMessage &detect);
};

class ConstraintSetPole : public DetectionBase{
public:
    ConstraintSetPole(const std::string &name = std::string("constraint set pole"));

    virtual ~ConstraintSetPole();

    virtual void init();

    virtual void getParameter(const AL::ParameterGolf &parameter);

    virtual void executeDetection(const cv::Mat &src, DetectionMessage &detect);
};



#endif //CONSTRAINT_LIMIT_H

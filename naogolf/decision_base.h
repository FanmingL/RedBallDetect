/*================================================================
*   Copyright (C) 2018 * Ltd. All rights reserved.
*   
*   File name   : decision_base.h
*   Author      : FanmingL
*   Created date: 2018-11-14 12:21:42
*   Description : 
*
*===============================================================*/


#ifndef _DECISION_BASE_H
#define _DECISION_BASE_H
#include "algorithm_base.h"
#include "parameter.h"

class StatusMessage{
public:
    enum Status{
        INIT = 0,
        LOST_BALL_AND_POLE,
        LOST_BALL,
        LOST_POLE,
        NORMAL
    };
    Status status_now;
    float ball_x, ball_y, pole_x, pole_y;
    int game_round;
};

class DecisionMessage{
public:
    float expect_speed_x;
    float expect_speed_y;
    float expect_speed_rotate;
    float expect_pitch;
    float expect_yaw;

};

class DecisionBase : public AlgorithmBase{
public:
    DecisionBase(const std::string &name) : AlgorithmBase(name){}

    virtual ~DecisionBase(){}

    virtual void init(){}

    virtual void getParameters(const AL::ParameterGolf &parameter){}

    virtual void executeDecision(const StatusMessage& status, DecisionMessage &decision) {}

};

#endif //DECISION_BASE_H

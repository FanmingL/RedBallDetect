/*================================================================
*   Copyright (C) 2018 * Ltd. All rights reserved.
*   
*   File name   : parameter.h
*   Author      : FanmingL
*   Created date: 2018-11-10 13:42:34
*   Description : 
*
*===============================================================*/


#ifndef _PARAMETER_H
#define _PARAMETER_H
#include <string>
#include <iostream>
namespace AL {
class ParameterGolf{
    friend std::ostream & operator<<(std::ostream &out, ParameterGolf &obj);
public:
    ParameterGolf();

    ~ParameterGolf();

    void readFromFile(const std::string &path);

    void writeToFile(const std::string &path);

    /* threshold */

    int ball_red_h_max, ball_red_s_max, ball_red_v_max;
    int ball_red_h_min, ball_red_s_min, ball_red_v_min;
    int ball_green_h_max, ball_green_s_max, ball_green_v_max;
    int ball_green_h_min, ball_green_s_min, ball_green_v_min;
    int pole_yellow_h_max, pole_yellow_s_max, pole_yellow_v_max;
    int pole_yellow_h_min, pole_yellow_s_min, pole_yellow_v_min;

    /* game data */

    int stage_index;

    /* move data */

    float move_pid_p;

    /* debug data */
    std::string thing, detect_algorithm, decision_algorithm;

    int width, height;

};
}

#endif //PARAMETER_H

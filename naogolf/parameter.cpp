/*
 * parameter io
 */

#include "parameter.h"
#include "naogolf.h"
#include "opencv2/core/core.hpp"
namespace AL
{
void naogolf::initParameter(){
    parameters.readFromFile(parameter_path);
}

void naogolf::loadParameter(){
    initParameter();
    qiLogInfo(getName().c_str()) << parameters<<std::endl;
}

ParameterGolf::ParameterGolf()
{

}

ParameterGolf::~ParameterGolf()
{

}

void ParameterGolf::readFromFile(const std::string &path)
{
    cv::FileStorage fs(path, cv::FileStorage::READ);
    fs["thing"] >> thing;
    fs["detect_algorithm"] >> detect_algorithm;
    fs["decision_algorithm"] >> decision_algorithm;
    fs["stage_index"] >> stage_index;
    fs["move_pid_p"] >> move_pid_p;

    fs["ball_red_h_min"] >> ball_red_h_min;
    fs["ball_red_h_max"] >> ball_red_h_max;
    fs["ball_red_s_min"] >> ball_red_s_min;
    fs["ball_red_s_max"] >> ball_red_s_max;
    fs["ball_red_v_min"] >> ball_red_v_min;
    fs["ball_red_v_max"] >> ball_red_v_max;

    fs["ball_green_h_min"] >> ball_green_h_min;
    fs["ball_green_h_max"] >> ball_green_h_max;
    fs["ball_green_s_min"] >> ball_green_s_min;
    fs["ball_green_s_max"] >> ball_green_s_max;
    fs["ball_green_v_min"] >> ball_green_v_min;
    fs["ball_green_v_max"] >> ball_green_v_max;

    fs["pole_yellow_h_min"] >> pole_yellow_h_min;
    fs["pole_yellow_h_max"] >> pole_yellow_h_max;
    fs["pole_yellow_s_min"] >> pole_yellow_s_min;
    fs["pole_yellow_s_max"] >> pole_yellow_s_max;
    fs["pole_yellow_v_min"] >> pole_yellow_v_min;
    fs["pole_yellow_v_max"] >> pole_yellow_v_max;
}

void ParameterGolf::writeToFile(const std::string &path)
{

}

std::ostream & operator<<(std::ostream &out, ParameterGolf &obj){
    out<<"thing: " << obj.thing << std::endl\
      <<"detect_algorithm: "<< obj.detect_algorithm<<std::endl\
      <<"decision_algorithm: "<< obj.decision_algorithm <<std::endl\
      <<"stage_index: "<<obj.stage_index<<std::endl\
      <<"move_pid_p: "<<obj.move_pid_p<<std::endl\
      <<"ball_red_h_min: "<<obj.ball_red_h_min<<std::endl\
      <<"ball_red_h_max: "<<obj.ball_red_h_max<<std::endl\
      <<"ball_red_s_min: "<<obj.ball_red_s_min<<std::endl\
      <<"ball_red_s_max: "<<obj.ball_red_s_max<<std::endl\
      <<"ball_red_v_min: "<<obj.ball_red_v_min<<std::endl\
      <<"ball_red_v_max: "<<obj.ball_red_v_max<<std::endl\
      <<"ball_green_h_min: "<<obj.ball_green_h_min<<std::endl\
      <<"ball_green_h_max: "<<obj.ball_green_h_max<<std::endl\
      <<"ball_green_s_min: "<<obj.ball_green_s_min<<std::endl\
      <<"ball_green_s_max: "<<obj.ball_green_s_max<<std::endl\
      <<"ball_green_v_min: "<<obj.ball_green_v_min<<std::endl\
      <<"ball_green_v_max: "<<obj.ball_green_v_max<<std::endl\
      <<"pole_yellow_h_min: "<<obj.pole_yellow_h_min<<std::endl\
      <<"pole_yellow_h_max: "<<obj.pole_yellow_h_max<<std::endl\
      <<"pole_yellow_s_min: "<<obj.pole_yellow_s_min<<std::endl\
      <<"pole_yellow_s_max: "<<obj.pole_yellow_s_max<<std::endl\
      <<"pole_yellow_v_min: "<<obj.pole_yellow_v_min<<std::endl\
      <<"pole_yellow_v_max: "<<obj.pole_yellow_v_max<<std::endl;
    return out;
}
}

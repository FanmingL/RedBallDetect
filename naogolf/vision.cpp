/*================================================================
*   Copyright (C) 2018 * Ltd. All rights reserved.
*   
*   File name   : vision.cpp
*   Author      : FanmingL
*   Created date: 2018-11-14 13:07:10
*   Description : 
*
*===============================================================*/


#include "naogolf.h"
#include "constraint_limit.h"

namespace AL{
void naogolf::initVision(){
    ball_detection_algorithm = boost::make_shared<ConstraintSetBall>();
    pole_detection_algorithm = boost::make_shared<ConstraintSetPole>();
    cv::initUndistortRectifyMap(parameters.camera_matrix, parameters.distort_matrix, cv::Mat(), parameters.camera_matrix,
                                cv::Size(parameters.width, parameters.height),CV_16SC2, undistort_map1, undistort_map2);
}

void naogolf::executeVision(){
    boost::thread thread_get_image(boost::bind(&naogolf::getImage, this, TOP_CAMERA));
    thread_get_image.detach();
    DELAY_THREAD_MS(100);
    boost::thread thread_ball_detection(boost::bind(&naogolf::detectBall, this));
    boost::thread thread_pole_detection(boost::bind(&naogolf::detectPole, this));
    thread_ball_detection.detach();
    thread_pole_detection.detach();
}

void naogolf::detectBall(){
    cv::Mat image_cpy;
    DetectionMessage detect_res;
    while (getExecuteStatus() && getVideoRegisterStatus()){
        {
            boost::unique_lock<boost::mutex> _unique_lock(mutex_image);
            image.copyTo(image_cpy);
        }
        if (image_cpy.empty())DELAY_THREAD_MS(50);
        else{
            ball_detection_algorithm->executeDetection(image_cpy, detect_res);
            DELAY_THREAD_MS(100);
            {
                boost::unique_lock<boost::mutex> _unique_lock(mutex_ball_detect_message);
                detect_ball_message = detect_res;
            }
        }
    }
    qiLogInfo(getName().c_str()) << "stop detect ball" << std::endl;
}

void naogolf::detectPole(){
    cv::Mat image_cpy;
    DetectionMessage detect_res;
    while (getExecuteStatus() && getVideoRegisterStatus()){
        {
            boost::unique_lock<boost::mutex> _unique_lock(mutex_image);
            image.copyTo(image_cpy);
        }
        if (image_cpy.empty())DELAY_THREAD_MS(50);
        else{
            DELAY_THREAD_MS(100);
            pole_detection_algorithm->executeDetection(image_cpy, detect_res);
            {
                boost::unique_lock<boost::mutex> _unique_lock(mutex_pole_detect_message);
                detect_pole_message = detect_res;
            }
        }
    }
    qiLogInfo(getName().c_str()) << "stop detect pole" << std::endl;
}

void naogolf::getImage(int selected_camera_index){
    registerVideoDevice(selected_camera_index, parameters.width, parameters.height);
    boost::chrono::high_resolution_clock::time_point now;
    cv::Mat image_cpy(cv::Size(parameters.width, parameters.height), CV_8UC3, cv::Scalar(0, 0, 0));
    while(getExecuteStatus() && getVideoRegisterStatus()){	
        now = boost::chrono::high_resolution_clock::now();
        image_ptr = (ALImage*)video_device_proxy.getImageLocal(video_device_handler);
        image_cpy.data = image_ptr->getData();
        {
            boost::unique_lock<boost::mutex> _unique_lock(mutex_image);
            cv::remap(image_cpy, image, undistort_map1, undistort_map2, cv::INTER_LINEAR);
        }
        video_device_proxy.releaseImage(video_device_handler);
        boost::this_thread::sleep_until(now + boost::chrono::milliseconds(1000 / parameters.fps));
    }
    unRegisterVideoDevice();
    qiLogInfo(getName().c_str()) << "stop get image" <<std::endl;
}

void naogolf::registerVideoDevice(int selected_camera_index, int width, int height, int color_space){
    if (getVideoRegisterStatus()){
        unRegisterVideoDevice();
        DELAY_THREAD_MS(300);
    }

    const std::string video_name("naogolf_video_module");
    int resolution_type = getResolutionFromSize(width, height);
    if (resolution_type == -1){
        qiLogError(getName().c_str()) << "resolution is invalid!!" << std::endl
                   << "had convert it to 640 x 480"<<std::endl;
        resolution_type = kVGA;
        setSizeFromResolution(kVGA, width, height);
    }
    {
        boost::unique_lock<boost::mutex> _unique_lock(mutex_image);
        image = cv::Mat(cv::Size(width, height), CV_8UC3, cv::Scalar(0, 0, 0));
    }
    video_device_handler = video_device_proxy.subscribeCamera(video_name, selected_camera_index, resolution_type, color_space, parameters.fps);
    qiLogInfo(getName().c_str()) << "successfully subsrcibe the device, which name is "<< video_device_handler << std::endl;

    setVideoRegisterStatus(true);

}

void naogolf::unRegisterVideoDevice(){
    if (!getVideoRegisterStatus())return;

    image.release();

    video_device_proxy.unsubscribe(video_device_handler);

    setVideoRegisterStatus(false);
}

bool naogolf::getVideoRegisterStatus(){
    boost::unique_lock<boost::mutex> _unique_lock(mutex_video_registered);
    return if_video_registered;
}

void naogolf::setVideoRegisterStatus(bool status){
    boost::unique_lock<boost::mutex> _unique_lock(mutex_video_registered);
    if_video_registered = status;
}

bool naogolf::saveImage(const std::string &image_name){
    cv::Mat image_cpy;
    {
        boost::unique_lock<boost::mutex> _unique_lock(mutex_image);
        image.copyTo(image_cpy);
    }
    std::string _path("/home/nao/naoqi/");

    _path += image_name;
    if ((getExecuteStatus() && getVideoRegisterStatus()))
    {
        return cv::imwrite(_path, image_cpy);
    }else{
        qiLogError(getName().c_str()) << "module has not been execute now" <<std::endl;
        return false;
    }
}


}

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
    detection_algorithm = boost::make_shared<ConstraintLimit>();
}

void naogolf::executeVision(){
    boost::thread thread_get_image(boost::bind(&naogolf::getImage, this, TOP_CAMERA));
    thread_get_image.detach();
}

void naogolf::processImage(){
    cv::Mat image_cpy;
    DetectionMessage detect_res;
    while (getExecuteStatus() && getVideoRegisterStatus()){
        {
            boost::unique_lock<boost::mutex> _unique_lock(mutex_image);
            image_cpy = image.clone();
        }
        if (image_cpy.empty())DELAY_THREAD_MS(50);
        else{
            detection_algorithm->executeDetection(image_cpy, detect_res);
            {
                boost::unique_lock<boost::mutex> _unique_lock(mutex_detect_message);
                detect_message = detect_res;
            }
        }
    }
    qiLogInfo(getName().c_str()) << "stop process" << std::endl;
}

void naogolf::getImage(int selected_camera_index){
    registerVideoDevice(selected_camera_index, parameters.width, parameters.height);
    while(getExecuteStatus() && getVideoRegisterStatus()){	
        image_ptr = (ALImage*)video_device_proxy.getImageLocal(video_device_handler);
        {
            boost::unique_lock<boost::mutex> _unique_lock(mutex_image);
            image.data = image_ptr->getData();
        }
        video_device_proxy.releaseImage(video_device_handler);
        DELAY_THREAD_MS(15);
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
    video_device_handler = video_device_proxy.subscribe(video_name, resolution_type, color_space, 30);
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
    boost::unique_lock<boost::mutex> _unique_lock(mutex_image);
    std::string _path("/home/nao/naoqi/");
    _path += image_name;
    if ((getExecuteStatus() && getVideoRegisterStatus()))
    {
        return cv::imwrite(_path, image);
    }else{
        qiLogError(getName().c_str()) << "module has not been execute now" <<std::endl;
        return false;
    }
}


}

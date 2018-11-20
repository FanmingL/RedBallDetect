/*================================================================
*   Copyright (C) 2018 * Ltd. All rights reserved.
*   
*   File name   : naogolf.h
*   Author      : FanmingL
*   Created date: 2018-11-10 13:43:20
*   Description : 
*
*===============================================================*/


#ifndef _NAOGOLF_H
#define _NAOGOLF_H
#include <alcommon/almodule.h>
#include <alcommon/albroker.h>
#include <qi/log.hpp>

#include <alproxies/altexttospeechproxy.h>
#include <alproxies/alvideodeviceproxy.h>
#include <alcommon/almodulecore.h>
#include <alvision/alimage.h>

#include <boost/shared_ptr.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/chrono/chrono.hpp>
#include "parameter.h"
#include "decision_base.h"
#include "detection_base.h"
#include "util.h"
#include "image_util.h"

namespace AL {
class naogolf : public ALModule
{
public:
    /*
     * constructor function
     */
    naogolf(boost::shared_ptr<ALBroker> pBroker, const std::string& pName);

    ~naogolf();

    /*
     * init function, inherit from standard ALModule
     */
    void init();

    /*
     * say some thing, used to debug
     * @param words, things to say
     */
    void say(const std::string &words);

    /*
     * execute the vision loop
     */
    void executeVision();

    /*
     * execute the decision loop
     */
    void executeDecision();

    /*
     * init vision module
     */
    void initVision();

    /*
     * init decision module
     */
    void initDecision();

    /*
     * execute all algorithms
     */
    void execute(const int &game_index = -1);

    /*
     * stop algorithm
     */
    void stopExecute();

    /*
     * say version for debuging
     */
    void sayVersion();

    /*
     * read parameter from files at parameter_path
     */
    void initParameter();

    /*
     * init control module
     */
    void initControl();

    /*
     * reload parameters
     */
    void loadParameter();

    /*
     * init time counter, which will record time(sec) from start up on
     */
    void initTime();

    /*
     * get the time length(sec) from start
     */
    double getSecNow();

    /*
     * the same as the naoqi did
     */
    enum CameraIndex{
        TOP_CAMERA = 0,
        BOTTOM_CAMERA = 1
    };

    /*
     *	continous get image from cameras
     */
    void getImage(int selected_camera_index = TOP_CAMERA);

    /*
     * get execute status true or false
     */
    bool getExecuteStatus();

    /*
     * safely set execute status
     */
    void setExecuteStatus(bool status);

    /*
     * get execute status true or false
     */
    bool getVideoRegisterStatus();

    /*
     * safely set execute status
     */
    void setVideoRegisterStatus(bool status);

    /*
     * register video device
     */
    void registerVideoDevice(int selected_camera_index = TOP_CAMERA, int width = 640, int height = 480, int color_space = kBGRColorSpace);

    /*
     * unregister video device
     */
    void unRegisterVideoDevice();

    /*
     * save image as certain name
     */
    bool saveImage(const std::string &image_name);

    /*
     * process image
     */
    void processImage();

private:

    /*
     * indicate version and parameter path
     */
    const std::string _version, parameter_path;

    /*
     * Proxy
     */
    ALTextToSpeechProxy text_to_speech;

    ALVideoDeviceProxy video_device_proxy;

    boost::mutex mutex_speech;

    ParameterGolf parameters;

    boost::shared_ptr<DecisionBase> decision_algorithm;

    boost::shared_ptr<DetectionBase> detection_algorithm;

    boost::chrono::high_resolution_clock::time_point start_time_point;

    cv::Mat image;

    ALImage *image_ptr;

    double image_time;

    boost::mutex mutex_image;

    bool execute_flag, if_video_registered;

    boost::mutex mutex_execute_flag, mutex_video_registered;

    std::string video_device_handler;

    DetectionMessage detect_message;

    boost::mutex mutex_detect_message;

    DecisionMessage decision_message;

    boost::mutex mutex_decision_message;
};
}

#endif //NAOGOLF_H

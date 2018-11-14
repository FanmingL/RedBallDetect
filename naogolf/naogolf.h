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
#include <boost/shared_ptr.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/chrono/chrono.hpp>
#include "parameter.h"
#include "decision_base.h"
#include "detection_base.h"
#include "util.h"

namespace AL {
class naogolf : public ALModule
{
public:
    /*
     * constructor function
     */
    naogolf(boost::shared_ptr<ALBroker> pBroker, const std::string& pName);

    ~naogolf();

    void init();

    void say(const std::string &words);

    void executeVision();

    void executeDecision();

    void initVision();

    void initDecision();

    void execute(const int &game_index = -1);

    void sayVersion();

    void initParameter();

    void initControl();

    void loadParameter();

    void initTime();

    double getSecNow();

private:
    const std::string _version, parameter_path;

    ALTextToSpeechProxy text_to_speech;

    boost::mutex mutex_speech;

    ParameterGolf parameters;

    boost::shared_ptr<DecisionBase> decision_algorithm;

    boost::shared_ptr<DetectionBase> detection_algorithm;

    boost::chrono::high_resolution_clock::time_point start_time_point;
};
}

#endif //NAOGOLF_H

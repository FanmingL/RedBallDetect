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
#include <alcommon/almodulecore.h>
#include <boost/shared_ptr.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/thread/mutex.hpp>
#include "parameter.h"

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

private:
    const std::string _version;
    ALTextToSpeechProxy text_to_speech;
    boost::mutex mutex_speech;
    parametergolf parameters;
};
}

#endif //NAOGOLF_H

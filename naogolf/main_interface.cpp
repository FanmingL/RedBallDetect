/*
 * main interface
 */

#include <signal.h>
#include <boost/shared_ptr.hpp>
#include <alcommon/albroker.h>
#include <alcommon/almodule.h>
#include <alcommon/albrokermanager.h>
#include <alcommon/altoolsmain.h>

#include "naogolf.h"

namespace AL {
naogolf::naogolf(boost::shared_ptr<ALBroker> broker, const std::string &name):
    ALModule(broker, name),
    _version("version 0.1.0"),
    text_to_speech(getParentBroker()),
    video_device_proxy(getParentBroker()),
    parameter_path("/home/nao/naoqi/config.yml"),
    execute_flag(false),
    if_video_registered(false)
{
    detect_message.time_now = decision_message.time_now = -1;
    setModuleDescription("Nanjing University Nao Golf Programe");

    /*
     * execute module algorithm
     */
    functionName("execute", getName(),"execute module algorithm");
    BIND_METHOD(naogolf::execute);

    functionName("stopExecute", getName(),"stop all algorithm");
    BIND_METHOD(naogolf::stopExecute);

    functionName("say", getName(),"say some thing");
    BIND_METHOD(naogolf::say);

    functionName("sayVersion", getName(),"say version now");
    BIND_METHOD(naogolf::sayVersion);

    functionName("loadParameter", getName(), "load and log parameters");
    BIND_METHOD(naogolf::loadParameter);

    functionName("getSecNow", getName(), "get time now");
    BIND_METHOD(naogolf::getSecNow);

    functionName("saveImage", getName(), "save image as certain name");
    BIND_METHOD(naogolf::saveImage);

}

void naogolf::init(){
    say(_version);
    initTime();
    initParameter();
    initControl();
    initDecision();
    initVision();
}

void naogolf::execute(const int &game_index){
    if (getExecuteStatus()){
        setExecuteStatus(false);
        DELAY_THREAD_MS(500);
    }
    setExecuteStatus(true);
    executeVision();
    int counter = 0;
    while(getExecuteStatus()){
        DELAY_THREAD_MS(1000);
        qiLogInfo(getName().c_str()) << counter++<<std::endl;
    }
    qiLogInfo(getName().c_str()) << "stop execute" <<std::endl;
}

void naogolf::stopExecute(){
    setExecuteStatus(false);
}

void naogolf::setExecuteStatus(bool status){
    boost::unique_lock<boost::mutex> _unique_lock(mutex_execute_flag);
    execute_flag = status;
}

bool naogolf::getExecuteStatus(){
    boost::unique_lock<boost::mutex> execute_flag_lock(mutex_execute_flag);
    return execute_flag;
}

naogolf::~naogolf(){

}
}


extern "C"
{
int _createModule(boost::shared_ptr<AL::ALBroker> pBroker)
{
  AL::ALBrokerManager::setInstance(pBroker->fBrokerManager.lock());
  AL::ALBrokerManager::getInstance()->addBroker(pBroker);
  AL::ALModule::createModule<AL::naogolf>(pBroker,"naogolf");
  return 0;
}

int _closeModule()
{
  return 0;
}

} // extern "C"


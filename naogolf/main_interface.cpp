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
    _version("version 0.0.7"),
    text_to_speech(getParentBroker()),
    parameter_path("/home/nao/naoqi/config.yml")
{
    setModuleDescription("Nanjing University Nao Golf Programe");

    /*
     * execute module algorithm
     */
    functionName("execute", getName(),"execute module algorithm");
    BIND_METHOD(naogolf::execute);


    functionName("say", getName(),"say some thing");
    BIND_METHOD(naogolf::say);


    functionName("sayVersion", getName(),"say version now");
    BIND_METHOD(naogolf::sayVersion);

    functionName("loadParameter", getName(), "load and log parameters");
    BIND_METHOD(naogolf::loadParameter);

    functionName("getSecNow", getName(), "get time now");
    BIND_METHOD(naogolf::getSecNow);

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
    for (int i = 0; i < 10; i++){
        loadParameter();
        qiLogInfo(getName().c_str())<<i<<", "<<getSecNow();
        delayMs(300);
    }
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


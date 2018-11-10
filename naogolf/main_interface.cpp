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
    _version("version 0.0.1"),
    text_to_speech(getParentBroker())
{
    setModuleDescription("Nanjing University Nao Golf programe");

    /*
     * execute module algorithm
     */
    functionName("execute", getName(),"execute module algorithm");
    BIND_METHOD(naogolf::execute);
}

void naogolf::init(){
    say(_version);
}

void naogolf::execute(const int &game_index){

}
}


extern "C"
{
int _createModule(boost::shared_ptr<AL::ALBroker> pBroker)
{
  // init broker with the main broker instance
  // from the parent executable
  AL::ALBrokerManager::setInstance(pBroker->fBrokerManager.lock());
  AL::ALBrokerManager::getInstance()->addBroker(pBroker);


  // create module instances
  AL::ALModule::createModule<AL::naogolf>(pBroker,"naogolf");

  return 0;
}

int _closeModule()
{
  return 0;
}

} // extern "C"


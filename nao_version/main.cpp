/*
 * Copyright (c) 2012-2015 Aldebaran Robotics. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the COPYING file.
 */

#include "main.hpp"
#define ALCALL
extern "C"
{
    ALCALL int _createModule(boost::shared_ptr<AL::ALBroker> pBroker)
    {
        AL::ALBrokerManager::setInstance(pBroker->fBrokerManager.lock());
        AL::ALBrokerManager::getInstance()->addBroker(pBroker);
        AL::ALModule::createModule<njunaoModule>(pBroker,"njunaoModule");
        return 0;
    }

    ALCALL int _closeModule()
    {
        return 0;
    }
} // extern "C"
/*
int main(int argc, char **argv)
{
  std::cout << "Hello, world" << std::endl;
  return 0;
}
*/

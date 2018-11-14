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

}
}

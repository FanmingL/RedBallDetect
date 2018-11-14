/*================================================================
*   Copyright (C) 2018 * Ltd. All rights reserved.
*   
*   File name   : algorithm_base.cpp
*   Author      : FanmingL
*   Created date: 2018-11-14 12:29:46
*   Description : 
*
*===============================================================*/


#include "algorithm_base.h"


AlgorithmBase::AlgorithmBase(const std::string &name) : algorithm_name(name)
{

}

AlgorithmBase::~AlgorithmBase()
{

}

std::string AlgorithmBase::getAlgorithmName()
{
    return algorithm_name;
}

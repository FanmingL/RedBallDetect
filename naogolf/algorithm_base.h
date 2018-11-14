/*================================================================
*   Copyright (C) 2018 * Ltd. All rights reserved.
*   
*   File name   : algorithm_base.h
*   Author      : FanmingL
*   Created date: 2018-11-14 12:21:19
*   Description : 
*
*===============================================================*/


#ifndef _ALGORITHM_BASE_H
#define _ALGORITHM_BASE_H
#include <string>
class AlgorithmBase{
public:
    AlgorithmBase(const std::string &name);

    virtual ~AlgorithmBase();

    std::string getAlgorithmName();

private:

    std::string algorithm_name;
};


#endif //ALGORITHM_BASE_H

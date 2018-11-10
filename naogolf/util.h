/*================================================================
*   Copyright (C) 2018 * Ltd. All rights reserved.
*   
*   File name   : util.h
*   Author      : FanmingL
*   Created date: 2018-11-10 13:42:43
*   Description : 
*
*===============================================================*/


#ifndef _UTIL_H
#define _UTIL_H
namespace AL {
class watch_dog
{
public:
    watch_dog(int max_count);

    ~watch_dog();

    bool checkDog();

    void feedDog();

private:
    int count_now, max_count;
};
}
#endif //UTIL_H

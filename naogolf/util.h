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
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#define DELAY_THREAD_MS(ms) (boost::this_thread::sleep(boost::posix_time::milliseconds(ms)))
#define DELAY_MS(ms) (boost::thread::sleep(boost::posix_time::milliseconds(ms))


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

//void delayMs(const unsigned int &msecs);

}
#endif //UTIL_H

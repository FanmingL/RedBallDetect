/*
 * utils
 */

#include "util.h"
#include "naogolf.h"
namespace AL{
void naogolf::say(const std::string &words){
    boost::unique_lock<boost::mutex> _unique_lock(mutex_speech);
    text_to_speech.say(words);
}

void naogolf::sayVersion(){
    say(_version);
}

void naogolf::initTime(){
    start_time_point = boost::chrono::high_resolution_clock::now();
}

double naogolf::getSecNow(){
    boost::chrono::duration<double> sec = boost::chrono::high_resolution_clock::now() - start_time_point;
    return sec.count();
}

watch_dog::watch_dog(int _max_count):max_count(_max_count),count_now(0)
{

}

watch_dog::~watch_dog()
{

}

bool watch_dog::checkDog()
{
    return ((count_now++) < max_count);
}

void watch_dog::feedDog()
{
    count_now = 0;
}

void delayMs(const unsigned int &msecs)
{
    boost::chrono::high_resolution_clock::time_point start = boost::chrono::high_resolution_clock::now();
    boost::chrono::duration<double> sec = boost::chrono::high_resolution_clock::now() - start;
    while (sec.count() * 1000 < msecs )
        sec = boost::chrono::high_resolution_clock::now() - start;
}

}

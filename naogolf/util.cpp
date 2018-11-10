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

}

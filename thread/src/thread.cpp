/**
 * @file
 *
 *
 *
 */
#include "thread.h"

namespace QRCode{
    thread::thread(){

    }
    thread::~thread(){

    }
    void thread::start_thread(unsigned int delay){
        delay_ = delay;
        pthread_create(&thread_, nullptr, pthread_fun, this);
        std::cout << this << std::endl;
//        return;
    }
    void* thread::pthread_fun(void* this_){
        while(true){
            try {
                ((thread*)this_)->process();
            }
            catch (...){
                break;
            }
            usleep(((thread*)this_)->delay_);
        }
    }
}
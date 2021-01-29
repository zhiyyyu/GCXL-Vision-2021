#ifndef QRCODE_THREAD_H
#define QRCODE_THREAD_H

#include <thread>
#include <mutex>
#include <unistd.h>
#include <iostream>

namespace QRCode{
    class thread{
    public:
        thread();
        ~thread();
        void start_thread(unsigned int delay);
        static void* pthread_fun(void* this_);
        virtual void process() = 0;
    private:
        pthread_t thread_;
        unsigned int delay_;
    };
}

#endif //QRCODE_THREAD_H

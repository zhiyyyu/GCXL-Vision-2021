#ifndef QRCODE_THREAD1_H
#define QRCODE_THREAD1_H

#include "thread.h"
#include <iostream>

namespace QRCode{
    class thread1: public thread
    {
    public:
        thread1();
        ~thread1() = default;
        void process() override ;
    };
}

#endif //QRCODE_THREAD1_H

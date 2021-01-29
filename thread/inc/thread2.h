#ifndef QRCODE_THREAD2_H
#define QRCODE_THREAD2_H

#include "thread.h"
#include <iostream>

namespace QRCode{
    class thread2: public thread
    {
    public:
        thread2();
        ~thread2() = default;
        void process() override ;
    };
}

#endif //QRCODE_THREAD2_H

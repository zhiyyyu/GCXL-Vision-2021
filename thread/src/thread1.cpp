/**
 * @file
 * @author
 * @brief
 * @note
 */
#include "thread1.h"

namespace QRCode{
    thread1::thread1(){
        start_thread(1000000);
    }

    void thread1::process() {
        std::cout << "thread1 start" << std::endl;
        return;
    }
}
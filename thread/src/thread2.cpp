/**
 * @file
 * @author
 * @brief
 * @note
 */
#include "thread2.h"

namespace QRCode{
    thread2::thread2(){
        start_thread(1000000);
    }

    void thread2::process() {
        std::cout << "thread2 start" << std::endl;
        return;
    }
}
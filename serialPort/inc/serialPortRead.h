#ifndef __SERIAL_READ_PORT_THREAD_H
#define __SERIAL_READ_PORT_THREAD_H

#include "serialPort.h"
#include <iostream>

namespace QRCode
{
    // 接收数据格式
    class receiveData{
    public:
        short num1;
        short num2;
        unsigned char num3;
    };
    // 串口设备的接收接口类
    class serialPortRead:public serialPort
    {
    public:
        serialPortRead();
        ~serialPortRead() = default;
        receiveData getReceiveMsg();
        void readData(unsigned char* flag);
    private:
        unsigned char temptemp[2*8];
        unsigned char temp[8];
        int max_receive_len_ = 8;
        unsigned char read_data_[8];
        receiveData receive_{};
        receiveData stm32;
        void process();
    };
}
#endif //__SERIAL_PORT_THREAD_H

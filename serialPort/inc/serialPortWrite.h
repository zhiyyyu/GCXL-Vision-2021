#ifndef __SERIAL_WRITE_PORT_THREAD_H
#define __SERIAL_WRITE_PORT_THREAD_H

#include "serialPort.h"

namespace QRCode
{
    // 发送数据格式
    class sendData{
    public:
        short yaw;
        short pitch;
        short distance;
    };
    // 串口设备的发送接口类
    class serialPortWrite:public serialPort
    {
    public:
        serialPortWrite() = default;
        explicit serialPortWrite(const serialPort_dev& config);
        ~serialPortWrite();
        void setSendMsg(sendData data);
        void writeData();
    private:
        int max_receive_len_ = 8;
        sendData send_{};
        void process();
    };
}
#endif //__SERIAL_PORT_THREAD_H

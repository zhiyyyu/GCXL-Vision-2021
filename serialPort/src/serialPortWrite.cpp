#include "serialPortWrite.h"
#include <iostream>
#include <utility>

namespace QRCode{
    /**
     * @brief 构造函数
     * @param portName 串口名称
     */
    serialPortWrite::serialPortWrite(const serialPort_dev& config)
    {
        if(config.enable)
        {
            init(config.deviceName);
        }
    }
    serialPortWrite::~serialPortWrite()
    {
    }
    /**
     * @brief 串口线程函数
     */
    void serialPortWrite::process()
    {
        writeData();
    }
    /**
     * @brief 写数据
     */
    void serialPortWrite::writeData()
    {
        unsigned char msg[max_receive_len_];

        auto temp = send_;

        //@TODO 添加CRC
        msg[0] = '!';
        msg[1] = 0x05;
        unsigned char *tmp = (unsigned char*)(&temp.pitch);
        std::cout<<"send pitch:"<<temp.pitch<<std::endl;
        msg[2]=tmp[1];
        msg[3]=tmp[0];

        tmp = (unsigned char*)(&temp.yaw);
        std::cout<<"send yaw:"<<temp.yaw<<"\n";
        msg[4]=tmp[1];
        msg[5]=tmp[0];

        msg[6]=temp.distance;

        msg[7] = '#';
        Append_CRC8_Check_Sum(msg+1, 8-2);

        //@TODO 发送数据
        SP_Write(msg, max_receive_len_);
    }
    /**
     * @brief 设置需要发送的数据
     * @param data 发送数据
     */
    void serialPortWrite::setSendMsg(sendData data)
    {
        send_ = data;
    }
}
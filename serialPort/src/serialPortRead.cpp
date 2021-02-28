#include "serialPortRead.h"

namespace QRCode{
    /**
     * @brief 构造函数
     * @param portName 串口名称
     */
    serialPortRead::serialPortRead(const serialPort_dev& config)
    {
        if(config.enable)
        {
            init(config.deviceName);
        }
    }
    serialPortRead::~serialPortRead()
    {
        //delete read_data_;
    }
    /**
     * @brief 串口线程函数
     */
    void serialPortRead::process()
    {
//        std::cout<<"read cost time:"<<1000.f/counter_.countEnd()<<std::endl;
//        counter_.countBegin();
        readData();
    }
    /**
     * @brief 读数据
     */
    void serialPortRead::readData()
    {
        short flag, counter, Gyro_Pitch, Gyro_Yaw;
        SP_Read(read_data_,max_receive_len_);
        //@TODO 读取数据
        memcpy(temptemp+8,read_data_,8);
        for(int start_bit=0;start_bit<8;start_bit++)
        {
            if(temptemp[start_bit]=='!')
            {
                for(int j=0;j<8;j++)
                {
                    temp[j]=temptemp[start_bit+j];
                }
                //@TODO CRC校验
                if(Verify_CRC8_Check_Sum(temp+1,8-2))//CRC校验
                {
                    flag = temp[1];
                    stm32.pitch = (temp[2]<<8)|temp[3];
                    stm32.yaw = (temp[4]<<8)|temp[5];

                    std::cout<<"-----------stm32.pitch:"<<stm32.pitch<<std::endl;
                    std::cout<<"stm32.yaw:"<<stm32.yaw<<std::endl;

                }
                break;
            }
        }
        memcpy(temptemp,temptemp+8,8);
        //互斥锁
        receive_ = stm32;
    }
    /**
     * @brief 获取收到的数据
     * @return 当前数据
     */
    receiveData serialPortRead::getReceiveMsg()
    {
        receiveData temp = receive_;
        return temp;
    }
}


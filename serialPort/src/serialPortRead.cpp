#include "serialPortRead.h"

namespace QRCode{
    /**
     * @brief 构造函数
     * @param portName 串口名称
     */
    serialPortRead::serialPortRead()
    {
        init("/dev/ttyUSB0");
    }
    /**
     * @brief 串口线程函数
     */
//    void serialPortRead::process()
//    {
//        readData();
//    }
    /**
     * @brief 读数据
     */
    void serialPortRead::readData(unsigned char* flag)
    {
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
                    flag = &temp[1];
                    stm32.num1 = (temp[2]<<8)|temp[3];
                    stm32.num2 = (temp[4]<<8)|temp[5];
                    stm32.num3 = (temp[6]);
//                    std::cout<<"stm32.x: "<<stm32.num1<<std::endl;
//                    std::cout<<"stm32.y: "<<stm32.num2<<std::endl;
//                    std::cout<<"stm32.z: "<<stm32.num3<<std::endl;
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


#include "serialPort.h"
#include <iostream>

using namespace boost::asio;
namespace QRCode {

/**
 * @brief 串口类的初始化
 * @param portName 串口的名称
 */
    serialPort::serialPort(std::string portName)
    {
        init(portName);
    }
    void serialPort::init(std::string portName)
    {
        try
        {
            sp_ = new serial_port(ioSev_, portName);
            setParam();
        }
        catch (...)
        {
            std::cerr << "Exception Error: " << err_.message() << std::endl;
        }
    }
/**
 * @brief  从而设置不同的参数
 */
    void serialPort::setParam()
    {
        sp_->set_option(serial_port::baud_rate(115200));
        sp_->set_option(serial_port::flow_control(serial_port::flow_control::none));
        sp_->set_option(serial_port::parity(serial_port::parity::none));
        sp_->set_option(serial_port::stop_bits(serial_port::stop_bits::one));
        sp_->set_option(serial_port::character_size(8));
    }

/**
 * @brief 读取串口数据，做错误处理
 */
    int serialPort::SP_Read(unsigned char *msg,int max_len)
    {
        try
        {
            read(*sp_, boost::asio::buffer(msg, max_len), err_);
        }
        catch (...)
        {
            std::cerr << "Exception Error: " << err_.message() << std::endl;
        }
        return 0;
    }

/**
 * @brief 写入串口数据，做错误处理
 */
    int serialPort::SP_Write(unsigned char *msg, int len)
    {
        try
        {
            boost::asio::write(*sp_, boost::asio::buffer(msg, (size_t)len), err_);
        }
        catch (...)
        {
            std::cerr << "Exception Error: " << err_.message() << std::endl;
        }
        return 0;
    }

// crc 校验函数

/**
 * @brief 在数据的末尾添加crc8的校验码
 * @param pchMessage 需要添加校验码的数据
 * @param dwLength   数据的长度
 */
    void serialPort::Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength)
    {
        unsigned char ucCRC = 0;
        if ((pchMessage == nullptr) || (dwLength <= 2))
            return;
        ucCRC = Get_CRC8_Check_Sum ( (unsigned char *)pchMessage, dwLength-1, CRC8_INIT);
        pchMessage[dwLength-1] = ucCRC;
    }
/**
 * @brief 获取当前校验数据的校验码
 * @param pchMessage 待校验数据
 * @param dwLength   待校验数组长度
 * @param ucCRC8     初始校验码
 * @return 校验码
 */
    unsigned char serialPort::Get_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength, unsigned char ucCRC8)
    {
        unsigned char ucIndex;
        while (dwLength--)
        {
            ucIndex = ucCRC8^(*pchMessage++);
            ucCRC8 = CRC8_TAB[ucIndex];
        }
        return(ucCRC8);
    }
/**
 * @brief 判断数组的校验数据是否正确
 * @param pchMessage  待校验数组
 * @param dwLength    待校验数据长度
 * @return  校验结构
 */
    unsigned int serialPort::Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength)
    {
        unsigned char ucExpected = 0;
        if ((pchMessage == 0) || (dwLength <= 2))
            return 0;
        ucExpected = Get_CRC8_Check_Sum (pchMessage, dwLength-1, CRC8_INIT);
        return ( ucExpected == pchMessage[dwLength-1] );
    }

}

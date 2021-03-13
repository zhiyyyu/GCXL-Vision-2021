#ifndef QRCODE_TRANSFORM_H
#define QRCODE_TRANSFORM_H

#include <opencv2/opencv.hpp>
#include "sophus/se3.h"
#include "sophus/so3.h"
#include "eigen3/Eigen/Dense"
#include "serialPortRead.h"
#include "serialPortWrite.h"

#define PI (3.14159)

namespace QRCode{

    class transform{
    public:
        transform();
        ~transform() = default;
        void receiveFromSerialPort(receiveData& receive_);
        void sendToSerialPort(receiveData sendData);
        void setArmor2World(Sophus::SE3 target2Camera, unsigned char flag);
        void setImu(short x, short y, unsigned char z);
        void calPose(Sophus::SE3 target);

    private:

        Sophus::SE3 camera2arm;
        Sophus::SE3 target;
        sendData send_{};
        serialPortWrite* serialPortWrite_;
    };
}

#endif //QRCODE_TRANSFORM_H

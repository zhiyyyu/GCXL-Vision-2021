#ifndef QRCODE_TRANSFORM_H
#define QRCODE_TRANSFORM_H

#include <opencv2/opencv.hpp>
#include "sophus/se3.h"
#include "sophus/so3.h"
#include "eigen3/Eigen/Dense"

#define pi 3.14159

namespace QRCode{

    typedef struct dataPack{
        short yaw;
        short pitch;
        short distance;
    } dataPack;

    class transform{
    public:
        transform();
        ~transform();
        void receiveFromSerialPort(dataPack sendData);
        void sendToSerialPort(dataPack sendData);
        void setArmor2World(Sophus::SE3 armor2Camera);
        void setImu(float yaw, float pitch);
        void calPose(Sophus::SE3 armor);

    private:

        dataPack receiceData;
        dataPack sendData;
        Sophus::SE3 world;
        Sophus::SE3 gimbal;
        Sophus::SE3 imu;
        Sophus::SE3 camera;
        Sophus::SE3 armor;
        Sophus::SE3 camera2gimbal;

    };
}

#endif //QRCODE_TRANSFORM_H

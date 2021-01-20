#ifndef QRCODE_TRANSFORM_H
#define QRCODE_TRANSFORM_H

#include <opencv2/opencv.hpp>
#include "sophus/se3.h"
#include "sophus/so3.h"
#include "eigen3/Eigen/Dense"

namespace QRCode{
    typedef struct dataPack{
        short yaw;
        short pitch;
    } dataPack;

    class transform{
    public:
        transform();
        ~transform();
        void receiveFromSerialPort(float yaw, float pitch);
        void sendToSerialPort(float yaw, float pitch);
        void setArmor2World();
        void setImu(float yaw, float pitch);

    private:

        dataPack receiceData;
        dataPack sendData;
        Sophus::SE3 world;
        Sophus::SE3 gimbal;
        Sophus::SE3 imu;
        Sophus::SE3 camera;
        Sophus::SE3 armor;

    };
}

#endif //QRCODE_TRANSFORM_H

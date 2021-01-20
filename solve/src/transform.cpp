/**
 * @file
 * @brief
 * @author
 * @notes
 */
#include "transform.h"

namespace QRCode{
    transform::transform(){

    }

    transform::~transform(){

    }

    void transform::receiveFromSerialPort(float yaw, float pitch){
        return;
    }

    void transform::sendToSerialPort(float yaw, float pitch){
        return;
    }

    void transform::setImu(float yaw, float pitch){
//        Eigen::Matrix3d imuRotation;
//        imuRotation = Eigen::AngleAxisd(0, Eigen::Vector3d::UnitY()) *
//                      Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ()) *
//                      Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitX());
//        imu = Sophus::SE3(imuRotation, Eigen::Vector3d(0, 0, 0));
        return;
    }

    void transform::setArmor2World(){

        return;
    }
}
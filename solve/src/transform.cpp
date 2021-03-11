/**
 * @file
 * @brief
 * @author
 * @notes
 */
#include "transform.h"

namespace QRCode{
    transform::transform(){
        world = Sophus::SE3(Eigen::Matrix3d::Identity(), Eigen::Vector3d(0,0,0));
        imu = Sophus::SE3(Eigen::Matrix3d::Identity(), Eigen::Vector3d(0,0,0));
        camera = Sophus::SE3(Eigen::Matrix3d::Identity(), Eigen::Vector3d(0,0,0));
        gimbal = Sophus::SE3(Eigen::Matrix3d::Identity(), Eigen::Vector3d(0,0,0));
//        Eigen::Vector3d cameraTrans = Eigen::Vector3d(0, 0, 0);
        Eigen::Vector3d cameraTrans = Eigen::Vector3d(-40, 40, 0);
        camera2gimbal = Sophus::SE3(Eigen::Matrix3d::Identity(), cameraTrans);
    }

    transform::~transform(){

    }

    void transform::receiveFromSerialPort(dataPack receiveData){
        receiveData.pitch = 0;
        receiveData.yaw = 0;
        return;
    }

    void transform::sendToSerialPort(dataPack sendData){

        return;
    }

    void transform::setImu(float yaw, float pitch){
        Eigen::Matrix3d imuRotation;
        imuRotation = Eigen::AngleAxisd(0, Eigen::Vector3d::UnitY()) *
                      Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ()) *
                      Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitX());
        imu = Sophus::SE3(imuRotation, Eigen::Vector3d(0, 0, 0));
        return;
    }

    void transform::setArmor2World(Sophus::SE3 armor2camera){
        receiveFromSerialPort(receiceData);
        setImu(receiceData.yaw, receiceData.pitch);
        gimbal = imu;
        camera = camera2gimbal * gimbal;
        armor = armor2camera * camera;
        std::cout << "armor" << armor << std::endl;
        calPose(armor);
        std::cout << "sendData" << sendData.yaw << " " << sendData.pitch << " " << sendData.distance << std::endl;
        return;
    }

    void transform::calPose(Sophus::SE3 armor){
        float distance = (float)sqrt(pow(armor.translation()[0], 2) +
                                      pow(armor.translation()[1], 2) +
                                      pow(armor.translation()[2], 2));
        float yaw = (float)(atan2(armor.translation()[0], armor.translation()[2])/PI*180);
        float pitch = (float)(atan2(armor.translation()[1], armor.translation()[2])/PI*180);
        sendData.yaw = (short)(yaw*100);
        sendData.pitch = (short)(pitch*100);
        sendData.distance = (short)(distance);
        return;
    }
}
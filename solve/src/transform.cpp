/**
 * @file
 * @brief
 * @author
 * @notes
 */
#include "transform.h"

namespace QRCode{
    transform::transform(){
        // TODO 标定
        Eigen::Vector3d cameraTrans = Eigen::Vector3d(0, 0, 0);
        camera2arm = Sophus::SE3(Eigen::Matrix3d::Identity(), cameraTrans);
        serialPortWrite_ = new serialPortWrite();
    }
    transform::~transform(){
        delete serialPortWrite_;
    }

    void transform::setArmor2World(Sophus::SE3 target2camera, unsigned char flag){
        // target->camera->arm->world
        target = camera2arm * target2camera;
        calPose(target);
        serialPortWrite_->setSendMsg(send_);
        serialPortWrite_->writeData(flag);
        return;
    }

    void transform::calPose(Sophus::SE3 target){
//        float distance = (float)sqrt(pow(armor.translation()[0], 2) +
//                                      pow(armor.translation()[1], 2) +
//                                      pow(armor.translation()[2], 2));
        float yaw = (float)(atan2(target.translation()[0], target.translation()[2])/PI*180);
        float pitch = (float)(atan2(target.translation()[1], target.translation()[2])/PI*180);

        send_.num1 = (short)(yaw*100);
        send_.num2 = (short)(pitch*100);
//        sendData.distance = (unsigned char)(distance);
        return;
    }
}
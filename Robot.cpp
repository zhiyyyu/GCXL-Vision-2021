#include "Robot.h"

namespace QRCode{
    Robot::Robot() {
        // TODO 相机标定
        solvePNP_ = new solvePNP();
        transform_ = new transform();
        detect_ = new detect();
        serialPortWrite_ = new serialPortWrite();
        serialPortRead_ = new serialPortRead();
    }
    Robot::~Robot(){
        delete solvePNP_;
        delete transform_;
        delete detect_;
    }

    /**
     * @brief 检测二维码->二维码检测完->等待检测物料指令->开始检测物料->抓到物料->等待指令检测色环->开始检测色环->放置完成
     *
     *
     * @param img
     */
    void Robot::process(cv::Mat img) {
        // 检测二维码
        if(qrData.empty()) {
            try{
                qrData = detect_->QRCodeDetector(img);
                sendData send_;
                flag = 0x05;
                send_.num1 = ((qrData[0]-'0') << 8) | ((qrData[1]-'0') << 4) | (qrData[2]-'0');
                send_.num2 = ((qrData[4]-'0') << 8) | ((qrData[5]-'0') << 4) | (qrData[6]-'0');
                serialPortWrite_->setSendMsg(send_);
                serialPortWrite_->writeData(flag);
//                std::cout << "data: " << qrData << std::endl;
            }
            catch (cv::Exception){
                std::cout << "cv::Exception ocurred." << std::endl;
            }
            return;
        }
        // 检测物料颜色顺序
        else if(!isGetMaterial){
//            std::cout << "catching!" << std::endl;
            try{
                receiveData receive_;
                serialPortRead_->readData(&flag);
                receive_ = serialPortRead_->getReceiveMsg();
                if(flag == 0x06 && receive_.num1 == 0x11 && receive_.num2 == 0x11) {                               // 开始抓取物料,开始检测物料
                    color = qrData[front];
                    detectMaterialColor(img, color);
                }
                else if(flag == 0x06 && receive_.num1 == 0x22 && receive_.num2 == 0x22){                                // 抓到物料了
                    isGetMaterial = true;
                }
            }
            catch(...){

            }
            return;
        }
        // 检测色环
        else if(!isPutDown){
//            std::cout << "putting it down." << std::endl;
            try{
                receiveData receive_;
                serialPortRead_->readData(&flag);
                receive_ = serialPortRead_->getReceiveMsg();
                if(flag == 0x07 && receive_.num1 == 0x11 && receive_.num2 == 0x11) {                           // 开始放置物料，检测色环
                    color = qrData[front];
                    detectRingColor(img, color);
                    updateColor = true;
                }
                else if(flag == 0x07 && receive_.num1 == 0x22 && receive_.num2 == 0x22){             // 物料放置成功
                    front++;
                    isPutDown = true;
                    isGetMaterial = front == 7;
                    updateColor = false;
                }
            }
            catch(...){

            }
            return;
        }
        return;
    }

    void Robot::detectMaterialColor(cv::Mat img, int color) {
        cv::Point2f point2f = detect_->findMaterialInCenter(img, color);
//        std::cout << "x: " << point2f.x << "y: " << point2f.y << std::endl;
        if(point2f.x == -1 && point2f.y == -1){
            return;
        }
        // 将坐标变换到爪子坐标系下
        Sophus::SE3 target2camera;
        transform_->setArmor2World(target2camera, 0x06);

        // 发送数据
        sendData send_{};
        send_.num1 = (short)point2f.x;
        send_.num2 = (short)point2f.y;
        flag = 0x06;
//        std::cout << "send num1: " << send_.num1 <<
//                      "send num2: " << send_.num2 << std::endl;
        serialPortWrite_->setSendMsg(send_);
        serialPortWrite_->writeData(flag);
        return;
    }

    void Robot::detectRingColor(cv::Mat img, int color) {
        cv::Point2f point2f = detect_->chromaticRingDetector(img, color);
//        std::cout << "x: " << point2f.x << "y: " << point2f.y << std::endl;
        if(point2f.x == -1 && point2f.y == -1){
            return;
        }
        float temp_x, temp_y, temp_z;
        temp_x = point2f.x;
        temp_y = point2f.y;
        temp_z = 0;
        point2f.x = -temp_z;
        point2f.y = -temp_y;
        temp_z = temp_x;
        // 将坐标变换到爪子坐标系下
        Sophus::SE3 target2camera;
        transform_->setArmor2World(target2camera, 0x07);

        // 发送数据
        sendData send_{};
        send_.num1 = (short)point2f.x;
        send_.num2 = (short)point2f.y;
        flag = 0x07;
//        std::cout << "send num1: " << send_.num1 <<
//                  "send num2: " << send_.num2 << std::endl;
        serialPortWrite_->setSendMsg(send_);
        serialPortWrite_->writeData(flag);

    }
}
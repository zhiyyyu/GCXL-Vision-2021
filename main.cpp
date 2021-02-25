#include "Robot.h"

int main() {

    QRCode::Robot* robot = new QRCode::Robot();

    cv::Mat qrcode = robot->loadImg(robot->root+robot->qrcode_path);
    cv::Mat barcode = robot->loadImg(robot->root+robot->barcode_path);
//    cv::Mat material = robot->loadImg();
//    robot->process(qrcode,  QRCODE_MODE);
    robot->process(barcode, BARCODE_MODE);
//    robot->process(material, MATERIAL_MODE);
    return 0;
}
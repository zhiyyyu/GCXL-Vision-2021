#ifndef QRCODE_AUTOATTACK_H
#define QRCODE_AUTOATTACK_H

#include "solvePnP.h"
#include "transform.h"
#include "thread1.h"
#include "thread2.h"
#include "detect.h"
#include <ctime>

#define USE_PIC 0
#define USE_VIDEO 0

//@TODO 改用枚举变量
#define QRCODE_MODE 0
#define BARCODE_MODE 1
#define MATERIAL_MODE 2

namespace QRCode{
    class Robot{
    public:
        Robot();
        ~Robot();
        cv::Mat loadImg(std::string path);
        void process(cv::Mat img, int mode);

        std::string root = "/home/narrow/QRCodeDetector/";
        std::string qrcode_path = "./QRCode.jpg";
        std::string barcode_path  = "./BarCode.jpg";

//        typedef enum detectMode{
//            QRCODE = 0,
//            BARCODE = 1,
//            MATERIAL = 2,
//        } detectMode;

        thread1* thread1_;
        thread2* thread2_;
        solvePNP* solvePNP_;
        transform* transform_;
        detect* detect_;
//        Sophus::SE3 armor2camera;
    };
}

#endif //QRCODE_AUTOATTACK_H

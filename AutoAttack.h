#ifndef QRCODE_AUTOATTACK_H
#define QRCODE_AUTOATTACK_H

#include "solvePnP.h"
#include "transform.h"
#include "thread1.h"
#include "thread2.h"
#include "detect.h"
#include "locate.h"
#include "camera.h"
#include "DAHEN_camera.h"
#include <ctime>

#define SHOW_RESULT 1
#define SHOW_TWO_PARTS 0
#define USE_PIC 0
#define USE_VIDEO 1
#define DEBUG 0

namespace QRCode{
    class Robot{
    public:
        Robot();
        ~Robot();
        void train();
        void attack();
        void process(cv::Mat img);
        cv::Mat getImage(std::string path);
        cv::Mat getFrame();

        std::string path = "/2.mp4";

        thread1* thread1_;
        thread2* thread2_;
        solvePNP* solvePNP_;
        transform* transform_;
        detect* detect_;
        locate* locate_;
        DAH_Camera* camera_;
//        Sophus::SE3 armor2camera;
    };
}

#endif //QRCODE_AUTOATTACK_H

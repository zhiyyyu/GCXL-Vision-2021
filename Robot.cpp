#include "Robot.h"

namespace QRCode{
    Robot::Robot() {
//        thread1_ = new thread1();
//        thread2_ = new thread2();
        solvePNP_ = new solvePNP();
        transform_ = new transform();
        detect_ = new detect();
    }
    Robot::~Robot(){
//        delete thread1_;
//        delete thread2_;
        delete solvePNP_;
        delete transform_;
        delete detect_;
    }

    cv::Mat Robot::loadImg(std::string path){
#if USE_PIC
        //        for(int i=0; i<5; i++){
//            cv::Mat imgR = cv::imread(locate_->root + "/R" + std::to_string(i) + ".png");
//            process(imgR);
//            cv::Mat imgB = cv::imread(locate_->root + "/B" + std::to_string(i) + ".png");
//            process(imgB);
//        }
        cv::Mat img = cv::imread(locate_->root + "/B1_1.jpg");
        process(img);
#endif
#if USE_VIDEO
        cv::VideoCapture video;
        bool isOpen = video.open(path);
//        std::cout << isOpen << std::endl;
        cv::Mat img;
        int cnt = 0;
        while(video.read(img)){
            cnt++;
            if(cnt % 10 == 0)
                process(img);
        }
        video.release();
#endif
    return cv::imread(path);
    }
    void Robot::process(cv::Mat img, int mode) {
        //@TODO 多线程
        switch(mode){
            case QRCODE_MODE:
                detect_->QRCodeDetector(img);
                break;
            case BARCODE_MODE:
                detect_->BarCodeDetector(img);
                break;
            case MATERIAL_MODE:
                detect_->MaterialDetector(img);
                break;
            default:
                break;
        }
        return;
    }
}
#include "AutoAttack.h"

namespace QRCode{
    RoboMasterS1::RoboMasterS1() {
//        thread1_ = new thread1();
//        thread2_ = new thread2();
//        solvePNP_ = new solvePNP();
//        transform_ = new transform();
        detect_ = new detect();
        locate_ = new locate();
    }
    RoboMasterS1::~RoboMasterS1(){
//        delete thread1_;
//        delete thread2_;
        delete solvePNP_;
        delete transform_;
        delete detect_;
        delete locate_;
    }
    void RoboMasterS1::attack(){
        cv::Mat img = cv::imread(locate_->root + "/3.png");
        std::vector<cv::RotatedRect> rect;
        rect = detect_->getDirectionFlags(img);
        cv::Mat visualLabel;
        visualLabel = detect_->correctVisualLabel(img.clone(), rect);
//        cv::Mat trainMat, labelMat;
//        std::pair<cv::Mat, cv::Mat> data = locate_->loadData();
//        trainMat = data.first, labelMat = data.second;
//        locate_->decode(trainMat, labelMat);
//        locate_->predict();
//        std::cout << trainMat << labelMat << std::endl;
//        solvePNP_->detect();
//        solvePNP_->display();
//        solvePNP_->solve();
//        transform_->setArmor2World(solvePNP_->TransformMatrix);
    }
}
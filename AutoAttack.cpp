#include "AutoAttack.h"

namespace QRCode{
    Robot::Robot() {
//        thread1_ = new thread1();
//        thread2_ = new thread2();
//        solvePNP_ = new solvePNP();
//        transform_ = new transform();
        detect_ = new detect();
        locate_ = new locate();
    }
    Robot::~Robot(){
//        delete thread1_;
//        delete thread2_;
        delete solvePNP_;
        delete transform_;
        delete detect_;
        delete locate_;
    }
    void Robot::train(){
        cv::Mat trainMat, labelMat;
        std::pair<cv::Mat, cv::Mat> data = locate_->loadData();
        trainMat = data.first, labelMat = data.second;
        locate_->decode(trainMat, labelMat);
        std::cout << labelMat.size << labelMat << std::endl;
    }

    void Robot::attack(){
        cv::Mat img = cv::imread(locate_->root + "/5.png");
        cv::imshow("img", img);
        std::vector<cv::RotatedRect> rect;
        rect = detect_->getDirectionFlags(img);
        cv::Mat visualLabel = detect_->correctVisualLabel(img.clone(), rect);
        std::pair<cv::Mat, cv::Mat> twoParts = locate_->divideIntoTwoParts(visualLabel);
        cv::Mat leftHalf = twoParts.first;
        cv::Mat rightHalf = twoParts.second;
//        cv::imshow("left", leftHalf);
//        cv::imshow("right", rightHalf);
//        cv::waitKey(0);
        int leftOcr = locate_->predict(leftHalf);
        int rightOcr = locate_->predict(rightHalf);
        std::cout << leftOcr << rightOcr << std::endl;
        std::vector<int> location = locate_->getLocation(leftOcr*10+rightOcr);
//        for(int i=0; i<4; i++){
//            std::cout << location[i] << std::endl;
//        }
        cv::putText(visualLabel, locate_->getString(location), cv::Point(10, 120), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255, 0, 0));
        cv::imshow("visualLable", visualLabel);
        cv::waitKey(0);
//        locate_->predict();
//        std::cout << trainMat << labelMat << std::endl;
//        solvePNP_->detect();
//        solvePNP_->display();
//        solvePNP_->solve();
//        transform_->setArmor2World(solvePNP_->TransformMatrix);
    }
}
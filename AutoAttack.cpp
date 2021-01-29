#include "AutoAttack.h"

namespace QRCode{
    RoboMasterS1::RoboMasterS1() {
//        thread1_ = new thread1();
//        thread2_ = new thread2();
        solvePNP_ = new solvePNP();
        transform_ = new transform();
        detect_ = new detect();
    }
    RoboMasterS1::~RoboMasterS1(){
//        delete thread1_;
//        delete thread2_;
        delete solvePNP_;
        delete transform_;
    }
    void RoboMasterS1::attack(){
//        solvePNP_->detect();
//        solvePNP_->display();
//        solvePNP_->solve();
//        transform_->setArmor2World(solvePNP_->TransformMatrix);
    }
}
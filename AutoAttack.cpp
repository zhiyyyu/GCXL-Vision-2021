#include "AutoAttack.h"

namespace QRCode{
    RoboMasterS1::RoboMasterS1() {

    }
    void RoboMasterS1::attack(){
        solvePNP* solvePNP_ = new solvePNP();
        solvePNP_->detect();
        solvePNP_->display();
        solvePNP_->solve();
    }
}
#include "solvePnP.h"
//#include "S1AutoAttack.h"

int main() {

    QRCode::solvePNP* solvePNP_ = new QRCode::solvePNP();
    solvePNP_->detect();
    solvePNP_->display();
    solvePNP_->solve();

    return 0;
}
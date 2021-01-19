#include "solvePnP.h"

int main() {

    QRCode::solvePNP* solvePNP_ = new QRCode::solvePNP();
    solvePNP_->detect();
    solvePNP_->display();
    solvePNP_->solve();

    return 0;
}
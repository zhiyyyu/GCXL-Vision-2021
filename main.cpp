#include "solvePnP.h"
#include "AutoAttack.h"
//#include "S1AutoAttack.h"

int main() {

    QRCode::RoboMasterS1* s1 = new QRCode::RoboMasterS1();
    s1->attack();
    return 0;
}
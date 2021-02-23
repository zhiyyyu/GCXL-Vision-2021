#include "AutoAttack.h"

int main() {

    QRCode::Robot* robot = new QRCode::Robot();
    robot->train();
    robot->attack();
    return 0;
}
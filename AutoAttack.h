#ifndef QRCODE_AUTOATTACK_H
#define QRCODE_AUTOATTACK_H

#include "solvePnP.h"
#include "transform.h"
#include "thread1.h"
#include "thread2.h"
#include "detect.h"

namespace QRCode{
    class RoboMasterS1{
    public:
        RoboMasterS1();
        ~RoboMasterS1();
        void attack();

        thread1* thread1_;
        thread2* thread2_;
        solvePNP* solvePNP_;
        transform* transform_;
        detect* detect_;
//        Sophus::SE3 armor2camera;
    };
}

#endif //QRCODE_AUTOATTACK_H

#ifndef QRCODE_AUTOATTACK_H
#define QRCODE_AUTOATTACK_H

#include "solvePnP.h"
#include "transform.h"
#include "detect.h"
#include <ctime>
#include "serialPortRead.h"
#include "serialPortWrite.h"

#define SHOW_RAW_IMG 0

#define RED 1
#define GREEN 2
#define BLUE 3

namespace QRCode{
    class Robot{
    public:
        Robot();
        ~Robot();
        void process(cv::Mat img);
        void detectMaterialColor(cv::Mat img, int color);
        void detectRingColor(cv::Mat img, int color);

        std::string qrData;
        int front = 0;
        int color = 0;

        const float offset_x = 47.25;
        const float offset_y = 49.5;
        const float offset_z = 13.75;

        unsigned char flag;

        bool updateColor = false;
        bool isGetMaterial = false;
        bool isPutDown = false;

    private:
        detect* detect_;
        serialPortRead* serialPortRead_;
        serialPortWrite* serialPortWrite_;
        solvePNP* solvePNP_;
        transform* transform_;
    };
}

#endif //QRCODE_AUTOATTACK_H

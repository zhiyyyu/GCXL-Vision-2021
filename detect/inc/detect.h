#ifndef QRCODE_DETECT_H
#define QRCODE_DETECT_H

#include "score.h"
#include "numeric"
#include <cmath>

#define PI (3.14)

namespace QRCode{
    class detect{
    public:
        detect() = default;
        ~detect() = default;
        cv::Mat QRCodeDetector(cv::Mat img);
        cv::Mat BarCodeDetector(cv::Mat img);
        cv::Mat MaterialDetector(cv::Mat img);
        void display(cv::Mat img, cv::Mat ROI, std::vector<cv::Point> list);

        const int thresh = 150;
        const int maxval = 255;
    };
}

#endif //QRCODE_DETECT_H
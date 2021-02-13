#ifndef QRCODE_DETECT_H
#define QRCODE_DETECT_H

#include "score.h"

#define DEBUG 1
#define LABEL_DEBUG 1

namespace QRCode{
    class detect{
    public:
        detect();
        ~detect() = default;
        std::vector<cv::RotatedRect> getDirectionFlags(cv::Mat image);
        cv::Mat correctVisualLabel(cv::Mat img, std::vector<cv::RotatedRect> rect);
        cv::Mat getROI(cv::Mat img, cv::Point2f tl, cv::Point2f tr, cv::Point2f bl);
        float calInnerProduct(cv::Point2f p1, cv::Point2f p2, cv::Point2f p3);
        float calOuterProduct(cv::Point2f p1, cv::Point2f p2, cv::Point2f p3);

        int thresh = 150;
        int maxval = 255;
    };
}

#endif //QRCODE_DETECT_H
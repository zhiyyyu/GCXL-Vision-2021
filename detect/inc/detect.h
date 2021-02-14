#ifndef QRCODE_DETECT_H
#define QRCODE_DETECT_H

#include "score.h"
#include "numeric"
#include <cmath>

#define DEBUG 0
#define LABEL_DEBUG 1
#define CONTOURS_DEBUG 0
#define IMAGE_SHOW 0
#define FEATURE_DEBUG 0
#define SHOW_ROI 1

#define PI (3.14)

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
        bool judgeFlags(cv::Mat img, std::vector<cv::Point> contour);

        int thresh = 150;
        int maxval = 255;
    };

    typedef struct labelFlag{
        float w;
        float h;
        float widthHeightRatio;
        /**
         *     _____
         *     \    \
         *      \    \
         *       \    \
         *        \____\_angle__
         *    |
         *    |
         *    |____ 90 degree
         *
         *     ____ 0 degree
         *
         */
        float angle;
        cv::Point2f center;
        cv::Point2f tl;
        cv::Point2f points[5];
        std::vector<int> judgePoints = {0, 0, 0, 0, 0};
        float offset = 5;
        float getWidthHeightRatio(){
            return w / h;
        }
        void setPointsFromCenter(cv::Point2f* points){
            /**points:
             *          0
             *      1   2   3
             *          4
             */
            float cosAngle = cos(angle/180*PI);
            float sinAngle = sin(angle/180*PI);
            float ht = h-offset*2;
            float wd = w-offset*2;
            points[0] = {center.x-ht/2*sinAngle, center.y-ht/2*cosAngle};
            points[1] = {center.x-wd/2*cosAngle, center.y+wd/2*sinAngle};
            points[2] = center;
            points[3] = {center.x+wd/2*cosAngle, center.y-wd/2*sinAngle};
            points[4] = {center.x+ht/2*sinAngle, center.y+ht/2*cosAngle};
            return;
        }
        void setPointsFromTopLeft(cv::Point2f* points){
            /**points:
             *          0
             *      1   2   3
             *          4
             */
            points[0] = {tl.x+w/2, tl.y+offset};
            points[1] = {tl.x+offset, tl.y+h/2};
            points[2] = {tl.x+w/2, tl.y+h/2};
            points[3] = {tl.x+w-offset, tl.y+h/2};
            points[4] = {tl.x+w/2, tl.y+h-offset};
            return;
        }
    } labelFlag;
}

#endif //QRCODE_DETECT_H
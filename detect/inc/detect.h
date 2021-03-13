#ifndef QRCODE_DETECT_H
#define QRCODE_DETECT_H

#include "zbar.h"
#include <opencv2/ml/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#define ZBAR_DETECTOR 1
#define MY_DETECTOR 0
#define SHOW_IMAGE 1

#define RED 1
#define GREEN 2
#define BLUE 3
#define TOP 1
#define BOT 0

#define PI (3.14159)

namespace QRCode{
    class detect{
    public:
        detect() = default;
        ~detect() = default;
        std::string QRCodeDetector(cv::Mat img);
        std::string BarCodeDetector(cv::Mat img);
        std::vector<cv::Point2f> MaterialDetector(cv::Mat img, int color);
        cv::Point2f chromaticRingDetector(cv::Mat img, int color);
        void display(cv::Mat img, cv::Mat ROI, std::vector<cv::Point> list);
        bool judgeBarCode(cv::RotatedRect rect);
        bool judgeMaterial(cv::RotatedRect rect, std::pair<int, int> size);
        cv::Point2f findMaterialInCenter(cv::Mat img, int color);
        cv::Mat getROI(cv::Mat img, std::vector<cv::RotatedRect> roi);

        const int thresh = 150;
        const int maxval = 255;
        const int width = 400;
        const int height = 300;

    };

    typedef struct barCodeNode{
        float height;
        float width;
        float widthHeightRatio;
        float areaRatio;
        float score;
    } barCodeNode;

    typedef struct materialNode{
        float height;
        float width;
        float widthHeightRatio;
        int color;
        float score;
    } masterialNode;
}

#endif //QRCODE_DETECT_H
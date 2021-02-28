#ifndef QRCODE_SOLVEPNP_H
#define QRCODE_SOLVEPNP_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>
#include "sophus/se3.h"
#include "sophus/so3.h"
//#include "transform.h"

namespace QRCode{
    class solvePNP{
    public:
        solvePNP();
        ~solvePNP();
        void solve(std::vector<cv::Point2f> Points2f);

        Sophus::SE3 TransformMatrix;

    private:
        void setCameraMatrix();
        void setDistCoeffs();

        cv::QRCodeDetector detector;
        cv::Mat result;

        cv::Mat cameraMatrix;
        cv::Mat distCoeffs;

        cv::Mat rvec;
        cv::Mat tvec;

//        transform* transform_;

    };
}

#endif //QRCODE_TRANSFORM_H
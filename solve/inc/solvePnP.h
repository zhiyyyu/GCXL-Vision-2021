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

#define MATERIAL_WIDTH 50
#define MATERIAL_HEIGHT 70
#define CR_RADIUS 47.5
#define MAT 1
#define CR 0

namespace QRCode{
    class solvePNP{
    public:
        solvePNP();
        ~solvePNP();
        Sophus::SE3 solve(std::vector<cv::Point2f> Points2f, int mode);

    private:
        void setCameraMatrix();
//        void setDistCoeffs();

        cv::QRCodeDetector detector;
        cv::Mat result;

        cv::Mat cameraMatrix;
        cv::Mat distCoeffs;
        cv::Mat measurement_;
        cv::KalmanFilter KF_;
        cv::Mat state_;
        cv::Mat processNoise_;

        cv::Mat rvec;
        cv::Mat tvec;

//        transform* transform_;

    };
}

#endif //QRCODE_TRANSFORM_H
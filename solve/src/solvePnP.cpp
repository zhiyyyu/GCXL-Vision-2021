/**
 * @file
 * @brief
 * @author
 * @notes
 */
#include "solvePnP.h"

namespace QRCode{
    solvePNP::solvePNP(){
        setCameraMatrix();
        setDistCoeffs();

        measurement_ = cv::Mat::zeros(6, 1, CV_32F);
        KF_ = cv::KalmanFilter(6,6,CV_32F);
        state_ = cv::Mat::zeros(6,1,CV_32F);
        processNoise_ = cv::Mat::zeros(6,1,CV_32F);

        KF_.transitionMatrix = (cv::Mat_<float>(6, 6) <<
                                                      1, 0, 0, 1, 0, 0,
                0, 1, 0, 0, 1, 0,
                0, 0, 1, 0, 0, 1,
                0, 0, 0, 1, 0, 0,
                0, 0, 0, 0, 1, 0,
                0, 0, 0, 0, 0, 1);
        setIdentity(KF_.measurementMatrix);
        setIdentity(KF_.processNoiseCov, cv::Scalar::all(1e-5));
        setIdentity(KF_.measurementNoiseCov, cv::Scalar::all(1e-1));
        setIdentity(KF_.errorCovPost, cv::Scalar::all(1));
        randn(KF_.statePost, cv::Scalar::all(0), cv::Scalar::all(0.1));
    }
    solvePNP::~solvePNP(){

    }

    void solvePNP::setCameraMatrix(){
        cameraMatrix = cv::Mat(3, 3, CV_64FC1, cv::Scalar::all(0));
        cameraMatrix.ptr<double>(0)[0] = 1273.6;
        cameraMatrix.ptr<double>(0)[2] = 636.9;
        cameraMatrix.ptr<double>(1)[1] = 1273.5;
        cameraMatrix.ptr<double>(1)[2] = 512.9;
        cameraMatrix.ptr<double>(2)[2] = 1.0f;
        return;
    }

    void solvePNP::setDistCoeffs(){
        distCoeffs = cv::Mat(5, 1, CV_64FC1, cv::Scalar::all(0));
        distCoeffs.ptr<double>(0)[0] = -0.2337;
        distCoeffs.ptr<double>(1)[0] = 0.2071;
        distCoeffs.ptr<double>(2)[0] = 0;
        distCoeffs.ptr<double>(3)[0] = 0;
        distCoeffs.ptr<double>(4)[0] = 0;
        return;
    }

    cv::Mat solvePNP::solve(std::vector<cv::Point2f> Points2f, int mode){
        std::vector<cv::Point2d> Points2d;
        if(Points2f.size() < 4){
            std::cout << "not detected: " << Points2f.size() << std::endl;
            cv::Mat empty;
            return empty;
        }
        for(auto i=0; i<4; i++){
            Points2d.emplace_back((cv::Point2d)Points2f[i]);
        }
        std::vector<cv::Point3d> Points3d;
        if(mode == MAT){
            Points3d.emplace_back(cv::Point3d(-MATERIAL_WIDTH/2, -MATERIAL_HEIGHT/2, 0));
            Points3d.emplace_back(cv::Point3d(MATERIAL_WIDTH/2, -MATERIAL_HEIGHT/2, 0));
            Points3d.emplace_back(cv::Point3d(MATERIAL_WIDTH/2, MATERIAL_HEIGHT/2, 0));
            Points3d.emplace_back(cv::Point3d(-MATERIAL_WIDTH/2, MATERIAL_HEIGHT/2, 0));
        }
        else if(mode == CR){
            Points3d.emplace_back(cv::Point3d(-CR_RADIUS, -CR_RADIUS, 0));
            Points3d.emplace_back(cv::Point3d(CR_RADIUS, -CR_RADIUS, 0));
            Points3d.emplace_back(cv::Point3d(CR_RADIUS, CR_RADIUS, 0));
            Points3d.emplace_back(cv::Point3d(-CR_RADIUS, CR_RADIUS, 0));
        }
        cv::solvePnP(Points3d, Points2d, cameraMatrix, distCoeffs, rvec, tvec, false, cv::SOLVEPNP_ITERATIVE);
//        std::cout << "rvec" << rvec <<  std::endl <<
//                  "tvec" << tvec << std::endl;

        KF_.predict();
        measurement_.at<float>(0) = (float)tvec.at<uchar>(0);
        measurement_.at<float>(1) = (float)tvec.at<uchar>(1);
        measurement_.at<float>(2) = (float)tvec.at<uchar>(2);
        measurement_.at<float>(3) = 0;
        measurement_.at<float>(4) = 0;
        measurement_.at<float>(5) = 0;
        KF_.correct(measurement_);

        std::cout << "distance: " << (float)tvec.at<uchar>(2) << std::endl;

        cv::Mat R;
        cv::Rodrigues(rvec, R);             //旋转向量->旋转矩阵
        Eigen::Matrix3d Rotate_M = Eigen::Matrix3d::Identity();
        cv::cv2eigen(R,Rotate_M);           //cv -> eigen
        Sophus::SO3 rotate(Rotate_M);       //eigen -> sophus
        Eigen::Vector3d translate(tvec.ptr<double>(0)[0],tvec.ptr<double>(0)[1],tvec.ptr<double>(0)[2]);
//        std::cout << "R" << R << std::endl;
//        std::cout << "Rotate_M" << Rotate_M << std::endl;
//        std::cout << "R_Eigen" << Rotate_M << std::endl;
//        std::cout << "R_SO3" << rotate << std::endl;
//        std::cout << "translate" << translate << std::endl;
        TransformMatrix = Sophus::SE3(rotate,translate);
//        std::cout << "TransformMatrix_SE3" << TransformMatrix << TransformMatrix.translation() << std::endl;

//        transform_ = new transform();
//        transform_->setArmor2World(TransformMatrix);
        return tvec;
    }
}
/**
 * @file
 * @brief
 * @author
 * @notes
 */
#include "solvePnP.h"

#define MATERIAL_WIDTH 50
#define MATERIAL_HEIGHT 70

namespace QRCode{
    solvePNP::solvePNP(){
        setCameraMatrix();
        setDistCoeffs();
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

    void solvePNP::solve(std::vector<cv::Point2f> Points2f){
        std::vector<cv::Point2d> Points2d;
        for(auto i=0; i<4; i++){
            Points2d.emplace_back((cv::Point2d)Points2f[i]);
        }
        std::vector<cv::Point3d> Points3d;
        Points3d.emplace_back(cv::Point3d(-MATERIAL_WIDTH/2, -MATERIAL_HEIGHT/2, 0));
        Points3d.emplace_back(cv::Point3d(MATERIAL_WIDTH/2, -MATERIAL_HEIGHT/2, 0));
        Points3d.emplace_back(cv::Point3d(MATERIAL_WIDTH/2, MATERIAL_HEIGHT/2, 0));
        Points3d.emplace_back(cv::Point3d(-MATERIAL_WIDTH/2, MATERIAL_HEIGHT/2, 0));

        cv::solvePnP(Points3d, Points2d, cameraMatrix, distCoeffs, rvec, tvec, false, cv::SOLVEPNP_ITERATIVE);

        std::cout << "rvec" << rvec << "\ntvec" << tvec << std::endl;

        cv::Mat R;
        cv::Rodrigues(rvec, R);             //旋转向量->旋转矩阵

        Eigen::Matrix3d Rotate_M = Eigen::Matrix3d::Identity();
        cv::cv2eigen(R,Rotate_M);           //cv -> eigen
        Sophus::SO3 rotate(Rotate_M);
        Eigen::Vector3d translate(tvec.ptr<double>(0)[0],tvec.ptr<double>(0)[1],tvec.ptr<double>(0)[2]);
//        std::cout << "R" << R << std::endl;
//        std::cout << "Rotate_M" << Rotate_M << std::endl;
//        std::cout << "R_Eigen" << Rotate_M << std::endl;
//        std::cout << "R_SO3" << rotate << std::endl;
//        std::cout << "translate" << translate << std::endl;
        TransformMatrix = Sophus::SE3(rotate,translate);
        std::cout << "TransformMatrix_SE3" << TransformMatrix << TransformMatrix.translation() << std::endl;

//        transform_ = new transform();
//        transform_->setArmor2World(TransformMatrix);
        return;
    }
}
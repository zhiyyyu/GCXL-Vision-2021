/**
 * @file
 * @brief
 * @author
 * @notes
 */
#include "solvePnP.h"

#define QRCODE_WIDTH 45.5

namespace QRCode{
    solvePNP::solvePNP(){
        setCameraMatrix();
        setDistCoeffs();
    }
    solvePNP::~solvePNP(){

    }
//    void solvePNP::hello() {
//        std::cout << "Hello, World! solvePNP" << std::endl;
//        return ;
//    }
    void solvePNP::loadImg(std::string path){
        qrcode = cv::imread(path);
//        cv::imshow("img", qrcode);
//        cv::waitKey(0);
        return;
    }

//    void solvePNP::myQRDetector(cv::Mat qrcode){
//        cv::cvtColor(qrcode, gray, cv::COLOR_BGR2GRAY);
//        cv::threshold(gray, thresh, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
//        cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
//        for(auto i=0; i<contours.size() ; i++){
//            candidates.at(i) = cv::minAreaRect(contours[i]);
//        }
//        // @TODO add
//        cv::imshow("thresh", thresh);
//    }

    void solvePNP::display(){
        cv::Mat img = qrcode.clone();
        for(int i=0;i<list.size()-1;i++){
            cv::line(img, list[i], list[i+1], cv::Scalar(0, 0, 255), 4);
        }
        cv::line(img, list[3], list[0], cv::Scalar(0, 0, 255), 4);

        cv::imshow("detected_qrcode", img);
        cv::imshow("result", result);
        cv::waitKey(0);
    }

//    void solvePNP::detect(){
//        loadImg(path);
////        myQRDetector(qrcode);
//        detector.detectAndDecode(qrcode, list, result);
//
//        return;
//    }

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

    void solvePNP::solve(){
        for(auto i=0; i<4; i++){
            Points2d.emplace_back(list[i]);
        }
        Points3d.emplace_back(cv::Point3d(-QRCODE_WIDTH/2, -QRCODE_WIDTH/2, 0));
        Points3d.emplace_back(cv::Point3d(QRCODE_WIDTH/2, -QRCODE_WIDTH/2, 0));
        Points3d.emplace_back(cv::Point3d(QRCODE_WIDTH/2, QRCODE_WIDTH/2, 0));
        Points3d.emplace_back(cv::Point3d(-QRCODE_WIDTH/2, QRCODE_WIDTH/2, 0));

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
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

namespace QRCode{
    class solvePNP{
    public:
        solvePNP();
        ~solvePNP();
        void hello();
        void loadImg(std::string path);
        void detect();
        void solve();
        void display();

    private:
        void setCameraMatrix();
        void setDistCoeffs();

        std::string path = "/home/narrow/QRCode3.bmp";
        cv::Mat qrcode;
//        cv::Mat gray;
//        cv::Mat thresh;
//        std::vector<std::vector<cv::Point>> contours;
//        std::vector<cv::RotatedRect> candidates;
        cv::QRCodeDetector detector;
        std::vector<cv::Point> list;
        cv::Mat result;

        std::vector<cv::Point2d> Points2d;
        std::vector<cv::Point3d> Points3d;

        cv::Mat cameraMatrix;
        cv::Mat distCoeffs;

        cv::Mat rvec;
        cv::Mat tvec;
    };
}
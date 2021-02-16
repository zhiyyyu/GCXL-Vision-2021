#ifndef QRCODE_LOCATE_H
#define QRCODE_LOCATE_H

#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#define DEBUG 0
#define TEST_IMG 0

using namespace cv::ml;

namespace QRCode{
    class locate{
    public:
        locate() = default;
        ~locate() = default;
        std::pair<cv::Mat, cv::Mat> loadData();
        void detect();
        void decode(cv::Mat trainMat, cv::Mat labelMat);
        int predict(cv::Mat testImg);
        std::pair<cv::Mat, cv::Mat> divideIntoTwoParts(cv::Mat img);
        std::vector<int> getLocation(int label);
        std::string getString(std::vector<int> location);

        int x;
        int y;
        int z;
        int angle;
        std::string root = "/home/narrow/ClionProjects/QRCodeDetector";

    private:

        std::string train_images_path = "./data/Images";
        std::string train_labels_path = "./data/Labels";
        std::string test_img_path = "./1.png";

        const int sampleNum = 8128; //8128
        const int height = 128;
        const int width = 128;
        std::vector<cv::Mat> trainSample;
        std::vector<int> labelSample;
        cv::Mat trainMat;
        cv::Mat labelMat;

        cv::Ptr<SVM> svm;

    };
}

#endif //QRCODE_LOCATE_H

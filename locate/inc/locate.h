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

using namespace cv::ml;

namespace QRCode{
    class locate{
    public:
        locate() = default;
        ~locate() = default;
        std::pair<cv::Mat, cv::Mat> loadData();
        void detect();
        void decode(cv::Mat trainMat, cv::Mat labelMat);
        void predict();

        std::string root = "/home/narrow/ClionProjects/QRCode";

    private:

        std::string train_images_path = "./data/Images";
        std::string train_labels_path = "./data/Labels";
        std::string test_img_path = "./1.png";

        const int sampleNum = 10; //8128
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

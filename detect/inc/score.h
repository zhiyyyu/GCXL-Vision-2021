#ifndef QRCODE_SCORE_H
#define QRCODE_SCORE_H

#include <opencv2/ml/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

namespace QRCode{
    class score{
    public:
        score() = default;
        ~score() = default;
        void loadData();
        void trainSVM(cv::Mat trainMat, cv::Mat labelMat);
        void trainBoost(cv::Mat trainMat, cv::Mat labelMat);
        float predict(cv::Mat testMat);
        void fitLightBars();

        template <typename ML> void display(ML model){
            cv::Mat img = cv::Mat::zeros(height, width, CV_8UC3);
            cv::Vec3b red(0, 0, 255), blue(255, 0, 0);

            for(int i=0;i<img.rows;i++){
                for(int j=0;j<img.cols;j++){
                    testMat = (cv::Mat_<float>(1, 2) << j, i);
                    float result = model->predict(testMat);
//                std::cout << result << std::endl;

                    if(result == 'B'){
                        img.at<cv::Vec3b>(i, j) = blue;
                    }
                    else if(result == 'R'){
                        img.at<cv::Vec3b>(i, j) = red;
                    }
                }
            }
            cv::imshow("img", img);
            cv::waitKey(0);
            return ;
        }

        int width = 125;
        int height = 125;
        int sampleNum = 42;
        int featureDim = 2;
        cv::Ptr<cv::ml::SVM> svm;
        cv::Ptr<cv::ml::Boost> boost;
        cv::Mat trainMat;
        cv::Mat labelMat;
        cv::Mat testMat;
    };
}

#endif //QRCODE_SCORE_H

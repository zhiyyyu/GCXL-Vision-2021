#include "locate.h"

using namespace cv::ml;
namespace fs = boost::filesystem;

namespace QRCode{
    std::pair<cv::Mat, cv::Mat> locate::loadData(){
        fs::path rootPath(root);
        fs::path imagePath(train_images_path);
        fs::path labelPath(train_labels_path);

        fs::path imagePath_ = rootPath / imagePath;
        fs::path labelPath_ = rootPath / labelPath;

        trainMat = cv::Mat::zeros(sampleNum, height*width, CV_32FC1);
        labelMat = cv::Mat::zeros(sampleNum, 1, CV_32SC1);

        int cnt = 0;
        fs::directory_iterator imageIter(imagePath_), end;
        while(imageIter != end){
            std::string image = imageIter->path().string();
            fs::path tempLabelPath_ = labelPath_ / fs::basename(image);
            std::string label = tempLabelPath_.string() + ".txt";

            cv::Mat img = cv::imread(image, 0);

            std::ifstream file;
            file.open(label);
            char class_;
            file >> class_;
            file.close();

            for(int i=0; i<height*width; i++){
                trainMat.at<float>(cnt, i) = (float)img.at<uchar>(i/height, i%height);
            }
            if(class_ == '0' || class_ == '1' || class_ == '2' || class_ == '3' || class_ == '4') {
                labelMat.at<int>(cnt, 0) = (int)(class_-'0');
            }
            else if(class_ == 'A'){ labelMat.at<int>(cnt, 0) = 5; }
            else if(class_ == 'B'){ labelMat.at<int>(cnt, 0) = 6; }
            else if(class_ == 'R'){ labelMat.at<int>(cnt, 0) = 7; }
            imageIter++, cnt++;
#if DEBUG
            std::cout << image << label << std::endl;
//            std::cout << img.cols << img.rows << img << std::endl;
            std::cout << class_ << std::endl;
#endif
            if(cnt == sampleNum) break;
        }
#if DEBUG
//        std::cout << imagePath << labelPath << std::endl;
//        std::cout << trainMat.cols << trainMat.rows << std::endl;
//        std::cout << sampleNum << std::endl;
//        std::cout << trainMat.size << std::endl;
//        std::cout << trainMat.size << "trainMat" << trainMat << std::endl;
#endif
        return std::make_pair(trainMat, labelMat);
    }
    void locate::detect() {

    }

    std::pair<cv::Mat, cv::Mat> locate::divideIntoTwoParts(cv::Mat img){
        int width = img.cols;
        int height = img.rows;
        cv::Mat leftHalf = img(cv::Rect(0, 0, width/2, height));
        cv::Mat rightHalf = img(cv::Rect(width/2, 0, width/2, height));
        return std::make_pair(leftHalf, rightHalf);
    };

    void locate::decode(cv::Mat trainDataMat, cv::Mat labelDataMat){
        // create SVM
        svm = SVM::create();
        // set params
        svm->setC(0.1);
        svm->setType(SVM::C_SVC);
        svm->setKernel(SVM::LINEAR);
        svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
        // train
        cv::Ptr<TrainData> trainData = TrainData::create(trainDataMat, ROW_SAMPLE, labelDataMat);
        svm->train(trainData);
        const std::string model = root+"/svm.xml";
        svm->save(model);
        return;
    }
    int locate::predict(cv::Mat testImg) {
        cv::cvtColor(testImg, testImg, cv::COLOR_BGR2GRAY);
        cv::threshold(testImg, testImg, 150, 255, cv::THRESH_BINARY);
#if TEST_IMG
        fs::path rootPath(root);
        fs::path test_path(test_img_path);
        fs::path img_path = rootPath / test_path;
        std::cout << img_path.string() << std::endl;
        testImg = cv::imread(img_path.string(), 0);
        std::cout << testImg.size << std::endl;
        cv::imshow("testImg", testImg);
        cv::waitKey(0);
#endif
        cv::Mat testMat = cv::Mat::zeros(1, height*width, CV_32FC1);
        for(int i=0; i<height*width; i++){
            testMat.at<float>(0, i) = (float)testImg.at<uchar>(i/height, i%height);
        }
        std::cout << testMat << std::endl;
#if DEBUG
        std::cout << testMat.size << "testMat" << testMat << std::endl;
//        std::cout << result << std::endl;
#endif
        static cv::Ptr<SVM> svm;
        svm = svm->load(root+"/svm.xml");
        float result = 0;
        result = svm->predict(testMat);
        std::cout << result << std::endl;
        return (int)result;
    }

    std::vector<int> locate::getLocation(int label){
        int left = label/10;
        int right = label%10;
        if(left == 5){

        }
        else if(left == 6){
            if(right == 0){
//                std::vector<int> <<
                return {19200, 0, 0, 0};
            }
            else if(right == 1){
                return {13950, -14580, -900, 180};
            }
            else if(right == 2){
                return {16940, -11880, -500, 105};
            }
            else if(right == 3){
                return {23100, -12960, -700, 180};
            }
            else if(right == 4){
                return {23100, -3330, -700, 180};
            }
        }
        else if(left == 7){
            if(right == 0){
//                std::vector<int> <<
                return {8800, 0, 0, 0};
            }
            else if(right == 1){
                return {14050, -420, -900, 0};
            }
            else if(right == 2){
                return {11060, -3130, -500, 35};
            }
            else if(right == 3){
                return {4890, -2400, -700, 0};
            }
            else if(right == 4){
                return {4890, -11690, -700, 0};
            }
        }
        return {-1, -1, -1, -1};
    }

    std::string locate::getString(std::vector<int> location) {
        return "x:" + std::to_string(location[0]) +
                " y:" + std::to_string(location[1]) +
                " z:" + std::to_string(location[2]) +
                " angle:" + std::to_string(location[3]);
    }
}
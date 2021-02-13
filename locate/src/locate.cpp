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
//        std::cout << imagePath << labelPath << std::endl;

        trainMat = cv::Mat::zeros(sampleNum, height*width, CV_32FC1);
        labelMat = cv::Mat::zeros(sampleNum, 1, CV_32SC1);
//        std::cout << trainMat.cols << trainMat.rows << std::endl;

        int cnt = 0;
        fs::directory_iterator imageIter(imagePath_), end;
        while(imageIter != end){
            std::string image = imageIter->path().string();
            fs::path tempLabelPath_ = labelPath_ / fs::basename(image);
            std::string label = tempLabelPath_.string() + ".txt";
            std::cout << image << label << std::endl;

            cv::Mat img = cv::imread(image, 0);
//            std::cout << img.cols << img.rows << img << std::endl;

            std::ifstream file;
            file.open(label);
            int class_;
            file >> class_;
            file.close();
//            std::cout << class_ << std::endl;

            for(int i=0; i<height*width; i++){
                trainMat.at<float>(cnt, i) = (float)img.at<uchar>(i/height, i%height);
            }
//            std::cout << trainMat << std::endl;
            labelMat.at<int>(cnt, 0) = class_;
            imageIter++, cnt++;
            if(cnt == sampleNum) break;
        }
//        std::cout << sampleNum << std::endl;
        std::cout << trainMat.size << std::endl;

        std::pair<cv::Mat, cv::Mat> data = std::make_pair(trainMat, labelMat);
        return data;
    }
    void locate::detect() {

    }

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
        svm->save(root+"/svm.xml");
    }
    void locate::predict() {
        fs::path rootPath(root);
        fs::path test_path(test_img_path);
        fs::path img_path = rootPath / test_path;
//        std::cout << img_path.string() << std::endl;
        cv::Mat testMat = cv::Mat::zeros(1, height*width, CV_32FC1);
        cv::Mat testImg = cv::imread(img_path.string(), 0);
//        std::cout << testMat.size << std::endl;
//        cv::imshow("img", testMat);
//        cv::waitKey(0);
        for(int i=0; i<height*width; i++){
            testMat.at<float>(0, i) = (float)testImg.at<uchar>(i/height, i%height);
        }
        svm->load(root+"/svm.xml");
        std::cout << testMat << std::endl;
        float result = svm->predict(testMat);
        std::cout << result << std::endl;
    }
}
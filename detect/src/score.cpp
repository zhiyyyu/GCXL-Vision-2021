/***
 * @file
 * @author
 * @brief
 * @note
 */

#include "score.h"
using namespace cv::ml;

namespace QRCode{
    void score::loadData(){
        float trainSamples[sampleNum][featureDim] = {{ 40,55 },{ 35,35 },{ 55,15 },{ 54,25 },{ 10,10 },{ 15,15 } ,{ 40,10 },
                                                     { 30,15 },{ 30,50 },{ 100,20 },{ 45,65 },{ 20,35 },{ 80,20 } ,{ 90,5 },
                                                     { 95,35 },{ 80,65 },{ 15,55 },{ 25,65 },{ 85,35 },{ 85,55 } ,{ 95,70 },
                                                     { 105,50 },{ 115,65 },{ 110,25 },{ 120,25 },{ 120,45 },{ 15,45 } ,{ 55,30 },
                                                     { 60,65 },{ 95,60 },{ 25,40 },{ 75,45 },{ 105,35 },{ 65,10 } ,{ 50,50 },
                                                     { 40,35 },{ 70,55 },{ 80,30 },{ 95,45 },{ 60,25 },{ 70,30 } ,{ 65,45 }};
        int labels[sampleNum] = {'R','R','R','R','R','R','R','R','R','R',
                                 'R','R','R','R','R','R','R','R','R','R','R',
                                 'B','B','B','B','B','B','B','B','B','B',
                                 'B','B','B','B','B','B','B','B','B','B','B' };
        trainMat = cv::Mat(sampleNum, featureDim, CV_32FC1, trainSamples);
        labelMat = cv::Mat(sampleNum, 1, CV_32SC1, labels);
        trainSVM(trainMat, labelMat);
        trainBoost(trainMat, labelMat);
    }
    void score::trainSVM(cv::Mat trainMat, cv::Mat labelMat) {

        std::cout << "trainMat:" << trainMat << "\nlabelMat" << labelMat << std::endl;
        // create SVM
        svm = SVM::create();
        // set params
        svm->setC(0.1);
        svm->setType(SVM::C_SVC);
        svm->setKernel(SVM::LINEAR);
        svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
        // train
        cv::Ptr<TrainData> trainData = TrainData::create(trainMat, ROW_SAMPLE, labelMat);
        svm->train(trainData);
//        svm->save("/svm.xml");
    }
    void score::trainBoost(cv::Mat trainMat, cv::Mat labelMat){

        // create
        boost = Boost::create();
        // set
        boost->setBoostType(Boost::REAL);
        boost->setMaxDepth(15);
        boost->setWeightTrimRate(0.95);
        boost->setWeakCount(15);
        // train
        cv::Ptr<TrainData> trainData = TrainData::create(trainMat, ROW_SAMPLE, labelMat);
        boost->train(trainData);
//        boost->save("~/boost.xml");
    }

//    float score::predict(cv::Mat testMat){
//        return svm->predict(testMat);
//    }
}
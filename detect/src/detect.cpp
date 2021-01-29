/**
 * @file
 * @author
 * @brief
 * @note
 */
#include "detect.h"

namespace QRCode{
    detect::detect(){
        score* score_ = new score();
        score_->loadData();
//        score_->trainSVM(score_->trainMat, score_->labelMat);;
        score_->display<cv::Ptr<cv::ml::SVM>>(score_->svm);
        score_->display<cv::Ptr<cv::ml::Boost>>(score_->boost);
    }
    detect::~detect(){

    }
}
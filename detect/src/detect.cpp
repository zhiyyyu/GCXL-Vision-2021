/**
 * @file
 * @author
 * @brief
 * @note
 */
#include "detect.h"

namespace QRCode{
    detect::detect(){
//        score* score_ = new score();
//        score_->loadData();
////        score_->trainSVM(score_->trainMat, score_->labelMat);;
//        score_->display<cv::Ptr<cv::ml::SVM>>(score_->svm);
//        score_->display<cv::Ptr<cv::ml::Boost>>(score_->boost);
    }
    std::vector<cv::RotatedRect> detect::getDirectionFlags(cv::Mat image){
        cv::Mat grayImg, threshImg, harris, showImg = image.clone();
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::RotatedRect> minRect;
        cv::cvtColor(image, grayImg, cv::COLOR_BGR2GRAY);
        cv::threshold(grayImg, threshImg, thresh, maxval, cv::THRESH_BINARY);
        cv::findContours(threshImg, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
        for(int i=0; i<contours.size(); i++){
            //@TODO 更换检测策略
            if(contours[i].size() == 11){
                minRect.emplace_back(cv::minAreaRect(contours[i]));
                cv::drawContours(showImg, contours, i, cv::Scalar(0, 255, 0), 3);
#if DEBUG
                std::cout << "find one:" << contours[i] << std::endl;
#endif
            }
        }
#if DEBUG
        for(int i=0; i<contours.size(); i++) {
            std::cout << contours[i].size() << std::endl;
            for (int j = 0; j < contours[i].size(); j++) {
                std::cout << contours[i].at(j) << std::endl;
            }
        }
        cv::drawContours(showImg, contours, -1, cv::Scalar(0, 255, 0), 3);
        std::cout << contours.size() << std::endl;
        cv::imshow("gray", grayImg);
        cv::imshow("thresh", threshImg);
        cv::imshow("contours", showImg);
        cv::waitKey(0);
#endif
        return minRect;
    }

    cv::Mat detect::correctVisualLabel(cv::Mat img, std::vector<cv::RotatedRect> rect) {
        if(rect.size() != 3){
            // @TODO 增加筛选功能
        }
        else{
            cv::Point2f cornerPoints[3][4], centerPoint[3];
            std::vector<cv::Moments> mu(3); //@TODO 使用矩方法
            for(int i=0; i<3; i++){
                rect[i].points(cornerPoints[i]);
                centerPoint[i] = rect[i].center;
#if LABEL_DEBUG
                std::cout << cornerPoints[i][0] << cornerPoints[i][1] << cornerPoints[i][2] << cornerPoints[i][3] << std::endl;
                std::cout << centerPoint[i] << std::endl;
#endif
            }
            float innerProduct, outerProduct;
            cv::Point2f tl, tr, bl;
            innerProduct = calInnerProduct(centerPoint[0], centerPoint[1], centerPoint[2]);
            if(innerProduct == 0){
                outerProduct = calOuterProduct(centerPoint[0], centerPoint[1], centerPoint[2]);
                tl = centerPoint[0];
                tr = outerProduct>0?centerPoint[1]:centerPoint[2];
                bl = outerProduct>0?centerPoint[2]:centerPoint[1];
                return getROI(img, tl, tr, bl);
            }
            innerProduct = calInnerProduct(centerPoint[1], centerPoint[0], centerPoint[2]);
            if(innerProduct == 0){
                outerProduct = calOuterProduct(centerPoint[1], centerPoint[0], centerPoint[2]);
                tl = centerPoint[1];
                tr = outerProduct>0?centerPoint[0]:centerPoint[2];
                bl = outerProduct>0?centerPoint[2]:centerPoint[0];
                return getROI(img, tl, tr, bl);
            }
            innerProduct = calInnerProduct(centerPoint[2], centerPoint[0], centerPoint[1]);
            if(innerProduct == 0){
                outerProduct = calOuterProduct(centerPoint[2], centerPoint[0], centerPoint[1]);
                tl = centerPoint[2];
                tr = outerProduct>0?centerPoint[0]:centerPoint[1];
                bl = outerProduct>0?centerPoint[1]:centerPoint[0];
                return getROI(img, tl, tr, bl);
            }
        }
    }

    float detect::calInnerProduct(cv::Point2f p1, cv::Point2f p2, cv::Point2f p3) {
        return (p2.x-p1.x)*(p3.x-p1.x) +
                (p2.y-p1.y)*(p3.y-p1.y);
    }
    float detect::calOuterProduct(cv::Point2f p1, cv::Point2f p2, cv::Point2f p3) {
        return (p2.x-p1.x)*(p3.y-p1.y) -
                (p3.x-p1.x)*(p2.y-p1.y);
    }

    cv::Mat detect::getROI(cv::Mat img, cv::Point2f tl, cv::Point2f tr, cv::Point2f bl) {
//        int width = (int)(tr.x-tl.x);
//        int height = (int)(bl.y-tl.y);
//        cv::Mat ROI = img(cv::Rect(tl.x, tl.y, width, height));
        cv::Mat warpMat = cv::Mat(2, 3, CV_32FC1);
        cv::Mat warpImg = img.clone();
        cv::Point2f srcPoints[3] = {tl, tr, bl};
        cv::Point2f dstPoints[3] = {tl, {tl.x+128, tl.y}, {tl.x, tl.y+128}};
        warpMat = cv::getAffineTransform(srcPoints, dstPoints);
        cv::warpAffine(img, warpImg, warpMat, warpImg.size());
        cv::imshow("warpImg", warpImg);
        cv::waitKey(0);
        return warpImg;
    }
}
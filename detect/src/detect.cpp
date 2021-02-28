/**
 * @file
 * @author
 * @brief
 * @note
 */
#include "detect.h"

namespace QRCode {
    std::string detect::QRCodeDetector(cv::Mat img) {
        cv::QRCodeDetector qrDetector;
        std::vector<cv::Point> list;
        cv::Mat qrcode;
        std::string data = qrDetector.detectAndDecode(img, list, qrcode);
//        display(img, qrcode, list);
//        std::cout << "data: " << data << std::endl;
        return data;
    }

    std::string detect::BarCodeDetector(cv::Mat img) {
#if ZBAR_DETECTOR
        zbar::ImageScanner scanner;
        scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
        cv::Mat grayImg;
        cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
        unsigned int width = (unsigned int)grayImg.cols;
        unsigned int height = (unsigned int)grayImg.rows;
        uchar* raw = grayImg.data;
        zbar::Image imageZbar(width, height, "Y800", raw, width * height);
        scanner.scan(imageZbar); //扫描条码
        zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();
        if(imageZbar.symbol_begin()==imageZbar.symbol_end())
        {
            std::cout<<"查询条码失败，请检查图片！"<<std::endl;
        }
        for(;symbol != imageZbar.symbol_end();++symbol)
        {
            std::cout<<"类型："<<symbol->get_type_name()<<std::endl;
            std::cout<<"条码："<<symbol->get_data()<<std::endl;
        }
//        cv::imshow("Source Image", img);
//        cv::waitKey(0);
        imageZbar.set_data(NULL,0);
        return symbol->get_data();
#endif
#if MY_DETECTOR
        cv::Mat grayImg;
        cv::Mat gBlurImg;
        cv::Mat threshImg;
        cv::Mat mBlurImg;
        cv::Mat resImg;
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::RotatedRect> minRect;
        cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(grayImg, gBlurImg, cv::Size(5, 5), 3, 3);
        cv::threshold(gBlurImg, threshImg, thresh, maxval, cv::THRESH_OTSU);
        cv::blur(threshImg, mBlurImg, cv::Size(3, 3));

        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        cv::morphologyEx(mBlurImg, resImg, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 2);
        cv::morphologyEx(resImg, resImg, cv::MORPH_CLOSE, kernel, cv::Point(-1, -1), 2);
        cv::findContours(threshImg.clone(), contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
        for(int i=0; i<contours.size(); i++){
            cv::RotatedRect rect = cv::minAreaRect(contours[i]);
            if(judgeBarCode(rect)){
                cv::drawContours(img, contours, i, cv::Scalar(0, 255, 0), 3);
                minRect.emplace_back(rect);
            }
        }
#if SHOW_IMAGE
        cv::imshow("img", img);
        cv::imshow("gray", grayImg);
        cv::imshow("gaussian blur", gBlurImg);
        cv::imshow("thresh", threshImg);
        cv::imshow("mean blur", mBlurImg);
        cv::imshow("result", resImg);
//        cv::drawContours(img, contours, -1, cv::Scalar(0, 255, 0), 3);
        cv::imshow("contours", img);
        cv::waitKey(0);
#endif
        return getROI(img, minRect);
#endif
    }

    std::vector<cv::Point2f> detect::MaterialDetector(cv::Mat img, int color, int level) {
        cv::Mat hsvImg;
        cv::Mat blurImg;
        cv::Mat threshImg;
        cv::Mat resImg;
        std::vector<std::vector<cv::Point>> contours;
        cv::Scalar lower, upper;

        if(color == RED){
            lower = cv::Scalar(0, 60, 60);
            upper = cv::Scalar(6, 255, 255);
        }
        else if(color == GREEN){
            lower = cv::Scalar(35, 43, 35);
            upper = cv::Scalar(90, 255, 255);
        }
        else if(color == BLUE) {
            lower = cv::Scalar(100, 80, 46);
            upper = cv::Scalar(124, 255, 255);
        }

        cv::cvtColor(img, hsvImg, cv::COLOR_BGR2HSV);
        cv::inRange(hsvImg, lower, upper, hsvImg);
        cv::GaussianBlur(hsvImg, blurImg, cv::Size(5, 5), 3, 3);
        cv::threshold(blurImg, threshImg, thresh, maxval, cv::THRESH_OTSU);
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        cv::morphologyEx(threshImg, resImg, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 4);
        cv::morphologyEx(resImg, resImg, cv::MORPH_CLOSE, kernel, cv::Point(-1, -1), 4);
        cv::findContours(resImg.clone(), contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

//        std::cout << contours.size();
        std::vector<cv::Point2f> retPoints;
        if(contours.size() == 2){
            cv::RotatedRect rect1, rect2;
            rect1 = cv::minAreaRect(contours[0]);
            rect2 = cv::minAreaRect(contours[1]);
            cv::Point2f points1[4], points2[4];
            rect1.points(points1);
            rect2.points(points2);
            if(level == TOP) {
               if(points1[0].y < points2[0].y){
                   retPoints.push_back(points1[0]);
                   retPoints.push_back(points1[1]);
                   retPoints.push_back(points1[2]);
                   retPoints.push_back(points1[3]);
               }
                else{
                   retPoints.push_back(points2[0]);
                   retPoints.push_back(points2[1]);
                   retPoints.push_back(points2[2]);
                   retPoints.push_back(points2[3]);
               }
            }
            else{
                if(points1[0].y > points2[0].y){
                    retPoints.push_back(points1[0]);
                    retPoints.push_back(points1[1]);
                    retPoints.push_back(points1[2]);
                    retPoints.push_back(points1[3]);
                }
                else{
                    retPoints.push_back(points2[0]);
                    retPoints.push_back(points2[1]);
                    retPoints.push_back(points2[2]);
                    retPoints.push_back(points2[3]);
                }
            }

        }

        cv::imshow("img", img);
        cv::imshow("hsv", hsvImg);
        cv::imshow("blur", blurImg);
        cv::imshow("thresh", threshImg);
        cv::imshow("res", resImg);
        cv::waitKey(0);
        return retPoints;
    }

    void detect::display(cv::Mat img, cv::Mat ROI, std::vector<cv::Point> list) {
        cv::Mat showImg = img.clone();
        for(int i=0;i<4;i++){
            cv::line(showImg, list[i], list[(i+1)%4], cv::Scalar(0, 0, 255), 4);
        }
        cv::imshow("roi", ROI);
        cv::imshow("detected_qrcode", img);
        cv::waitKey(0);
    }

    bool detect::judgeBarCode(cv::RotatedRect rect) {
        barCodeNode barcode;
        barcode.width = rect.size.width;
        barcode.height = rect.size.height;
        barcode.widthHeightRatio = barcode.width / barcode.height;
        barcode.areaRatio = barcode.width*barcode.height / (width*height);
//        std::cout << "width: " << barcode.width << " height: " << barcode.height << std::endl;
        barcode.score = (barcode.widthHeightRatio <= 0.5 && barcode.widthHeightRatio >= 0.25) +
             (barcode.areaRatio >= 0.005 && barcode.areaRatio <= 0.5);
        return barcode.score >= 2;
    }

    bool detect::judgeMaterial(cv::RotatedRect rect) {

        return true;
    }

    cv::Mat detect::getROI(cv::Mat img, std::vector<cv::RotatedRect> roi) {
        if(roi.size() == 1){
            cv::RotatedRect roi_ = roi[0];
            int w = (int)roi_.size.width;
            int h = (int)roi_.size.height;
            float angle = -roi_.angle;
            double cosAngle = cos(angle/180*PI);
            double sinAngle = sin(angle/180*PI);
        }
    }

}
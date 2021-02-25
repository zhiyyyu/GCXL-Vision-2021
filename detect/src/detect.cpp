/**
 * @file
 * @author
 * @brief
 * @note
 */
#include "detect.h"

namespace QRCode {
    cv::Mat detect::QRCodeDetector(cv::Mat img) {
        cv::QRCodeDetector qrDetector;
        std::vector<cv::Point> list;
        cv::Mat qrcode;
        std::string data = qrDetector.detectAndDecode(img, list, qrcode);
        display(img, qrcode, list);
        std::cout << "data: " << data << std::endl;
        return qrcode;
    }
    cv::Mat detect::BarCodeDetector(cv::Mat img) {
#if ZBAR_DETECTOR
        zbar::ImageScanner scanner;
        scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
        cv::Mat grayImg;
        cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
        int width = grayImg.cols;
        int height = grayImg.rows;
        uchar *raw = grayImg.data;
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
        cv::imshow("Source Image", img);
        cv::waitKey(0);
        imageZbar.set_data(NULL,0);
        return img;
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

    cv::Mat detect::MaterialDetector(cv::Mat img) {

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
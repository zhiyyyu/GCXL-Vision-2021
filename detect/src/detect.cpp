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

}
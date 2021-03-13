#include "Robot.h"

using namespace QRCode;

int main() {

//    serialPortWrite* serialPortWrite_ = new serialPortWrite();
//    while(1){
//        sendData send;
//        std::string data = "231+123";
//        send.num1 = 231;
//        send.num2 = 123;
//        send.num3 = '+';
//        serialPortWrite_->setSendMsg(send);
//        serialPortWrite_->writeData();
//        std::cout << "data: " << data << std::endl;
//    }
    QRCode::Robot* robot = new QRCode::Robot();
    cv::VideoCapture cap(0);
    if(!cap.isOpened()){
        std::cout << "open failed! " << std::endl;
        return -1;
    }
    while(true){
        cv::Mat frame;
        cap >> frame;
        robot->process(frame);
#if SHOW_RAW_IMG
        cv::imshow("frame", frame);
#endif
        if(cv::waitKey(1) == 'q'){
            break;
        }
    }
    cap.release();
    return 0;
}
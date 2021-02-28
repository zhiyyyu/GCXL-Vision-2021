#ifndef __CAMERA__H
#define __CAMERA__H

#include <opencv2/opencv.hpp>


enum type
{
    HIK_CAM = 1,
    DAH_CAM,
    PIC,
    VIDEO
};

namespace QRCode {
    // 相机，纯虚类
    class camera
    {
    public:
        camera() = default;
        ~camera() = default;
        virtual cv::Mat getFrame() = 0;
        virtual cv::Mat getFrame2(){}
        virtual int getType() = 0;

        virtual inline double getUpdateTime(){ return update_time_;}
        virtual inline int getFrameId(){ return frame_id_;}
        inline void updateId(){frame_id_ ++;}
        inline void setUpdateTime(double time){update_time_ = time;}
        cv::Mat frame_;
        cv::Mat frame2_;
    private:
        static long long int frame_id_;
        double update_time_;
    };
    // 由虚类派生成四个相机类代表海康相机、大恒相机、图片数据和视频数据

    // class DAH_Camera:public camera
    // {
    // public:
    //     DAH_Camera()=default ;
    //     cv::Mat getFrame() override;
    //     int getType() override{ return DAH_CAM;}
    // private:
    // };
    class video:public camera
    {
    public:
        video() = default;
        explicit video(const std::string& video_path);
        cv::Mat getFrame() override;
        int getType() override{ return VIDEO;}
    private:
        cv::VideoCapture cap_;
    };
    class picture:public camera
    {
    public:
        picture() = default;
        explicit picture(std::string pic_path);
        cv::Mat getFrame() override;
        int getType() override{ return PIC;}
    private:
        std::string pic_path_;
    };
}




#endif //__CAMERA__H


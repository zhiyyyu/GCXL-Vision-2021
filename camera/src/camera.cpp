#include "camera.h"

namespace QRCode
{
    long long int camera::frame_id_ = 0;

    // cv::Mat DAH_Camera::getFrame()
    // {
    //     return cv::Mat();
    // }
    /**
     * @brief 依次获取图片
     * @return 图片
     */
    cv::Mat picture::getFrame()
    {
        cv::Mat pic;
//        setUpdateTime(frame_counter_.getTimeMs());
        std::stringstream ss;
        ss<<pic_path_<<"/"<<getFrameId()<<".jpg";
        pic = cv::imread(ss.str());
        updateId();
//        frame_counter_.countBegin();
        return pic;
    }
    picture::picture(std::string pic_path)
    {
        pic_path_ = std::move(pic_path);
    }

    /**
     * @brief 获取视频中的一帧
     * @return 图片
     */
    cv::Mat video::getFrame()
    {
//        setUpdateTime(frame_counter_.getTimeMs());
        cap_>>frame_;
        updateId();
//        frame_counter_.countBegin();
        return frame_;
    }
    /**
     * @brief 构造函数
     * @param video_path 视频路径
     */
    video::video(const std::string& video_path)
    {
        cap_.open(video_path);
        if(!cap_.isOpened())//如果视频不能正常打开则返回
        {
            std::cerr<<"error !!! can not open the video!"<<std::endl;
            return ;
        }
        else
        {
            std::cout<<"open video success !"<<std::endl<<"path:"<<video_path<<std::endl;
        }
    }
}






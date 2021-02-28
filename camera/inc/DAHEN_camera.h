#ifndef __DAHENG_CAMERA_H
#define __DAHENG_CAMERA_H

#include "camera.h"
#include "../lib/dahen_include/DxImageProc.h"
#include "../lib/dahen_include/GxIAPI.h"
#include <unistd.h>

namespace QRCode{
    class DAH_Camera:public camera
{
public:
    DAH_Camera();
    ~DAH_Camera();
    int Init();
    int SetPixelFormat(int64_t pixelformat = GX_PIXEL_FORMAT_BAYER_BG10);
    int SetResolution(unsigned int width, unsigned int height);
    int SetTriggerMode(unsigned int mode);
    int SetFPS(double fps);
    int SetExposureTime(float time);
    int SetGain(float gain);
    int SetBalanceWhite();
    int SetDeviceName(char *name);
    int StartStream();
    int operator >>(cv::Mat &val_out);
    cv::Mat getFrame() ;
    int getType() { return DAH_CAM;}
private:
    char m_DeviceID[128];

    PGX_FRAME_BUFFER m_pFrameBuffer=NULL;
    GX_DEV_HANDLE m_CamHandle = NULL;


    int m_ImageWidth;
    int m_ImageHeight;
    double m_fps;                       //OK
    int m_TriggerMode;          //OK
    int64_t m_PixelFormat;//OK  相机仅支持像素格式为BayerRG8和BayerRG10
    float m_ExpTime;   //OK
    float m_Gain;               //OK
    double m_Gamma;         //OK   相机不支持gamma的设置

    int m_GrabCnt;
};

#endif // __DAHENG_CAMERA_H
}

#include "DAHEN_camera.h"

namespace QRCode{
    
DAH_Camera::DAH_Camera()
{


    m_ImageWidth=1280;
    m_ImageHeight=1024;
    m_fps=220;                                  //OK
    m_TriggerMode=0;                            //OK
    m_PixelFormat=GX_PIXEL_FORMAT_BAYER_RG10;   //OK
    m_ExpTime=1000.0;                           //OK
    m_Gain=0;                                   //OK
    m_Gamma=0;                                  //OK
    // memset(m_DeviceID, 0, 128);
    // memcpy(m_DeviceID, device.c_str(), device.size());

    Init();

}

DAH_Camera::~DAH_Camera()
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    //发 送 停 采 命 令
    status = GXStreamOff(m_CamHandle);
    if(status!=GX_STATUS_SUCCESS){
        std::cout<<"GXStreamOff Failed!\n";
    }
    status = GXCloseDevice(m_CamHandle);
    if(status!=GX_STATUS_SUCCESS){
        std::cout<<"GXCloseDevice Failed!\n";
    }
    status = GXCloseLib();
    if(status!=GX_STATUS_SUCCESS){
        std::cout<<"GXCloseLib Failed!\n";
    }
    // delete m_pFrameBuffer;
}

int DAH_Camera::Init()
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    //获取库信息
    const char *pLibVersion = GXGetLibVersion();
    std::cout<<pLibVersion<<"\n";
    // delete []pLibVersion;
    //初 始 化 库
    status = GXInitLib();
    if (status != GX_STATUS_SUCCESS)
    {
        std::cout<<"InitLib Failed!\n";
        return 0;
    }
    // 枚举设备
    // enum device
    uint32_t nDeviceNum = 0;
    status = GXUpdateDeviceList(&nDeviceNum, 1000);
    if (status == GX_STATUS_SUCCESS && nDeviceNum> 0)
    {
        GX_DEVICE_BASE_INFO *pBaseinfo = new GX_DEVICE_BASE_INFO[nDeviceNum];
        long unsigned int nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);
        //获 取 所 有 设 备 的 基 础 信 息
        status = GXGetAllDeviceBaseInfo(pBaseinfo, &nSize);
        std::cout<<pBaseinfo->szVendorName<<"\n";
        std::cout<<pBaseinfo->szModelName<<"\n";
        std::cout <<pBaseinfo->szSN<<"\n";
        std::cout<<pBaseinfo->szDisplayName<<"\n";
        // delete []pBaseinfo;
    }
   //打 开 设 备
    status = GXOpenDeviceByIndex(1, &m_CamHandle);
    if(status!=GX_STATUS_SUCCESS) std::cout<<"OpenDevice Failed!\n";

    return (SetBalanceWhite()|SetExposureTime(m_ExpTime)|SetFPS(m_fps)|SetGain(m_Gain)|SetPixelFormat(m_PixelFormat)|SetResolution(m_ImageWidth, m_ImageHeight)|SetTriggerMode(m_TriggerMode));

    }

int DAH_Camera::SetBalanceWhite()
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    //选 择 白 平 衡 通 道(选择红色)
    status = GXSetEnum(m_CamHandle,GX_ENUM_BALANCE_RATIO_SELECTOR,
    GX_BALANCE_RATIO_SELECTOR_RED);
    //获 取 白 平 衡 调 节 范 围
    GX_FLOAT_RANGE ratioRange;
    status = GXGetFloatRange(m_CamHandle, GX_FLOAT_BALANCE_RATIO, &ratioRange);
    //设 置 最 小 白 平 衡 系 数
    status = GXSetFloat(m_CamHandle, GX_FLOAT_BALANCE_RATIO, ratioRange.dMin);
    //设 置 最 大 白 平 衡 系 数
    status = GXSetFloat(m_CamHandle, GX_FLOAT_BALANCE_RATIO, ratioRange.dMax);
    //设 置 自 动 白 平 衡 感 兴 趣 区 域(代 码 中 参 数 为 举 例 , 用 户 根 据 自 己 需 要 自 行 修 改 参 数 值 )
    status=GXSetInt(m_CamHandle,GX_INT_AWBROI_WIDTH,m_ImageWidth);
    status=GXSetInt(m_CamHandle,GX_INT_AWBROI_HEIGHT,m_ImageHeight);
    status=GXSetInt(m_CamHandle,GX_INT_AWBROI_OFFSETX,0);
    status=GXSetInt(m_CamHandle,GX_INT_AWBROI_OFFSETY,0);
    //自 动 白 平 衡 设 置--------------------------
    //设 置 自 动 白 平 衡 光 照 环 境 , 比 如 当 前 相 机 所 处 环 境 为 荧 光 灯
    status = GXSetEnum(m_CamHandle,GX_ENUM_AWB_LAMP_HOUSE,GX_AWB_LAMP_HOUSE_FLUORESCENCE);
    //设 置 连 续 自 动 白 平 衡
    status = GXSetEnum(m_CamHandle,GX_ENUM_BALANCE_WHITE_AUTO,GX_BALANCE_WHITE_AUTO_CONTINUOUS);
    if(status!=GX_STATUS_SUCCESS){
        std::cout<<"SetBalanceWhite Failed！\n";
        return -1;
    }
    return 1;
}
int DAH_Camera::SetPixelFormat(int64_t pixelformat)
{
   GX_STATUS status = GX_STATUS_SUCCESS;
    // //读 取 当 前 pixelformat
    // int64_t nPixelFormat = 0;
    // status = GXGetEnum(m_CamHandle, GX_ENUM_PIXEL_FORMAT, &nPixelFormat);
    // //设 置 pixelformat 为 bayer 格 式 为 BG 的 位 深 数 据
    // nPixelFormat = GX_PIXEL_FORMAT_BAYER_BG10;
    status = GXSetEnum(m_CamHandle, GX_ENUM_PIXEL_FORMAT, pixelformat);
    if (status != GX_STATUS_SUCCESS)
    {
        printf("SetPixelFormat fail! \n");
        return -1;
    }
    return 1;
}

int DAH_Camera::SetResolution(unsigned int width, unsigned int height)
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    status = GXSetInt(m_CamHandle, GX_INT_WIDTH, width);
    status = GXSetInt(m_CamHandle, GX_INT_HEIGHT, height);
    if (status != GX_STATUS_SUCCESS)
    {
        printf("SetWidth|SetHeight fail!\n");
        return -1;
    }
    return 1;
}

int DAH_Camera::SetTriggerMode(unsigned int mode)
{
     GX_STATUS status = GX_STATUS_SUCCESS;
    // 设置触发模式
    // set trigger mode
    if(mode!=0){
        //设 置 触 发 模 式 为 ON
        status =GXSetEnum(m_CamHandle,GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
        //设 置 触 发 激 活 方 式 为 上 升 沿
        status = GXSetEnum(m_CamHandle,GX_ENUM_TRIGGER_ACTIVATION, GX_TRIGGER_ACTIVATION_RISINGEDGE);
    }
    if (status != GX_STATUS_SUCCESS)
    {
        printf("SetTriggerMode fail! \n");
        return -1;
    }
    return 1;
}

int DAH_Camera::SetFPS(double fps)
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    //使 能 采 集 帧 率 调 节 模 式
    status = GXSetEnum(m_CamHandle, GX_ENUM_ACQUISITION_FRAME_RATE_MODE,GX_ACQUISITION_FRAME_RATE_MODE_ON);
    //设 置 采 集 帧 率,假 设 设 置 为 10.0, 用 户 按 照 实 际 需 求 设 置 此 值
    status = GXSetFloat(m_CamHandle, GX_FLOAT_ACQUISITION_FRAME_RATE,fps);
    if (status != GX_STATUS_SUCCESS)
    {
        printf("SetFrameRate fail! \n");
        return 0;
    }

    
    printf("FPS: %f\n",fps);


    return 1;
}

int DAH_Camera::SetExposureTime(float time)
{

    GX_STATUS status = GX_STATUS_SUCCESS;



    //设置曝光模式为TIMED模式
    status = GXSetEnum(m_CamHandle, GX_ENUM_EXPOSURE_MODE, GX_EXPOSURE_MODE_TIMED);
    //设 置 曝 光 延 迟  time (us)
    status = GXSetFloat(m_CamHandle, GX_FLOAT_EXPOSURE_TIME, time);
    if(status!=GX_STATUS_SUCCESS){
            std::cout<<"SetExposureTime Failed!\n";
            return -1;
    }

    return 1;
}

int DAH_Camera::SetGain(float gain)
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    //选 择 增 益 通 道 类 型
    status =GXSetEnum(m_CamHandle, GX_ENUM_GAIN_SELECTOR, GX_GAIN_SELECTOR_ALL);
    //设 置  增 益 值
    status = GXSetFloat(m_CamHandle, GX_FLOAT_GAIN, m_Gain);
    if(status!=GX_STATUS_SUCCESS){
            std::cout<<"SetGain Failed!\n";
            return -1;
    }


    return 1;
}


int DAH_Camera::SetDeviceName(char *name)
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    size_t nSize = 0;
    //首 先 获 取 字 符 串 允 许 的 最 大 长 度(此 长 度 包 含 结 束 符 ’\0’)
    status = GXGetStringMaxLength(m_CamHandle, GX_STRING_DEVICE_USERID, &nSize);
    //根 据 获 取 的 长 度 申 请 内 存
    char *pszText = new char[nSize];
    status = GXGetString(m_CamHandle, GX_STRING_DEVICE_USERID, pszText, &nSize);
    //设 置 用 户 自 定 义 名 称
    status = GXSetString(m_CamHandle, GX_STRING_DEVICE_USERID, name);
    // delete []pszText;

    if (status != GX_STATUS_SUCCESS)
    {
        printf("MV_CC_SetStringValue fail!\n");
        return -1;
    }
    return 1;
}

int DAH_Camera::StartStream()
{
    GX_STATUS status = GX_STATUS_SUCCESS;

    //开 采
    status = GXStreamOn(m_CamHandle);
    if (status != GX_STATUS_SUCCESS)
    {
        printf("StartGrabbing fail! \n");
        return -1;
    }
    return 1;
}


cv::Mat DAH_Camera::getFrame(){
    cv::Mat val_out;
    GX_STATUS status = GX_STATUS_SUCCESS;
    status = StartStream();
    if(status!=GX_STATUS_SUCCESS) std::cout<<"GXStreamOn Failed\n";
    status = GXDQBuf(m_CamHandle, &m_pFrameBuffer, 1000);
    if(status!=GX_STATUS_SUCCESS) {
        std::cout<<"GXDQBuf Failed\n";
        return val_out;
    }
    
    if (m_pFrameBuffer->nStatus==GX_FRAME_STATUS_SUCCESS)
    {

        //输出图像格式转换
        unsigned char* g_pRGBImageBuf = NULL;               ///< Memory for RAW8toRGB24
        int64_t g_nPayloadSize = 0;                         ///< Payload size
        int64_t g_i64ColorFilter = GX_COLOR_FILTER_NONE;    ///< Color filter of device
        status = GXGetInt(m_CamHandle, GX_INT_PAYLOAD_SIZE, &g_nPayloadSize);
        status = GXGetEnum(m_CamHandle, GX_ENUM_PIXEL_COLOR_FILTER, &g_i64ColorFilter);
        unsigned char* g_pRaw8Image = new unsigned char[g_nPayloadSize];
        g_pRGBImageBuf = new unsigned char[g_nPayloadSize * 3]; 
        // status = DxRaw8toRGB24((unsigned char*)pFrameBuffer->pImgBuf, g_pRGBImageBuf, pFrameBuffer->nWidth, pFrameBuffer->nHeight,
        // RAW2RGB_NEIGHBOUR, BAYERRG, true);
        switch (m_pFrameBuffer->nPixelFormat)
        {
                case GX_PIXEL_FORMAT_BAYER_GR8:
                case GX_PIXEL_FORMAT_BAYER_RG8:
                case GX_PIXEL_FORMAT_BAYER_GB8:
                case GX_PIXEL_FORMAT_BAYER_BG8:
                {
                // Convert to the RGB image
                status = DxRaw8toRGB24((unsigned char*)m_pFrameBuffer->pImgBuf, g_pRGBImageBuf, m_pFrameBuffer->nWidth, m_pFrameBuffer->nHeight,
                                RAW2RGB_NEIGHBOUR, DX_PIXEL_COLOR_FILTER(g_i64ColorFilter), false);
                if (status != DX_OK)
                {
                        printf("DxRaw8toRGB24 Failed, Error Code: %d\n", status);
                        
                }
                break;
                }
                case GX_PIXEL_FORMAT_BAYER_GR10:
                case GX_PIXEL_FORMAT_BAYER_RG10:
                case GX_PIXEL_FORMAT_BAYER_GB10:
                case GX_PIXEL_FORMAT_BAYER_BG10:
                case GX_PIXEL_FORMAT_BAYER_GR12:
                case GX_PIXEL_FORMAT_BAYER_RG12:
                case GX_PIXEL_FORMAT_BAYER_GB12:
                case GX_PIXEL_FORMAT_BAYER_BG12:
                {
                // Convert to the Raw8 image
                status = DxRaw16toRaw8((unsigned char*)m_pFrameBuffer->pImgBuf, g_pRaw8Image, m_pFrameBuffer->nWidth, m_pFrameBuffer->nHeight, DX_BIT_2_9);
                if (status != DX_OK)
                {
                        printf("DxRaw16toRaw8 Failed, Error Code: %d\n", status);
                        
                }
                // Convert to the RGB24 image
                status = DxRaw8toRGB24((unsigned char*)g_pRaw8Image, g_pRGBImageBuf, m_pFrameBuffer->nWidth, m_pFrameBuffer->nHeight,
                                RAW2RGB_NEIGHBOUR, DX_PIXEL_COLOR_FILTER(g_i64ColorFilter), false);
                if (status != DX_OK)
                {
                        printf("DxRaw8toRGB24 Failed, Error Code: %d\n", status);                       
                }
                break;
                }
                default:
                {
                printf("Error : PixelFormat of this camera is not supported\n");
                }
        }
        cv::Mat tempimg(m_pFrameBuffer->nHeight,m_pFrameBuffer->nWidth,CV_8UC3);
        memcpy(tempimg.data,g_pRGBImageBuf,m_pFrameBuffer->nWidth*m_pFrameBuffer->nHeight*3);
	    cv::cvtColor(tempimg,tempimg,cv::COLOR_RGB2BGR);//这里不转为BGR会把红色识别为蓝色
        delete g_pRGBImageBuf;
        delete g_pRaw8Image;
        val_out=tempimg.clone();
        tempimg.release();
        m_GrabCnt=0;
        status = GXQBuf(m_CamHandle, m_pFrameBuffer);
        return val_out;
    }
    else
    {
        m_GrabCnt++;
    }
    if (m_GrabCnt>3)
    {
            //发 送 停 采 命 令
        GXStreamOff(m_CamHandle);
        GXCloseDevice(m_CamHandle);
        GXCloseLib();
        sleep(1);
        Init();
        StartStream();
        printf("Failed to get buffer.\n");
        return val_out;
    }
 }
}

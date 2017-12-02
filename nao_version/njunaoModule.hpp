//
//  njunaoModule.hpp
//  RedBallDetect
//
//  Created by Fanming Luo on 2017/11/19.
//  Copyright © 2017年 Fanming Luo. All rights reserved.
//
#ifndef _NJUNAOMODULE_H_
#define _NJUNAOMODULE_H_
#include <iostream>
#include "opencv2/core/core.hpp"
#include <boost/shared_ptr.hpp>
#include <alproxies/almemoryproxy.h>
#include <alcommon/almodule.h>
#include <alproxies/alvideodeviceproxy.h>
#include <alproxies/altexttospeechproxy.h>
#include <vector>
namespace AL
{
    class ALBroker;
};

class njunaoModule:public AL::ALModule
{
public:
    
    /**
     *  构造函数，在里面通过代理注册模块，加上一些能够被调用的函数的模块
     */
    njunaoModule(boost::shared_ptr<AL::ALBroker> pBroker,const std::string& pName);
    
    /**
     *析构函数，最关键是停止订阅相机
     */
    virtual ~njunaoModule();
    
    /**
     *订阅相机，订阅相机期间若有其他程序订阅相机，会死机
     *@param pResolution 相机分辨率 AL::k4VGA(1280*960) AL::kVGA(640*480) AL::kQVGA(320*240)
     *@param pColorSpace 颜色空间 AL::kRGBColorSpace AL::kBGRColorSpace
     */
    void registerToVideoDevice(const int &pResolution, const int &pColorSpace);
    
    /**
     *停止订阅相机
     */
    void unRegisterFromVideoDevice();
    
    /**
     *保存当前帧至机器人本地
     *@param pName 绝对路径，如"/home/nao/naoqi/img.jpg"
     */
    void saveImageLocal(const std::string& pName);
    
    /**
     *返回一个容器(x,y)，x为小球x坐标与图像width的比值，y为小球y坐标与图像height的比值
     */
    std::vector<float> RedBallFind();
    
    /**
     *持续寻找球，并将球的位置存在"njunaoBallPosition"中
     */
    void ContinuousFindBall();
    
    /**
     *返回一个容器(x)，x为杆横坐标与图像width的比值
     */
    std::vector<float> PoleFind();
    
    /**
     *持续寻找杆，并将球的位置存在"njunaoPolePosition"中
     */
    void ContinuousFindPole();
    
    /**
     *init函数，在构造函数之后立刻被自动调用
     */
    void init();
    
    /**
     *关闭
     */
    void exit();
    
    /**
     *语音播报当前代码版本
     */
    void sayVersion();
    
    /**
     *
     *开始不停的刷新图像
     */
    void ContinuousRefreshCam();
    
    /**
     *
     *更换活动的摄像头
     */
    void ChangeCamera(int index);
private:
    /**
     *更新Mat变量
     */
    void RefeshMat();
    
    //读取图像所用的代理
    boost::shared_ptr<AL::ALVideoDeviceProxy> fCamProxy;
    
    //读写memory代理
    AL::ALMemoryProxy fMemProxy;
    
    //说话代理
    AL::ALTextToSpeechProxy tts;
    
    //代理名字
    std::string fVideoClientName;
    
    //订阅摄像头时，置为1
    bool fRegisteredToVideoDevice;
    
    //用于存放当前帧的Mat
    cv::Mat fIplImageHeader;
    
    std::string phraseToSay;
    
    bool StartDetect;
    
    bool Detecting;
    
    int imgWidth;
    
    int imgHeight;
    
    bool RefeshingFlag;
    
    bool PoleDetecting;
    
    int expect_camera;
    
    std::string kOriginalName;
    
    int Reso;
    
    int pcs;

};
#endif

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
#include <alcommon/almodule.h>
#include <alproxies/alvideodeviceproxy.h>
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
     *初始化代理
     */
    void init();
    
    /**
     *关闭
     */
    void exit();
    
private:
    /**
     *更新Mat变量
     */
    void RefeshMat();
    
    //读取图像所用的代理
    boost::shared_ptr<AL::ALVideoDeviceProxy> fCamProxy;
    
    //代理名字
    std::string fVideoClientName;
    
    //订阅摄像头时，置为1
    bool fRegisteredToVideoDevice;
    
    //用于存放当前帧的Mat
    cv::Mat fIplImageHeader;
    
    const std::string kOriginalName = "njuRedBallmodule";
    
    const int kImgDepth = 8;
    
    const int kFps = 30;
    
    int imgWidth;
    
    int imgHeight;

};
#endif

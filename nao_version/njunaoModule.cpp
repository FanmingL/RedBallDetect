//
//  njunaoModule.cpp
//  RedBallDetect
//
//  Created by Fanming Luo on 2017/11/19.
//  Copyright © 2017年 Fanming Luo. All rights reserved.
//
#include "main.hpp"

#include <qi/log.hpp>
#include <alvision/alimage.h>
using namespace AL;
njunaoModule::njunaoModule(boost::shared_ptr<ALBroker> broker,const std::string& name):
  ALModule(broker, name),
  fRegisteredToVideoDevice(false),
  fIplImageHeader(cv::Mat())
{
    setModuleDescription( "A module write by NJUer for contest"
                         "This is an example of a generic video module. "
                         "It can be used to save images returned by the camera. "
                         "It internally uses OpenCv's cvSaveImage function. "
                         "Refer to the Example codes section below for an example of use.");
    
    // Define the module example.
    addModuleExample( "Python",
                     "  # Create a proxy to the module \n"
                     "  sampleProxy = ALProxy('GenericVideoModule', '127.0.0.1', 9559)\n\n"
                     "  # Register our module to the Video Input Module. \n"
                     "  sampleProxy.registerToVideoDevice(1, 13)\n\n"
                     "  # Save image in remote mode. \n"
                     "  sampleProxy.saveImageRemote('/home/nao/img', 'jpg') \n\n"
                     "  # Unregister.\n"
                     "  sampleProxy.unRegisterFromVideoDevice()\n"
                     );
    
    // Define bound methods with their description.
    functionName( "registerToVideoDevice", getName(), "Register to the V.I.M." );
    BIND_METHOD( njunaoModule::registerToVideoDevice );
    
    functionName( "unRegisterFromVideoDevice", getName(), "Unregister from the V.I.M." );
    BIND_METHOD( njunaoModule::unRegisterFromVideoDevice );
    
    functionName( "saveImageLocal", getName(), "Save an image received from the camera." );
    BIND_METHOD( njunaoModule::saveImageLocal );
    
    functionName( "RedBallFind", getName(), "Find the red ball on the green ground." );
    BIND_METHOD( njunaoModule::RedBallFind );
    
    functionName( "init", getName(), "Init." );
    BIND_METHOD( njunaoModule::init );
    
    
}

njunaoModule::~njunaoModule()
{
    try
    {
        if(fCamProxy)
            fCamProxy->unsubscribe(fVideoClientName);
        
        fCamProxy.reset();
    }
    catch(const AL::ALError& e)
    {
        qiLogError("vision.njunaoModule") <<  e.toString() << std::endl;
    }
}

void njunaoModule::registerToVideoDevice(const int &pResolution, const int &pColorSpace)
{
    if (fRegisteredToVideoDevice) {
        return;
    }
    int type;
    int imgNbLayers = 0;
    if (!fIplImageHeader.empty()) fIplImageHeader.release();
    setSizeFromResolution(pResolution, imgWidth, imgHeight);
    imgNbLayers = getNumLayersInColorSpace(pColorSpace);
    type = (imgNbLayers) == 3 ? CV_8UC3 : CV_8UC1;
    fIplImageHeader = cv::Mat(cv::Size(imgWidth, imgHeight), type);
    if(fCamProxy)
        fVideoClientName = fCamProxy->subscribe(kOriginalName, pResolution, pColorSpace, kFps );
    qiLogInfo("vision.genericvideomodule") << "Module registered as " << fVideoClientName << std::endl;
    fRegisteredToVideoDevice = true;
}

void njunaoModule::unRegisterFromVideoDevice()
{
    if (!fIplImageHeader.empty())
        fIplImageHeader.release();
    qiLogInfo("vision.njunaoModule") << "try to unregister " << fVideoClientName << " module." << std::endl;
    if (!fRegisteredToVideoDevice) {return;}
    if(fCamProxy)
        fCamProxy->unsubscribe(fVideoClientName);
    fRegisteredToVideoDevice = false;
}

void njunaoModule::saveImageLocal(const std::string& pName)
{
    if (!fRegisteredToVideoDevice) {
        throw ALError(getName(), "saveImageLocal()",  "No video module is currently "
                      "registered! Call registerToVideoDevice() first.");
    }
    RefeshMat();
    cv::imwrite(pName,fIplImageHeader);
}

void njunaoModule::RefeshMat()
{
    ALImage* imageIn = NULL;
    imageIn = (ALImage*)fCamProxy->getImageLocal(fVideoClientName);
    if (!imageIn) {
        throw ALError(getName(), "saveImageLocal", "Invalid image returned.");
    }
    // You can get some image information that you may find useful.
    //const int width = imageIn->fWidth;
    //const int height = imageIn->fHeight;
    //const int nbLayers = imageIn->fNbLayers;
    //const int colorSpace = imageIn->fColorSpace;
    //const long long timeStamp = imageIn->getTimeStamp();
    //const int seconds = (int)(timeStamp/1000000LL);
    fIplImageHeader.data = imageIn->getData();
    fCamProxy->releaseImage(fVideoClientName);
}

std::vector<float> njunaoModule::RedBallFind()
{
    RefeshMat();
    std::vector<double> RedBallPosition=DetectRedBall(fIplImageHeader);
    std::vector<float> PosTrans;
    PosTrans.push_back(0);
    PosTrans.push_back(0);
    if (!(RedBallPosition[0]==0&&RedBallPosition[1]==0&&RedBallPosition[2]==0))
    {
        if (!(imgWidth==0||imgHeight==0))
        {
            PosTrans[0]=RedBallPosition[0]/(1.0f*imgWidth);
            PosTrans[1]=RedBallPosition[1]/(1.0f*imgHeight);
        }
    }
    return PosTrans;
}

void njunaoModule::exit()
{
    AL::ALModule::exit();
}


void njunaoModule::init()
{
    try {
        fCamProxy = boost::shared_ptr<ALVideoDeviceProxy>(new ALVideoDeviceProxy(getParentBroker()));
    } catch (const AL::ALError& e) {
        qiLogError("vision.genericvideomodule") << "Error while getting proxy on ALVideoDevice.  Error msg " << e.toString() << std::endl;
        njunaoModule::exit();
        return;
    }
    if(fCamProxy == NULL)
    {
        qiLogError("vision.genericvideomodule") << "Error while getting proxy on ALVideoDevice. Check ALVideoDevice is running." << std::endl;
        njunaoModule::exit();
        return;
    }
    
    qiLogInfo("vision.genericvideomodule") << "Use registerToVideoDevice + "
    "saveImageLocal + unRegisterFromVideoDevice to save images." << std::endl;
}



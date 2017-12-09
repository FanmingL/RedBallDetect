//
//  njunaoModule.cpp
//  RedBallDetect
//
//  Created by Fanming Luo on 2017/11/19.
//  Copyright © 2017年 Fanming Luo. All rights reserved.
//
#include "main.hpp"
#include <vector>
#include <qi/log.hpp>
#include <alvision/alimage.h>
#include <alproxies/almemoryproxy.h>
#include <alproxies/altexttospeechproxy.h>
#include <alproxies/almotionproxy.h>
using namespace AL;
njunaoModule::njunaoModule(boost::shared_ptr<ALBroker> broker,const std::string& name):
  ALModule(broker, name),
    fMemProxy(getParentBroker()),
    motion(getParentBroker()),
  fRegisteredToVideoDevice(false),
  fIplImageHeader(cv::Mat()),
    Detecting(false),
    StartDetect(false),
    RefeshingFlag(false),
    PoleDetecting(false),
    zyFlag(false),
    expect_camera(0),
tts(getParentBroker())
{
    setModuleDescription( "A module write by NJUer for contest"
                         "This is an example of a generic video module. "
                         "It can be used to save images returned by the camera. "
                         "It internally uses OpenCv's cvSaveImage function. "
                         "Refer to the Example codes section below for an example of use.");
    
    // Define the module example.
    addModuleExample( "Python",
                     "  # Create a proxy to the module \n"
                     "  sampleProxy = ALProxy('njunaoModule', '127.0.0.1', 9559)\n\n"
                     "  # Register our module to the Video Input Module. \n"
                     "  sampleProxy.registerToVideoDevice(1, 13)\n\n"
                     "  # Save image in remote mode. \n"
                     "  sampleProxy.saveImageLocal('/home/nao/img.jpg') \n\n"
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
    
    functionName( "ContinuousFindBall", getName(), "Continuous Find RedBall" );
    BIND_METHOD( njunaoModule::ContinuousFindBall );
    
    functionName( "PoleFind", getName(), "Find the yellow Pole on the green ground." );
    BIND_METHOD( njunaoModule::PoleFind );
    
    functionName( "ContinuousFindPole", getName(), "Continuous Find Pole" );
    BIND_METHOD( njunaoModule::ContinuousFindPole );
    
    functionName( "sayVersion", getName(), "tell what version is it" );
    BIND_METHOD( njunaoModule::sayVersion );
    
    functionName( "ContinuousRefreshCam", getName(), "Refresh Camera" );
    BIND_METHOD( njunaoModule::ContinuousRefreshCam );
    
    functionName( "ChangeCamera", getName(), "Change Camera index" );
    BIND_METHOD( njunaoModule::ChangeCamera );
}

njunaoModule::~njunaoModule()
{
    try
    {
        if(fCamProxy)
            fCamProxy->unsubscribe(fVideoClientName);
        if (StartDetect)StartDetect=false;
        RefeshingFlag=false;
        fCamProxy.reset();
    }
    catch(const AL::ALError& e)
    {
        qiLogError("njunaoModule") <<  e.toString() << std::endl;
    }
}

void njunaoModule::registerToVideoDevice(const int &pResolution, const int &pColorSpace)
{
    if (fRegisteredToVideoDevice) {
        return;
    }
    kOriginalName = "njuRedBallmodule";
    Reso=pResolution;
    pcs=pColorSpace;
    const int kImgDepth = 8;
    const int kFps = 30;
    int type;
    int imgNbLayers = 0;
    if (!fIplImageHeader.empty()) fIplImageHeader.release();
    setSizeFromResolution(pResolution, imgWidth, imgHeight);
    imgNbLayers = getNumLayersInColorSpace(pColorSpace);
    type = (imgNbLayers) == 3 ? CV_8UC3 : CV_8UC1;
    fIplImageHeader = cv::Mat(cv::Size(imgWidth, imgHeight), type);
    //fCamProxy->setParameter(fCamProxy->getActiveCamera(),11,0);
    fCamProxy->setParameter(fCamProxy->getActiveCamera(),12,0);
    fCamProxy->setParameter(fCamProxy->getActiveCamera(),13,0);
    if(fCamProxy)
        fVideoClientName = fCamProxy->subscribe(kOriginalName, pResolution, pColorSpace, kFps );
    qiLogInfo("njunaoModule") << "Module registered as " << fVideoClientName << std::endl;
    fRegisteredToVideoDevice = true;
    StartDetect=true;
    fMemProxy.insertData("njunaoBallPositionStopFlag",0);
    fMemProxy.insertData("njunaoPolePositionStopFlag",0);
    Detecting=false;
}

void njunaoModule::unRegisterFromVideoDevice()
{
    StartDetect=false;
    RefeshingFlag=false;
    usleep(300000);
    if (!fIplImageHeader.empty())
        fIplImageHeader.release();
    
    qiLogInfo("njunaoModule") << "try to unregister " << fVideoClientName << " module." << std::endl;
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
   // RefeshMat();
    cv::imwrite(pName,fIplImageHeader);
}

void njunaoModule::RefeshMat()
{
    zyFlag=true;
    ALImage* imageIn = NULL;
    imageIn = (ALImage*)fCamProxy->getImageLocal(fVideoClientName);
    if (!imageIn) {
        throw ALError(getName(), "saveImageLocal", "Invalid image returned.");
    }
    cv::Mat fimage_temp;
    fIplImageHeader.copyTo(fimage_temp);
    // You can get some image information that you may find useful.
    //const int width = imageIn->fWidth;
    //const int height = imageIn->fHeight;
    //const int nbLayers = imageIn->fNbLayers;
    //const int colorSpace = imageIn->fColorSpace;
    //const long long timeStamp = imageIn->getTimeStamp();
    //const int seconds = (int)(timeStamp/1000000LL);
    fimage_temp.data = imageIn->getData();
    fimage_temp.copyTo(fIplImageHeader);
    cv::remap(fIplImageHeader,fIplImageHeader,map1,map2,cv::INTER_LINEAR);
    fCamProxy->releaseImage(fVideoClientName);
    zyFlag=false;
}

std::vector<float> njunaoModule::RedBallFind()
{

    std::vector<float> PosTrans(15,0);
    if ((!Detecting)&&RefeshingFlag)
    {
        if (StartDetect)
        {
            Detecting=true;
            std::vector<float> cam_result = cam_result_global;
            std::vector<float> head_pos =  head_pos_global;
            std::vector<float> cam_trans = cam_trans_global;
            
            std::vector<float> RedBallPosition=DetectRedBall(fIplImageHeader);
            
            for (int i=0;i<PosTrans.size();i++)PosTrans[i]=0;
            if (!(RedBallPosition[0]==0&&RedBallPosition[1]==0&&RedBallPosition[2]==0))
            {
                if (!(imgWidth==0||imgHeight==0))
                {
                    PosTrans[0]=RedBallPosition[0]/(1.0f*imgWidth);
                    PosTrans[1]=RedBallPosition[1]/(1.0f*imgHeight);
                    std::vector<float> _temp;
                    _temp.push_back(PosTrans[0]);
                    _temp.push_back(PosTrans[1]);
                    std::vector<float> anglar=fCamProxy->getAngularPositionFromImagePosition(fCamProxy->getActiveCamera(),_temp);
                    
                    PosTrans[2]=anglar[0];
                    PosTrans[3]=anglar[1];
                    for (int i=0;i<6;i++)PosTrans[i+4]=cam_result[i];
                    cv::Mat all_mat=(cv::Mat_<double>(4, 4) << cam_trans[0],cam_trans[1], cam_trans[2],cam_trans[3],
                                     cam_trans[4],cam_trans[5],cam_trans[6],cam_trans[7],
                                     cam_trans[8],cam_trans[9],cam_trans[10],cam_trans[11],
                                     0,0,0,1);
                    cv::Mat all_mat_inv=all_mat.inv();
                    cv::Mat all_mat_sel=(cv::Mat_<double>(3, 3) << -all_mat_inv.at<double>(1,0),-all_mat_inv.at<double>(1,1), -all_mat_inv.at<double>(1,3),
                                         -all_mat_inv.at<double>(2,0),-all_mat_inv.at<double>(2,1),-all_mat_inv.at<double>(2,3),
                                         all_mat_inv.at<double>(0,0),all_mat_inv.at<double>(0,1),all_mat_inv.at<double>(0,3));
                    
                    cv::Mat BallPos=(cv::Mat_<double>(3, 1) << RedBallPosition[0],RedBallPosition[1],1);
                    
                    cv::Mat BallPos_World=all_mat_sel.inv()*inner_inv*BallPos;
                    
                    
                    PosTrans[10]=BallPos_World.at<double>(0,0)/BallPos_World.at<double>(2,0);
                    PosTrans[11]=BallPos_World.at<double>(1,0)/BallPos_World.at<double>(2,0);
                    PosTrans[12]=head_pos[3];
                    PosTrans[13]=head_pos[4];
                    PosTrans[14]=head_pos[5];
                    
                    qiLogInfo("vision.njunaoModule")<<"find the ball ("<<PosTrans[10]<<", "<<PosTrans[11]<<")."<<std::endl;
                    
                }
                
            }
            fMemProxy.insertData("njunaoBallPosition",PosTrans);
            fMemProxy.raiseEvent("njuFindBall",PosTrans);
        }
        Detecting=false;
        
    }
    

    return PosTrans;
}

void njunaoModule::ContinuousFindBall()
{
    std::vector<float> PosTrans(15,0);

    
    if ((!Detecting)&&RefeshingFlag)
    {
        while (StartDetect)
        {
            Detecting=true;
            std::vector<float> cam_result = cam_result_global;
            std::vector<float> head_pos =  head_pos_global;
            std::vector<float> cam_trans = cam_trans_global;
            
            std::vector<float> RedBallPosition=DetectRedBall(fIplImageHeader);
            
            for (int i=0;i<PosTrans.size();i++)PosTrans[i]=0;
            if (!(RedBallPosition[0]==0&&RedBallPosition[1]==0&&RedBallPosition[2]==0))
            {
                if (!(imgWidth==0||imgHeight==0))
                {
                    PosTrans[0]=RedBallPosition[0]/(1.0f*imgWidth);
                    PosTrans[1]=RedBallPosition[1]/(1.0f*imgHeight);
                    std::vector<float> _temp;
                    _temp.push_back(PosTrans[0]);
                    _temp.push_back(PosTrans[1]);
                    std::vector<float> anglar=fCamProxy->getAngularPositionFromImagePosition(fCamProxy->getActiveCamera(),_temp);
                    
                    PosTrans[2]=anglar[0];
                    PosTrans[3]=anglar[1];
                    for (int i=0;i<6;i++)PosTrans[i+4]=cam_result[i];
                    cv::Mat all_mat=(cv::Mat_<double>(4, 4) << cam_trans[0],cam_trans[1], cam_trans[2],cam_trans[3],
                                     cam_trans[4],cam_trans[5],cam_trans[6],cam_trans[7],
                                     cam_trans[8],cam_trans[9],cam_trans[10],cam_trans[11],
                                     0,0,0,1);
                    cv::Mat all_mat_inv=all_mat.inv();
                    cv::Mat all_mat_sel=(cv::Mat_<double>(3, 3) << -all_mat_inv.at<double>(1,0),-all_mat_inv.at<double>(1,1), -all_mat_inv.at<double>(1,3),
                                         -all_mat_inv.at<double>(2,0),-all_mat_inv.at<double>(2,1),-all_mat_inv.at<double>(2,3),
                                         all_mat_inv.at<double>(0,0),all_mat_inv.at<double>(0,1),all_mat_inv.at<double>(0,3));
                    
                    cv::Mat BallPos=(cv::Mat_<double>(3, 1) << RedBallPosition[0],RedBallPosition[1],1);
                    
                    cv::Mat BallPos_World=all_mat_sel.inv()*inner_inv*BallPos;
                    
                   
                    PosTrans[10]=BallPos_World.at<double>(0,0)/BallPos_World.at<double>(2,0);
                    PosTrans[11]=BallPos_World.at<double>(1,0)/BallPos_World.at<double>(2,0);
                    PosTrans[12]=head_pos[3];
                    PosTrans[13]=head_pos[4];
                    PosTrans[14]=head_pos[5];
                    
                    qiLogInfo("vision.njunaoModule")<<"find the ball ("<<PosTrans[10]<<", "<<PosTrans[11]<<")."<<std::endl;
                    
                }
                
            }
            fMemProxy.insertData("njunaoBallPosition",PosTrans);
            fMemProxy.raiseEvent("njuFindBall",PosTrans);
            if (fMemProxy.getData("njunaoBallPositionStopFlag"))
            {
                break;
            }
        }
        Detecting=false;
        
    }
    
}

std::vector<float> njunaoModule::PoleFind()
{
    std::vector<float> PolePos(15,0);
    
    if (!PoleDetecting){
        PoleDetecting=true;
        
        if (StartDetect&&RefeshingFlag){
            std::vector<float> cam_result = cam_result_global;
            std::vector<float> head_pos =  head_pos_global;
            std::vector<float> cam_trans = cam_trans_global;
            std::vector<float> PolePos_temp=DetectPole(fIplImageHeader);
            if (!(PolePos_temp[0]==0))
            {
                PolePos[0]=PolePos_temp[0]/(1.0f*imgWidth);
                PolePos[1]=PolePos_temp[1]/(1.0f*imgHeight);
                for (int i=0;i<6;i++)PolePos[2+i]=cam_result[i];
                PolePos[8]=head_pos[3];
                PolePos[9]=head_pos[4];
                PolePos[10]=head_pos[5];
                
                cv::Mat all_mat=(cv::Mat_<double>(4, 4) << cam_trans[0],cam_trans[1], cam_trans[2],cam_trans[3],
                                 cam_trans[4],cam_trans[5],cam_trans[6],cam_trans[7],
                                 cam_trans[8],cam_trans[9],cam_trans[10],cam_trans[11],
                                 0,0,0,1);
                cv::Mat all_mat_inv=all_mat.inv();
                cv::Mat all_mat_sel=(cv::Mat_<double>(3, 3) << -all_mat_inv.at<double>(1,0),-all_mat_inv.at<double>(1,1), -all_mat_inv.at<double>(1,3),
                                     -all_mat_inv.at<double>(2,0),-all_mat_inv.at<double>(2,1),-all_mat_inv.at<double>(2,3),
                                     all_mat_inv.at<double>(0,0),all_mat_inv.at<double>(0,1),all_mat_inv.at<double>(0,3));
                
                cv::Mat PolePos_mat=(cv::Mat_<double>(3, 1) << PolePos_temp[0],PolePos_temp[1],1);
                
                cv::Mat BallPos_World=all_mat_sel.inv()*inner_inv*PolePos_mat;
                
                
                PolePos[11]=(float)(BallPos_World.at<double>(0,0)/BallPos_World.at<double>(2,0));
                PolePos[12]=(float)(BallPos_World.at<double>(1,0)/BallPos_World.at<double>(2,0));
                std::vector<float> _temp;
                _temp.push_back(PolePos[0]);
                _temp.push_back(PolePos[1]);
                std::vector<float> anglar=fCamProxy->getAngularPositionFromImagePosition(fCamProxy->getActiveCamera(),_temp);
                PolePos[13]=anglar[0];
                PolePos[14]=anglar[1];
                
                
            }
            fMemProxy.insertData("njunaoPolePosition",PolePos);
            fMemProxy.raiseEvent("njuFindPole",PolePos);
        }
        PoleDetecting=false;
    }
    return PolePos;
}

void njunaoModule::ContinuousFindPole()
{
    std::vector<float> PolePos(15,0);
    if (!PoleDetecting){
        PoleDetecting=true;
        while (StartDetect&&RefeshingFlag){
            
            std::vector<float> cam_result = cam_result_global;
            std::vector<float> head_pos =  head_pos_global;
            std::vector<float> cam_trans = cam_trans_global;
            std::vector<float> PolePos_temp=DetectPole(fIplImageHeader);
            for (int i=0;i<PolePos.size();i++)PolePos[i]=0;
            if (!(PolePos_temp[0]==0))
            {
                PolePos[0]=PolePos_temp[0]/(1.0f*imgWidth);
                PolePos[1]=PolePos_temp[1]/(1.0f*imgHeight);
                for (int i=0;i<6;i++)PolePos[2+i]=cam_result[i];
                PolePos[8]=head_pos[3];
                PolePos[9]=head_pos[4];
                PolePos[10]=head_pos[5];
                
                cv::Mat all_mat=(cv::Mat_<double>(4, 4) << cam_trans[0],cam_trans[1], cam_trans[2],cam_trans[3],
                                 cam_trans[4],cam_trans[5],cam_trans[6],cam_trans[7],
                                 cam_trans[8],cam_trans[9],cam_trans[10],cam_trans[11],
                                 0,0,0,1);
                cv::Mat all_mat_inv=all_mat.inv();
                cv::Mat all_mat_sel=(cv::Mat_<double>(3, 3) << -all_mat_inv.at<double>(1,0),-all_mat_inv.at<double>(1,1), -all_mat_inv.at<double>(1,3),
                                     -all_mat_inv.at<double>(2,0),-all_mat_inv.at<double>(2,1),-all_mat_inv.at<double>(2,3),
                                     all_mat_inv.at<double>(0,0),all_mat_inv.at<double>(0,1),all_mat_inv.at<double>(0,3));
                
                cv::Mat PolePos_mat=(cv::Mat_<double>(3, 1) << PolePos_temp[0],PolePos_temp[1],1);
                
                cv::Mat BallPos_World=all_mat_sel.inv()*inner_inv*PolePos_mat;
                
                
                PolePos[11]=(float)(BallPos_World.at<double>(0,0)/BallPos_World.at<double>(2,0));
                PolePos[12]=(float)(BallPos_World.at<double>(1,0)/BallPos_World.at<double>(2,0));
                
                std::vector<float> _temp;
                _temp.push_back(PolePos[0]);
                _temp.push_back(PolePos[1]);
                std::vector<float> anglar=fCamProxy->getAngularPositionFromImagePosition(fCamProxy->getActiveCamera(),_temp);
                PolePos[13]=anglar[0];
                PolePos[14]=anglar[1];
            }
            fMemProxy.insertData("njunaoPolePosition",PolePos);
            fMemProxy.raiseEvent("njuFindPole",PolePos);
            if (fMemProxy.getData("njunaoPolePositionStopFlag"))
            {
                break;
            }
        }
        PoleDetecting=false;
    }
}

void njunaoModule::exit()
{
    AL::ALModule::exit();
}


void njunaoModule::init()
{
    phraseToSay = "this version is 7.8";
    tts.post.say(phraseToSay);
    std::vector<float> PosTrans(15,0);
    std::vector<float> PolePos(15,0);
    
    con=(cv::Mat_<double>(4, 1) << -0.0625362260288806,0.0969517251981310, 0.0, 0.0);
    inner = (cv::Mat_<double>(3, 3) << 562.546487626155 , 0, 331.020749304873, 0, 560.726740317221, 229.930167086478, 0, 0, 1.0000);
    inner_inv = inner.inv();
    cv::initUndistortRectifyMap(inner,con,cv::Mat(),inner,cv::Size(640,480),CV_16SC2,map1,map2);
    fMemProxy.insertData("njunaoBallPositionStopFlag",0);
    fMemProxy.insertData("njunaoBallPosition",PosTrans);
    fMemProxy.insertData("njunaoPolePositionStopFlag",0);
    fMemProxy.insertData("njunaoPolePosition",PolePos);
    fMemProxy.raiseEvent("njuFindPole",PolePos);
    fMemProxy.raiseEvent("njuFindBall",PosTrans);
   // fMemProxy.subscribeToEvent("njuFindBall","njunaoModule","njunaoBallPosition1");
   // fMemProxy.subscribeToEvent("njuFindPole","njunaoModule","njunaoPolePosition1");
    try {
        fCamProxy = boost::shared_ptr<ALVideoDeviceProxy>(new ALVideoDeviceProxy(getParentBroker()));
        
    } catch (const AL::ALError& e) {
        qiLogError("njunaoModule") << "Error while getting proxy on ALVideoDevice.  Error msg " << e.toString() << std::endl;
        njunaoModule::exit();
        return;
    }
    if(fCamProxy == NULL)
    {
        qiLogError("njunaoModule") << "Error while getting proxy on ALVideoDevice. Check ALVideoDevice is running." << std::endl;
        njunaoModule::exit();
        return;
    }
    
    qiLogInfo("njunaoModule") << "Use registerToVideoDevice + "
    "saveImageLocal + unRegisterFromVideoDevice to save images." << std::endl;
}

void njunaoModule::ContinuousRefreshCam()
{
    if (!RefeshingFlag){
    RefeshingFlag=true;
    while (RefeshingFlag)
    {
        RefeshMat();
        cam_result_global = motion.getPosition(fCamProxy->getCameraName(fCamProxy->getActiveCamera()), 0, true);
        cam_trans_global = motion.getTransform(fCamProxy->getCameraName(fCamProxy->getActiveCamera()), 2, true);
        head_pos_global =  motion.getPosition("Head",0, true);

        usleep(60000);
      //  if (expect_camera!=fCamProxy->getActiveCamera())
       // {
            //fCamProxy->unsubscribe(fVideoClientName);
        //    fCamProxy->setActiveCamera(expect_camera);
            //fVideoClientName = fCamProxy->subscribe(kOriginalName, Reso, pcs, 30 );
        //    usleep(500000);
       // }
    }
    RefeshingFlag=false;
    }
}

void njunaoModule::sayVersion()
{
    tts.post.say(phraseToSay);
}

void njunaoModule::ChangeCamera(int index)
{
    expect_camera=index;
    if ((expect_camera==fCamProxy->getActiveCamera()))return;
    while (zyFlag);
    if (fRegisteredToVideoDevice)
        unRegisterFromVideoDevice();
    fCamProxy->setActiveCamera(index);
   // usleep(1000000);
    //fVideoClientName = fCamProxy->subscribe(kOriginalName, Reso, pcs, 30 );
}




//
//  Detect.cpp
//  RedBallDetect
//
//  Created by Fanming Luo on 2017/11/19.
//  Copyright © 2017年 Fanming Luo. All rights reserved.
//
#include "Detect.hpp"
void on_mouse(int EVENT, int x, int y, int flags, void* userdata);
void fillHole(const cv::Mat srcBw, cv::Mat &dstBw);

//std::vector<float> DetectRedBall(cv::Mat &originalImage, const int &redhmin, const int &redhmax, const int &redsmin, const int &redsmax, const int &redvmin, const int &redvmax, const int &greenhmin, const int &greenhmax, const int &greensmin, const int &greensmax, const int &greenvmin, const int &greenvmax)
std::vector<float> DetectRedBall(cv::Mat &originalImage, int redhmin, int redhmax, int redsmin, int redsmax, int redvmin, int redvmax, int greenhmin, int greenhmax, int greensmin, int greensmax, int greenvmin, int greenvmax)
//std::vector<float> DetectRedBall(cv::Mat &originalImage)
{
    std::vector<float> BallPosition;
    BallPosition.push_back(0);BallPosition.push_back(0);BallPosition.push_back(0);
    if (originalImage.empty())
    {
        return BallPosition;
        
    }

    
    cv::Mat hsvImage,GreenMask,RedMask,RedMaskFiltered,GreenMaskFiltered,OutMask,OutMaskFiltered;
#ifdef TEST
    cv::Mat GreenSpace,GreenSpaceII;
    cv::cvtColor(originalImage, GreenSpace, CV_BGR2GRAY);
#endif
//    cv::Mat YUV_Image;
//    cv::cvtColor(originalImage, YUV_Image, CV_BGR2YUV);
//    std::vector<cv::Mat> yuvChannels;
//    cv::split(YUV_Image, yuvChannels);
//    cv::Mat YUV_REDMASK=(yuvChannels[1]>170);
    
    cv::Mat element;
    std::vector<cv::Mat> hsvChannels;
    cv::cvtColor(originalImage, hsvImage, CV_BGR2HSV);
    cv::split(hsvImage, hsvChannels);
    // 阈值设置
	RedMask=((hsvChannels[0]>=redhmin)&(hsvChannels[0]<=redhmax))&((hsvChannels[1]>=redsmin)&(hsvChannels[1]<=redsmax))&((hsvChannels[2]>=redvmin)&(hsvChannels[2]<=redvmax));
	GreenMask=((hsvChannels[0]>=greenhmin)&(hsvChannels[0]<=greenhmax))&((hsvChannels[1]>=greensmin)&(hsvChannels[1]<=greensmax))&((hsvChannels[2]>=greenvmin)&(hsvChannels[2]<=greenvmax));
	//RedMask=((hsvChannels[0]>=5)&(hsvChannels[0]<20)|(hsvChannels[0]>175))&(hsvChannels[1]>155);
	//GreenMask=(hsvChannels[0]>32)&(hsvChannels[0]<45)&(hsvChannels[1]>80);
    //RedMask=((hsvChannels[0]>=0)&(hsvChannels[0]<20)|(hsvChannels[0]>175))&(hsvChannels[1]>155);
    //GreenMask=(hsvChannels[0]>37)&(hsvChannels[0]<70)&(hsvChannels[1]>80);//nju
  //  RedMask=((hsvChannels[0]>=0)&(hsvChannels[0]<8)|(hsvChannels[0]>172))&(hsvChannels[1]>120);
  //  GreenMask=(hsvChannels[0]>70)&(hsvChannels[0]<85)&(hsvChannels[1]>100);//nst
    element=cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2*3+1,2*3+1));
    cv::morphologyEx(GreenMask,GreenMask,cv::MORPH_OPEN,element);
    
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat MaskToFindContour;
//    GreenMask.copyTo(MaskToFindContour);
//    findContours( MaskToFindContour, contours , hierarchy , CV_RETR_CCOMP , CV_CHAIN_APPROX_SIMPLE );
    double maxArea = 0;
//    double SecondMaxArea = 0;
    int maxAreaIndex=0;
//    int SecondMaxAreaIndex=0;
//    for( size_t i = 0; i < contours.size();i++ )
//    {
//        double area = contourArea( contours[i] );
//        if( area > maxArea )
//        {
//            SecondMaxArea=maxArea;
//            SecondMaxAreaIndex=maxAreaIndex;
//            maxArea = area;
//            maxAreaIndex=(int)i;
//        }
//    }
  //  std::cout<<"No BUG"<<std::endl;
#ifdef TEST
//    std::cout<<"max: "<<maxArea<<" second: "<<SecondMaxArea<<std::endl;
//    std::cout<<"number of contours "<<contours.size()<<std::endl;
#endif
    GreenMaskFiltered=cv::Mat(GreenMask.rows,GreenMask.cols,CV_8UC1,cv::Scalar(0));
//    cv::drawContours(GreenMaskFiltered, contours, maxAreaIndex, cv::Scalar(255,255,255), -1);
//    if (SecondMaxArea>0){
//        cv::drawContours(GreenMaskFiltered, contours, SecondMaxAreaIndex, cv::Scalar(255,255,255), -1);
//    }
    GreenMask.copyTo(GreenMaskFiltered);
    element=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2*40+1,2*40+1));
    cv::morphologyEx(GreenMaskFiltered,GreenMaskFiltered,cv::MORPH_CLOSE,element);
    OutMask=RedMask&GreenMaskFiltered;
    element=cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2*1+1,2*1+1));
    cv::morphologyEx(OutMask,OutMaskFiltered,cv::MORPH_OPEN,element);
    OutMaskFiltered.copyTo(MaskToFindContour);
    cv::findContours( MaskToFindContour, contours , hierarchy , CV_RETR_CCOMP , CV_CHAIN_APPROX_SIMPLE );
    maxArea=0;
    maxAreaIndex=0;
    std::vector<cv::Point> maxcontours;
    for( size_t i = 0; i < contours.size();i++ )
    {
        double area = contourArea( contours[i] );
        if( area > maxArea )
        {
            maxArea = area;
            maxAreaIndex=(int)i;
            maxcontours=contours[i];
        }
    }
   // OutMaskFiltered=cv::Mat(OutMask.rows,OutMask.cols,CV_8UC1,cv::Scalar(0));
    if (contours.size()>0&&maxcontours.size()>1){
#ifdef TEST
        cv::drawContours(OutMaskFiltered, contours, maxAreaIndex, cv::Scalar(255), -1);
#endif
        //element=cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2*4+1,2*4+1));       //for a better circle
        //cv::morphologyEx(OutMaskFiltered,OutMaskFiltered,cv::MORPH_CLOSE,element);
        cv::Point2f center; float radius;
        minEnclosingCircle(maxcontours,center,radius);
        
        BallPosition[0]=center.x;
        BallPosition[1]=center.y;
        BallPosition[2]=radius;
    }
#ifdef TEST
    cv::namedWindow("originalImage");
    cv::imshow("originalImage", originalImage);
    cv::namedWindow("RedMask");
    cv::imshow("RedMask", RedMask);
    cv::namedWindow("GreenMask");
    cv::imshow("GreenMask", GreenMask);
    cv::namedWindow("GreenMaskFiltered");
    cv::imshow("GreenMaskFiltered", GreenMaskFiltered);
    cv::namedWindow("hsvImage");
    cv::imshow("hsvImage", hsvImage);
    cv::namedWindow("OutMaskFiltered");
    cv::imshow("OutMaskFiltered", OutMaskFiltered);
    cv::namedWindow("OutMask");
    cv::imshow("OutMask", OutMask);
    cv::setMouseCallback("originalImage", on_mouse, &hsvImage);

    fillHole(GreenMask,GreenSpaceII);
    cv::namedWindow("GreenSpaceII");
    cv::imshow("GreenSpaceII", GreenSpaceII);
    GreenSpace=GreenSpace&GreenSpaceII;
    cv::namedWindow("GreenSpace");
    cv::imshow("GreenSpace", GreenSpace);
    while (1){
        char Key=cv::waitKey(30);
        if(Key==' ')break;
    }
    cv::destroyAllWindows();
#endif
    return BallPosition;
}

void on_mouse(int EVENT, int x, int y, int flags, void* userdata)
{
    cv::Mat hh;
    hh = *(cv::Mat*)userdata;
    cv::Point p(x, y);
    switch (EVENT)
    {
        case cv::EVENT_LBUTTONDOWN:
        {
            std::cout<<" H "<<(int)hh.at<cv::Vec3b>(p)[0]
            <<" S "<<(int)hh.at<cv::Vec3b>(p)[1]
            <<" V "<<(int)hh.at<cv::Vec3b>(p)[2]
            <<std::endl;
        }
            break;
    }
}

void fillHole(const cv::Mat srcBw, cv::Mat &dstBw)
{
    cv::Size m_Size = srcBw.size();
    cv::Mat Temp=cv::Mat::zeros(m_Size.height+2,m_Size.width+2,srcBw.type());//延展图像
    srcBw.copyTo(Temp(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)));
    
    cv::floodFill(Temp, cv::Point(0, 0), cv::Scalar(255));
    
    cv::Mat cutImg;//裁剪延展的图像
    Temp(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)).copyTo(cutImg);
    
    dstBw = srcBw | (~cutImg);
}





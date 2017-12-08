//
//  pole.cpp
//  RedBallDetect
//
//  Created by Fanming Luo on 2017/11/23.
//  Copyright © 2017年 Fanming Luo. All rights reserved.
//

#include "main.hpp"
void on_mouse2(int EVENT, int x, int y, int flags, void* userdata);
#define _MY_ABS(x) (((x)>0)?((x)):(-(x)))
std::vector<float> DetectPole(cv::Mat &originalImage)
{
    std::vector<float> angle;
    angle.push_back(0);
    angle.push_back(0);
    if (originalImage.empty())
    {
        return angle;
    }
    std::vector<cv::Mat> hsv;
    cv::Mat hsvImage,yellowMask,yellowMaskFiltered,LineMask,OutMask,OutMaskFiltered;
    cv::Mat roi_img;
    cv::Mat element;
    cv::Mat LineImage;
    originalImage.copyTo(LineImage);
    cv::cvtColor(originalImage, hsvImage, CV_BGR2HSV);
    cv::split(hsvImage, hsv);
    //yellowMask=((hsv[0]>20)&(hsv[0]<34)&(hsv[1]>80));//nju
    yellowMask=((hsv[0]>25)&(hsv[0]<31)&(hsv[1]>150));//nst
    
    element=cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(4,4));
    cv::morphologyEx(yellowMask,yellowMaskFiltered,cv::MORPH_CLOSE,element);
    
    std::vector<cv::Vec2f> lines;
    cv::HoughLines(yellowMaskFiltered, lines, 1, CV_PI/180, 50 );
    float Xcenter=0.0f,x_num=0;
    float x1_sum=0.0f,x2_sum=0.0f;
    std::vector<float> Xcenter_filter,Xcenter_error;
    std::vector<float> X1_filter,X2_filter;
    
    for( size_t i = 0; i < lines.size(); i++ )//将求得的线条画出来
    {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        if (!((theta>=-0.1f)&&(theta<=0.1f)))continue;
        float x1=rho/cos(theta),
        x2=rho/cos(theta)-originalImage.rows*tan(theta);
        x1_sum+=x1;
        x2_sum+=x2;
        X1_filter.push_back(x1);
        X2_filter.push_back(x2);
        float x_center=(x1+x2)/2.0f;
        Xcenter_filter.push_back(x_center);
      //  Xcenter+=x_center;
        x_num++;
#ifdef TEST2
        std::cout<<"x_center "<<x_center<<std::endl;
        std::cout<<"Xcenter "<<Xcenter<<" num "<<x_num<<std::endl;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
       //cv::line( LineImage, pt1, pt2, cv::Scalar(0,0,255), 3, CV_AA);
#endif
    }
    if (Xcenter_filter.size()==0)
    {
        std::cout<<"no pole"<<std::endl;
        return angle;
    }
    for (int i=0;i<Xcenter_filter.size();i++)
    {
        Xcenter+=Xcenter_filter[i];
        x_num++;
    }
    Xcenter/=x_num;
    for (int i=0;i<Xcenter_filter.size();i++)
    {
        Xcenter_error.push_back(Xcenter_filter[i]-Xcenter);
    }
    float mean_error=0;
    for (int i=0;i<Xcenter_filter.size();i++)
    {
        mean_error+=_MY_ABS(Xcenter_error[i]);
    }
    mean_error/=Xcenter_error.size();
    Xcenter=0;
    x_num=0;
    x1_sum=0;
    x2_sum=0;
    for (int i=0;i<Xcenter_filter.size();i++)
    {
        if (_MY_ABS(Xcenter_error[i])<_MY_ABS(mean_error))
        {
            x1_sum+=X1_filter[i];
            x2_sum+=X2_filter[i];
            Xcenter+=Xcenter_filter[i];
            x_num++;
        }
    }
    
    LineMask=cv::Mat(originalImage.rows,originalImage.cols,CV_8UC1,cv::Scalar(0));
    if (!(x_num==0.0f))
    {
        Xcenter/=x_num;
        x1_sum/=x_num;
        x2_sum/=x_num;
#ifdef TEST2
        std::cout<<"Xcenter "<<Xcenter<<" num "<<x_num<<std::endl;
        cv::Point pt1, pt2;
        pt1.y=0;
        pt2.y=originalImage.rows;
        pt1.x=(int)x1_sum;
        pt2.x=(int)x2_sum;
        cv::line(LineMask, pt1, pt2, cv::Scalar(255), 3, CV_AA);
#endif
    }
    else
    {
        std::cout<<"no pole"<<std::endl;
        return angle;
    }
    element=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2,2));
    cv::morphologyEx(yellowMaskFiltered,yellowMaskFiltered,cv::MORPH_OPEN,element);
    float OutPoleCenter_x=0.0,OutPoleCenter_num=0;
    for (int i=0;i<originalImage.rows;i++)
    {
        if (yellowMask.at<uchar>( i,(int)(x1_sum+(x2_sum-x1_sum)/originalImage.rows*i)))
        {
            OutPoleCenter_x+=x1_sum+(x2_sum-x1_sum)/originalImage.rows*i;
            OutPoleCenter_num++;
           
        }
    }
    if (OutPoleCenter_num!=0)
    {
        OutPoleCenter_x/=OutPoleCenter_num;

        
        int x_rect_min=OutPoleCenter_x-30;
        int x_rect_max=OutPoleCenter_x+30;
        if (x_rect_min<0)x_rect_min=0;
        if (x_rect_max>originalImage.cols)x_rect_max=originalImage.cols;
        

        
        
        roi_img = yellowMaskFiltered(cv::Range(0,originalImage.rows),cv::Range(x_rect_min,x_rect_max));
        element=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
        cv::morphologyEx(roi_img,roi_img,cv::MORPH_CLOSE,element);
        element=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
        cv::morphologyEx(roi_img,roi_img,cv::MORPH_OPEN,element);
        
        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::Mat MaskToFindContour;
        roi_img.copyTo(MaskToFindContour);
        std::vector<cv::Point> maxcontours;
        cv::findContours( MaskToFindContour, contours , hierarchy , CV_RETR_CCOMP , CV_CHAIN_APPROX_SIMPLE );
        double maxArea = 0;
        int maxAreaIndex=0;
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
        float max_y=0;
        for (int i=0;i<maxcontours.size();i++)
        {
            if (maxcontours[i].y>max_y)
            {
                max_y=maxcontours[i].y;
            }
        }

#ifdef TEST2
        cv::RotatedRect rect=cv::minAreaRect(maxcontours);
        cv::Point2f P[4];
        rect.points(P);
        for(int j=0;j<=3;j++)
        {
            P[j].x+=x_rect_min;
        }
        for(int j=0;j<=3;j++)
        {
           cv::line(LineImage,P[j],P[(j+1)%4],cv::Scalar(255),1);
        }
#endif
    
        angle[0]=OutPoleCenter_x;
        angle[1]=max_y;
        
#ifdef  TEST2
        std::cout<<"end "<<OutPoleCenter_x<<std::endl;
#endif
    }
    else
    {
        std::cout<<"find bug"<<std::endl;
    }
    
    
#ifdef TEST2
    cv::Point pt1, pt2;
    pt1.y=0;
    pt2.y=originalImage.rows;
    pt1.x=(int)OutPoleCenter_x;
    pt2.x=(int)OutPoleCenter_x;
   // cv::line(LineImage, pt1, pt2, cv::Scalar(255,0,0), 3, CV_AA);
    cv::namedWindow("yellowMaskFiltered");
    cv::imshow("yellowMaskFiltered", yellowMaskFiltered);
    cv::namedWindow("yellowMask");
    cv::imshow("yellowMask", yellowMask);
    cv::namedWindow("hsvImage");
    cv::imshow("hsvImage", hsvImage);
    cv::namedWindow("originalImage");
    cv::imshow("originalImage", originalImage);

    cv::namedWindow("LineMask");
    cv::imshow("LineMask", LineMask);
    cv::namedWindow("LineImage");
    cv::imshow("LineImage", LineImage);
    
    cv::namedWindow("roi_img");
    cv::imshow("roi_img", roi_img);
    
    cv::setMouseCallback("yellowMask", on_mouse2, &hsvImage);
    while (1){
        char Key=cv::waitKey(30);
        if(Key==' ')break;
    }
    cv::destroyAllWindows();
    
#endif
    
    
    
    return angle;
}

void on_mouse2(int EVENT, int x, int y, int flags, void* userdata)
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

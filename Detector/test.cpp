//
//  test.cpp
//  RedBallDetect
//
//  Created by Fanming Luo on 2017/11/19.
//  Copyright © 2017年 Fanming Luo. All rights reserved.
//
//用这几张图基本能找到
#include <iostream>
#include "Detect.hpp"
#include "pole.hpp"
int main(int argc, const char * argv[]) {
    cv::Mat origin;
    if (argc==1)
    {
        origin=cv::imread("/Users/erdou/Documents/RedBallDetect/testpic/test5.jpg",1);
    }
    else
    {
        origin=cv::imread(argv[1],1);
    }
    std::vector<float> BallPosition;
    std::vector<float> polePosition;
    BallPosition=DetectRedBall(origin);
    polePosition=DetectPole(origin);
    if ((BallPosition[0]&&BallPosition[1]&&BallPosition[2])){
        std::cout<<" x= "<<BallPosition[0]<<" y= "<<BallPosition[1]<<" r= "<<BallPosition[2]<<std::endl;
        cv::circle(origin, cv::Point(BallPosition[0],BallPosition[1]), BallPosition[2], cv::Scalar(255,255,255),-1);

    }
    else
    {
        std::cout<<"find no ball\n";
    }

    if (polePosition[0])
    {
        std::cout<<"Pole x = "<<polePosition[0]<<std::endl;
        cv::Point pt1, pt2;
        pt1.y=0;
        pt2.y=origin.rows;
        pt1.x=(int)(polePosition[0]);
        pt2.x=(int)(polePosition[0]);
        cv::line(origin, pt1, pt2, cv::Scalar(255,0,0), 3, CV_AA);
    }
    else
    {
        
        std::cout<<"find no poles\n";
    }
    
    cv::namedWindow("originalImage");
    cv::imshow("originalImage", origin);
    cv::waitKey();
    cv::destroyAllWindows();
    return 0;
}


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

int main(int argc, const char * argv[]) {
    cv::Mat origin;
    if (argc==1)
    {
        origin=cv::imread("/Users/erdou/Downloads/test1.jpg",1);
    }
    else
    {
        origin=cv::imread(argv[1],1);
    }
    std::vector<double> BallPosition;
    BallPosition=DetectRedBall(origin);
    if (!(BallPosition[0]&&BallPosition[1]&&BallPosition[2])){
        std::cout<<"find no ball!\n";
        cv::namedWindow("originalImage");
        cv::imshow("originalImage", origin);
        cv::waitKey();
        cv::destroyAllWindows();
        return 0;
    }
    std::cout<<" x= "<<BallPosition[0]<<" y= "<<BallPosition[1]<<" r= "<<BallPosition[2]<<std::endl;
    cv::circle(origin, cv::Point(BallPosition[0],BallPosition[1]), BallPosition[2], cv::Scalar(255,255,255),-1);
    cv::namedWindow("originalImage");
    cv::imshow("originalImage", origin);
    cv::waitKey();
    cv::destroyAllWindows();
    return 0;
}


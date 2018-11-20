/*
 *	image utils
 */

#include "image_util.h"

void getOneChannel(const cv::Mat &src, cv::Mat &dst, const int channel)
{
    std::vector<cv::Mat> img_array;
    cv::split(src, img_array);
    if (channel < img_array.size()){
        dst = img_array[channel].clone();
    }else{
        std::cerr << "out of range." << std::endl;
    }
}


void calculateTransform(const cv::Point2f &src_p, const cv::Mat &homography_matrix, cv::Point2f &p_dst)
{
    cv::Mat point_src(3, 1, CV_64FC1, cv::Scalar(1)), point_dst(3, 1, CV_64FC1, cv::Scalar(1));
    point_src.at<double>(0, 0) = src_p.x;
    point_src.at<double>(1, 0) = src_p.y;

    point_dst = homography_matrix * point_src;
    point_dst /= point_dst.at<double>(2, 0);

    p_dst.x = (float)(point_dst.at<double>(0, 0));
    p_dst.y = (float)(point_dst.at<double>(1, 0));
}

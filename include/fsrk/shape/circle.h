#ifndef CIRCLE_H
#define CIRCLE_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

namespace cv
{

class Circlef
{
    public:
    Circlef( ) {}
    Circlef( cv::Point2f _center, float _r );

    bool inside( cv::Point2f pt );
    bool onCircle( cv::Point2f pt );
    std::vector< cv::Point2f > getCirclePoints( cv::Size size );

    public:
    cv::Point2f center;
    float radius;
};

class CircleInt
{
    public:
    CircleInt( cv::Point _center, int _r );

    /* draws simple or filled circle */
    void draw( Mat& img, const Scalar& color );
    CircleInt( Mat& img, Point _center, int _radius, const Scalar& color );
    CircleInt( cv::Mat& img, cv::Point _center, int _radius, const Scalar& color, std::vector< cv::Point2f >& pts );

    bool inside( cv::Point pt );
    bool onCircle( cv::Point pt );
    std::vector< cv::Point2f > getCirclePoints( Size size );

    public:
    cv::Point center;
    int radius;
};
}

#endif // CIRCLE_H

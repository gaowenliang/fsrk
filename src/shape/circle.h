#ifndef CIRCLE_H
#define CIRCLE_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

namespace cv
{

class Circlef
{
    Circlef( ) {}
    Circlef( cv::Point2f _center, float _r )
    : center( _center )
    , radius( _r )
    {
    }

    bool inside( cv::Point2f pt )
    {
        float dx = pt.x - center.x;
        float dy = pt.y - center.y;

        float dis2 = dx * dx + dy * dy;

        return dis2 >= radius * radius ? false : true;
    }
    bool onCircle( cv::Point2f pt )
    {
        float dx = pt.x - center.x;
        float dy = pt.y - center.y;

        float dis2 = dx * dx + dy * dy;

        return ( dis2 - radius ) * ( dis2 - radius ) < 0.001 ? false : true;
    }

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

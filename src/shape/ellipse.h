#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "rectrot.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

namespace cv
{

enum
{
    XY_SHIFT              = 16,
    XY_ONE                = 1 << XY_SHIFT,
    DRAWING_STORAGE_BLOCK = ( 1 << 12 ) - 256
};

class Ellipse
{
    public:
    Ellipse( ) {}
    Ellipse( const cv::RectRot& _box );
    Ellipse( const Point2f& center, const Size2f& size, float angle );

    float dis2( cv::Point2f pt0, cv::Point2f pt1 );
    bool inside( cv::Point2f pt );
    bool onEllipse( cv::Point2f pt );

    void fit( InputArray _points );

    void draw( InputOutputArray img, const Scalar& color );

    float getAxisLong( ) const { return 2 * axisHalfLong; }
    float getAxisShort( ) const { return 2 * axisHalfShort; }
    float getHalfAxisLong( ) const { return axisHalfLong; }
    float getHalfAxisShort( ) const { return axisHalfShort; }
    float getFocalLength( ) const { return focalLengthHalf; }

    cv::Point2f getFocalPt0( ) const;
    cv::Point2f getFocalPt1( ) const;

    cv::RectRot getBox( ) const;
    cv::RotatedRect getBox2( ) const;

    public:
    float axisHalfLong;    // a
    float axisHalfShort;   // b
    float focalLengthHalf; // c

    cv::Point2f focalPoint0;
    cv::Point2f focalPoint1;

    cv::RectRot box;
};
}
#endif // ELLIPSE_H

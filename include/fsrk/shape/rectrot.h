#ifndef RECTROT_H
#define RECTROT_H

#include "opencv2/opencv.hpp"

namespace cv
{

///////////////////////////// RectRot /////////////////////////////

/** @brief The class represents rotated (i.e. not up-right) rectangles on a plane.

Each rectangle is specified by the center point (mass center), length of each side
(represented by
cv::Size2f structure) and the rotation angle in degrees.

@sa CamShift, fitEllipse, minAreaRect, CvBox2D
*/
class RectRot
{
    public:
    //! various constructors
    RectRot( );

    RectRot( const RectRot& rect );
    /**
    @param center The rectangle mass center.
    @param size Width and height of the rectangle.
    @param angle The rotation angle in a clockwise direction. When the angle is 0, 90, 180,
    270 etc.,
    the rectangle becomes an up-right rectangle.
    */
    RectRot( const Point2f& _center, const Size2f& _size, float _angle );
    /**
     * Any 3 end points of the RectRot. They must be given in order (either clockwise or
     *   anticlockwise).
   */
    RectRot( const Point2f& _point1, const Point2f& _point2, const Point2f& _point3 );

    /** returns 4 vertices of the rectangle
    @param pts The points array for storing rectangle vertices.
    */
    void points( Point2f pts[] ) const;
    void points( std::vector< Point2f >& pts ) const;

    void edgeCenterPoints( std::vector< cv::Point2f >& pts ) const;

    //! returns the minimal up-right integer rectangle containing the rotated rectangle
    Rect boundingRect( ) const;
    //! returns the minimal (exact) floating point rectangle containing the rotated
    //! rectangle, not intended for use with images
    Rect_< float > boundingRect2f( ) const;

    void draw( cv::Mat& img, const cv::Scalar& color );

    Point2f center; //< the rectangle mass center
    Size2f size;    //< width and height of the rectangle
    float angle;    //< the rotation angle. When the angle is 0, 90, 180, 270 etc., the
                    // rectangle becomes an up-right rectangle.
};
}

#endif // RECTROT_H

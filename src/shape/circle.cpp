#include "circle.h"
#include "cv_utils.h"

cv::CircleInt::CircleInt( cv::Point _center, int _r )
: center( _center )
, radius( _r )
{
}

void
cv::CircleInt::draw( cv::Mat& img, const cv::Scalar& color )
{
    double color_data[4];

    cv_utils::scalarToData( color, color_data, img.type( ), 0 );

    Size size    = img.size( );
    size_t step  = img.step;
    int pix_size = ( int )img.elemSize( );

    uchar* ptr = img.ptr( );

    int err = 0, dx = radius, dy = 0, plus = 1, minus = ( radius << 1 ) - 1;
    int inside = center.x >= radius && center.x < size.width - radius && center.y >= radius
                 && center.y < size.height - radius;

#define ICV_PUT_POINT( ptr, x ) memcpy( ptr + ( x )*pix_size, color_data, pix_size );

    while ( dx >= dy )
    {
        int mask;
        int y11 = center.y - dy, y12 = center.y + dy, y21 = center.y - dx, y22 = center.y + dx;
        int x11 = center.x - dx, x12 = center.x + dx, x21 = center.x - dy, x22 = center.x + dy;

        if ( inside )
        {
            uchar* tptr0 = ptr + y11 * step;
            uchar* tptr1 = ptr + y12 * step;

            ICV_PUT_POINT( tptr0, x11 );
            ICV_PUT_POINT( tptr1, x11 );
            ICV_PUT_POINT( tptr0, x12 );
            ICV_PUT_POINT( tptr1, x12 );

            tptr0 = ptr + y21 * step;
            tptr1 = ptr + y22 * step;

            ICV_PUT_POINT( tptr0, x21 );
            ICV_PUT_POINT( tptr1, x21 );
            ICV_PUT_POINT( tptr0, x22 );
            ICV_PUT_POINT( tptr1, x22 );
        }
        else if ( x11 < size.width && x12 >= 0 && y21 < size.height && y22 >= 0 )
        {

            if ( ( unsigned )y11 < ( unsigned )size.height )
            {
                uchar* tptr = ptr + y11 * step;

                if ( x11 >= 0 )
                    ICV_PUT_POINT( tptr, x11 );
                if ( x12 < size.width )
                    ICV_PUT_POINT( tptr, x12 );
            }

            if ( ( unsigned )y12 < ( unsigned )size.height )
            {
                uchar* tptr = ptr + y12 * step;

                if ( x11 >= 0 )
                    ICV_PUT_POINT( tptr, x11 );
                if ( x12 < size.width )
                    ICV_PUT_POINT( tptr, x12 );
            }

            if ( x21 < size.width && x22 >= 0 )
            {

                if ( ( unsigned )y21 < ( unsigned )size.height )
                {
                    uchar* tptr = ptr + y21 * step;

                    if ( x21 >= 0 )
                        ICV_PUT_POINT( tptr, x21 );
                    if ( x22 < size.width )
                        ICV_PUT_POINT( tptr, x22 );
                }

                if ( ( unsigned )y22 < ( unsigned )size.height )
                {
                    uchar* tptr = ptr + y22 * step;

                    if ( x21 >= 0 )
                        ICV_PUT_POINT( tptr, x21 );
                    if ( x22 < size.width )
                        ICV_PUT_POINT( tptr, x22 );
                }
            }
        }
        dy++;
        err += plus;
        plus += 2;

        mask = ( err <= 0 ) - 1;

        err -= minus & mask;
        dx += mask;
        minus -= mask & 2;
    }

#undef ICV_PUT_POINT
}

cv::CircleInt::CircleInt(
cv::Mat& img, cv::Point _center, int _radius, const Scalar& color, std::vector< cv::Point2f >& pts )
: center( _center )
, radius( _radius )
{

    double color_data[4];
    cv_utils::scalarToData( color, color_data, img.type( ), 0 );

    Size size    = img.size( );
    size_t step  = img.step;
    int pix_size = ( int )img.elemSize( );

    uchar* ptr = img.ptr( );

    int err = 0, dx = radius, dy = 0, plus = 1, minus = ( radius << 1 ) - 1;
    int inside = center.x >= radius && center.x < size.width - radius && center.y >= radius
                 && center.y < size.height - radius;

#define ICV_PUT_POINT( ptr, x ) memcpy( ptr + ( x )*pix_size, color_data, pix_size );

    pts.clear( );

    while ( dx >= dy )
    {
        int mask;
        int y11 = center.y - dy, y12 = center.y + dy, y21 = center.y - dx, y22 = center.y + dx;
        int x11 = center.x - dx, x12 = center.x + dx, x21 = center.x - dy, x22 = center.x + dy;

        if ( inside )
        {
            uchar* tptr0 = ptr + y11 * step;
            uchar* tptr1 = ptr + y12 * step;

            ICV_PUT_POINT( tptr0, x11 );
            ICV_PUT_POINT( tptr1, x11 );
            ICV_PUT_POINT( tptr0, x12 );
            ICV_PUT_POINT( tptr1, x12 );

            pts.push_back( cv::Point2f( x11, y11 ) );
            pts.push_back( cv::Point2f( x11, y12 ) );
            pts.push_back( cv::Point2f( x12, y11 ) );
            pts.push_back( cv::Point2f( x12, y12 ) );

            tptr0 = ptr + y21 * step;
            tptr1 = ptr + y22 * step;

            ICV_PUT_POINT( tptr0, x21 );
            ICV_PUT_POINT( tptr1, x21 );
            ICV_PUT_POINT( tptr0, x22 );
            ICV_PUT_POINT( tptr1, x22 );

            pts.push_back( cv::Point2f( x21, y21 ) );
            pts.push_back( cv::Point2f( x21, y22 ) );
            pts.push_back( cv::Point2f( x22, y21 ) );
            pts.push_back( cv::Point2f( x22, y22 ) );
        }
        else if ( x11 < size.width && x12 >= 0 && y21 < size.height && y22 >= 0 )
        {

            if ( ( unsigned )y11 < ( unsigned )size.height )
            {
                uchar* tptr = ptr + y11 * step;

                if ( x11 >= 0 )
                {
                    ICV_PUT_POINT( tptr, x11 );
                    pts.push_back( cv::Point2f( x11, y11 ) );
                }
                if ( x12 < size.width )
                {
                    ICV_PUT_POINT( tptr, x12 );
                    pts.push_back( cv::Point2f( x12, y11 ) );
                }
            }

            if ( ( unsigned )y12 < ( unsigned )size.height )
            {
                uchar* tptr = ptr + y12 * step;

                if ( x11 >= 0 )
                {
                    ICV_PUT_POINT( tptr, x11 );
                    pts.push_back( cv::Point2f( x11, y12 ) );
                }
                if ( x12 < size.width )
                {
                    ICV_PUT_POINT( tptr, x12 );
                    pts.push_back( cv::Point2f( x12, y12 ) );
                }
            }

            if ( x21 < size.width && x22 >= 0 )
            {

                if ( ( unsigned )y21 < ( unsigned )size.height )
                {
                    uchar* tptr = ptr + y21 * step;

                    if ( x21 >= 0 )
                    {
                        ICV_PUT_POINT( tptr, x21 );
                        pts.push_back( cv::Point2f( x21, y21 ) );
                    }
                    if ( x22 < size.width )
                    {
                        ICV_PUT_POINT( tptr, x22 );
                        pts.push_back( cv::Point2f( x22, y21 ) );
                    }
                }

                if ( ( unsigned )y22 < ( unsigned )size.height )
                {
                    uchar* tptr = ptr + y22 * step;

                    if ( x21 >= 0 )
                    {
                        ICV_PUT_POINT( tptr, x21 );
                        pts.push_back( cv::Point2f( x21, y22 ) );
                    }
                    if ( x22 < size.width )
                    {
                        ICV_PUT_POINT( tptr, x22 );
                        pts.push_back( cv::Point2f( x22, y22 ) );
                    }
                }
            }
        }
        dy++;
        err += plus;
        plus += 2;

        mask = ( err <= 0 ) - 1;

        err -= minus & mask;
        dx += mask;
        minus -= mask & 2;
    }

#undef ICV_PUT_POINT
}

std::vector< cv::Point2f >
cv::CircleInt::getCirclePoints( cv::Size size )
{

    int err = 0, dx = radius, dy = 0, plus = 1, minus = ( radius << 1 ) - 1;
    int inside = center.x >= radius && center.x < size.width - radius && center.y >= radius
                 && center.y < size.height - radius;

    std::vector< cv::Point2f > pts;

    while ( dx >= dy )
    {
        int mask;
        int y11 = center.y - dy, y12 = center.y + dy, y21 = center.y - dx, y22 = center.y + dx;
        int x11 = center.x - dx, x12 = center.x + dx, x21 = center.x - dy, x22 = center.x + dy;

        if ( inside )
        {

            pts.push_back( cv::Point2f( x11, y11 ) );
            pts.push_back( cv::Point2f( x11, y12 ) );
            pts.push_back( cv::Point2f( x12, y11 ) );
            pts.push_back( cv::Point2f( x12, y12 ) );

            pts.push_back( cv::Point2f( x21, y21 ) );
            pts.push_back( cv::Point2f( x21, y22 ) );
            pts.push_back( cv::Point2f( x22, y21 ) );
            pts.push_back( cv::Point2f( x22, y22 ) );
        }
        else if ( x11 < size.width && x12 >= 0 && y21 < size.height && y22 >= 0 )
        {

            if ( ( unsigned )y11 < ( unsigned )size.height )
            {
                if ( x11 >= 0 )
                {
                    pts.push_back( cv::Point2f( x11, y11 ) );
                }
                if ( x12 < size.width )
                {
                    pts.push_back( cv::Point2f( x12, y11 ) );
                }
            }

            if ( ( unsigned )y12 < ( unsigned )size.height )
            {
                if ( x11 >= 0 )
                {
                    pts.push_back( cv::Point2f( x11, y12 ) );
                }
                if ( x12 < size.width )
                {
                    pts.push_back( cv::Point2f( x12, y12 ) );
                }
            }

            if ( x21 < size.width && x22 >= 0 )
            {
                if ( ( unsigned )y21 < ( unsigned )size.height )
                {
                    if ( x21 >= 0 )
                    {
                        pts.push_back( cv::Point2f( x21, y21 ) );
                    }
                    if ( x22 < size.width )
                    {
                        pts.push_back( cv::Point2f( x22, y21 ) );
                    }
                }

                if ( ( unsigned )y22 < ( unsigned )size.height )
                {
                    if ( x21 >= 0 )
                    {
                        pts.push_back( cv::Point2f( x21, y22 ) );
                    }
                    if ( x22 < size.width )
                    {
                        pts.push_back( cv::Point2f( x22, y22 ) );
                    }
                }
            }
        }
        dy++;
        err += plus;
        plus += 2;

        mask = ( err <= 0 ) - 1;

        err -= minus & mask;
        dx += mask;
        minus -= mask & 2;
    }

    return pts;
}

bool
cv::CircleInt::inside( cv::Point pt )
{
    int dx = pt.x - center.x;
    int dy = pt.y - center.y;

    int dis2 = dx * dx + dy * dy;

    return dis2 >= radius * radius ? false : true;
}

bool
cv::CircleInt::onCircle( cv::Point pt )
{
    int dx = pt.x - center.x;
    int dy = pt.y - center.y;

    int dis2 = dx * dx + dy * dy;

    return ( dis2 - radius ) * ( dis2 - radius ) < 1 ? false : true;
}

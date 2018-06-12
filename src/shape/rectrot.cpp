#include "fsrk/shape/rectrot.h"

cv::RectRot::RectRot( )
: center( Point2f( 0, 0 ) )
, size( Size2f( 0, 0 ) )
, angle( 0 )
{
}

cv::RectRot::RectRot( const cv::RectRot& rect )
: center( rect.center )
, size( rect.size )
, angle( rect.angle )
{
}

cv::RectRot::RectRot( const cv::Point2f& _center, const cv::Size2f& _size, float _angle )
: center( _center )
, size( _size )
, angle( _angle )
{
}

cv::RectRot::RectRot( const cv::Point2f& _point1, const cv::Point2f& _point2, const cv::Point2f& _point3 )
{
    Point2f _center = 0.5f * ( _point1 + _point3 );
    Vec2f vecs[2];
    vecs[0] = Vec2f( _point1 - _point2 );
    vecs[1] = Vec2f( _point2 - _point3 );
    // check that given sides are perpendicular
    CV_Assert( abs( vecs[0].dot( vecs[1] ) ) / ( norm( vecs[0] ) * norm( vecs[1] ) ) <= FLT_EPSILON );

    // wd_i stores which vector (0,1) or (1,2) will make the width
    // One of them will definitely have slope within -1 to 1
    int wd_i = 0;
    if ( abs( vecs[1][1] ) < abs( vecs[1][0] ) )
        wd_i = 1;
    int ht_i = ( wd_i + 1 ) % 2;

    float _angle  = atan( vecs[wd_i][1] / vecs[wd_i][0] ) * 180.0f / ( float )CV_PI;
    float _width  = ( float )norm( vecs[wd_i] );
    float _height = ( float )norm( vecs[ht_i] );

    center = _center;
    size   = Size2f( _width, _height );
    angle  = _angle;
}

void
cv::RectRot::points( cv::Point2f pts[] ) const
{
    double _angle = angle * CV_PI / 180.;

    float half_cos = ( float )cos( _angle ) * 0.5f;
    float half_sin = ( float )sin( _angle ) * 0.5f;

    pts[0].x = center.x - half_sin * size.height - half_cos * size.width;
    pts[0].y = center.y + half_cos * size.height - half_sin * size.width;

    pts[1].x = center.x + half_sin * size.height - half_cos * size.width;
    pts[1].y = center.y - half_cos * size.height - half_sin * size.width;

    pts[2].x = 2 * center.x - pts[0].x;
    pts[2].y = 2 * center.y - pts[0].y;

    pts[3].x = 2 * center.x - pts[1].x;
    pts[3].y = 2 * center.y - pts[1].y;
}

void
cv::RectRot::points( std::vector< cv::Point2f >& pts ) const
{
    pts.resize( 4 );

    double _angle = angle * CV_PI / 180.;

    float half_cos = ( float )cos( _angle ) * 0.5f;
    float half_sin = ( float )sin( _angle ) * 0.5f;

    pts[0].x = center.x - half_sin * size.height - half_cos * size.width;
    pts[0].y = center.y + half_cos * size.height - half_sin * size.width;

    pts[1].x = center.x + half_sin * size.height - half_cos * size.width;
    pts[1].y = center.y - half_cos * size.height - half_sin * size.width;

    pts[2].x = 2 * center.x - pts[0].x;
    pts[2].y = 2 * center.y - pts[0].y;

    pts[3].x = 2 * center.x - pts[1].x;
    pts[3].y = 2 * center.y - pts[1].y;
}

void
cv::RectRot::edgeCenterPoints( std::vector< cv::Point2f >& pts ) const
{
    pts.clear( );

    std::vector< cv::Point2f > pts_tmp;
    points( pts_tmp );

    for ( int i = 0; i < 4; i++ )
        pts.push_back( cv::Point2f( ( pts_tmp[i].x + pts_tmp[( i + 1 ) % 4].x ) / 2,
                                    ( pts_tmp[i].y + pts_tmp[( i + 1 ) % 4].y ) / 2 ) );
}

cv::Rect
cv::RectRot::boundingRect( ) const
{
    Point2f pt[4];
    points( pt );

    Rect r( cvFloor( std::min( std::min( std::min( pt[0].x, pt[1].x ), pt[2].x ), pt[3].x ) ),
            cvFloor( std::min( std::min( std::min( pt[0].y, pt[1].y ), pt[2].y ), pt[3].y ) ),
            cvCeil( std::max( std::max( std::max( pt[0].x, pt[1].x ), pt[2].x ), pt[3].x ) ),
            cvCeil( std::max( std::max( std::max( pt[0].y, pt[1].y ), pt[2].y ), pt[3].y ) ) );

    r.width -= r.x - 1;
    r.height -= r.y - 1;

    return r;
}

cv::Rect_< float >
cv::RectRot::boundingRect2f( ) const
{
    Point2f pt[4];
    points( pt );

    Rect_< float > r( Point_< float >( min( min( min( pt[0].x, pt[1].x ), pt[2].x ), pt[3].x ),
                                       min( min( min( pt[0].y, pt[1].y ), pt[2].y ), pt[3].y ) ),
                      Point_< float >( max( max( max( pt[0].x, pt[1].x ), pt[2].x ), pt[3].x ),
                                       max( max( max( pt[0].y, pt[1].y ), pt[2].y ), pt[3].y ) ) );

    return r;
}

void
cv::RectRot::draw( cv::Mat& img, const cv::Scalar& color )
{
    Point2f pt[4];
    points( pt );

    for ( int i = 0; i < 4; i++ )
        line( img, pt[i], pt[( i + 1 ) % 4], color );

    Rect bRect = boundingRect( );
    rectangle( img, bRect, color );
}

#include "ellipse.h"
#include "cv_utils.h"
#include <eigen3/Eigen/Eigen>

static const float SinTable[]
= { 0.0000000f,  0.0174524f,  0.0348995f,  0.0523360f,  0.0697565f,  0.0871557f,
    0.1045285f,  0.1218693f,  0.1391731f,  0.1564345f,  0.1736482f,  0.1908090f,
    0.2079117f,  0.2249511f,  0.2419219f,  0.2588190f,  0.2756374f,  0.2923717f,
    0.3090170f,  0.3255682f,  0.3420201f,  0.3583679f,  0.3746066f,  0.3907311f,
    0.4067366f,  0.4226183f,  0.4383711f,  0.4539905f,  0.4694716f,  0.4848096f,
    0.5000000f,  0.5150381f,  0.5299193f,  0.5446390f,  0.5591929f,  0.5735764f,
    0.5877853f,  0.6018150f,  0.6156615f,  0.6293204f,  0.6427876f,  0.6560590f,
    0.6691306f,  0.6819984f,  0.6946584f,  0.7071068f,  0.7193398f,  0.7313537f,
    0.7431448f,  0.7547096f,  0.7660444f,  0.7771460f,  0.7880108f,  0.7986355f,
    0.8090170f,  0.8191520f,  0.8290376f,  0.8386706f,  0.8480481f,  0.8571673f,
    0.8660254f,  0.8746197f,  0.8829476f,  0.8910065f,  0.8987940f,  0.9063078f,
    0.9135455f,  0.9205049f,  0.9271839f,  0.9335804f,  0.9396926f,  0.9455186f,
    0.9510565f,  0.9563048f,  0.9612617f,  0.9659258f,  0.9702957f,  0.9743701f,
    0.9781476f,  0.9816272f,  0.9848078f,  0.9876883f,  0.9902681f,  0.9925462f,
    0.9945219f,  0.9961947f,  0.9975641f,  0.9986295f,  0.9993908f,  0.9998477f,
    1.0000000f,  0.9998477f,  0.9993908f,  0.9986295f,  0.9975641f,  0.9961947f,
    0.9945219f,  0.9925462f,  0.9902681f,  0.9876883f,  0.9848078f,  0.9816272f,
    0.9781476f,  0.9743701f,  0.9702957f,  0.9659258f,  0.9612617f,  0.9563048f,
    0.9510565f,  0.9455186f,  0.9396926f,  0.9335804f,  0.9271839f,  0.9205049f,
    0.9135455f,  0.9063078f,  0.8987940f,  0.8910065f,  0.8829476f,  0.8746197f,
    0.8660254f,  0.8571673f,  0.8480481f,  0.8386706f,  0.8290376f,  0.8191520f,
    0.8090170f,  0.7986355f,  0.7880108f,  0.7771460f,  0.7660444f,  0.7547096f,
    0.7431448f,  0.7313537f,  0.7193398f,  0.7071068f,  0.6946584f,  0.6819984f,
    0.6691306f,  0.6560590f,  0.6427876f,  0.6293204f,  0.6156615f,  0.6018150f,
    0.5877853f,  0.5735764f,  0.5591929f,  0.5446390f,  0.5299193f,  0.5150381f,
    0.5000000f,  0.4848096f,  0.4694716f,  0.4539905f,  0.4383711f,  0.4226183f,
    0.4067366f,  0.3907311f,  0.3746066f,  0.3583679f,  0.3420201f,  0.3255682f,
    0.3090170f,  0.2923717f,  0.2756374f,  0.2588190f,  0.2419219f,  0.2249511f,
    0.2079117f,  0.1908090f,  0.1736482f,  0.1564345f,  0.1391731f,  0.1218693f,
    0.1045285f,  0.0871557f,  0.0697565f,  0.0523360f,  0.0348995f,  0.0174524f,
    0.0000000f,  -0.0174524f, -0.0348995f, -0.0523360f, -0.0697565f, -0.0871557f,
    -0.1045285f, -0.1218693f, -0.1391731f, -0.1564345f, -0.1736482f, -0.1908090f,
    -0.2079117f, -0.2249511f, -0.2419219f, -0.2588190f, -0.2756374f, -0.2923717f,
    -0.3090170f, -0.3255682f, -0.3420201f, -0.3583679f, -0.3746066f, -0.3907311f,
    -0.4067366f, -0.4226183f, -0.4383711f, -0.4539905f, -0.4694716f, -0.4848096f,
    -0.5000000f, -0.5150381f, -0.5299193f, -0.5446390f, -0.5591929f, -0.5735764f,
    -0.5877853f, -0.6018150f, -0.6156615f, -0.6293204f, -0.6427876f, -0.6560590f,
    -0.6691306f, -0.6819984f, -0.6946584f, -0.7071068f, -0.7193398f, -0.7313537f,
    -0.7431448f, -0.7547096f, -0.7660444f, -0.7771460f, -0.7880108f, -0.7986355f,
    -0.8090170f, -0.8191520f, -0.8290376f, -0.8386706f, -0.8480481f, -0.8571673f,
    -0.8660254f, -0.8746197f, -0.8829476f, -0.8910065f, -0.8987940f, -0.9063078f,
    -0.9135455f, -0.9205049f, -0.9271839f, -0.9335804f, -0.9396926f, -0.9455186f,
    -0.9510565f, -0.9563048f, -0.9612617f, -0.9659258f, -0.9702957f, -0.9743701f,
    -0.9781476f, -0.9816272f, -0.9848078f, -0.9876883f, -0.9902681f, -0.9925462f,
    -0.9945219f, -0.9961947f, -0.9975641f, -0.9986295f, -0.9993908f, -0.9998477f,
    -1.0000000f, -0.9998477f, -0.9993908f, -0.9986295f, -0.9975641f, -0.9961947f,
    -0.9945219f, -0.9925462f, -0.9902681f, -0.9876883f, -0.9848078f, -0.9816272f,
    -0.9781476f, -0.9743701f, -0.9702957f, -0.9659258f, -0.9612617f, -0.9563048f,
    -0.9510565f, -0.9455186f, -0.9396926f, -0.9335804f, -0.9271839f, -0.9205049f,
    -0.9135455f, -0.9063078f, -0.8987940f, -0.8910065f, -0.8829476f, -0.8746197f,
    -0.8660254f, -0.8571673f, -0.8480481f, -0.8386706f, -0.8290376f, -0.8191520f,
    -0.8090170f, -0.7986355f, -0.7880108f, -0.7771460f, -0.7660444f, -0.7547096f,
    -0.7431448f, -0.7313537f, -0.7193398f, -0.7071068f, -0.6946584f, -0.6819984f,
    -0.6691306f, -0.6560590f, -0.6427876f, -0.6293204f, -0.6156615f, -0.6018150f,
    -0.5877853f, -0.5735764f, -0.5591929f, -0.5446390f, -0.5299193f, -0.5150381f,
    -0.5000000f, -0.4848096f, -0.4694716f, -0.4539905f, -0.4383711f, -0.4226183f,
    -0.4067366f, -0.3907311f, -0.3746066f, -0.3583679f, -0.3420201f, -0.3255682f,
    -0.3090170f, -0.2923717f, -0.2756374f, -0.2588190f, -0.2419219f, -0.2249511f,
    -0.2079117f, -0.1908090f, -0.1736482f, -0.1564345f, -0.1391731f, -0.1218693f,
    -0.1045285f, -0.0871557f, -0.0697565f, -0.0523360f, -0.0348995f, -0.0174524f,
    -0.0000000f, 0.0174524f,  0.0348995f,  0.0523360f,  0.0697565f,  0.0871557f,
    0.1045285f,  0.1218693f,  0.1391731f,  0.1564345f,  0.1736482f,  0.1908090f,
    0.2079117f,  0.2249511f,  0.2419219f,  0.2588190f,  0.2756374f,  0.2923717f,
    0.3090170f,  0.3255682f,  0.3420201f,  0.3583679f,  0.3746066f,  0.3907311f,
    0.4067366f,  0.4226183f,  0.4383711f,  0.4539905f,  0.4694716f,  0.4848096f,
    0.5000000f,  0.5150381f,  0.5299193f,  0.5446390f,  0.5591929f,  0.5735764f,
    0.5877853f,  0.6018150f,  0.6156615f,  0.6293204f,  0.6427876f,  0.6560590f,
    0.6691306f,  0.6819984f,  0.6946584f,  0.7071068f,  0.7193398f,  0.7313537f,
    0.7431448f,  0.7547096f,  0.7660444f,  0.7771460f,  0.7880108f,  0.7986355f,
    0.8090170f,  0.8191520f,  0.8290376f,  0.8386706f,  0.8480481f,  0.8571673f,
    0.8660254f,  0.8746197f,  0.8829476f,  0.8910065f,  0.8987940f,  0.9063078f,
    0.9135455f,  0.9205049f,  0.9271839f,  0.9335804f,  0.9396926f,  0.9455186f,
    0.9510565f,  0.9563048f,  0.9612617f,  0.9659258f,  0.9702957f,  0.9743701f,
    0.9781476f,  0.9816272f,  0.9848078f,  0.9876883f,  0.9902681f,  0.9925462f,
    0.9945219f,  0.9961947f,  0.9975641f,  0.9986295f,  0.9993908f,  0.9998477f,
    1.0000000f };

static void
sincosTable( int angle, float& cosval, float& sinval )
{
    angle += ( angle < 0 ? 360 : 0 );
    sinval = SinTable[angle];       // sin theta
    cosval = SinTable[450 - angle]; // cos theta
}

static void
sincos( int angle, float& cosval, float& sinval )
{
    double _angle = angle * 0.0174532925; // degree to rad

    // with Intel x86 fsincos
    sinval = sin( _angle );
    cosval = cos( _angle );
}

cv::Ellipse::Ellipse( const cv::Ellipse& ell_in )
: box( ell_in.box )
{
    axisHalfLong  = std::max( box.size.width, box.size.height ) / 2;
    axisHalfShort = std::min( box.size.width, box.size.height ) / 2;

    focalLengthHalf = std::sqrt( axisHalfLong * axisHalfLong - axisHalfShort * axisHalfShort );

    int _angle;
    if ( box.size.width > box.size.height )
        _angle = 180 - box.angle;
    else
        _angle = 90 - box.angle;

    float cosval, sinval;
    sincos( _angle, cosval, sinval );

    focalPoint0 = cv::Point2f( box.center.x + focalLengthHalf * cosval,
                               box.center.y - focalLengthHalf * sinval );
    focalPoint1 = cv::Point2f( box.center.x - focalLengthHalf * cosval,
                               box.center.y + focalLengthHalf * sinval );
}

cv::Ellipse::Ellipse( const cv::RectRot& _box )
: box( _box )
{
    axisHalfLong  = std::max( box.size.width, box.size.height ) / 2;
    axisHalfShort = std::min( box.size.width, box.size.height ) / 2;

    focalLengthHalf = std::sqrt( axisHalfLong * axisHalfLong - axisHalfShort * axisHalfShort );

    int _angle;
    if ( box.size.width > box.size.height )
        _angle = 180 - box.angle;
    else
        _angle = 90 - box.angle;

    float cosval, sinval;
    sincos( _angle, cosval, sinval );

    focalPoint0 = cv::Point2f( box.center.x + focalLengthHalf * cosval,
                               box.center.y - focalLengthHalf * sinval );
    focalPoint1 = cv::Point2f( box.center.x - focalLengthHalf * cosval,
                               box.center.y + focalLengthHalf * sinval );
}

cv::Ellipse::Ellipse( const cv::Point2f& center, const cv::Size2f& size, float angle )
: box( center, size, angle )
{
    axisHalfLong  = std::max( box.size.width, box.size.height ) / 2;
    axisHalfShort = std::min( box.size.width, box.size.height ) / 2;

    focalLengthHalf = std::sqrt( axisHalfLong * axisHalfLong - axisHalfShort * axisHalfShort );

    int _angle;
    if ( box.size.width > box.size.height )
        _angle = 180 - box.angle;
    else
        _angle = 90 - box.angle;

    float cosval, sinval;
    sincos( _angle, cosval, sinval );

    focalPoint0 = cv::Point2f( box.center.x + focalLengthHalf * cosval,
                               box.center.y - focalLengthHalf * sinval );
    focalPoint1 = cv::Point2f( box.center.x - focalLengthHalf * cosval,
                               box.center.y + focalLengthHalf * sinval );
}

float
cv::Ellipse::dis2( cv::Point2f pt0, cv::Point2f pt1 )
{
    cv::Point2f dpt = pt0 - pt1;
    return std::sqrt( dpt.x * dpt.x + dpt.y * dpt.y );
}

bool
cv::Ellipse::inside( cv::Point2f pt )
{
    float norm0 = dis2( pt, focalPoint0 );
    float norm1 = dis2( pt, focalPoint1 );

    return ( norm0 + norm1 ) > ( 2 * axisHalfLong ) ? false : true;
}

bool
cv::Ellipse::onEllipse( cv::Point2f pt )
{
    float norm0 = dis2( pt, focalPoint0 );
    float norm1 = dis2( pt, focalPoint1 );

    if ( ( axisHalfLong - focalLengthHalf ) > 2 )
        return std::abs( ( norm0 + norm1 ) - ( 2 * axisHalfLong ) ) < 1 ? true : false;
    else
        return std::abs( ( norm0 + norm1 ) - ( 2 * axisHalfLong ) ) < 0.3 ? true : false;
}

void
cv::Ellipse::fit( cv::InputArray _points )
{
    Mat points = _points.getMat( );
    int i, n = points.checkVector( 2 );
    int depth = points.depth( );
    CV_Assert( n >= 0 && ( depth == CV_32F || depth == CV_32S ) );

    if ( n < 5 )
        CV_Error( CV_StsBadSize, "There should be at least 5 points to fit the ellipse" );

    // New fitellipse algorithm, contributed by Dr. Daniel Weiss
    Point2f c( 0, 0 );
    double gfp[5] = { 0 }, rp[5] = { 0 }, t;
    const double min_eps = 1e-8;
    bool is_float        = depth == CV_32F;
    const Point* ptsi    = points.ptr< Point >( );
    const Point2f* ptsf  = points.ptr< Point2f >( );

    AutoBuffer< double > _Ad( n * 5 ), _bd( n );
    double *Ad = _Ad, *bd = _bd;

    // first fit for parameters A - E
    Mat A( n, 5, CV_64F, Ad );
    Mat b( n, 1, CV_64F, bd );
    Mat x( 5, 1, CV_64F, gfp );

    for ( i = 0; i < n; i++ )
    {
        Point2f p = is_float ? ptsf[i] : Point2f( ( float )ptsi[i].x, ( float )ptsi[i].y );
        c += p;
    }
    c.x /= n;
    c.y /= n;

    for ( i = 0; i < n; i++ )
    {
        Point2f p = is_float ? ptsf[i] : Point2f( ( float )ptsi[i].x, ( float )ptsi[i].y );
        p -= c;

        bd[i]         = 10000.0;              // 1.0?
        Ad[i * 5]     = -( double )p.x * p.x; // A - C signs inverted as proposed by APP
        Ad[i * 5 + 1] = -( double )p.y * p.y;
        Ad[i * 5 + 2] = -( double )p.x * p.y;
        Ad[i * 5 + 3] = p.x;
        Ad[i * 5 + 4] = p.y;
    }

    solve( A, b, x, DECOMP_SVD );

    // now use general-form parameters A - E to find the ellipse center:
    // differentiate general form wrt x/y to get two equations for cx and cy
    A = Mat( 2, 2, CV_64F, Ad );
    b = Mat( 2, 1, CV_64F, bd );
    x = Mat( 2, 1, CV_64F, rp );

    Ad[0] = 2 * gfp[0];
    Ad[1] = Ad[2] = gfp[2];

    Ad[3] = 2 * gfp[1];
    bd[0] = gfp[3];
    bd[1] = gfp[4];

    solve( A, b, x, DECOMP_SVD );

    // re-fit for parameters A - C with those center coordinates
    A = Mat( n, 3, CV_64F, Ad );
    b = Mat( n, 1, CV_64F, bd );
    x = Mat( 3, 1, CV_64F, gfp );
    for ( i = 0; i < n; i++ )
    {
        Point2f p = is_float ? ptsf[i] : Point2f( ( float )ptsi[i].x, ( float )ptsi[i].y );
        p -= c;
        bd[i]         = 1.0;
        Ad[i * 3]     = ( p.x - rp[0] ) * ( p.x - rp[0] );
        Ad[i * 3 + 1] = ( p.y - rp[1] ) * ( p.y - rp[1] );
        Ad[i * 3 + 2] = ( p.x - rp[0] ) * ( p.y - rp[1] );
    }
    solve( A, b, x, DECOMP_SVD );

    // store angle and radii
    rp[4] = -0.5 * atan2( gfp[2], gfp[1] - gfp[0] ); // convert from APP angle usage

    if ( fabs( gfp[2] ) > min_eps )
        t = gfp[2] / sin( -2.0 * rp[4] );
    else
    {
        // ellipse is rotated by an integer multiple of pi/2
        t = gfp[1] - gfp[0];
    }

    rp[2] = fabs( gfp[0] + gfp[1] - t );

    if ( rp[2] > min_eps )
        rp[2] = std::sqrt( 2.0 / rp[2] );

    rp[3] = fabs( gfp[0] + gfp[1] + t );

    if ( rp[3] > min_eps )
        rp[3] = std::sqrt( 2.0 / rp[3] );

    box.center.x    = ( float )rp[0] + c.x;
    box.center.y    = ( float )rp[1] + c.y;
    box.size.width  = ( float )( rp[2] * 2 );
    box.size.height = ( float )( rp[3] * 2 );
    if ( box.size.width > box.size.height )
    {
        float tmp;
        CV_SWAP( box.size.width, box.size.height, tmp );
        box.angle = ( float )( 90 + rp[4] * 180 / CV_PI );
    }
    if ( box.angle < -180 )
        box.angle += 360;
    if ( box.angle > 360 )
        box.angle -= 360;

    axisHalfLong  = std::max( box.size.width, box.size.height ) / 2;
    axisHalfShort = std::min( box.size.width, box.size.height ) / 2;
    focalLengthHalf = std::sqrt( axisHalfLong * axisHalfLong - axisHalfShort * axisHalfShort );

    double _angle;
    if ( box.size.width > box.size.height )
        _angle = CV_PI - box.angle * CV_PI / 180.;
    else
        _angle = CV_PI / 2 - box.angle * CV_PI / 180.;

    focalPoint0 = cv::Point2f( box.center.x + focalLengthHalf * ( float )cos( _angle ),
                               box.center.y - focalLengthHalf * ( float )sin( _angle ) );
    focalPoint1 = cv::Point2f( box.center.x - focalLengthHalf * ( float )cos( _angle ),
                               box.center.y + focalLengthHalf * ( float )sin( _angle ) );
}

void
cv::Ellipse::draw( InputOutputArray _img, const cv::Scalar& color )
{
    Mat img = _img.getMat( );

    CV_Assert( box.size.width >= 0 && box.size.height >= 0 );

    double buf[4];
    cv_utils::scalarToData( color, buf, img.type( ), 0 );

    int delta = int( 57.29 / std::max( box.size.width / 2, box.size.height / 2 ) );
    delta     = delta < 2 ? 2 : delta;

    std::vector< Point > _v;
    ellipse2Poly( Point( ( int )box.center.x, ( int )box.center.y ),
                  Size( ( int )( box.size.width / 2 ), ( int )( box.size.height / 2 ) ),
                  box.angle,
                  0,
                  360,
                  delta,
                  _v );

    for ( size_t index = 0; index < _v.size( ) - 1; ++index )
    {
        LineIterator iterator( img, _v[index], _v[index + 1], 8, true );
        const uchar* _color = ( const uchar* )buf;
        int i, count = iterator.count;
        int pix_size = ( int )img.elemSize( );

        for ( i = 0; i < count; i++, ++iterator )
        {
            uchar* ptr = *iterator;
            if ( pix_size == 1 )
                ptr[0] = _color[0];
            else if ( pix_size == 3 )
            {
                ptr[0] = _color[0];
                ptr[1] = _color[1];
                ptr[2] = _color[2];
            }
            else
                memcpy( *iterator, _color, pix_size );
        }
    }
}

cv::RectRot
cv::Ellipse::getBox( ) const
{
    return box;
}

cv::RotatedRect
cv::Ellipse::getBox2( ) const
{
    return cv::RotatedRect( box.center, box.size, box.angle );
}

cv::Point2f
cv::Ellipse::getFocalPt1( ) const
{
    return focalPoint1;
}

cv::Point2f
cv::Ellipse::getFocalPt0( ) const
{
    return focalPoint0;
}

#include "shape/circle.h"
#include "shape/ellipse.h"
#include <camera_model/camera_models/CameraFactory.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define WINDOW_NAME "image"
#define WINDOW_NAME2 "image2"

camera_model::CameraPtr cam;
Mat g_srcImage;
Mat mask;
int color_rand1, color_rand2, color_rand3;
cv::Point2f image_center;

struct PatternEllipse
{
    EllipsePtr pPatt;
};
std::vector< std::vector< PatternEllipse > > patternTable;

float
disOfPoints( cv::Point2f pt0, cv::Point2f pt1 )
{
    return std::sqrt( ( pt0.x - pt1.x ) * ( pt0.x - pt1.x )
                      + ( pt0.y - pt1.y ) * ( pt0.y - pt1.y ) );
}
float
sinAngOfPoints( cv::Point2f pt0, cv::Point2f pt1, float dis )
{
    return ( pt1.y - pt0.y ) / dis;
}
float
cosAngOfPoints( cv::Point2f pt0, cv::Point2f pt1, float dis )
{
    return ( pt1.x - pt0.x ) / dis;
}

void
On_mouse( int event, int x, int y, int flags, void* );

void
drawImageCalc( Mat image_color, Point pt, float angle );

void
drawImageTable( cv::Mat image_color, cv::Point pt, float angle );

void
buildTable( camera_model::CameraPtr cam );

Eigen::Vector2d
calcAngle( const camera_model::CameraPtr _cam, const Eigen::Vector2d pu )
{
    Eigen::Vector3d P;
    _cam->liftSphere( pu, P );

    double theta = acos( P( 2 ) / P.norm( ) );
    double phi   = atan2( P( 1 ), P( 0 ) );

    Eigen::Vector2d angle( theta, phi );

    return angle;
}

int
main( )
{
    std::string cam_file  = "/home/gao/ws/src/vins/config/dual/cam_down_camera_calib.yaml";
    std::string img_file  = "/home/gao/ws/devel/lib/camera_model/image_down/IMG_35.png";
    std::string file_mask = "/home/gao/ws/src/vins/config/dual/mask_down.jpg";

    cam = camera_model::CameraFactory::instance( )->generateCameraFromYamlFile( cam_file );
    image_center = cam->getPrinciple( );

    std::cout << image_center << "\n";

    g_srcImage = cv::imread( img_file );
    mask       = cv::imread( file_mask, IMREAD_GRAYSCALE );

    buildTable( cam );

    namedWindow( WINDOW_NAME, WINDOW_NORMAL );
    namedWindow( WINDOW_NAME2, WINDOW_NORMAL );
    imshow( WINDOW_NAME, g_srcImage );
    imshow( WINDOW_NAME2, g_srcImage );
    setMouseCallback( WINDOW_NAME, On_mouse, 0 );
    waitKey( 0 );
    return 0;
}

void
On_mouse( int event, int x, int y, int flags, void* )
{
    Point pt;

    if ( event == EVENT_LBUTTONDOWN )
    {
        pt = Point( x, y );
    }
    else if ( event == EVENT_MOUSEMOVE && ( flags & EVENT_FLAG_LBUTTON ) )
    {
        pt = Point( x, y );
    }
    else
        return;

    if ( mask.at< uchar >( pt ) < 20 )
    {
        std::cout << "Error pt " << pt << "\n";
        return;
    }
    std::cout << "pt " << pt << "\n";

    cv::Mat image_color;
    cv::Mat image_color2;
    g_srcImage.copyTo( image_color );
    g_srcImage.copyTo( image_color2 );
    color_rand1 = rand( ) % 256;
    color_rand2 = rand( ) % 256;
    color_rand3 = rand( ) % 256;

    float angle = 30;

    drawImageCalc( image_color, pt, angle );
    drawImageTable( image_color2, pt, angle );

    cv::imshow( WINDOW_NAME, image_color );
    cv::imshow( WINDOW_NAME2, image_color2 );
}

#define FREAK_NB_ORIENTATION 36
#define FREAK_NB_POINTS 43

void
drawImageTable( cv::Mat image_color, cv::Point pt, float angle )
{
    float dist     = disOfPoints( image_center, pt );
    float sinTheta = sinAngOfPoints( image_center, pt, dist );
    float cosTheta = cosAngOfPoints( image_center, pt, dist );
    int index      = int( dist );
    //    float theta    = acos( cosTheta ) * 360. / CV_PI;
    float theta = atan2( ( float )( pt.y - image_center.y ), //
                         ( float )( pt.x - image_center.x ) )
                  * ( 180.0 / CV_PI );

    // std::cout << theta << " " << asin( sinTheta ) * 180. / CV_PI << " "
    //           << acos( cosTheta ) * 180. / CV_PI << "\n";

    Eigen::Matrix2f R;
    R << cosTheta, -sinTheta, sinTheta, cosTheta;

    int thetaIdx;

    if ( theta < 0.f )
        thetaIdx = int( FREAK_NB_ORIENTATION * ( -theta + angle ) * ( 1 / 360.0 ) - 0.5 );
    else
        thetaIdx = int( FREAK_NB_ORIENTATION * ( -theta + angle ) * ( 1 / 360.0 ) + 0.5 );

    if ( thetaIdx < 0 )
        thetaIdx += FREAK_NB_ORIENTATION;

    if ( thetaIdx >= FREAK_NB_ORIENTATION )
        thetaIdx -= FREAK_NB_ORIENTATION;

    std::cout << "thetaIdx " << thetaIdx << "\n";

    for ( int i = FREAK_NB_POINTS; i--; )
    {
        const PatternEllipse& FreakEllip = patternTable[index][thetaIdx * FREAK_NB_POINTS + i]; // FIXME

        Eigen::Vector2f pt_v( FreakEllip.pPatt->box.center.x, FreakEllip.pPatt->box.center.y );
        Eigen::Vector2f pt_2 = R * pt_v;

        const float xf = pt_2( 0 ) + pt.x;
        const float yf = pt_2( 1 ) + pt.y;

        Ellipse ell( cv::Point2f( xf, yf ),
                     FreakEllip.pPatt->box.size,
                     FreakEllip.pPatt->box.angle + theta );

        if ( i == 0 )
            ell.draw( image_color, cv::Scalar( 0, 0, 255 ) );
        else if ( i == 1 )
            ell.draw( image_color, cv::Scalar( 0, 255, 0 ) );
        else if ( i == 2 )
            ell.draw( image_color, cv::Scalar( 255, 0, 0 ) );
        else
            ell.draw( image_color, cv::Scalar( color_rand1, color_rand2, color_rand3 ) );
    }
}

void
drawImageCalc( cv::Mat image_color, cv::Point pt, float angle )
{
    double alpha, beta, theta = angle * CV_PI / 180.;

    const int n[8] = { 6, 6, 6, 6, 6, 6, 6, 1 };
    const double bigR( 2.0 / 3.0 );    // bigger radius
    const double smallR( 2.0 / 24.0 ); // smaller radius
    // define spaces between concentric circles (from center to outer: 1,2,3,4,5,6)
    const double unitSpace( ( bigR - smallR ) / 21.0 );
    // radii of the concentric cirles (from outer to inner)
    const double radius[8] = { bigR,                  // 6
                               bigR - 6 * unitSpace,  // 6
                               bigR - 11 * unitSpace, // 6
                               bigR - 15 * unitSpace, // 6
                               bigR - 18 * unitSpace, // 6
                               bigR - 20 * unitSpace, // 6
                               smallR,                // 6
                               0.0 };                 // 1
    // sigma of pattern points (each group of 6 points on a concentric cirle
    // has the same sigma)
    const double sigma[8] = { radius[0] / 2.0, //
                              radius[1] / 2.0, //
                              radius[2] / 2.0, //
                              radius[3] / 2.0, //
                              radius[4] / 2.0, //
                              radius[5] / 2.0, //
                              radius[6] / 2.0, //
                              radius[6] / 2.0 };
    double patternScale = 36; // scaling of the pattern

    for ( size_t i = 0; i < 8; ++i )
    {
        // 6 circles per group
        for ( int k = 0; k < n[i]; ++k )
        {
            // orientation offset so that groups of points
            // on each circles are staggered
            beta  = CV_PI / n[i] * ( i % 2 );
            alpha = double( k ) * 2 * CV_PI / double( n[i] ) + beta + theta;

            float center_x = static_cast< float >( radius[i] * cos( alpha ) * patternScale );
            float center_y = static_cast< float >( radius[i] * sin( alpha ) * patternScale );

            float center_c_x = center_x + image_center.x;
            float center_c_y = center_y + image_center.y;
            float radius_c   = static_cast< float >( sigma[i] * patternScale );

            cv::CircleInt cirle( Point( center_c_x, center_c_y ), int( radius_c ) );
            std::vector< cv::Point2f > pts_circle = cirle.getCirclePoints( cam->imageSize( ) );

            // src pt
            Eigen::Vector2d p_u00 = Eigen::Vector2d( image_center.x, image_center.y );
            Eigen::Vector3d P_center;
            cam->liftSphere( p_u00, P_center );
            P_center.normalize( );

            // dst pt
            Eigen::Vector2d p_u( pt.x, pt.y );
            Eigen::Vector2d angle_center = calcAngle( cam, p_u );
            Eigen::Vector3d P_center2( cos( angle_center( 1 ) ) * sin( angle_center( 0 ) ),
                                       sin( angle_center( 1 ) ) * sin( angle_center( 0 ) ),
                                       cos( angle_center( 0 ) ) );
            P_center2.normalize( );

            // delta vector
            double delta_angle      = acos( P_center.dot( P_center2 ) );
            Eigen::Vector3d dVector = P_center.cross( P_center2 );
            dVector.normalize( );

            // rotation vector
            Eigen::Quaterniond q_v1v0( cos( 0.5 * delta_angle ),
                                       sin( 0.5 * delta_angle ) * dVector( 0 ),
                                       sin( 0.5 * delta_angle ) * dVector( 1 ),
                                       sin( 0.5 * delta_angle ) * dVector( 2 ) );

            std::vector< cv::Point2f > pt_ellipse;
            for ( auto& pt : pts_circle )
            {
                Eigen::Vector2d p_src2( pt.x, pt.y );
                Eigen::Vector3d P_src2;
                cam->liftSphere( p_src2, P_src2 );

                Eigen::Vector3d offset_P2 = q_v1v0 * P_src2;

                Eigen::Vector2d offset_p2;
                cam->spaceToPlane( offset_P2, offset_p2 );
                // drawPoint2Yellow( image_color, offset_p2 );

                pt_ellipse.push_back( cv::Point2f( offset_p2( 0 ), offset_p2( 1 ) ) );
            }

            Ellipse ellip_fit;
            ellip_fit.fit( pt_ellipse );

            EllipsePtr ellip( new Ellipse( ellip_fit.box.center - cv::Point2f( pt.x, pt.y ),
                                           ellip_fit.box.size,
                                           ellip_fit.box.angle ) );

            if ( i == 0 && k == 0 )
                ellip_fit.draw( image_color, cv::Scalar( 0, 0, 255 ) );
            else if ( i == 0 && k == 1 )
                ellip_fit.draw( image_color, cv::Scalar( 0, 255, 0 ) );
            else if ( i == 0 && k == 2 )
                ellip_fit.draw( image_color, cv::Scalar( 255, 0, 0 ) );
            else
                ellip_fit.draw( image_color, cv::Scalar( color_rand1, color_rand2, color_rand3 ) );
        }
    }
}

void
buildTable( camera_model::CameraPtr cam )
{
    int img_width          = cam->imageWidth( );
    int img_height         = cam->imageHeight( );
    cv::Point2f center_cam = cam->getPrinciple( );

    std::vector< float > pixels_size;
    pixels_size.push_back( std::sqrt( ( center_cam.x - 0 ) * ( center_cam.x - 0 )
                                      + ( center_cam.y - 0 ) * ( center_cam.y - 0 ) ) );
    pixels_size.push_back( std::sqrt( ( center_cam.x - img_width ) * ( center_cam.x - img_width )
                                      + ( center_cam.y - 0 ) * ( center_cam.y - 0 ) ) );
    pixels_size.push_back( std::sqrt( ( center_cam.x - img_width ) * ( center_cam.x - img_width )
                                      + ( center_cam.y - img_height ) * ( center_cam.y - img_height ) ) );
    pixels_size.push_back( std::sqrt( ( center_cam.x - 0 ) * ( center_cam.x - 0 )
                                      + ( center_cam.y - img_height ) * ( center_cam.y - img_height ) ) );

    int pixel_size = std::max( std::max( pixels_size[0], pixels_size[1] ),
                               std::max( pixels_size[2], pixels_size[3] ) );
    std::cout << "pixel_size " << pixel_size << "\n";

    patternTable.resize( pixel_size );
    for ( int index = 0; index < pixel_size; ++index )
    {
        patternTable[index].resize( FREAK_NB_ORIENTATION * FREAK_NB_POINTS );
    }

    // number of points on each concentric  circle (from outer to inner)
    const int n[8] = { 6, 6, 6, 6, 6, 6, 6, 1 };

    const double bigR( 2.0 / 3.0 );    // bigger radius
    const double smallR( 2.0 / 24.0 ); // smaller radius

    // define spaces between concentric circles (from center to outer: 1,2,3,4,5,6)
    const double unitSpace( ( bigR - smallR ) / 21.0 );

    // radii of the concentric cirles (from outer to inner)
    const double radius[8] = { bigR,                  // 6
                               bigR - 6 * unitSpace,  // 6
                               bigR - 11 * unitSpace, // 6
                               bigR - 15 * unitSpace, // 6
                               bigR - 18 * unitSpace, // 6
                               bigR - 20 * unitSpace, // 6
                               smallR,                // 6
                               0.0 };                 // 1

    // sigma of pattern points (each group of 6 points on a concentric cirle
    // has the same sigma)
    const double sigma[8] = { radius[0] / 2.0, //
                              radius[1] / 2.0, //
                              radius[2] / 2.0, //
                              radius[3] / 2.0, //
                              radius[4] / 2.0, //
                              radius[5] / 2.0, //
                              radius[6] / 2.0, //
                              radius[6] / 2.0 };

    double alpha, beta, theta = 0;
    double patternScale = 36;

    for ( int index = 0; index < pixel_size; ++index )
    {
        for ( int orientationIdx = 0; orientationIdx < FREAK_NB_ORIENTATION; ++orientationIdx )
        {
            int pointIdx = 0;

            PatternEllipse* patternLookupPtr = &patternTable[index][0];

            theta = double( orientationIdx ) * 2 * CV_PI / double( FREAK_NB_ORIENTATION );

            for ( size_t i = 0; i < 8; ++i )
            {
                // 6 circles per group
                for ( int k = 0; k < n[i]; ++k )
                {
                    // orientation offset so that groups of points
                    // on each circles are staggered
                    beta  = CV_PI / n[i] * ( i % 2 );
                    alpha = double( k ) * 2 * CV_PI / double( n[i] ) + beta + theta;

                    // add the point to the look-up table
                    PatternEllipse& ell = patternLookupPtr[orientationIdx * FREAK_NB_POINTS + pointIdx];

                    float center_x = static_cast< float >( radius[i] * cos( alpha ) * patternScale );
                    float center_y = static_cast< float >( radius[i] * sin( alpha ) * patternScale );

                    float center_c_x = center_x + image_center.x;
                    float center_c_y = center_y + image_center.y;
                    float radius_c   = static_cast< float >( sigma[i] * patternScale );

                    cv::CircleInt cirle( Point( center_c_x, center_c_y ), int( radius_c ) );
                    std::vector< cv::Point2f > pts_circle
                    = cirle.getCirclePoints( cam->imageSize( ) );

                    // src pt
                    Eigen::Vector2d p_u00 = Eigen::Vector2d( image_center.x, image_center.y );
                    Eigen::Vector3d P_center;
                    cam->liftSphere( p_u00, P_center );
                    P_center.normalize( );

                    // dst pt
                    Eigen::Vector2d p_u( image_center.x + index, image_center.y );
                    Eigen::Vector2d angle_center = calcAngle( cam, p_u );
                    Eigen::Vector3d P_center2( cos( angle_center( 1 ) ) * sin( angle_center( 0 ) ),
                                               sin( angle_center( 1 ) ) * sin( angle_center( 0 ) ),
                                               cos( angle_center( 0 ) ) );
                    P_center2.normalize( );

                    // delta vector
                    double delta_angle      = acos( P_center.dot( P_center2 ) );
                    Eigen::Vector3d dVector = P_center.cross( P_center2 );
                    dVector.normalize( );

                    // rotation vector
                    Eigen::Quaterniond q_v1v0( cos( 0.5 * delta_angle ),
                                               sin( 0.5 * delta_angle ) * dVector( 0 ),
                                               sin( 0.5 * delta_angle ) * dVector( 1 ),
                                               sin( 0.5 * delta_angle ) * dVector( 2 ) );

                    std::vector< cv::Point2f > pt_ellipse;
                    for ( auto& pt : pts_circle )
                    {
                        Eigen::Vector2d p_src2( pt.x, pt.y );
                        Eigen::Vector3d P_src2;
                        cam->liftSphere( p_src2, P_src2 );

                        Eigen::Vector3d offset_P2 = q_v1v0 * P_src2;

                        Eigen::Vector2d offset_p2;
                        cam->spaceToPlane( offset_P2, offset_p2 );

                        pt_ellipse.push_back( cv::Point2f( offset_p2( 0 ), offset_p2( 1 ) ) );
                    }

                    Ellipse ellip_fit;
                    ellip_fit.fit( pt_ellipse );

                    EllipsePtr ellip(
                    new Ellipse( ellip_fit.box.center - cv::Point2f( p_u( 0 ), p_u( 1 ) ),
                                 ellip_fit.box.size,
                                 ellip_fit.box.angle ) );
                    ell.pPatt = ellip;

                    ++pointIdx;
                }
            }
        }
    }
}

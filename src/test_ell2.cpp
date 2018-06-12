#define BACKWARD_HAS_DW 1
#include "backward.hpp"
namespace backward
{
backward::SignalHandling sh;
}

#include "fsrk/shape/circle.h"
#include "fsrk/shape/ellipse.h"
#include <code_utils/cv_utils/randomcolor.h>
#include <code_utils/sys_utils/tic_toc.h>
#include <opencv2/opencv.hpp>

using namespace std;

#define WINDOW_NAME "image"
#define WINDOW_NAME2 "image2"

void
test1( )
{
    cv::Mat img( 1000, 1000, CV_8UC3, cv::Scalar( 0 ) );
    cv::Rect rect( 0, 0, 1000, 1000 );

    for ( int index1 = 1; index1 < 30; ++index1 )
        for ( int index2 = 1; index2 < 10; ++index2 )
        {
            cv::RandomColor3 color;
            cv::Ellipse ell( cv::Point2f( index1 * 30, index2 * 100 ),
                             cv::Size2f( 100 + index1 * 10, 100 + index2 * 10 ),
                             ( float )index1 * 360 / 10 );

            ell.draw( img, color.getColor( ) );

            if ( rect.contains( ell.getFocalPt0( ) ) )
                img.at< cv::Vec3b >( ell.getFocalPt0( ) ) = color.getColorVec( );
            if ( rect.contains( ell.getFocalPt1( ) ) )
                img.at< cv::Vec3b >( ell.getFocalPt1( ) ) = color.getColorVec( );
        }

    cv::namedWindow( WINDOW_NAME, cv::WINDOW_NORMAL );
    cv::imshow( WINDOW_NAME, img );
}

void
test2( )
{
    cv::Mat img( 1000, 1000, CV_8UC3, cv::Scalar( 0 ) );
    cv::Rect rect( 0, 0, 1000, 1000 );

    cv::RandomColor3 color;
    cv::Ellipse ell( cv::Point2f( 500, 500 ), cv::Size2f( 500, 700 ), 65 );
    ell.draw( img, color.getColor( ) );

    color.randColor( );

    cv::Ellipse ell2( cv::Point2f( 500, 500 ), cv::Size2f( 480, 680 ), 65 );
    std::vector< cv::Point > pts;
    ell2.toPoly( 65, pts );
    for ( auto& pt : pts )
    {
        img.at< cv::Vec3b >( pt ) = color.getColorVec( );
        color.randColor( );
    }
    std::cout << "pt size " << pts.size( ) << "\n";

    cv::ellipse( img,
                 cv::RotatedRect( cv::Point2f( 500, 500 ), cv::Size2f( 400, 600 ), 65 ),
                 color.getColor( ),
                 -1 );

    cv::namedWindow( WINDOW_NAME2, cv::WINDOW_NORMAL );
    cv::imshow( WINDOW_NAME2, img );
}

void
test3( )
{
    cv::namedWindow( "image3", cv::WINDOW_NORMAL );
    cv::Mat img( 1000, 1000, CV_8UC3, cv::Scalar( 0 ) );
    cv::RandomColor3 color;

    sys_utils::tic::TicTocPart time;

    // cv::ellipse( img,
    //              cv::RotatedRect( cv::Point2f( 500, 500 ), cv::Size2f( 50, 100 ), 65 ),
    //              color.getColor( ),
    //              -1 );

    std::cout << "ellipse cost " << time.toc( ) << " ms\n";

    cv::Ellipse ell2( cv::Point2f( 500, 500 ), cv::Size2f( 50, 100 ), 65 );

    ell2.calcParam( );
    int _num = 0;

    cv::Rect rect = ell2.box.boundingRect( );
    for ( int row_id = 0; row_id < rect.height; ++row_id )
        for ( int col_id = 0; col_id < rect.width; ++col_id )
        {
            cv::Point2f pt( col_id + rect.x, row_id + rect.y );
            if ( ell2.inside( pt ) )
            {
                _num++;
            }
        }

    std::cout << "ell2 cost " << time.toc( ) << " ms\n";
    std::cout << " _num " << _num << "\n";

    for ( int i = 0; i < 10; ++i )
        for ( int j = 0; j < 10; ++j )
        {
            cv::Ellipse ell3( cv::Point2f( 50 + 100 * i, 50 + 100 * j ), //
                              cv::Size2f( 1 + 10 * i, 1 + 10 * j ),
                              5 * ( i + j ) );
            color.randColor( );
            ell3.drawPoly( img, color.getColor( ) );
            std::cout << "ell3 cost " << time.toc( ) << " ms\n";
        }

    cv::imshow( "image3", img );
    //    cv::waitKey( 0 );

    // Create the mask with the polygon
    // cv::Mat1b mask( rect.height, rect.width, uchar( 0 ) );
    // cv::fillPoly( mask, pts, cv::Scalar( 255 ) );
    //
    //// Compute the mean with the computed mask
    // cv::Scalar average = cv::mean( img( rect ), mask );
    //
    //    std::cout << average << std::endl;
}

void
test4( )
{
    cv::namedWindow( "image4", cv::WINDOW_NORMAL );
    cv::Mat img = cv::imread( "/home/gao/ws/devel/lib/camera_model/image_down/IMG_35.png", 0 );

    cv::RandomColor1 color;

    sys_utils::tic::TicTocPart time;

    cv::Ellipse ell2( cv::Point2f( 500, 500 ), cv::Size2f( 50, 100 ), 65 );

    ell2.calcParam( );

    time.toc( );

    int ell_sum   = 0;
    int ell_num   = 0;
    cv::Rect rect = ell2.box.boundingRect( );
    for ( int row_id = 0; row_id < rect.height; ++row_id )
        for ( int col_id = 0; col_id < rect.width; ++col_id )
        {
            cv::Point2f pt( col_id + rect.x, row_id + rect.y );
            if ( ell2.inside( pt ) )
            {
                ell_sum += img.at< uchar >( pt );
                ell_num++;
            }
        }

    std::cout << "ellipse cost " << time.toc( ) << " ms " << ell_sum << " " << ell_num << "\n";

    int sum_ell2 = 0;
    int num_ell2 = 0;
    cv::Ellipse ell3( cv::Point2f( 500, 500 ), cv::Size2f( 50, 100 ), 65 );
    // for ( int i = 0; i < 10; ++i )
    //    for ( int j = 0; j < 10; ++j )
    {
        // cv::Ellipse ell3( cv::Point2f( 50 + 100 * i, 50 + 100 * j ), //
        //                  cv::Size2f( 1 + 10 * i, 1 + 10 * j ),
        //                  5 * ( i + j ) );

        // color.randColor( );
        // std::cout << "color " << color.getColor( ) << "\n";
        ell3.sumPoly( img, sum_ell2, num_ell2 );
        std::cout << "ell3 cost " << time.toc( ) << " ms " << sum_ell2 << " " << num_ell2 << "\n";
    }

    cv::imshow( "image4", img );
    //    cv::waitKey( 0 );

    // Create the mask with the polygon
    // cv::Mat1b mask( rect.height, rect.width, uchar( 0 ) );
    // cv::fillPoly( mask, pts, cv::Scalar( 255 ) );
    //
    //// Compute the mean with the computed mask
    // cv::Scalar average = cv::mean( img( rect ), mask );
    //
    //    std::cout << average << std::endl;
}

int
main( )
{
    // test1( );
    // test2( );
    // test3( );
    test4( );

    cv::waitKey( 0 );
    return 0;
}

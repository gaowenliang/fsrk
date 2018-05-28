#define BACKWARD_HAS_DW 1
#include "backward.hpp"
namespace backward
{
backward::SignalHandling sh;
}

#include "fsf/sagast/sagast.h"
#include "sfreak/sfreak.hpp"
#include "shape/circle.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <vignetting_model/vignetting/vignettingtable.h>

using namespace std;
using namespace cv;

double t;

void
showImg( std::string name, cv::Mat img )
{
    cv::namedWindow( name, WINDOW_NORMAL );
    cv::imshow( name, img );
}

void
detectFREAK( Mat& img1, Mat& img2, vector< KeyPoint >& kp1, vector< KeyPoint >& kp2, Mat& des1, Mat& des2 )
{

    // detector
    std::string file_cam  = "/home/gao/ws/src/vins/config/dual/cam_down_camera_calib.yaml";
    std::string file_cam2 = "/home/gao/ws/src/fsf/images/cam_config_F.yaml";
    std::string file_mask = "/home/gao/ws/src/vins/config/dual/mask_down.jpg";
    cv::Ptr< cv::AgastDetector > agast12
    = cv::AgastDetector::create( 40, true, cv::AgastDetector::SAGAST_12d );
    agast12->loadCamera( file_cam );
    agast12->loadMask( file_mask );
    if ( 0 )
    {
        agast12->buildOffsetsTable( );
        agast12->saveOffsetsCfg( "/home/gao/data_SAGAST12d_down" );
    }
    else
    {
        agast12->loadOffsetsCfg( "/home/gao/data_SAGAST12d_down" );
    }
    agast12->detect( img1, kp1 );
    std::cout << "size1: " << kp1.size( ) << std::endl;
    // TODO
    // kp1.clear( );
    // kp1.push_back( cv::KeyPoint( 300, 300, 5 ) );
    // kp1.push_back( cv::KeyPoint( 400, 400, 5 ) );
    // kp1.push_back( cv::KeyPoint( 500, 500, 5 ) );
    // kp1.push_back( cv::KeyPoint( 600, 600, 5 ) );
    // kp1.push_back( cv::KeyPoint( 700, 700, 5 ) );
    // kp1.push_back( cv::KeyPoint( 800, 800, 5 ) );
    // kp1.push_back( cv::KeyPoint( 400, 300, 5 ) );
    // kp1.push_back( cv::KeyPoint( 500, 300, 5 ) );
    // kp1.push_back( cv::KeyPoint( 600, 300, 5 ) );
    // kp1.push_back( cv::KeyPoint( 700, 300, 5 ) );
    // kp1.push_back( cv::KeyPoint( 800, 300, 5 ) );
    // kp1.push_back( cv::KeyPoint( 900, 300, 5 ) );
    // kp1.push_back( cv::KeyPoint( 1000, 300, 5 ) );
    // kp1.push_back( cv::KeyPoint( 1000, 400, 5 ) );
    // kp1.push_back( cv::KeyPoint( 1000, 500, 5 ) );
    // kp1.push_back( cv::KeyPoint( 1000, 600, 5 ) );
    // kp1.push_back( cv::KeyPoint( 1000, 700, 5 ) );
    // kp1.push_back( cv::KeyPoint( 900, 850, 5 ) );
    // kp1.push_back( cv::KeyPoint( 1046, 423, 5 ) );

    cv::Ptr< cv::AgastDetector > agast122
    = cv::AgastDetector::create( 20, true, cv::AgastDetector::SAGAST_12d );
    agast122->loadCamera( file_cam2 );
    agast122->loadMask( );
    if ( 0 )
    {
        agast122->buildOffsetsTable( );
        agast122->saveOffsetsCfg( "/home/gao/data_SAGAST12d_f" );
    }
    else
    {
        agast122->loadOffsetsCfg( "/home/gao/data_SAGAST12d_f" );
    }

    //    cv::Mat image2;
    // cv::Ptr< cv::CLAHE > clahe = cv::createCLAHE( );
    // clahe->apply( img2, image2 );
    //   image2 = img2;

    agast122->detect( img2, kp2 );

    // kp2.clear( );
    // kp2.push_back( cv::KeyPoint( 50, 100, 5 ) );
    // kp2.push_back( cv::KeyPoint( 50, 150, 5 ) );
    // kp2.push_back( cv::KeyPoint( 50, 200, 5 ) );
    // kp2.push_back( cv::KeyPoint( 50, 250, 5 ) );
    // kp2.push_back( cv::KeyPoint( 50, 300, 5 ) );
    // kp2.push_back( cv::KeyPoint( 50, 350, 5 ) );

    //////////////////////////////////////////////////////
    // kp2.clear( );
    // kp1.clear( );
    // cv::Ptr< cv::FastFeatureDetector > fast
    // = cv::FastFeatureDetector::create( 40, true, cv::FastFeatureDetector::TYPE_7_12 );
    // fast->detect( img2, kp2 );
    // fast->detect( img1, kp1 );
    // std::cout << " size1 " << kp1.size( ) << std::endl;
    // std::cout << " size2 " << kp2.size( ) << std::endl;
    //////////////////////////////////////////////////////

    Ptr< SFREAK > freak = cv::SFREAK::create( true, true, 36, 4 );
    freak->loadCamera( file_cam );
    freak->loadMask( file_mask );

    Ptr< SFREAK > freak2 = cv::SFREAK::create( true, true, 36, 4 );
    freak2->loadCamera( file_cam2 );
    freak2->loadMask( );

    std::cout << "/////////////////////////////////////// \n";
    std::cout << "/////////////////////////////////////// \n";

    t = getTickCount( );
    freak->compute( img1, kp1, des1 );
    t = ( ( double )getTickCount( ) - t ) / getTickFrequency( );
    std::cout << kp1.size( ) << " cost " << t * 1000 << " ms" << std::endl;
    std::cout << " avg " << t * 1000 / kp1.size( ) << " ms" << std::endl;

    std::cout << "[#INFO] image 1 process done.  \n";

    t = getTickCount( );
    freak2->compute( img2, kp2, des2 );
    t = ( ( double )getTickCount( ) - t ) / getTickFrequency( );
    std::cout << kp2.size( ) << " cost " << t * 1000 << " ms" << std::endl;
    std::cout << " avg " << t * 1000 / kp2.size( ) << " ms" << std::endl;

    std::cout << "[#INFO] image 2 process done.  \n";

    // ORB ///////////////////////////////
    // Ptr< ORB > orb = cv::ORB::create( 3000 );
    // kp2.clear( );
    // kp1.clear( );
    // orb->detect( img1, kp1 );
    // orb->detect( img2, kp2 );
    // std::cout << " size1 " << kp1.size( ) << std::endl;
    // std::cout << " size2 " << kp2.size( ) << std::endl;
    // orb->compute( img2, kp2, des2 );
    // orb->compute( img1, kp1, des1 );

    // BRISK ///////////////////////////////
    // Ptr< BRISK > brisk = cv::BRISK::create( 80, 4 );
    // kp2.clear( );
    // kp1.clear( );
    // brisk->detect( img1, kp1 );
    // brisk->detect( img2, kp2 );
    // std::cout << " size1 " << kp1.size( ) << std::endl;
    // std::cout << " size2 " << kp2.size( ) << std::endl;
    // brisk->compute( img2, kp2, des2 );
    // brisk->compute( img1, kp1, des1 );
}

void
runRANSAC( vector< DMatch >& good_matches, const vector< KeyPoint >& kp1, const vector< KeyPoint >& kp2 )
{
    Mat image1Points( good_matches.size( ), 2, CV_32F );
    Mat image2Points( good_matches.size( ), 2, CV_32F );
    for ( size_t i = 0; i < good_matches.size( ); i++ )
    {
        int sub_des1 = good_matches[i].queryIdx;
        int sub_des2 = good_matches[i].trainIdx;
        float* ptr1  = image1Points.ptr< float >( i );
        float* ptr2  = image2Points.ptr< float >( i );
        ptr1[0]      = kp1[sub_des1].pt.x;
        ptr1[1]      = kp1[sub_des1].pt.y;
        ptr2[0]      = kp2[sub_des2].pt.x;
        ptr2[1]      = kp2[sub_des2].pt.y;
    }

    Mat fundamental;
    vector< uchar > RANSAC_state;
    fundamental = cv::findFundamentalMat( image1Points, image2Points, RANSAC_state, cv::FM_RANSAC );

    vector< DMatch >::iterator iter;
    int i = 0;
    for ( iter = good_matches.begin( ); iter != good_matches.end( ); )
    {
        if ( RANSAC_state[i] == 0 )
        {
            iter = good_matches.erase( iter );
        }
        else
            iter++;
        i++;
    }
}

void
dispAngole( vector< DMatch >& good_matches, const vector< KeyPoint >& kp1, const vector< KeyPoint >& kp2 )
{
    for ( size_t index = 0; index < good_matches.size( ); ++index )
    {
        int sub_des1 = good_matches[index].queryIdx;
        int sub_des2 = good_matches[index].trainIdx;
        std::cout << " " << kp1[sub_des1].angle << " " << kp2[sub_des2].angle << "\n";
    }
}

void
matchFREAK( Mat& img1, Mat& img2, vector< KeyPoint >& kp1, vector< KeyPoint >& kp2, Mat& des1, Mat& des2 )
{
    // cv::BruteForceMatcher< HammingLUT > matcher;
    std::cout << "[#INFO] matchFREAK.  \n";

    BFMatcher matcher( NORM_HAMMING );
    //    vector< DMatch > matches;
    //    matcher.match( des1, des2, matches );

    vector< DMatch > matches1to2;
    vector< DMatch > matches2to1;

    double sum_t = 0.0;
    for ( int a = 0; a < 10; ++a )
    {
        matches1to2.clear( );
        t = getTickCount( );
        matcher.match( des1, des2, matches1to2 );
        t = ( ( double )getTickCount( ) - t ) / getTickFrequency( );
        sum_t += t;
    }
    std::cout << "match avg cost " << sum_t * 1000 / 10 << " ms "
              << sum_t * 1000 * 1000 / 10 / matches1to2.size( ) << " ns " << std::endl;

    matcher.match( des2, des1, matches2to1 );
    int* flag = new int[des2.rows];
    for ( int i = 0; i < des2.rows; i++ )
    {
        flag[i] = matches2to1[i].trainIdx;
    }

    double max_dist = 0;
    double min_dist = 100;
    for ( int i = 0; i < des1.rows; i++ )
    {
        double dist = matches1to2[i].distance;
        if ( dist < min_dist )
            min_dist = dist;
        if ( dist > max_dist )
            max_dist = dist;
    }

    cout << " " << max_dist << " " << min_dist << endl;

    std::vector< DMatch > good_matches;
    for ( int i = 0; i < des1.rows; i++ )
    {
        if ( matches1to2[i].distance < max_dist )
        {
            if ( matches2to1[matches1to2[i].trainIdx].trainIdx == i )
                good_matches.push_back( matches1to2[i] );
        }
    }

    cout << "matches " << good_matches.size( ) << endl;

    Mat img_matches;
    drawMatches( img1,
                 kp1,
                 img2,
                 kp2,
                 good_matches,
                 img_matches,
                 Scalar( 0, 255, 0 ),
                 Scalar( 0, 0, 255 ),
                 vector< char >( ),
                 DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    showImg( "dst_0", img_matches );

    runRANSAC( good_matches, kp1, kp2 );

    drawMatches( img1,
                 kp1,
                 img2,
                 kp2,
                 good_matches,
                 img_matches,
                 Scalar( 0, 255, 0 ),
                 Scalar( 0, 0, 255 ),
                 vector< char >( ),
                 DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    cout << "good_matches " << good_matches.size( ) << " of " << kp1.size( ) << " and "
         << kp2.size( ) << endl;

    //    dispAngole( good_matches, kp1, kp2 );

    showImg( "dst", img_matches );
    cv::waitKey( 0 );
}

int
main( )
{
    Mat img1 = imread( "/home/gao/ws/devel/lib/camera_model/image_down/IMG_35.png", 0 );
    Mat img2 = imread( "/home/gao/ws/src/fsf/images/image_F.png", 0 );

    //    cv::resize( img1, img2, cv::Size( 640, 512 ) );

    vector< KeyPoint > kp1, kp2;
    Mat des1, des2;

    detectFREAK( img1, img2, kp1, kp2, des1, des2 );

    std::cout << "[#INFO] detectFREAK done.\n";

    matchFREAK( img1, img2, kp1, kp2, des1, des2 );

    return 0;
}

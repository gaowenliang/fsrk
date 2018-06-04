#define BACKWARD_HAS_DW 1
#include "backward.hpp"
namespace backward
{
backward::SignalHandling sh;
}

#include "shape/circle.h"
#include "shape/ellipse.h"
#include <code_utils/cv_utils/randomcolor.h>
#include <opencv2/opencv.hpp>

using namespace std;

#define WINDOW_NAME "image"
#define WINDOW_NAME2 "image2"

int
main( )
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
    cv::waitKey( 0 );
    return 0;
}

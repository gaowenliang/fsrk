//  freak.cpp
//
//  Copyright (C) 2011-2012  Signal processing laboratory 2, EPFL,
//  Kirell Benzi (kirell.benzi@epfl.ch),
//  Raphael Ortiz (raphael.ortiz@a3.epfl.ch)
//  Alexandre Alahi (alexandre.alahi@epfl.ch)
//  and Pierre Vandergheynst (pierre.vandergheynst@epfl.ch)
//
//  Redistribution and use in source and binary forms, with or without modification,
//  are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
//  This software is provided by the copyright holders and contributors "as is" and
//  any express or implied warranties, including, but not limited to, the implied
//  warranties of merchantability and fitness for a particular purpose are disclaimed.
//  In no event shall the Intel Corporation or contributors be liable for any direct,
//  indirect, incidental, special, exemplary, or consequential damages
//  (including, but not limited to, procurement of substitute goods or services;
//  loss of use, data, or profits; or business interruption) however caused
//  and on any theory of liability, whether in contract, strict liability,
//  or tort (including negligence or otherwise) arising in any way out of
//  the use of this software, even if advised of the possibility of such damage.

#include "sfreak.hpp"
#include "sfreak_impl.h"
#include <algorithm>
#include <code_utils/sys_utils/tic_toc.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <stdlib.h>
#include <string.h>

namespace cv
{

// int color_rand1;
// int color_rand2;
// int color_rand3;
// Mat image_color;

// create an image showing the brisk pattern
// void
// SFREAK_Impl::drawPattern( )
//{
//    Mat pattern = Mat::zeros( 1000, 1000, CV_8UC3 ) + Scalar( 255, 255, 255 );
//    int sFac    = 500 / patternScale;
//    for ( int n = 0; n < FREAK_NB_POINTS; ++n )
//    {
//        PatternEllipse& pt = patternTable[0][n];
//        circle( pattern,
//                Point( pt.pPatt->box.center.x * sFac, pt.pPatt->box.center.y * sFac ) +
//                Point( 500, 500 ),
//                pt.pPatt->box.size.width * sFac,
//                Scalar( 0, 0, 255 ),
//                2 );
//        //  rectangle( pattern,
//        //             Point( ( pt.pPatt->box.center.x - pt.pPatt->box.size.width ) *
//        sFac,
//        //                    ( pt.pPatt->box.center.y - pt.pPatt->box.size.width ) * sFac
//        )
//        //             + Point( 500, 500 ),
//        //             Point( ( pt.pPatt->box.center.x + pt.pPatt->box.size.width ) *
//        sFac,
//        //                    ( pt.pPatt->box.center.y + pt.pPatt->box.size.width ) * sFac
//        )
//        //             + Point( 500, 500 ),
//        //             Scalar( 0, 0, 255 ),
//        //             2 );

//        circle( pattern,
//                Point( pt.pPatt->box.center.x * sFac, pt.pPatt->box.center.y * sFac ) +
//                Point( 500, 500 ),
//                1,
//                Scalar( 0, 0, 0 ),
//                3 );
//        std::ostringstream oss;
//        oss << n;
//        putText( pattern,
//                 oss.str( ),
//                 Point( pt.pPatt->box.center.x * sFac, pt.pPatt->box.center.y * sFac ) +
//                 Point( 500, 500 ),
//                 FONT_HERSHEY_SIMPLEX,
//                 0.5,
//                 Scalar( 0, 0, 0 ),
//                 1 );
//    }
//    imshow( "FreakDescriptorExtractor pattern", pattern );
//    waitKey( 0 );
//}

// -------------------------------------------------
Ptr< SFREAK >
SFREAK::create( bool orientationNormalized,
                bool scaleNormalized,
                float patternScale,
                int nOctaves,
                const std::vector< int >& selectedPairs )
{
    return makePtr< cv::SFREAK_Impl >( orientationNormalized, //
                                       scaleNormalized,
                                       patternScale,
                                       nOctaves,
                                       selectedPairs );
}

Ptr< SFREAK >
SFREAK::create( std::string cam_file,
                bool orientationNormalized,
                bool scaleNormalized,
                float patternScale,
                int nOctaves,
                const std::vector< int >& selectedPairs )
{
    return makePtr< cv::SFREAK_Impl >( orientationNormalized, //
                                       scaleNormalized,
                                       patternScale,
                                       nOctaves,
                                       selectedPairs );
}

bool
SFREAK::loadCamera( std::string cam_file )
{
    std::cout << "#INFO: camera config is " << cam_file << std::endl;
    cam = camera_model::CameraFactory::instance( )->generateCameraFromYamlFile( cam_file );

    std::cout << cam->parametersToString( ) << std::endl;
    image_center = cam->getPrinciple( );
    std::cout << "#INFO: LOADing camera config is DONE." << cam_file << std::endl;

    return true;
}

bool
SFREAK::loadMask( std::string file )
{
    m_mask = cv::imread( file, cv::IMREAD_GRAYSCALE );

    return true;
}

bool
SFREAK::loadMask( )
{
    m_mask = cv::Mat( cam->imageHeight( ), cam->imageWidth( ), CV_8UC1, cv::Scalar( 255 ) );

    return true;
}

Eigen::Vector2d
SFREAK::calcAngle( const camera_model::CameraPtr cam, const Eigen::Vector2d pu )
{
    Eigen::Vector3d P;
    cam->liftSphere( pu, P );

    double theta = acos( P( 2 ) / P.norm( ) );
    double phi   = atan2( P( 1 ), P( 0 ) );

    Eigen::Vector2d angle( theta, phi );

    return angle;
}

} // END NAMESPACE CV

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

#include "fsrk/sfreak/sfreak.hpp"
#include "fsrk/sfreak/sfreak_impl.h"
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

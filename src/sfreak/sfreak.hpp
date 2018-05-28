/*
By downloading, copying, installing or using the software you agree to this
license. If you do not agree to this license, do not download, install,
copy or use the software.

                          License Agreement
               For Open Source Computer Vision Library
                       (3-clause BSD License)

Copyright (C) 2013, OpenCV Foundation, all rights reserved.
Third party copyrights are property of their respective owners.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the names of the copyright holders nor the names of the contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

This software is provided by the copyright holders and contributors "as is" and
any express or implied warranties, including, but not limited to, the implied
warranties of merchantability and fitness for a particular purpose are
disclaimed. In no event shall copyright holders or contributors be liable for
any direct, indirect, incidental, special, exemplary, or consequential damages
(including, but not limited to, procurement of substitute goods or services;
loss of use, data, or profits; or business interruption) however caused
and on any theory of liability, whether in contract, strict liability,
or tort (including negligence or otherwise) arising in any way out of
the use of this software, even if advised of the possibility of such damage.
*/

#ifndef __OPENCV_SFREAK_HPP__
#define __OPENCV_SFREAK_HPP__

#include "opencv2/features2d.hpp"
#include <camera_model/camera_models/CameraFactory.h>
#include <opencv2/opencv.hpp>

namespace cv
{

/** @brief Class implementing the FREAK (*Fast Retina Keypoint*) keypoint descriptor,
described in @cite AOV12 .

The algorithm propose a novel keypoint descriptor inspired by the human visual system and
more
precisely the retina, coined Fast Retina Key- point (FREAK). A cascade of binary strings is
computed by efficiently comparing image intensities over a retinal sampling pattern. FREAKs
are in
general faster to compute with lower memory load and also more robust than SIFT, SURF or
BRISK.
They are competitive alternatives to existing keypoints in particular for embedded
applications.

@note
   -   An example on how to use the FREAK descriptor can be found at
        opencv_source_code/samples/cpp/freak_demo.cpp
 */
class CV_EXPORTS_W SFREAK : public Feature2D
{
    public:
    enum
    {
        NB_SCALES     = 1, // 64
        NB_PAIRS      = 512,
        NB_ORIENPAIRS = 45
    };

    /**
    @param orientationNormalized Enable orientation normalization.
    @param scaleNormalized Enable scale normalization.
    @param patternScale Scaling of the description pattern.
    @param nOctaves Number of octaves covered by the detected keypoints.
    @param selectedPairs (Optional) user defined selected pairs indexes,
     */
    static Ptr< SFREAK > create( bool orientationNormalized              = true,
                                 bool scaleNormalized                    = true,
                                 float patternScale                      = 22.0f,
                                 int nOctaves                            = 4,
                                 const std::vector< int >& selectedPairs = std::vector< int >( ) );

    static Ptr< SFREAK > create( std::string cam_file,
                                 bool orientationNormalized              = true,
                                 bool scaleNormalized                    = true,
                                 float patternScale                      = 22.0f,
                                 int nOctaves                            = 4,
                                 const std::vector< int >& selectedPairs = std::vector< int >( ) );

    bool loadCamera( std::string cam_file )
    {
        std::cout << "#INFO: camera config is " << cam_file << std::endl;
        cam = camera_model::CameraFactory::instance( )->generateCameraFromYamlFile( cam_file );

        std::cout << cam->parametersToString( ) << std::endl;
        image_center = cam->getPrinciple( );
        std::cout << "#INFO: LOADing camera config is DONE." << cam_file << std::endl;
        return true;
    }
    bool loadMask( std::string file )
    {
        m_mask = cv::imread( file, cv::IMREAD_GRAYSCALE );
        return true;
    }
    bool loadMask( )
    {
        m_mask = cv::Mat( cam->imageHeight( ), cam->imageWidth( ), CV_8UC1, cv::Scalar( 255 ) );
        return true;
    }
    Eigen::Vector2d calcAngle( const camera_model::CameraPtr cam, const Eigen::Vector2d pu )
    {
        Eigen::Vector3d P;
        cam->liftSphere( pu, P );

        double theta = acos( P( 2 ) / P.norm( ) );
        double phi   = atan2( P( 1 ), P( 0 ) );

        Eigen::Vector2d angle( theta, phi );

        return angle;
    }

    void drawPoint2Yellow( cv::Mat& image, Eigen::Vector2d pt )
    {
        image.at< cv::Vec3b >( double2int( pt( 1 ) ), double2int( pt( 0 ) ) )[0] = 0;   // b
        image.at< cv::Vec3b >( double2int( pt( 1 ) ), double2int( pt( 0 ) ) )[1] = 255; // g
        image.at< cv::Vec3b >( double2int( pt( 1 ) ), double2int( pt( 0 ) ) )[2] = 255; // r
    }
    void drawPoint2Red( cv::Mat& image, Eigen::Vector2d pt )
    {
        image.at< cv::Vec3b >( double2int( pt( 1 ) ), double2int( pt( 0 ) ) )[0] = 0;   // b
        image.at< cv::Vec3b >( double2int( pt( 1 ) ), double2int( pt( 0 ) ) )[1] = 0;   // g
        image.at< cv::Vec3b >( double2int( pt( 1 ) ), double2int( pt( 0 ) ) )[2] = 255; // r
    }
    void drawPoint2Blue( cv::Mat& image, Eigen::Vector2d pt )
    {
        image.at< cv::Vec3b >( double2int( pt( 1 ) ), double2int( pt( 0 ) ) )[0] = 255; // b
        image.at< cv::Vec3b >( double2int( pt( 1 ) ), double2int( pt( 0 ) ) )[1] = 0;   // g
        image.at< cv::Vec3b >( double2int( pt( 1 ) ), double2int( pt( 0 ) ) )[2] = 0;   // r
    }
    void drawPoint2Green( cv::Mat& image, Eigen::Vector2d pt )
    {
        image.at< cv::Vec3b >( double2int( pt( 1 ) ), double2int( pt( 0 ) ) )[0] = 0;   // b
        image.at< cv::Vec3b >( double2int( pt( 1 ) ), double2int( pt( 0 ) ) )[1] = 255; // g
        image.at< cv::Vec3b >( double2int( pt( 1 ) ), double2int( pt( 0 ) ) )[2] = 0;   // r
    }

    int double2int( double a )
    {
        if ( 1 )
        {
            double abs_a = abs( a );
            int int_a    = int( abs_a );
            if ( abs_a - int_a >= 0.5 )
                return a > 0 ? ( abs_a + 1 ) : ( -( abs_a + 1 ) );
            else
                return a > 0 ? int_a : ( -int_a );
        }
    }

    void buildOffsetsTable( );
    bool saveOffsetsTable( std::string path );
    bool loadOffsetsTable( std::string path );

    public:
    cv::Point2f image_center;
    int pixel_size;
    camera_model::CameraPtr cam;
    cv::Mat m_tableOffsets;
    cv::Mat m_mask;
};
}

#endif // __OPENCV_SFREAK_HPP__

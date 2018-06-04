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

    bool loadCamera( std::string cam_file );
    bool loadMask( std::string file );
    bool loadMask( );
    Eigen::Vector2d calcAngle( const camera_model::CameraPtr cam, const Eigen::Vector2d pu );

    virtual void buildPatternTable( )                             = 0;
    virtual bool savePatternTableToData( std::string filename )   = 0;
    virtual bool loadPatternTableFromData( std::string filename ) = 0;

    public:
    cv::Point2f image_center;
    int pixel_size;
    camera_model::CameraPtr cam;
    cv::Mat m_mask;
};
}

#endif // __OPENCV_SFREAK_HPP__

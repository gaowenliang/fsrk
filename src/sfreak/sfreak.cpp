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

//#include "precomp.hpp"
#include "sfreak.hpp"
#include "../shape/circle.h"
#include "../shape/ellipse.h"
#include <algorithm>
#include <bitset>
#include <code_utils/cv_utils/cv_type.hpp>
#include <code_utils/sys_utils/cvmat_file_io.hpp>
#include <code_utils/sys_utils/tic_toc.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <stdlib.h>
#include <string.h>

#define Table

namespace cv
{

/*!
 FREAK implementation
 */
class SFREAK_Impl : public SFREAK
{
    public:
    /** Constructor
     * @param orientationNormalized enable orientation normalization
     * @param scaleNormalized enable scale normalization
     * @param patternScale scaling of the description pattern
     * @param nbOctave number of octaves covered by the detected keypoints
     * @param selectedPairs (optional) user defined selected pairs
     */
    explicit SFREAK_Impl( bool orientationNormalized              = true,
                          bool scaleNormalized                    = true,
                          float patternScale                      = 22.0f,
                          int nOctaves                            = 4,
                          const std::vector< int >& selectedPairs = std::vector< int >( ) );

    explicit SFREAK_Impl( std::string cam_file,
                          bool orientationNormalized              = true,
                          bool scaleNormalized                    = true,
                          float patternScale                      = 22.0f,
                          int nOctaves                            = 4,
                          const std::vector< int >& selectedPairs = std::vector< int >( ) );

    virtual ~SFREAK_Impl( );

    /** returns the descriptor length in bytes */
    virtual int descriptorSize( ) const;

    /** returns the descriptor type */
    virtual int descriptorType( ) const;

    /** returns the default norm type */
    virtual int defaultNorm( ) const;

    /** select the 512 "best description pairs"
     * @param images grayscale images set
     * @param keypoints set of detected keypoints
     * @param corrThresh correlation threshold
     * @param verbose print construction information
     * @return list of best pair indexes
     */
    std::vector< int > selectPairs( const std::vector< Mat >& images,
                                    std::vector< std::vector< KeyPoint > >& keypoints,
                                    const double corrThresh = 0.7,
                                    bool verbose            = true );
    virtual void compute( InputArray image, std::vector< KeyPoint >& keypoints, OutputArray descriptors );
    virtual void buildOffsetsTable( );
    virtual bool saveTable2Data( std::string filename );
    virtual bool loadTableFromData( std::string filename );

    //    void drawPattern( );

    protected:
    void buildPattern( );
    bool loadPattern( );

    template< typename imgType, typename iiType >
    imgType meanIntensity( InputArray image,
                           const float kp_x,
                           const float kp_y,
                           const unsigned int scale,
                           const unsigned int rot,
                           const unsigned int point );

    template< typename imgType, typename iiType >
    imgType meanIntensityTable( InputArray image,
                                const int index,
                                const float cosTheta,
                                const float sinTheta,
                                const float theta,
                                const float kp_x,
                                const float kp_y,
                                const unsigned int scale,
                                const unsigned int rot,
                                const unsigned int point );

    template< typename srcMatType, typename iiMatType >
    void computeDescriptors( InputArray image, std::vector< KeyPoint >& keypoints, OutputArray descriptors );

    template< typename srcMatType >
    void extractDescriptor( srcMatType* pointsValue, void** ptr );

    bool orientationNormalized; // true if the orientation is normalized, false otherwise
    bool scaleNormalized;       // true if the scale is normalized, false otherwise
    double patternScale;        // scaling of the pattern
    int nOctaves;               // number of octaves
    bool extAll; // true if all pairs need to be extracted for pairs selection

    double patternScale0;
    int nOctaves0;
    std::vector< int > selectedPairs0;

    struct PatternEllipse
    {
        EllipsePtr pPatt;
    };

    struct DescriptionPair
    {
        uchar i; // index of the first point
        uchar j; // index of the second point
    };

    struct OrientationPair
    {
        uchar i;       // index of the first point
        uchar j;       // index of the second point
        int weight_dx; // dx/(norm_sq))*4096
        int weight_dy; // dy/(norm_sq))*4096
    };

    // look-up table for the pattern points
    // (position+sigma of all points at all
    // scales and orientation)
    //    std::vector< std::vector< PatternEllipse > > patternTable;

    cv::Mat m_tableOffsets;

    int patternSizes[NB_SCALES]; // size of the pattern at a specific scale (used to check
                                 // if a point is within image boundaries)
    DescriptionPair descriptionPairs[NB_PAIRS];
    OrientationPair orientationPairs[NB_ORIENPAIRS];
};

static const double FREAK_LOG2 = 0.693147180559945;
#define FREAK_NB_ORIENTATION 180 // 180
#define FREAK_NB_POINTS 43
static const int FREAK_SMALLEST_KP_SIZE = 7; // smallest size of keypoints
static const int FREAK_NB_SCALES        = SFREAK::NB_SCALES;
static const int FREAK_NB_PAIRS         = SFREAK::NB_PAIRS;
static const int FREAK_NB_ORIENPAIRS    = SFREAK::NB_ORIENPAIRS;

// default pairs
static const int FREAK_DEF_PAIRS[SFREAK_Impl::NB_PAIRS]
= { 404, 431, 818, 511, 181, 52,  311, 874, 774, 543, 719, 230, 417, 205, 11,  560, 149,
    265, 39,  306, 165, 857, 250, 8,   61,  15,  55,  717, 44,  412, 592, 134, 761, 695,
    660, 782, 625, 487, 549, 516, 271, 665, 762, 392, 178, 796, 773, 31,  672, 845, 548,
    794, 677, 654, 241, 831, 225, 238, 849, 83,  691, 484, 826, 707, 122, 517, 583, 731,
    328, 339, 571, 475, 394, 472, 580, 381, 137, 93,  380, 327, 619, 729, 808, 218, 213,
    459, 141, 806, 341, 95,  382, 568, 124, 750, 193, 749, 706, 843, 79,  199, 317, 329,
    768, 198, 100, 466, 613, 78,  562, 783, 689, 136, 838, 94,  142, 164, 679, 219, 419,
    366, 418, 423, 77,  89,  523, 259, 683, 312, 555, 20,  470, 684, 123, 458, 453, 833,
    72,  113, 253, 108, 313, 25,  153, 648, 411, 607, 618, 128, 305, 232, 301, 84,  56,
    264, 371, 46,  407, 360, 38,  99,  176, 710, 114, 578, 66,  372, 653, 129, 359, 424,
    159, 821, 10,  323, 393, 5,   340, 891, 9,   790, 47,  0,   175, 346, 236, 26,  172,
    147, 574, 561, 32,  294, 429, 724, 755, 398, 787, 288, 299, 769, 565, 767, 722, 757,
    224, 465, 723, 498, 467, 235, 127, 802, 446, 233, 544, 482, 800, 318, 16,  532, 801,
    441, 554, 173, 60,  530, 713, 469, 30,  212, 630, 899, 170, 266, 799, 88,  49,  512,
    399, 23,  500, 107, 524, 90,  194, 143, 135, 192, 206, 345, 148, 71,  119, 101, 563,
    870, 158, 254, 214, 276, 464, 332, 725, 188, 385, 24,  476, 40,  231, 620, 171, 258,
    67,  109, 844, 244, 187, 388, 701, 690, 50,  7,   850, 479, 48,  522, 22,  154, 12,
    659, 736, 655, 577, 737, 830, 811, 174, 21,  237, 335, 353, 234, 53,  270, 62,  182,
    45,  177, 245, 812, 673, 355, 556, 612, 166, 204, 54,  248, 365, 226, 242, 452, 700,
    685, 573, 14,  842, 481, 468, 781, 564, 416, 179, 405, 35,  819, 608, 624, 367, 98,
    643, 448, 2,   460, 676, 440, 240, 130, 146, 184, 185, 430, 65,  807, 377, 82,  121,
    708, 239, 310, 138, 596, 730, 575, 477, 851, 797, 247, 27,  85,  586, 307, 779, 326,
    494, 856, 324, 827, 96,  748, 13,  397, 125, 688, 702, 92,  293, 716, 277, 140, 112,
    4,   80,  855, 839, 1,   413, 347, 584, 493, 289, 696, 19,  751, 379, 76,  73,  115,
    6,   590, 183, 734, 197, 483, 217, 344, 330, 400, 186, 243, 587, 220, 780, 200, 793,
    246, 824, 41,  735, 579, 81,  703, 322, 760, 720, 139, 480, 490, 91,  814, 813, 163,
    152, 488, 763, 263, 425, 410, 576, 120, 319, 668, 150, 160, 302, 491, 515, 260, 145,
    428, 97,  251, 395, 272, 252, 18,  106, 358, 854, 485, 144, 550, 131, 133, 378, 68,
    102, 104, 58,  361, 275, 209, 697, 582, 338, 742, 589, 325, 408, 229, 28,  304, 191,
    189, 110, 126, 486, 211, 547, 533, 70,  215, 670, 249, 36,  581, 389, 605, 331, 518,
    442, 822 };

// used to sort pairs during pairs selection
struct PairStat
{
    double mean;
    int idx;
};

struct sortMean
{
    bool operator( )( const PairStat& a, const PairStat& b ) const
    {
        return a.mean < b.mean;
    }
};

bool
SFREAK_Impl::saveTable2Data( std::string filename )
{
    if ( m_tableOffsets.empty( ) )
    {
        std::cout << "[#INFO] Table is empty !!\n";
        return false;
    }

    sys_utils::io::writeMatrixToBinary( filename, m_tableOffsets );

    return true;
}

bool
SFREAK_Impl::loadTableFromData( std::string filename )
{
    sys_utils::io::parseMatrixFromBinary( filename, m_tableOffsets );

    if ( m_tableOffsets.empty( ) )
    {
        std::cout << "[#INFO] load Table From Data failed !!\n";
        return false;
    }

    return loadPattern( );
}

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

void
SFREAK_Impl::buildPattern( )
{
    if ( patternScale == patternScale0 //
         && nOctaves == nOctaves0 )
        return;

    nOctaves0     = nOctaves;
    patternScale0 = patternScale;

    int img_width          = cam->imageWidth( );
    int img_height         = cam->imageHeight( );
    cv::Point2f center_cam = image_center;

    std::vector< float > pixels_size;
    pixels_size.push_back( std::sqrt( ( center_cam.x - 0 ) * ( center_cam.x - 0 )
                                      + ( center_cam.y - 0 ) * ( center_cam.y - 0 ) ) );
    pixels_size.push_back( std::sqrt( ( center_cam.x - img_width ) * ( center_cam.x - img_width )
                                      + ( center_cam.y - 0 ) * ( center_cam.y - 0 ) ) );
    pixels_size.push_back( std::sqrt( ( center_cam.x - img_width ) * ( center_cam.x - img_width )
                                      + ( center_cam.y - img_height ) * ( center_cam.y - img_height ) ) );
    pixels_size.push_back( std::sqrt( ( center_cam.x - 0 ) * ( center_cam.x - 0 )
                                      + ( center_cam.y - img_height ) * ( center_cam.y - img_height ) ) );

    pixel_size = std::max( std::max( pixels_size[0], pixels_size[1] ),
                           std::max( pixels_size[2], pixels_size[3] ) );
    std::cout << "pixel_size " << pixel_size << "\n";

#ifdef Table

    m_tableOffsets = cv::Mat( pixel_size, //
                              FREAK_NB_ORIENTATION * FREAK_NB_POINTS,
                              CV_32FC5,
                              cv::Scalar( 0 ) );

    std::cout << "[#Debug] patternTable size: " << m_tableOffsets.size( ) << "\n\n\n";
#endif
    // 2 ^ ( (nOctaves-1) /nbScales)
    double scaleStep = std::pow( 2.0, ( double )( nOctaves ) / FREAK_NB_SCALES );
    double scalingFactor, alpha, beta, theta = 0;

    // pattern definition, radius normalized to 1.0 (outer point position + sigma = 1.0)

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
#ifdef Table

    // fill the lookup table
    for ( int index = 0; index < pixel_size; ++index )
    {
        // color_rand1 = rand( ) % 256;
        // color_rand2 = rand( ) % 256;
        // color_rand3 = rand( ) % 256;

        for ( int scaleIdx = 0; scaleIdx < FREAK_NB_SCALES; ++scaleIdx )
        {
            patternSizes[scaleIdx] = 0; // proper initialization

            // scale of the pattern, scaleStep ^ scaleIdx
            scalingFactor = std::pow( scaleStep, scaleIdx );

            for ( int orientationIdx = 0; orientationIdx < FREAK_NB_ORIENTATION; ++orientationIdx )
            {
                // orientation of the pattern
                // double full_angle = 60.0 * CV_PI / 180.0;
                //                    theta = double( orientationIdx ) * full_angle /
                //                    double( FREAK_NB_ORIENTATION );
                theta = double( orientationIdx ) * 2 * CV_PI / double( FREAK_NB_ORIENTATION );

                int pointIdx = 0;

                // 8 groups
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
                        float center_x = static_cast< float >(
                        radius[i] * cos( alpha ) * scalingFactor * patternScale );
                        float center_y = static_cast< float >(
                        radius[i] * sin( alpha ) * scalingFactor * patternScale );

                        float center_c_x = center_x + image_center.x;
                        float center_c_y = center_y + image_center.y;
                        float radius_c = static_cast< float >( sigma[i] * scalingFactor * patternScale );

                        cv::CircleInt cirle( Point( center_c_x, center_c_y ), int( radius_c ) );
                        std::vector< cv::Point2f > pts_circle;
                        pts_circle = cirle.getCirclePoints( cam->imageSize( ) );

                        // src pt
                        Eigen::Vector2d p_u00( image_center.x, image_center.y );
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
                        double delta_angle   = acos( P_center.dot( P_center2 ) );
                        Eigen::Vector3d dVec = P_center.cross( P_center2 );
                        dVec.normalize( );

                        // rotation vector
                        Eigen::Quaterniond q_v1v0( cos( 0.5 * delta_angle ),
                                                   sin( 0.5 * delta_angle ) * dVec( 0 ),
                                                   sin( 0.5 * delta_angle ) * dVec( 1 ),
                                                   sin( 0.5 * delta_angle ) * dVec( 2 ) );

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

                        EllipsePtr ellip(
                        new Ellipse( ellip_fit.box.center - cv::Point2f( p_u( 0 ), p_u( 1 ) ),
                                     ellip_fit.box.size,
                                     ellip_fit.box.angle ) );
                        //                        ell.pPatt = ellip;

                        m_tableOffsets.at< cv::Vec5f >( index,
                                                        scaleIdx * FREAK_NB_ORIENTATION * FREAK_NB_POINTS //
                                                        + orientationIdx * FREAK_NB_POINTS
                                                        + pointIdx )
                        = cv::Vec5f( ellip->box.center.x,
                                     ellip->box.center.y,
                                     ellip->box.size.width,
                                     ellip->box.size.height,
                                     ellip->box.angle );

                        // NOTE
                        //   if ( index % 15 == 0 )
                        //       ellip_fit.draw( image_color, cv::Scalar( color_rand1,
                        //       color_rand2, color_rand3 ) );

                        // adapt the sizeList if necessary
                        const int sizeMax
                        = static_cast< int >( ceil( ( radius[i] + sigma[i] ) * scalingFactor * patternScale ) )
                          + 1;

                        if ( patternSizes[scaleIdx] < sizeMax )
                            patternSizes[scaleIdx] = sizeMax;

                        ++pointIdx;
                    }
                }
            }
        }
    }
#endif

    // cv::imshow( "image_color_src", image_color );
    // cv::waitKey( 0 );

    // build the list of orientation pairs
    // clang-format off
       orientationPairs[0].i  = 0;     orientationPairs[0].j  = 3;
       orientationPairs[1].i  = 1;     orientationPairs[1].j  = 4;
       orientationPairs[2].i  = 2;     orientationPairs[2].j  = 5;
       orientationPairs[3].i  = 0;     orientationPairs[3].j  = 2;
       orientationPairs[4].i  = 1;     orientationPairs[4].j  = 3;
       orientationPairs[5].i  = 2;     orientationPairs[5].j  = 4;
       orientationPairs[6].i  = 3;     orientationPairs[6].j  = 5;
       orientationPairs[7].i  = 4;     orientationPairs[7].j  = 0;
       orientationPairs[8].i  = 5;     orientationPairs[8].j  = 1;
       orientationPairs[9].i  = 6;     orientationPairs[9].j  = 9;
       orientationPairs[10].i = 7;     orientationPairs[10].j = 10;
       orientationPairs[11].i = 8;     orientationPairs[11].j = 11;
       orientationPairs[12].i = 6;     orientationPairs[12].j = 8;
       orientationPairs[13].i = 7;     orientationPairs[13].j = 9;
       orientationPairs[14].i = 8;     orientationPairs[14].j = 10;
       orientationPairs[15].i = 9;     orientationPairs[15].j = 11;
       orientationPairs[16].i = 10;    orientationPairs[16].j = 6;
       orientationPairs[17].i = 11;    orientationPairs[17].j = 7;
       orientationPairs[18].i = 12;    orientationPairs[18].j = 15;
       orientationPairs[19].i = 13;    orientationPairs[19].j = 16;
       orientationPairs[20].i = 14;    orientationPairs[20].j = 17;
       orientationPairs[21].i = 12;    orientationPairs[21].j = 14;
       orientationPairs[22].i = 13;    orientationPairs[22].j = 15;
       orientationPairs[23].i = 14;    orientationPairs[23].j = 16;
       orientationPairs[24].i = 15;    orientationPairs[24].j = 17;
       orientationPairs[25].i = 16;    orientationPairs[25].j = 12;
       orientationPairs[26].i = 17;    orientationPairs[26].j = 13;
       orientationPairs[27].i = 18;    orientationPairs[27].j = 21;
       orientationPairs[28].i = 19;    orientationPairs[28].j = 22;
       orientationPairs[29].i = 20;    orientationPairs[29].j = 23;
       orientationPairs[30].i = 18;    orientationPairs[30].j = 20;
       orientationPairs[31].i = 19;    orientationPairs[31].j = 21;
       orientationPairs[32].i = 20;    orientationPairs[32].j = 22;
       orientationPairs[33].i = 21;    orientationPairs[33].j = 23;
       orientationPairs[34].i = 22;    orientationPairs[34].j = 18;
       orientationPairs[35].i = 23;    orientationPairs[35].j = 19;
       orientationPairs[36].i = 24;    orientationPairs[36].j = 27;
       orientationPairs[37].i = 25;    orientationPairs[37].j = 28;
       orientationPairs[38].i = 26;    orientationPairs[38].j = 29;
       orientationPairs[39].i = 30;    orientationPairs[39].j = 33;
       orientationPairs[40].i = 31;    orientationPairs[40].j = 34;
       orientationPairs[41].i = 32;    orientationPairs[41].j = 35;
       orientationPairs[42].i = 36;    orientationPairs[42].j = 39;
       orientationPairs[43].i = 37;    orientationPairs[43].j = 40;
       orientationPairs[44].i = 38;    orientationPairs[44].j = 41;
    // clang-format on

    for ( unsigned m = FREAK_NB_ORIENPAIRS; m--; )
    {
        cv::Vec5f param_i = m_tableOffsets.at< cv::Vec5f >( 0, orientationPairs[m].i );
        cv::Vec5f param_j = m_tableOffsets.at< cv::Vec5f >( 0, orientationPairs[m].j );

        const float dx = param_i[0] - param_j[0];
        const float dy = param_i[1] - param_j[1];

        const float norm_sq = ( dx * dx + dy * dy );

        orientationPairs[m].weight_dx = int( ( dx / ( norm_sq ) ) * 4096.0 + 0.5 );
        orientationPairs[m].weight_dy = int( ( dy / ( norm_sq ) ) * 4096.0 + 0.5 );
    }

    // build the list of description pairs
    std::vector< DescriptionPair > allPairs;
    for ( unsigned int i = 1; i < ( unsigned int )FREAK_NB_POINTS; ++i )
    {
        // (generate all the pairs)
        for ( unsigned int j = 0; ( unsigned int )j < i; ++j )
        {
            DescriptionPair pair = { ( uchar )i, ( uchar )j };
            allPairs.push_back( pair );
        }
    }
    std::cout << "allPairs " << allPairs.size( ) << "\n";

    // Input vector provided
    if ( !selectedPairs0.empty( ) )
    {
        if ( ( int )selectedPairs0.size( ) == FREAK_NB_PAIRS )
        {
            for ( int i = 0; i < FREAK_NB_PAIRS; ++i )
            {
                descriptionPairs[i] = allPairs[selectedPairs0.at( i )];
            }
        }
        else
        {
            CV_Error( Error::StsVecLengthErr,
                      "Input vector does not match the required size" );
        }
    }
    else // default selected pairs
    {
        for ( int i = 0; i < FREAK_NB_PAIRS; ++i )
        {
            descriptionPairs[i] = allPairs[FREAK_DEF_PAIRS[i]];
        }
    }

    //    drawPattern( );
}

bool
SFREAK_Impl::loadPattern( )
{
    nOctaves0     = nOctaves;
    patternScale0 = patternScale;

    int img_width          = cam->imageWidth( );
    int img_height         = cam->imageHeight( );
    cv::Point2f center_cam = image_center;

    std::vector< float > pixels_size;
    pixels_size.push_back( std::sqrt( ( center_cam.x - 0 ) * ( center_cam.x - 0 )
                                      + ( center_cam.y - 0 ) * ( center_cam.y - 0 ) ) );
    pixels_size.push_back( std::sqrt( ( center_cam.x - img_width ) * ( center_cam.x - img_width )
                                      + ( center_cam.y - 0 ) * ( center_cam.y - 0 ) ) );
    pixels_size.push_back( std::sqrt( ( center_cam.x - img_width ) * ( center_cam.x - img_width )
                                      + ( center_cam.y - img_height ) * ( center_cam.y - img_height ) ) );
    pixels_size.push_back( std::sqrt( ( center_cam.x - 0 ) * ( center_cam.x - 0 )
                                      + ( center_cam.y - img_height ) * ( center_cam.y - img_height ) ) );

    int pixel_tmp = std::max( std::max( pixels_size[0], pixels_size[1] ),
                              std::max( pixels_size[2], pixels_size[3] ) );

    pixel_size = m_tableOffsets.rows;

    if ( pixel_tmp != pixel_size )
    {
        std::cout << "[#INFO] Table Size error !!!\n";
        return false;
    }
    else
        std::cout << "pixel_size " << pixel_size << " " << pixel_tmp << "\n";

    // 2 ^ ( (nOctaves-1) /nbScales)
    double scaleStep = std::pow( 2.0, ( double )( nOctaves ) / FREAK_NB_SCALES );
    double scalingFactor;

    // pattern definition, radius normalized to 1.0 (outer point position + sigma = 1.0)

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

    // fill the lookup table
    for ( int index = 0; index < pixel_size; ++index )
    {
        for ( int scaleIdx = 0; scaleIdx < FREAK_NB_SCALES; ++scaleIdx )
        {
            patternSizes[scaleIdx] = 0; // proper initialization

            // scale of the pattern, scaleStep ^ scaleIdx
            scalingFactor = std::pow( scaleStep, scaleIdx );

            for ( int orientationIdx = 0; orientationIdx < FREAK_NB_ORIENTATION; ++orientationIdx )
            {
                // 8 groups
                for ( size_t i = 0; i < 8; ++i )
                {
                    // 6 circles per group
                    for ( int k = 0; k < n[i]; ++k )
                    {
                        // adapt the sizeList if necessary
                        const int sizeMax
                        = static_cast< int >( ceil( ( radius[i] + sigma[i] ) * scalingFactor * patternScale ) )
                          + 1;

                        if ( patternSizes[scaleIdx] < sizeMax )
                            patternSizes[scaleIdx] = sizeMax;
                    }
                }
            }
        }
    }

    // cv::imshow( "image_color_src", image_color );
    // cv::waitKey( 0 );

    // build the list of orientation pairs
    // clang-format off
     orientationPairs[0].i  = 0;     orientationPairs[0].j  = 3;
     orientationPairs[1].i  = 1;     orientationPairs[1].j  = 4;
     orientationPairs[2].i  = 2;     orientationPairs[2].j  = 5;
     orientationPairs[3].i  = 0;     orientationPairs[3].j  = 2;
     orientationPairs[4].i  = 1;     orientationPairs[4].j  = 3;
     orientationPairs[5].i  = 2;     orientationPairs[5].j  = 4;
     orientationPairs[6].i  = 3;     orientationPairs[6].j  = 5;
     orientationPairs[7].i  = 4;     orientationPairs[7].j  = 0;
     orientationPairs[8].i  = 5;     orientationPairs[8].j  = 1;
     orientationPairs[9].i  = 6;     orientationPairs[9].j  = 9;
     orientationPairs[10].i = 7;     orientationPairs[10].j = 10;
     orientationPairs[11].i = 8;     orientationPairs[11].j = 11;
     orientationPairs[12].i = 6;     orientationPairs[12].j = 8;
     orientationPairs[13].i = 7;     orientationPairs[13].j = 9;
     orientationPairs[14].i = 8;     orientationPairs[14].j = 10;
     orientationPairs[15].i = 9;     orientationPairs[15].j = 11;
     orientationPairs[16].i = 10;    orientationPairs[16].j = 6;
     orientationPairs[17].i = 11;    orientationPairs[17].j = 7;
     orientationPairs[18].i = 12;    orientationPairs[18].j = 15;
     orientationPairs[19].i = 13;    orientationPairs[19].j = 16;
     orientationPairs[20].i = 14;    orientationPairs[20].j = 17;
     orientationPairs[21].i = 12;    orientationPairs[21].j = 14;
     orientationPairs[22].i = 13;    orientationPairs[22].j = 15;
     orientationPairs[23].i = 14;    orientationPairs[23].j = 16;
     orientationPairs[24].i = 15;    orientationPairs[24].j = 17;
     orientationPairs[25].i = 16;    orientationPairs[25].j = 12;
     orientationPairs[26].i = 17;    orientationPairs[26].j = 13;
     orientationPairs[27].i = 18;    orientationPairs[27].j = 21;
     orientationPairs[28].i = 19;    orientationPairs[28].j = 22;
     orientationPairs[29].i = 20;    orientationPairs[29].j = 23;
     orientationPairs[30].i = 18;    orientationPairs[30].j = 20;
     orientationPairs[31].i = 19;    orientationPairs[31].j = 21;
     orientationPairs[32].i = 20;    orientationPairs[32].j = 22;
     orientationPairs[33].i = 21;    orientationPairs[33].j = 23;
     orientationPairs[34].i = 22;    orientationPairs[34].j = 18;
     orientationPairs[35].i = 23;    orientationPairs[35].j = 19;
     orientationPairs[36].i = 24;    orientationPairs[36].j = 27;
     orientationPairs[37].i = 25;    orientationPairs[37].j = 28;
     orientationPairs[38].i = 26;    orientationPairs[38].j = 29;
     orientationPairs[39].i = 30;    orientationPairs[39].j = 33;
     orientationPairs[40].i = 31;    orientationPairs[40].j = 34;
     orientationPairs[41].i = 32;    orientationPairs[41].j = 35;
     orientationPairs[42].i = 36;    orientationPairs[42].j = 39;
     orientationPairs[43].i = 37;    orientationPairs[43].j = 40;
     orientationPairs[44].i = 38;    orientationPairs[44].j = 41;
    // clang-format on

    for ( unsigned m = FREAK_NB_ORIENPAIRS; m--; )
    {
        cv::Vec5f param_i = m_tableOffsets.at< cv::Vec5f >( 0, orientationPairs[m].i );
        cv::Vec5f param_j = m_tableOffsets.at< cv::Vec5f >( 0, orientationPairs[m].j );

        const float dx = param_i[0] - param_j[0];
        const float dy = param_i[1] - param_j[1];

        const float norm_sq = ( dx * dx + dy * dy );

        orientationPairs[m].weight_dx = int( ( dx / ( norm_sq ) ) * 4096.0 + 0.5 );
        orientationPairs[m].weight_dy = int( ( dy / ( norm_sq ) ) * 4096.0 + 0.5 );
    }

    // build the list of description pairs
    std::vector< DescriptionPair > allPairs;
    for ( unsigned int i = 1; i < ( unsigned int )FREAK_NB_POINTS; ++i )
    {
        // (generate all the pairs)
        for ( unsigned int j = 0; ( unsigned int )j < i; ++j )
        {
            DescriptionPair pair = { ( uchar )i, ( uchar )j };
            allPairs.push_back( pair );
        }
    }
    std::cout << "allPairs " << allPairs.size( ) << "\n";

    // Input vector provided
    if ( !selectedPairs0.empty( ) )
    {
        if ( ( int )selectedPairs0.size( ) == FREAK_NB_PAIRS )
        {
            for ( int i = 0; i < FREAK_NB_PAIRS; ++i )
            {
                descriptionPairs[i] = allPairs[selectedPairs0.at( i )];
            }
        }
        else
        {
            CV_Error( Error::StsVecLengthErr,
                      "Input vector does not match the required size" );
        }
    }
    else // default selected pairs
    {
        for ( int i = 0; i < FREAK_NB_PAIRS; ++i )
        {
            descriptionPairs[i] = allPairs[FREAK_DEF_PAIRS[i]];
        }
    }

    return true;
}

void
SFREAK_Impl::buildOffsetsTable( )
{
    ( ( SFREAK_Impl* )this )->buildPattern( );
}

void
SFREAK_Impl::compute( InputArray _image, std::vector< KeyPoint >& keypoints, OutputArray _descriptors )
{
    Mat image = _image.getMat( );
    if ( image.empty( ) )
        return;
    if ( keypoints.empty( ) )
        return;

    // cv::cvtColor( image, image_color, cv::COLOR_GRAY2BGR );

    //    ( ( SFREAK_Impl* )this )->buildPattern( );

    // Convert to gray if not already
    Mat grayImage;
    if ( image.channels( ) == 3 || image.channels( ) == 4 )
        cv::cvtColor( image, grayImage, cv::COLOR_BGR2GRAY );
    else
    {
        CV_Assert( image.channels( ) == 1 );
        grayImage = image;
    }

    // Use 32-bit integers if we won't overflow in the integral image
    // 8388608 = 2 ^ (32 - 8(bit depth) - 1(sign bit))
    if ( ( image.depth( ) == CV_8U || image.depth( ) == CV_8S ) && ( image.rows * image.cols ) < 8388608 )
    {
        // Create the integral image appropriate for our type & usage
        if ( image.depth( ) == CV_8U )
        {
            std::cout << " < uchar, int > \n";
            computeDescriptors< uchar, int >( grayImage, keypoints, _descriptors );
        }
        else if ( image.depth( ) == CV_8S )
        {
            std::cout << " < char, int > \n";
            computeDescriptors< char, int >( grayImage, keypoints, _descriptors );
        }
        else
            CV_Error( Error::StsUnsupportedFormat, "" );
    }
    else
    {
        // Create the integral image appropriate for our type & usage
        if ( image.depth( ) == CV_8U )
        {
            std::cout << " < uchar, double > \n";
            computeDescriptors< uchar, double >( grayImage, keypoints, _descriptors );
        }
        else if ( image.depth( ) == CV_8S )
        {
            std::cout << " < char, double > \n";
            computeDescriptors< char, double >( grayImage, keypoints, _descriptors );
        }
        else if ( image.depth( ) == CV_16U )
        {
            std::cout << " < ushort, double > \n";
            computeDescriptors< ushort, double >( grayImage, keypoints, _descriptors );
        }
        else if ( image.depth( ) == CV_16S )
        {
            std::cout << " < short, double > \n";
            computeDescriptors< short, double >( grayImage, keypoints, _descriptors );
        }
        else
            CV_Error( Error::StsUnsupportedFormat, "" );
    }
}

template< typename srcMatType >
void
SFREAK_Impl::extractDescriptor( srcMatType* pointsValue, void** ptr )
{
    std::bitset< FREAK_NB_PAIRS >** ptrScalar = ( std::bitset< FREAK_NB_PAIRS >** )ptr;

    // extracting descriptor preserving the order of SSE version
    int cnt = 0;
    for ( int n = 7; n < FREAK_NB_PAIRS; n += 128 )
    {
        for ( int m = 8; m--; )
        {
            int nm = n - m;

            for ( int kk = nm + 15 * 8; kk >= nm; kk -= 8, ++cnt )
            {
                ( *ptrScalar )
                ->set( kk,
                       pointsValue[descriptionPairs[cnt].i]
                       >= pointsValue[descriptionPairs[cnt].j] );
            }
        }
    }
    --( *ptrScalar );
}

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
float
angOfPoints( cv::Point2f pt0, cv::Point2f pt1 )
{
    return atan2( ( float )( pt1.y - pt0.y ), //
                  ( float )( pt1.x - pt0.x ) );
}

template< typename srcMatType, typename iiMatType >
void
SFREAK_Impl::computeDescriptors( InputArray _image, std::vector< KeyPoint >& keypoints, OutputArray _descriptors )
{
    Mat image = _image.getMat( );

    // used to save pattern scale index corresponding to each keypoints
    std::vector< int > kpScaleIdx( keypoints.size( ) );

    // used in std::vector erase function
    const std::vector< int >::iterator ScaleIdxBegin = kpScaleIdx.begin( );
    // used in std::vector erase function
    const std::vector< cv::KeyPoint >::iterator kpBegin = keypoints.begin( );

    const float sizeCst = static_cast< float >( FREAK_NB_SCALES / ( FREAK_LOG2 * nOctaves ) );
    srcMatType pointsValue2[FREAK_NB_POINTS];

    int thetaIdx   = 0;
    int direction0 = 0;
    int direction1 = 0;

    // compute the scale index corresponding to the keypoint size and
    // remove keypoints close to the border
    if ( scaleNormalized )
    {
        for ( size_t k = keypoints.size( ); k--; )
        {
            // Is k non-zero? If so, decrement it and continue"
            kpScaleIdx[k]
            = std::max( ( int )( std::log( keypoints[k].size / FREAK_SMALLEST_KP_SIZE ) * sizeCst + 0.5 ), 0 );
            //            std::cout << "  " << kpScaleIdx[k] << " ";

            if ( kpScaleIdx[k] >= FREAK_NB_SCALES )
                kpScaleIdx[k] = FREAK_NB_SCALES - 1;

            // check if the description at this specific position and scale fits
            // inside the image
            if ( keypoints[k].pt.x <= patternSizes[kpScaleIdx[k]]
                 || keypoints[k].pt.y <= patternSizes[kpScaleIdx[k]]
                 || keypoints[k].pt.x >= image.cols - patternSizes[kpScaleIdx[k]]
                 || keypoints[k].pt.y >= image.rows - patternSizes[kpScaleIdx[k]] )
            {
                keypoints.erase( kpBegin + k );
                kpScaleIdx.erase( ScaleIdxBegin + k );
            }
        }
    }
    else
    {
        const int scIdx = std::max( ( int )( 1.0986122886681 * sizeCst + 0.5 ), 0 );
        for ( size_t k = keypoints.size( ); k--; )
        {

            kpScaleIdx[k] = scIdx; // equivalent to the formule when the scale is normalized
                                   // with a constant size of
                                   // keypoints[k].size=3*SMALLEST_KP_SIZE

            if ( kpScaleIdx[k] >= FREAK_NB_SCALES )
            {
                kpScaleIdx[k] = FREAK_NB_SCALES - 1;
            }

            if ( keypoints[k].pt.x <= patternSizes[kpScaleIdx[k]]
                 || keypoints[k].pt.y <= patternSizes[kpScaleIdx[k]]
                 || keypoints[k].pt.x >= image.cols - patternSizes[kpScaleIdx[k]]
                 || keypoints[k].pt.y >= image.rows - patternSizes[kpScaleIdx[k]] )
            {
                keypoints.erase( kpBegin + k );
                kpScaleIdx.erase( ScaleIdxBegin + k );
            }
        }
    }
    //    std::cout << " keypoints " << keypoints.size( ) << "\n";

    // allocate descriptor memory, estimate orientations, extract descriptors
    if ( !extAll )
    {
        // extract the best comparisons only
        _descriptors.create( ( int )keypoints.size( ), FREAK_NB_PAIRS / 8, CV_8U );
        _descriptors.setTo( Scalar::all( 0 ) );
        Mat descriptors = _descriptors.getMat( );

        void* ptr = descriptors.data + ( keypoints.size( ) - 1 ) * descriptors.step[0];

        for ( size_t k = keypoints.size( ); k--; )
        {

            //  color_rand1 = rand( ) % 256;
            //  color_rand2 = rand( ) % 256;
            //  color_rand3 = rand( ) % 256;

            // NOTE
            float dist     = disOfPoints( image_center, keypoints[k].pt );
            float sinTheta = sinAngOfPoints( image_center, keypoints[k].pt, dist );
            float cosTheta = cosAngOfPoints( image_center, keypoints[k].pt, dist );
            float theta = angOfPoints( image_center, keypoints[k].pt ) * ( 180.0 / CV_PI );
            int index   = int( dist );

            // estimate orientation (gradient)
            if ( !orientationNormalized )
            {
                thetaIdx           = 0; // assign 0° to all keypoints
                keypoints[k].angle = 0.0;
            }
            else
            {
                int thetaIdxTmp;
                if ( theta < 0.f )
                    thetaIdxTmp = int( FREAK_NB_ORIENTATION * ( -theta ) * ( 1 / 360.0 ) - 0.5 );
                else
                    thetaIdxTmp = int( FREAK_NB_ORIENTATION * ( -theta ) * ( 1 / 360.0 ) + 0.5 );

                if ( thetaIdxTmp < 0 )
                    thetaIdxTmp += FREAK_NB_ORIENTATION;
                if ( thetaIdxTmp >= FREAK_NB_ORIENTATION )
                    thetaIdxTmp -= FREAK_NB_ORIENTATION;

                // get the points intensity value in the un-rotated pattern
                for ( int i = FREAK_NB_POINTS; i--; )
                {
                    pointsValue2[i]
                    = meanIntensityTable< srcMatType, iiMatType >( image, //
                                                                   index,
                                                                   theta,
                                                                   cosTheta,
                                                                   sinTheta,
                                                                   keypoints[k].pt.x,
                                                                   keypoints[k].pt.y,
                                                                   kpScaleIdx[k],
                                                                   thetaIdxTmp,
                                                                   i );
                }

                direction0 = 0;
                direction1 = 0;
                for ( int m = 45; m--; )
                {
                    // iterate through the orientation pairs
                    const int delta = ( pointsValue2[orientationPairs[m].i] //
                                        - pointsValue2[orientationPairs[m].j] );
                    direction0 += delta * ( orientationPairs[m].weight_dx ) / 2048;
                    direction1 += delta * ( orientationPairs[m].weight_dy ) / 2048;
                }

                // estimate orientation
                keypoints[k].angle = static_cast< float >( atan2( ( float )direction1, //
                                                                  ( float )direction0 )
                                                           * ( 180.0 / CV_PI ) );

                if ( keypoints[k].angle < 0.f )
                    thetaIdx
                    = int( FREAK_NB_ORIENTATION * ( -theta + keypoints[k].angle ) * ( 1 / 360.0 ) - 0.5 );
                else
                    thetaIdx
                    = int( FREAK_NB_ORIENTATION * ( -theta + keypoints[k].angle ) * ( 1 / 360.0 ) + 0.5 );

                if ( thetaIdx < 0 )
                    thetaIdx += FREAK_NB_ORIENTATION;

                if ( thetaIdx >= FREAK_NB_ORIENTATION )
                    thetaIdx -= FREAK_NB_ORIENTATION;
            }

            const unsigned char* const mask = m_mask.ptr( )
                                              + int( keypoints[k].pt.y ) * cam->imageWidth( )
                                              + int( keypoints[k].pt.x );

            // extract descriptor at the computed orientation
            if ( mask[0] > 20 )
            {
                for ( int i = FREAK_NB_POINTS; i--; )
                {
                    pointsValue2[i]
                    = meanIntensityTable< srcMatType, iiMatType >( image, //
                                                                   index,
                                                                   theta,
                                                                   cosTheta,
                                                                   sinTheta,
                                                                   keypoints[k].pt.x,
                                                                   keypoints[k].pt.y,
                                                                   kpScaleIdx[k],
                                                                   thetaIdx,
                                                                   i );
                }
            }
            else
            {
                for ( int i = FREAK_NB_POINTS; i--; )
                {
                    pointsValue2[i] = 0;
                }
            }

            // Extract descriptor
            extractDescriptor< srcMatType >( pointsValue2, &ptr );
        }
        //        std::cout << "\n";
    }
    /*
    else // extract all possible comparisons for selection
    {
        _descriptors.create( ( int )keypoints.size( ), 128, CV_8U );
        _descriptors.setTo( Scalar::all( 0 ) );
        Mat descriptors = _descriptors.getMat( );
        std::bitset< 1024 >* ptr
        = ( std::bitset< 1024 >* )( descriptors.data
                                    + ( keypoints.size( ) - 1 ) * descriptors.step[0] );

        for ( size_t k = keypoints.size( ); k--; )
        {
            color_rand1 = rand( ) % 256;
            color_rand2 = rand( ) % 256;
            color_rand3 = rand( ) % 256;

            // NOTE
            float dist     = disOfPoints( image_center, keypoints[k].pt );
            float sinTheta = sinAngOfPoints( image_center, keypoints[k].pt, dist );
            float cosTheta = cosAngOfPoints( image_center, keypoints[k].pt, dist );
            float theta = angOfPoints( image_center, keypoints[k].pt ) * ( 180.0 / CV_PI
);
            int index   = int( dist );

            // estimate orientation (gradient)
            if ( !orientationNormalized )
            {
                thetaIdx           = 0; // assign 0° to all keypoints
                keypoints[k].angle = 0.0;
            }
            else
            {
                int thetaIdxTmp;
                if ( theta < 0.f )
                    thetaIdxTmp = int( FREAK_NB_ORIENTATION * ( -theta ) * ( 1 / 360.0 )
-
0.5 );
                else
                    thetaIdxTmp = int( FREAK_NB_ORIENTATION * ( -theta ) * ( 1 / 360.0 )
+
0.5 );

                if ( thetaIdxTmp < 0 )
                    thetaIdxTmp += FREAK_NB_ORIENTATION;
                if ( thetaIdxTmp >= FREAK_NB_ORIENTATION )
                    thetaIdxTmp -= FREAK_NB_ORIENTATION;

#ifndef Table
                thetaIdxTmp = 0;
#endif

                // get the points intensity value in the un-rotated pattern
                for ( int i = FREAK_NB_POINTS; i--; )
                {
#ifndef Table
                    pointsValue[i] = meanIntensity< srcMatType, iiMatType >( image, //
                                                                             keypoints[k].pt.x,
                                                                             keypoints[k].pt.y,
                                                                             kpScaleIdx[k],
                                                                             0,
                                                                             i );

#else

                    pointsValue2[i]
                    = meanIntensityTable< srcMatType, iiMatType >( image, //
                                                                   index,
                                                                   theta,
                                                                   cosTheta,
                                                                   sinTheta,
                                                                   keypoints[k].pt.x,
                                                                   keypoints[k].pt.y,
                                                                   kpScaleIdx[k],
                                                                   thetaIdxTmp,
                                                                   i );
#endif
                }

                direction0 = 0;
                direction1 = 0;
                for ( int m = 45; m--; )
                {
                    // iterate through the orientation pairs
                    const int delta
                    = ( pointsValue[orientationPairs[m].i] -
pointsValue[orientationPairs[m].j] );
                    direction0 += delta * ( orientationPairs[m].weight_dx ) / 2048;
                    direction1 += delta * ( orientationPairs[m].weight_dy ) / 2048;
                }

                // estimate orientation
                keypoints[k].angle = static_cast< float >(
                atan2( ( float )direction1, ( float )direction0 ) * ( 180.0 / CV_PI ) );

                if ( keypoints[k].angle < 0.f )
                    thetaIdx
                    = int( FREAK_NB_ORIENTATION * ( -theta + keypoints[k].angle ) * ( 1
/
360.0 ) - 0.5 );
                else
                    thetaIdx
                    = int( FREAK_NB_ORIENTATION * ( -theta + keypoints[k].angle ) * ( 1
/
360.0 ) + 0.5 );

                if ( thetaIdx < 0 )
                    thetaIdx += FREAK_NB_ORIENTATION;

                if ( thetaIdx >= FREAK_NB_ORIENTATION )
                    thetaIdx -= FREAK_NB_ORIENTATION;
            }

            // get the points intensity value in the rotated pattern
            for ( int i = FREAK_NB_POINTS; i--; )
            {
#ifndef Table
                pointsValue[i] = meanIntensity< srcMatType, iiMatType >( image, //
                                                                         keypoints[k].pt.x,
                                                                         keypoints[k].pt.y,
                                                                         kpScaleIdx[k],
                                                                         thetaIdx,
                                                                         i );
#else

                pointsValue2[i]
                = meanIntensityTable< srcMatType, iiMatType >( image, //
                                                               index,
                                                               theta,
                                                               cosTheta,
                                                               sinTheta,
                                                               keypoints[k].pt.x,
                                                               keypoints[k].pt.y,
                                                               kpScaleIdx[k],
                                                               thetaIdx,
                                                               i );
#endif
            }

            int cnt( 0 );
            for ( int i = 1; i < FREAK_NB_POINTS; ++i )
            {
                //(generate all the pairs)
                for ( int j = 0; j < i; ++j )
                {
                    ptr->set( cnt, pointsValue[i] >= pointsValue[j] );
                    ++cnt;
                }
            }
            --ptr;
        }
    }*/

    // cv::namedWindow( "image_color", WINDOW_NORMAL );
    // cv::imshow( "image_color", image_color );
    // cv::waitKey( 0 );

    // cv::imwrite( "/home/gao/1.bmp", image_color );
}

// simply take average on a square patch, not even gaussian approx
template< typename imgType, typename iiType >
imgType
SFREAK_Impl::meanIntensity( InputArray _image,
                            const float kp_x,
                            const float kp_y,
                            const unsigned int scale,
                            const unsigned int rot,
                            const unsigned int point )
{
    Mat image = _image.getMat( );

    /*
    // get point position in image
    const PatternEllipse& FreakEllip = patternTable[0][scale * FREAK_NB_ORIENTATION *
FREAK_NB_POINTS //
                                                       + rot * FREAK_NB_POINTS
                                                       + point];

    const float xf = FreakEllip.pPatt->box.center.x + kp_x;
    const float yf = FreakEllip.pPatt->box.center.y + kp_y;

    const int x = int( xf );
    const int y = int( yf );

    // get the sigma:
    const float radius = FreakEllip.pPatt->box.size.width;

    // calculate output:
    if ( radius < 0.5 )
    {
        // interpolation multipliers:
        const int r_x   = static_cast< int >( ( xf - x ) * 1024 );
        const int r_y   = static_cast< int >( ( yf - y ) * 1024 );
        const int r_x_1 = ( 1024 - r_x );
        const int r_y_1 = ( 1024 - r_y );

        unsigned int ret_val;
        // linear interpolation:
        ret_val = r_x_1 * r_y_1 * int( image.at< imgType >( y, x ) )
                  + r_x * r_y_1 * int( image.at< imgType >( y, x + 1 ) )
                  + r_x_1 * r_y * int( image.at< imgType >( y + 1, x ) )
                  + r_x * r_y * int( image.at< imgType >( y + 1, x + 1 ) );

        // return the rounded mean
        ret_val += 2 * 1024 * 1024;

        return static_cast< imgType >( ret_val / ( 4 * 1024 * 1024 ) );
    }
    else
    {

        float _sum = 0;
        int _num   = 0;

        const float xf_src = FreakEllip.pPatt->box.center.x + image_center.x;
        const float yf_src = FreakEllip.pPatt->box.center.y + image_center.y;

        cv::CircleInt cirle( Point( xf_src, yf_src ), int( radius ) );
        //        cirle.draw( image_color, cv::Scalar( 0, 255, 255 ) );
        std::vector< cv::Point2f > pts_circle = cirle.getCirclePoints( cam->imageSize( ) );

        // src pt
        Eigen::Vector2d p_u00 = Eigen::Vector2d( image_center.x, image_center.y ); // 0
        Eigen::Vector3d P_center;
        cam->liftSphere( p_u00, P_center );
        P_center.normalize( );

        // dst pt
        Eigen::Vector2d p_u( kp_x, kp_y );
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

        // Eigen::Quaterniond q_v1v1( cos( 0.5 * angle_center( 1 ) ),
        //                            sin( 0.5 * angle_center( 1 ) ) * P_center2( 0 ),
        //                            sin( 0.5 * angle_center( 1 ) ) * P_center2( 1 ),
        //                            sin( 0.5 * angle_center( 1 ) ) * P_center2( 2 ) );

        // Eigen::Vector2d p_src( xf_src, yf_src );
        // Eigen::Vector3d P_src;
        // cam->liftSphere( p_src, P_src );
        // Eigen::Vector3d offset_P =   q_v1v0* P_src;
    // Eigen::Vector2d offset_p;
    // cam->spaceToPlane( offset_P, offset_p );
    // drawPoint2Red( image_color, offset_p );
    // drawPoint2Red( image_color, p_src );

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

    Ellipse ellip;
    ellip.fit( pt_ellipse );
    //        ellip.draw( image_color, cv::Scalar( 255, 255, 0 ) );

    cv::Rect rect = ellip.box.boundingRect( );
    for ( int row_id = 0; row_id < rect.height; ++row_id )
        for ( int col_id = 0; col_id < rect.width; ++col_id )
        {
            cv::Point2f pt( col_id + rect.x, row_id + rect.y );
            if ( ellip.inside( pt ) )
            {
                _sum += image.at< imgType >( pt );
                _num++;
                // image_color.at< cv::Vec3b >( pt ) = cv::Vec3b( 0, 255, 255 );
            }
        }

    iiType ret_val;
    ret_val = iiType( _sum / _num );

    return static_cast< imgType >( ret_val );
}*/
}

template< typename imgType, typename iiType >
imgType
SFREAK_Impl::meanIntensityTable( InputArray _image,
                                 const int index,
                                 const float theta,
                                 const float cosTheta,
                                 const float sinTheta,
                                 const float kp_x,
                                 const float kp_y,
                                 const unsigned int scale,
                                 const unsigned int rot,
                                 const unsigned int point )
{
    Mat image = _image.getMat( );

    // get point position in image
    cv::Vec5f param = m_tableOffsets.at< cv::Vec5f >( index,
                                                      scale * FREAK_NB_ORIENTATION * FREAK_NB_POINTS //
                                                      + rot * FREAK_NB_POINTS
                                                      + point );

    const float cx  = param[0];
    const float cy  = param[1];
    const float sw  = param[2];
    const float sh  = param[3];
    const float ang = param[4];

    Eigen::Matrix2f R;
    R << cosTheta, -sinTheta, sinTheta, cosTheta;

    Eigen::Vector2f pt_v( cx, cy );
    Eigen::Vector2f pt_2 = R * pt_v;

    const float xf = pt_2( 0 ) + kp_x;
    const float yf = pt_2( 1 ) + kp_y;

    const int x = int( xf );
    const int y = int( yf );

    // calculate output:
    if ( sw < 0.5 //
         || sh < 0.5 )
    {
        // interpolation multipliers:
        const int r_x   = static_cast< int >( ( xf - x ) * 1024 );
        const int r_y   = static_cast< int >( ( yf - y ) * 1024 );
        const int r_x_1 = ( 1024 - r_x );
        const int r_y_1 = ( 1024 - r_y );

        unsigned int ret_val;
        // linear interpolation:
        ret_val = r_x_1 * r_y_1 * int( image.at< imgType >( y, x ) )
                  + r_x * r_y_1 * int( image.at< imgType >( y, x + 1 ) )
                  + r_x_1 * r_y * int( image.at< imgType >( y + 1, x ) )
                  + r_x * r_y * int( image.at< imgType >( y + 1, x + 1 ) );

        // return the rounded mean
        ret_val += 2 * 1024 * 1024;

        return static_cast< imgType >( ret_val / ( 4 * 1024 * 1024 ) );
    }
    else
    {
        float _sum = 0;
        int _num   = 0;

        // NOTE
        Ellipse ell( cv::Point2f( xf, yf ), cv::Size2f( sw, sh ), ang + theta );
        // ell.draw( image_color, cv::Scalar( color_rand1, color_rand2, color_rand3 ) );

        cv::Rect rect = ell.box.boundingRect( );
        for ( int row_id = 0; row_id < rect.height; ++row_id )
            for ( int col_id = 0; col_id < rect.width; ++col_id )
            {
                cv::Point2f pt( col_id + rect.x, row_id + rect.y );
                if ( ell.inside( pt ) )
                {
                    _sum += image.at< imgType >( pt );
                    _num++;
                }
            }

        iiType ret_val;
        ret_val = iiType( _sum / _num );

        return static_cast< imgType >( ret_val );
    }
}

// pair selection algorithm from a set of training images and corresponding keypoints
std::vector< int >
SFREAK_Impl::selectPairs( const std::vector< Mat >& images,
                          std::vector< std::vector< KeyPoint > >& keypoints,
                          const double corrTresh,
                          bool verbose )
{
    extAll = true;
    // compute descriptors with all pairs
    Mat descriptors;

    if ( verbose )
        std::cout << "Number of images: " << images.size( ) << std::endl;

    for ( size_t i = 0; i < images.size( ); ++i )
    {
        Mat descriptorsTmp;
        compute( images[i], keypoints[i], descriptorsTmp );
        descriptors.push_back( descriptorsTmp );
    }

    if ( verbose )
        std::cout << "number of keypoints: " << descriptors.rows << std::endl;

    // descriptor in floating point format (each bit is a float)
    Mat descriptorsFloat = Mat::zeros( descriptors.rows, 903, CV_32F );

    std::bitset< 1024 >* ptr
    = ( std::bitset< 1024 >* )( descriptors.data + ( descriptors.rows - 1 ) * descriptors.step[0] );
    for ( int m = descriptors.rows; m--; )
    {
        for ( int n = 903; n--; )
        {
            if ( ptr->test( n ) == true )
                descriptorsFloat.at< float >( m, n ) = 1.0f;
        }
        --ptr;
    }

    std::vector< PairStat > pairStat;
    for ( int n = 903; n--; )
    {
        // the higher the variance, the better --> mean = 0.5
        PairStat tmp = { fabs( mean( descriptorsFloat.col( n ) )[0] - 0.5 ), n };
        pairStat.push_back( tmp );
    }

    std::sort( pairStat.begin( ), pairStat.end( ), sortMean( ) );

    std::vector< PairStat > bestPairs;
    for ( int m = 0; m < 903; ++m )
    {
        if ( verbose )
            std::cout << m << ":" << bestPairs.size( ) << " " << std::flush;
        double corrMax( 0 );

        for ( size_t n = 0; n < bestPairs.size( ); ++n )
        {
            int idxA = bestPairs[n].idx;
            int idxB = pairStat[m].idx;
            double corr( 0 );
            // compute correlation between 2 pairs
            corr = fabs( compareHist( descriptorsFloat.col( idxA ), descriptorsFloat.col( idxB ), HISTCMP_CORREL ) );

            if ( corr > corrMax )
            {
                corrMax = corr;
                if ( corrMax >= corrTresh )
                    break;
            }
        }

        if ( corrMax < corrTresh /*0.7*/ )
            bestPairs.push_back( pairStat[m] );

        if ( bestPairs.size( ) >= 512 )
        {
            if ( verbose )
                std::cout << m << std::endl;
            break;
        }
    }

    std::vector< int > idxBestPairs;
    if ( ( int )bestPairs.size( ) >= FREAK_NB_PAIRS )
    {
        for ( int i = 0; i < FREAK_NB_PAIRS; ++i )
            idxBestPairs.push_back( bestPairs[i].idx );
    }
    else
    {
        if ( verbose )
            std::cout << "correlation threshold too small (restrictive)" << std::endl;
        CV_Error( Error::StsError, "correlation threshold too small (restrictive)" );
    }
    extAll = false;
    return idxBestPairs;
}

// -------------------------------------------------
/* FREAK interface implementation */
SFREAK_Impl::SFREAK_Impl( bool _orientationNormalized,
                          bool _scaleNormalized,
                          float _patternScale,
                          int _nOctaves,
                          const std::vector< int >& _selectedPairs )
: orientationNormalized( _orientationNormalized )
, scaleNormalized( _scaleNormalized )
, patternScale( _patternScale )
, nOctaves( _nOctaves )
, extAll( false )
, patternScale0( 0.0 )
, nOctaves0( 0 )
, selectedPairs0( _selectedPairs )
{
}

SFREAK_Impl::SFREAK_Impl( std::string cam_file,
                          bool _orientationNormalized,
                          bool _scaleNormalized,
                          float _patternScale,
                          int _nOctaves,
                          const std::vector< int >& _selectedPairs )
: orientationNormalized( _orientationNormalized )
, scaleNormalized( _scaleNormalized )
, patternScale( _patternScale )
, nOctaves( _nOctaves )
, extAll( false )
, patternScale0( 0.0 )
, nOctaves0( 0 )
, selectedPairs0( _selectedPairs )
{

    loadCamera( cam_file );
}

SFREAK_Impl::~SFREAK_Impl( ) {}

int
SFREAK_Impl::descriptorSize( ) const
{
    return FREAK_NB_PAIRS / 8; // descriptor length in bytes
}

int
SFREAK_Impl::descriptorType( ) const
{
    return CV_8U;
}

int
SFREAK_Impl::defaultNorm( ) const
{
    return NORM_HAMMING;
}

Ptr< SFREAK >
SFREAK::create( bool orientationNormalized,
                bool scaleNormalized,
                float patternScale,
                int nOctaves,
                const std::vector< int >& selectedPairs )
{
    return makePtr< SFREAK_Impl >( orientationNormalized, //
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
    return makePtr< SFREAK_Impl >( orientationNormalized, //
                                   scaleNormalized,
                                   patternScale,
                                   nOctaves,
                                   selectedPairs );
}

} // END NAMESPACE CV

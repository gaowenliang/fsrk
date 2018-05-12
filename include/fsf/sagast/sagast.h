#ifndef AGAST_F_H
#define AGAST_F_H

#include <camera_model/camera_models/CameraFactory.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>

#include <opencv2/opencv.hpp>

namespace cv
{

// int
// double2int( double a )
//{
//    int b = int( a );
//    if ( a - b > 0.5 )
//        return ( b + 1 );
//    else
//        return b;
//}

/** @overload */

/** @brief Detects corners using the AGAST algorithm

@param image grayscale image where keypoints (corners) are detected.
@param keypoints keypoints detected on the image.
@param threshold threshold on difference between intensity of the central pixel and pixels
of a
circle around this pixel.
@param nonmaxSuppression if true, non-maximum suppression is applied to detected corners
(keypoints).
@param type one of the four neighborhoods as defined in the paper:
AgastFeatureDetector::AGAST_5_8, AgastFeatureDetector::AGAST_7_12d,
AgastFeatureDetector::AGAST_7_12s, AgastFeatureDetector::OAST_9_16

For non-Intel platforms, there is a tree optimised variant of AGAST with same numerical
results.
The 32-bit binary tree tables were generated automatically from original code using perl
script.
The perl script and examples of tree generation are placed in features2d/doc folder.
Detects corners using the AGAST algorithm by @cite mair2010_agast .

*/

#define CV_8SC12 CV_MAKETYPE( CV_8S, 12 )
#define CV_8SC16 CV_MAKETYPE( CV_8S, 16 )
#define CV_8SC24 CV_MAKETYPE( CV_8S, 24 )
#define CV_8SC32 CV_MAKETYPE( CV_8S, 32 )

#define CV_16SC12 CV_MAKETYPE( CV_8S, 12 )

#define CV_32SC12 CV_MAKETYPE( CV_32S, 12 )
#define CV_32SC24 CV_MAKETYPE( CV_32S, 24 )

typedef Vec< short, 12 > Vec12s;
typedef Vec< short, 16 > Vec16s;
typedef Vec< short, 24 > Vec24s;
typedef Vec< char, 3 > Vec3c;
typedef Vec< char, 4 > Vec4c;
typedef Vec< char, 12 > Vec12c;
typedef Vec< char, 16 > Vec16c;
typedef Vec< char, 24 > Vec24c;
typedef Vec< char, 32 > Vec32c;
typedef Vec< int, 12 > Vec12i;
typedef Vec< int, 24 > Vec24i;

class AgastDetector : public Feature2D
{
    public:
    enum
    {
        SAGAST_8           = 0,
        SAGAST_12d         = 1,
        SAGAST_12s         = 2,
        SAGAST_16          = 3,
        THRESHOLD          = 10000,
        NONMAX_SUPPRESSION = 10001,
    };

    static Ptr< AgastDetector >
    create( int threshold = 10, bool nonmaxSuppression = true, int type = AgastDetector::SAGAST_16 );

    bool loadCamera( std::string cam_file );
    bool loadMask( std::string file );
    bool loadMask( );

    void buildOffsetsTable( );
    bool saveOffsetsTable( std::string path );
    bool loadOffsetsTable( std::string path );

    void SAGAST2( InputArray image, std::vector< KeyPoint >& keypoints, int threshold, bool nonmaxSuppression, int type );

    virtual void setThreshold( int threshold ) = 0;
    virtual int getThreshold( ) const          = 0;

    virtual void setNonmaxSuppression( bool f ) = 0;
    virtual bool getNonmaxSuppression( ) const  = 0;

    virtual void setType( int type ) = 0;
    virtual int getType( ) const     = 0;

    private:
    void calc_8( InputArray _img, std::vector< KeyPoint >& keypoints, int threshold );
    bool loadOffsetsTable_8( std::string path );
    bool saveOffsetsTable_8( std::string path );
    void getOffsets_8( short pixel[16], short rowStride, int xx, int yy );
    void buildOffsetsTable_8( );

    void calc_12d( InputArray _img, std::vector< KeyPoint >& keypoints, int threshold );
    bool loadOffsetsTable_12d( std::string path );
    bool saveOffsetsTable_12d( std::string path );
    void getOffsets_12d( short pixel[16], short rowStride, int xx, int yy );
    void buildOffsetsTable_12d( );

    void calc_12s( InputArray _img, std::vector< KeyPoint >& keypoints, int threshold );
    bool loadOffsetsTable_12s( std::string path );
    bool saveOffsetsTable_12s( std::string path );
    void getOffsets_12s( short pixel[16], short rowStride, int xx, int yy );
    void buildOffsetsTable_12s( );

    void calc_16( InputArray _img, std::vector< KeyPoint >& keypoints, int threshold );
    bool loadOffsetsTable_16( std::string path );
    bool saveOffsetsTable_16( std::string path );
    void getOffsets_16( short pixel[16], short rowStride, int xx, int yy );
    void buildOffsetsTable_16( );

    void getOffsets( short pixel[16], short rowStride, int type, int xx, int yy );
    void getOffsets( int pixel[16], int rowStride, int type );
    Eigen::Vector2d calcAngle( const camera_model::CameraPtr cam, const Eigen::Vector2d pu );
    int double2int( double a );

    public:
    cv::Point2f image_center;
    camera_model::CameraPtr cam;
    cv::Mat m_tableOffsets;
    cv::Mat m_mask;
};
}

#endif // AGAST_F_H


#include "fsf/sagast/sagast.h"
#include "fsf/sagast/score/sagast_score.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;

void
AgastDetector::getOffsets( int pixel[16], int rowStride, int type )
{
    static const int offsets16[][2] = { { -3, 0 },  //
                                        { -3, -1 }, //
                                        { -2, -2 }, //
                                        { -1, -3 }, //
                                        { 0, -3 },  //
                                        { 1, -3 },  //
                                        { 2, -2 },  //
                                        { 3, -1 },  //
                                        { 3, 0 },   //
                                        { 3, 1 },   //
                                        { 2, 2 },   //
                                        { 1, 3 },   //
                                        { 0, 3 },   //
                                        { -1, 3 },  //
                                        { -2, 2 },  //
                                        { -3, 1 } };

    static const int offsets12d[][2] = { { -3, 0 },  //
                                         { -2, -1 }, //
                                         { -1, -2 }, //
                                         { 0, -3 },  //
                                         { 1, -2 },  //
                                         { 2, -1 },  //
                                         { 3, 0 },   //
                                         { 2, 1 },   //
                                         { 1, 2 },   //
                                         { 0, 3 },   //
                                         { -1, 2 },  //
                                         { -2, 1 } };

    static const int offsets12s[][2] = { { -2, 0 },  //
                                         { -2, -1 }, //
                                         { -1, -2 }, //
                                         { 0, -2 },  //
                                         { 1, -2 },  //
                                         { 2, -1 },  //
                                         { 2, 0 },   //
                                         { 2, 1 },   //
                                         { 1, 2 },   //
                                         { 0, 2 },   //
                                         { -1, 2 },  //
                                         { -2, 1 } };

    static const int offsets8[][2] = { { -1, 0 },  //
                                       { -1, -1 }, //
                                       { 0, -1 },  //
                                       { 1, -1 },  //
                                       { 1, 0 },   //
                                       { 1, 1 },   //
                                       { 0, 1 },   //
                                       { -1, 1 } };

    const int( *offsets )[2] = type == AgastDetector::SAGAST_16 ?
                               offsets16 :
                               type == AgastDetector::SAGAST_12d ?
                               offsets12d :
                               type == AgastDetector::SAGAST_12s ?
                               offsets12s :
                               type == AgastDetector::SAGAST_8 ? offsets8 : 0;

    const int offsets_len
    = type == AgastDetector::SAGAST_16 ?
      16 :
      type == AgastDetector::SAGAST_12d ?
      12 :
      type == AgastDetector::SAGAST_12s ? 12 : type == AgastDetector::SAGAST_8 ? 8 : 0;

    CV_Assert( pixel && offsets );

    int k = 0;
    for ( ; k < offsets_len; k++ )
        pixel[k] = offsets[k][0] + offsets[k][1] * rowStride;
}

int
AgastDetector::double2int( double a )
{
    if ( 0 )
    {
        int abs_a = round( abs( a ) );

        return a > 0 ? abs_a : ( -abs_a );
    }

    if ( 1 )
    {
        double abs_a = abs( a );
        int int_a    = int( abs_a );
        if ( abs_a - int_a >= 0.3 )
            return a > 0 ? ( abs_a + 1 ) : ( -( abs_a + 1 ) );
        else
            return a > 0 ? int_a : ( -int_a );
    }
}

Eigen::Vector2d
AgastDetector::calcAngle( const camera_model::CameraPtr cam, const Eigen::Vector2d pu )
{
    Eigen::Vector3d P;
    cam->liftSphere( pu, P );

    double theta = acos( P( 2 ) / P.norm( ) );
    double phi   = atan2( P( 1 ), P( 0 ) );

    Eigen::Vector2d angle( theta, phi );

    return angle;
}

void
AgastDetector::buildOffsetsTable( )
{
    switch ( getType( ) )
    {
        case AgastDetector::SAGAST_8:
        {
            buildOffsetsTable_8( );
            break;
        }
        case AgastDetector::SAGAST_12s:
        {
            buildOffsetsTable_12s( );
            break;
        }
        case AgastDetector::SAGAST_12d:
        {
            buildOffsetsTable_12d( );
            break;
        }
        case AgastDetector::SAGAST_16:
        {
            buildOffsetsTable_16( );
            break;
        }
    }
}

void
AgastDetector::getOffsets( short pixel[16], short rowStride, int type, int xx, int yy )
{
    switch ( type )
    {
        case AgastDetector::SAGAST_8:
        {
            getOffsets_8( pixel, rowStride, xx, yy );
            break;
        }
        case AgastDetector::SAGAST_12s:
        {
            getOffsets_12s( pixel, rowStride, xx, yy );
            break;
        }
        case AgastDetector::SAGAST_12d:
        {
            getOffsets_12d( pixel, rowStride, xx, yy );
            break;
        }
        case AgastDetector::SAGAST_16:
        {
            getOffsets_16( pixel, rowStride, xx, yy );
            break;
        }
    }
}

bool
AgastDetector::saveOffsetsTable( std::string path )
{
    bool done;
    switch ( getType( ) )
    {
        case AgastDetector::SAGAST_8:
        {
            done = saveOffsetsTable_8( path );
            break;
        }
        case AgastDetector::SAGAST_12s:
        {
            done = saveOffsetsTable_12s( path );
            break;
        }
        case AgastDetector::SAGAST_12d:
        {
            done = saveOffsetsTable_12d( path );
            break;
        }
        case AgastDetector::SAGAST_16:
        {
            done = saveOffsetsTable_16( path );
            break;
        }
    }

    return done;
}

bool
AgastDetector::loadOffsetsTable( std::string path )
{
    bool done;
    switch ( getType( ) )
    {
        case AgastDetector::SAGAST_8:
        {
            done = loadOffsetsTable_8( path );
            break;
        }
        case AgastDetector::SAGAST_12s:
        {
            done = loadOffsetsTable_12s( path );
            break;
        }
        case AgastDetector::SAGAST_12d:
        {
            done = loadOffsetsTable_12d( path );
            break;
        }
        case AgastDetector::SAGAST_16:
        {
            done = loadOffsetsTable_16( path );
            break;
        }
    }

    return done;
}


#include "fsrk/sagast/sagast.h"
#include "fsrk/sagast/sagast_core/AGAST_ALL.h"
#include "fsrk/sagast/score/sagast_score.hpp"

using namespace cv;

#if ( defined __i386__ || defined( _M_IX86 ) || defined __x86_64__ || defined( _M_X64 ) )

void
AgastDetector::calc_8( InputArray _img, std::vector< KeyPoint >& keypoints, int threshold )
{

    cv::Mat img;
    if ( !_img.getMat( ).isContinuous( ) )
        img = _img.getMat( ).clone( );
    else
        img = _img.getMat( );

    size_t total            = 0;
    int xsize               = img.cols;
    int ysize               = img.rows;
    size_t nExpectedCorners = keypoints.capacity( );
    int x, y;
    int xsizeB = xsize - 2;
    int ysizeB = ysize - 1;
    int width;

    keypoints.resize( 0 );

    short pixel_5_8_[16];
    short offset0;
    short offset1;
    short offset2;
    short offset3;
    short offset4;
    short offset5;
    short offset6;
    short offset7;

    width = xsize;

    for ( y = 1; y < ysizeB; y++ )
    {
        x = 0;
        while ( true )
        {
        homogeneous:
        {
            x++;
            if ( x > xsizeB )
                break;
            else
            {

                const unsigned char* const mask = m_mask.ptr( ) + y * width + x;
                if ( mask[0] < 20 )
                    continue;

                const unsigned char* const ptr = img.ptr( ) + y * width + x;

                const int cb  = *ptr + threshold;
                const int c_b = *ptr - threshold;

                getOffsets_8( pixel_5_8_, ( short )img.step, x, y );

                offset0 = pixel_5_8_[0];
                offset1 = pixel_5_8_[1];
                offset2 = pixel_5_8_[2];
                offset3 = pixel_5_8_[3];
                offset4 = pixel_5_8_[4];
                offset5 = pixel_5_8_[5];
                offset6 = pixel_5_8_[6];
                offset7 = pixel_5_8_[7];

                if ( ptr[offset0] > cb )
                    if ( ptr[offset2] > cb )
                        if ( ptr[offset3] > cb )
                            if ( ptr[offset5] > cb )
                                if ( ptr[offset1] > cb )
                                    if ( ptr[offset4] > cb )
                                        goto success_structured;
                                    else if ( ptr[offset7] > cb )
                                        goto success_structured;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset4] > cb )
                                    if ( ptr[offset6] > cb )
                                        goto success_structured;
                                    else
                                        goto homogeneous;
                                else
                                    goto homogeneous;
                            else if ( ptr[offset1] > cb )
                                if ( ptr[offset4] > cb )
                                    goto success_homogeneous;
                                else if ( ptr[offset7] > cb )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else if ( ptr[offset7] > cb )
                            if ( ptr[offset6] > cb )
                                if ( ptr[offset5] > cb )
                                    if ( ptr[offset1] > cb )
                                        goto success_structured;
                                    else if ( ptr[offset4] > cb )
                                        goto success_structured;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset1] > cb )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else if ( ptr[offset5] < c_b )
                            if ( ptr[offset3] < c_b )
                                if ( ptr[offset7] < c_b )
                                    if ( ptr[offset4] < c_b )
                                        if ( ptr[offset6] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto homogeneous;
                    else if ( ptr[offset5] > cb )
                        if ( ptr[offset7] > cb )
                            if ( ptr[offset6] > cb )
                                if ( ptr[offset1] > cb )
                                    goto success_homogeneous;
                                else if ( ptr[offset4] > cb )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto homogeneous;
                    else if ( ptr[offset5] < c_b )
                        if ( ptr[offset3] < c_b )
                            if ( ptr[offset2] < c_b )
                                if ( ptr[offset1] < c_b )
                                    if ( ptr[offset4] < c_b )
                                        goto success_structured;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset4] < c_b )
                                    if ( ptr[offset6] < c_b )
                                        goto success_structured;
                                    else
                                        goto homogeneous;
                                else
                                    goto homogeneous;
                            else if ( ptr[offset7] < c_b )
                                if ( ptr[offset4] < c_b )
                                    if ( ptr[offset6] < c_b )
                                        goto success_structured;
                                    else
                                        goto homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto homogeneous;
                    else
                        goto homogeneous;
                else if ( ptr[offset0] < c_b )
                    if ( ptr[offset2] < c_b )
                        if ( ptr[offset7] > cb )
                            if ( ptr[offset3] < c_b )
                                if ( ptr[offset5] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        if ( ptr[offset4] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset4] < c_b )
                                        if ( ptr[offset6] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset1] < c_b )
                                    if ( ptr[offset4] < c_b )
                                        goto success_structured;
                                    else
                                        goto homogeneous;
                                else
                                    goto homogeneous;
                            else if ( ptr[offset5] > cb )
                                if ( ptr[offset3] > cb )
                                    if ( ptr[offset4] > cb )
                                        if ( ptr[offset6] > cb )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else if ( ptr[offset7] < c_b )
                            if ( ptr[offset3] < c_b )
                                if ( ptr[offset5] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        goto success_structured;
                                    else if ( ptr[offset4] < c_b )
                                        if ( ptr[offset6] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset1] < c_b )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else if ( ptr[offset6] < c_b )
                                if ( ptr[offset5] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        goto success_structured;
                                    else if ( ptr[offset4] < c_b )
                                        goto success_structured;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset1] < c_b )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else if ( ptr[offset3] < c_b )
                            if ( ptr[offset5] < c_b )
                                if ( ptr[offset1] < c_b )
                                    if ( ptr[offset4] < c_b )
                                        goto success_structured;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset4] < c_b )
                                    if ( ptr[offset6] < c_b )
                                        goto success_structured;
                                    else
                                        goto homogeneous;
                                else
                                    goto homogeneous;
                            else if ( ptr[offset1] < c_b )
                                if ( ptr[offset4] < c_b )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto homogeneous;
                    else if ( ptr[offset5] > cb )
                        if ( ptr[offset3] > cb )
                            if ( ptr[offset2] > cb )
                                if ( ptr[offset1] > cb )
                                    if ( ptr[offset4] > cb )
                                        goto success_structured;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset4] > cb )
                                    if ( ptr[offset6] > cb )
                                        goto success_structured;
                                    else
                                        goto homogeneous;
                                else
                                    goto homogeneous;
                            else if ( ptr[offset7] > cb )
                                if ( ptr[offset4] > cb )
                                    if ( ptr[offset6] > cb )
                                        goto success_structured;
                                    else
                                        goto homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto homogeneous;
                    else if ( ptr[offset5] < c_b )
                        if ( ptr[offset7] < c_b )
                            if ( ptr[offset6] < c_b )
                                if ( ptr[offset1] < c_b )
                                    goto success_homogeneous;
                                else if ( ptr[offset4] < c_b )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto homogeneous;
                    else
                        goto homogeneous;
                else if ( ptr[offset3] > cb )
                    if ( ptr[offset5] > cb )
                        if ( ptr[offset2] > cb )
                            if ( ptr[offset1] > cb )
                                if ( ptr[offset4] > cb )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else if ( ptr[offset4] > cb )
                                if ( ptr[offset6] > cb )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else if ( ptr[offset7] > cb )
                            if ( ptr[offset4] > cb )
                                if ( ptr[offset6] > cb )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto homogeneous;
                    else
                        goto homogeneous;
                else if ( ptr[offset3] < c_b )
                    if ( ptr[offset5] < c_b )
                        if ( ptr[offset2] < c_b )
                            if ( ptr[offset1] < c_b )
                                if ( ptr[offset4] < c_b )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else if ( ptr[offset4] < c_b )
                                if ( ptr[offset6] < c_b )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else if ( ptr[offset7] < c_b )
                            if ( ptr[offset4] < c_b )
                                if ( ptr[offset6] < c_b )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto homogeneous;
                    else
                        goto homogeneous;
                else
                    goto homogeneous;
            }
        }
        structured:
        {
            x++;
            if ( x > xsizeB )
                break;
            else
            {
                const unsigned char* const mask = m_mask.ptr( ) + y * width + x;
                if ( mask[0] < 20 )
                    continue;

                const unsigned char* const ptr = img.ptr( ) + y * width + x;

                const int cb  = *ptr + threshold;
                const int c_b = *ptr - threshold;

                getOffsets_8( pixel_5_8_, ( short )img.step, x, y );

                offset0 = pixel_5_8_[0];
                offset1 = pixel_5_8_[1];
                offset2 = pixel_5_8_[2];
                offset3 = pixel_5_8_[3];
                offset4 = pixel_5_8_[4];
                offset5 = pixel_5_8_[5];
                offset6 = pixel_5_8_[6];
                offset7 = pixel_5_8_[7];

                if ( ptr[offset0] > cb )
                    if ( ptr[offset2] > cb )
                        if ( ptr[offset3] > cb )
                            if ( ptr[offset5] > cb )
                                if ( ptr[offset7] > cb )
                                    if ( ptr[offset1] > cb )
                                        goto success_structured;
                                    else if ( ptr[offset4] > cb )
                                        if ( ptr[offset6] > cb )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset1] > cb )
                                    if ( ptr[offset4] > cb )
                                        goto success_structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset4] > cb )
                                    if ( ptr[offset6] > cb )
                                        goto success_structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else if ( ptr[offset7] > cb )
                                if ( ptr[offset1] > cb )
                                    goto success_structured;
                                else
                                    goto structured;
                            else if ( ptr[offset1] > cb )
                                if ( ptr[offset4] > cb )
                                    goto success_structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else if ( ptr[offset7] > cb )
                            if ( ptr[offset6] > cb )
                                if ( ptr[offset5] > cb )
                                    if ( ptr[offset1] > cb )
                                        goto success_structured;
                                    else if ( ptr[offset4] > cb )
                                        goto success_structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset1] > cb )
                                    goto success_structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else if ( ptr[offset5] < c_b )
                            if ( ptr[offset3] < c_b )
                                if ( ptr[offset7] < c_b )
                                    if ( ptr[offset4] < c_b )
                                        if ( ptr[offset6] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto structured;
                    else if ( ptr[offset5] > cb )
                        if ( ptr[offset7] > cb )
                            if ( ptr[offset6] > cb )
                                if ( ptr[offset1] > cb )
                                    goto success_structured;
                                else if ( ptr[offset4] > cb )
                                    goto success_structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto structured;
                    else if ( ptr[offset5] < c_b )
                        if ( ptr[offset3] < c_b )
                            if ( ptr[offset2] < c_b )
                                if ( ptr[offset1] < c_b )
                                    if ( ptr[offset4] < c_b )
                                        goto success_structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset4] < c_b )
                                    if ( ptr[offset6] < c_b )
                                        goto success_structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else if ( ptr[offset7] < c_b )
                                if ( ptr[offset4] < c_b )
                                    if ( ptr[offset6] < c_b )
                                        goto success_homogeneous;
                                    else
                                        goto homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto structured;
                    else
                        goto homogeneous;
                else if ( ptr[offset0] < c_b )
                    if ( ptr[offset2] < c_b )
                        if ( ptr[offset7] > cb )
                            if ( ptr[offset3] < c_b )
                                if ( ptr[offset5] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        if ( ptr[offset4] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset4] < c_b )
                                        if ( ptr[offset6] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset1] < c_b )
                                    if ( ptr[offset4] < c_b )
                                        goto success_structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else if ( ptr[offset5] > cb )
                                if ( ptr[offset3] > cb )
                                    if ( ptr[offset4] > cb )
                                        if ( ptr[offset6] > cb )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto homogeneous;
                            else
                                goto structured;
                        else if ( ptr[offset7] < c_b )
                            if ( ptr[offset3] < c_b )
                                if ( ptr[offset5] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        goto success_structured;
                                    else if ( ptr[offset4] < c_b )
                                        if ( ptr[offset6] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset1] < c_b )
                                    goto success_structured;
                                else
                                    goto structured;
                            else if ( ptr[offset6] < c_b )
                                if ( ptr[offset5] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        goto success_structured;
                                    else if ( ptr[offset4] < c_b )
                                        goto success_structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset1] < c_b )
                                    goto success_structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else if ( ptr[offset3] < c_b )
                            if ( ptr[offset5] < c_b )
                                if ( ptr[offset1] < c_b )
                                    if ( ptr[offset4] < c_b )
                                        goto success_homogeneous;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset4] < c_b )
                                    if ( ptr[offset6] < c_b )
                                        goto success_homogeneous;
                                    else
                                        goto homogeneous;
                                else
                                    goto homogeneous;
                            else if ( ptr[offset1] < c_b )
                                if ( ptr[offset4] < c_b )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto homogeneous;
                    else if ( ptr[offset5] > cb )
                        if ( ptr[offset3] > cb )
                            if ( ptr[offset2] > cb )
                                if ( ptr[offset1] > cb )
                                    if ( ptr[offset4] > cb )
                                        goto success_structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset4] > cb )
                                    if ( ptr[offset6] > cb )
                                        goto success_structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else if ( ptr[offset7] > cb )
                                if ( ptr[offset4] > cb )
                                    if ( ptr[offset6] > cb )
                                        goto success_homogeneous;
                                    else
                                        goto homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto structured;
                    else if ( ptr[offset5] < c_b )
                        if ( ptr[offset7] < c_b )
                            if ( ptr[offset6] < c_b )
                                if ( ptr[offset1] < c_b )
                                    goto success_structured;
                                else if ( ptr[offset4] < c_b )
                                    goto success_structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto structured;
                    else
                        goto homogeneous;
                else if ( ptr[offset3] > cb )
                    if ( ptr[offset5] > cb )
                        if ( ptr[offset2] > cb )
                            if ( ptr[offset1] > cb )
                                if ( ptr[offset4] > cb )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else if ( ptr[offset4] > cb )
                                if ( ptr[offset6] > cb )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else if ( ptr[offset7] > cb )
                            if ( ptr[offset4] > cb )
                                if ( ptr[offset6] > cb )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto homogeneous;
                    else
                        goto homogeneous;
                else if ( ptr[offset3] < c_b )
                    if ( ptr[offset5] < c_b )
                        if ( ptr[offset2] < c_b )
                            if ( ptr[offset1] < c_b )
                                if ( ptr[offset4] < c_b )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else if ( ptr[offset4] < c_b )
                                if ( ptr[offset6] < c_b )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else if ( ptr[offset7] < c_b )
                            if ( ptr[offset4] < c_b )
                                if ( ptr[offset6] < c_b )
                                    goto success_homogeneous;
                                else
                                    goto homogeneous;
                            else
                                goto homogeneous;
                        else
                            goto homogeneous;
                    else
                        goto homogeneous;
                else
                    goto homogeneous;
            }
        }
        success_homogeneous:
            if ( total == nExpectedCorners )
            {
                if ( nExpectedCorners == 0 )
                {
                    nExpectedCorners = 512;
                    keypoints.reserve( nExpectedCorners );
                }
                else
                {
                    nExpectedCorners *= 2;
                    keypoints.reserve( nExpectedCorners );
                }
            }
            keypoints.push_back( KeyPoint( Point2f( ( float )x, ( float )y ), 7.0f ) );
            total++;
            goto homogeneous;
        success_structured:
            if ( total == nExpectedCorners )
            {
                if ( nExpectedCorners == 0 )
                {
                    nExpectedCorners = 512;
                    keypoints.reserve( nExpectedCorners );
                }
                else
                {
                    nExpectedCorners *= 2;
                    keypoints.reserve( nExpectedCorners );
                }
            }
            keypoints.push_back( KeyPoint( Point2f( ( float )x, ( float )y ), 7.0f ) );
            total++;
            goto structured;
        }
    }
}

#endif // (defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64))

#if !( defined __i386__ || defined( _M_IX86 ) || defined __x86_64__ || defined( _M_X64 ) )

static void
AGAST_5_8( InputArray _img, std::vector< KeyPoint >& keypoints, int threshold )
{
    AGAST_ALL( _img, keypoints, threshold, AgastDetector::AGAST_5_8 );
}

#endif // (defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64))

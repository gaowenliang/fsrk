
#include "fsrk/sagast/sagast.h"
#include "fsrk/sagast/sagast_core/AGAST_ALL.h"
#include "fsrk/sagast/score/sagast_score.hpp"

using namespace cv;

#if ( defined __i386__ || defined( _M_IX86 ) || defined __x86_64__ || defined( _M_X64 ) )

void
AgastDetector::calc_16( InputArray _img, std::vector< KeyPoint >& keypoints, int threshold )
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
    int xsizeB = xsize - 4;
    int ysizeB = ysize - 3;
    int width;

    keypoints.resize( 0 );

    short pixel_9_16_[16];

    short offset0;
    short offset1;
    short offset2;
    short offset3;
    short offset4;
    short offset5;
    short offset6;
    short offset7;
    short offset8;
    short offset9;
    short offset10;
    short offset11;
    short offset12;
    short offset13;
    short offset14;
    short offset15;

    width = xsize;

    for ( y = 3; y < ysizeB; y++ )
    {
        x = 2;
        while ( true )
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

                getOffsets_16( pixel_9_16_, ( int )img.step, x, y );

                offset0  = pixel_9_16_[0];
                offset1  = pixel_9_16_[1];
                offset2  = pixel_9_16_[2];
                offset3  = pixel_9_16_[3];
                offset4  = pixel_9_16_[4];
                offset5  = pixel_9_16_[5];
                offset6  = pixel_9_16_[6];
                offset7  = pixel_9_16_[7];
                offset8  = pixel_9_16_[8];
                offset9  = pixel_9_16_[9];
                offset10 = pixel_9_16_[10];
                offset11 = pixel_9_16_[11];
                offset12 = pixel_9_16_[12];
                offset13 = pixel_9_16_[13];
                offset14 = pixel_9_16_[14];
                offset15 = pixel_9_16_[15];

                if ( ptr[offset0] > cb )
                    if ( ptr[offset2] > cb )
                        if ( ptr[offset4] > cb )
                            if ( ptr[offset5] > cb )
                                if ( ptr[offset7] > cb )
                                    if ( ptr[offset3] > cb )
                                        if ( ptr[offset1] > cb )
                                            if ( ptr[offset6] > cb )
                                                if ( ptr[offset8] > cb )
                                                {
                                                }
                                                else if ( ptr[offset15] > cb )
                                                {
                                                }
                                                else
                                                    continue;
                                            else if ( ptr[offset13] > cb )
                                                if ( ptr[offset14] > cb )
                                                    if ( ptr[offset15] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset8] > cb )
                                            if ( ptr[offset9] > cb )
                                                if ( ptr[offset10] > cb )
                                                    if ( ptr[offset6] > cb )
                                                    {
                                                    }
                                                    else if ( ptr[offset11] > cb )
                                                        if ( ptr[offset12] > cb )
                                                            if ( ptr[offset13] > cb )
                                                                if ( ptr[offset14] > cb )
                                                                    if ( ptr[offset15] > cb )
                                                                    {
                                                                    }
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset10] > cb )
                                        if ( ptr[offset11] > cb )
                                            if ( ptr[offset12] > cb )
                                                if ( ptr[offset8] > cb )
                                                    if ( ptr[offset9] > cb )
                                                        if ( ptr[offset6] > cb )
                                                        {
                                                        }
                                                        else if ( ptr[offset13] > cb )
                                                            if ( ptr[offset14] > cb )
                                                                if ( ptr[offset15] > cb )
                                                                {
                                                                }
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else if ( ptr[offset1] > cb )
                                                        if ( ptr[offset13] > cb )
                                                            if ( ptr[offset14] > cb )
                                                                if ( ptr[offset15] > cb )
                                                                {
                                                                }
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if ( ptr[offset1] > cb )
                                                    if ( ptr[offset13] > cb )
                                                        if ( ptr[offset14] > cb )
                                                            if ( ptr[offset15] > cb )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if ( ptr[offset7] < c_b )
                                    if ( ptr[offset14] > cb )
                                        if ( ptr[offset15] > cb )
                                            if ( ptr[offset1] > cb )
                                                if ( ptr[offset3] > cb )
                                                    if ( ptr[offset6] > cb )
                                                    {
                                                    }
                                                    else if ( ptr[offset13] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else if ( ptr[offset10] > cb )
                                                    if ( ptr[offset11] > cb )
                                                        if ( ptr[offset12] > cb )
                                                            if ( ptr[offset13] > cb )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset8] > cb )
                                                if ( ptr[offset9] > cb )
                                                    if ( ptr[offset10] > cb )
                                                        if ( ptr[offset11] > cb )
                                                            if ( ptr[offset12] > cb )
                                                                if ( ptr[offset13] > cb )
                                                                {
                                                                }
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset14] < c_b )
                                        if ( ptr[offset8] < c_b )
                                            if ( ptr[offset9] < c_b )
                                                if ( ptr[offset10] < c_b )
                                                    if ( ptr[offset11] < c_b )
                                                        if ( ptr[offset12] < c_b )
                                                            if ( ptr[offset13] < c_b )
                                                                if ( ptr[offset6] < c_b )
                                                                {
                                                                }
                                                                else if ( ptr[offset15] < c_b )
                                                                {
                                                                }
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if ( ptr[offset14] > cb )
                                    if ( ptr[offset15] > cb )
                                        if ( ptr[offset1] > cb )
                                            if ( ptr[offset3] > cb )
                                                if ( ptr[offset6] > cb )
                                                {
                                                }
                                                else if ( ptr[offset13] > cb )
                                                {
                                                }
                                                else
                                                    continue;
                                            else if ( ptr[offset10] > cb )
                                                if ( ptr[offset11] > cb )
                                                    if ( ptr[offset12] > cb )
                                                        if ( ptr[offset13] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset8] > cb )
                                            if ( ptr[offset9] > cb )
                                                if ( ptr[offset10] > cb )
                                                    if ( ptr[offset11] > cb )
                                                        if ( ptr[offset12] > cb )
                                                            if ( ptr[offset13] > cb )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if ( ptr[offset5] < c_b )
                                if ( ptr[offset12] > cb )
                                    if ( ptr[offset13] > cb )
                                        if ( ptr[offset14] > cb )
                                            if ( ptr[offset15] > cb )
                                                if ( ptr[offset1] > cb )
                                                    if ( ptr[offset3] > cb )
                                                    {
                                                    }
                                                    else if ( ptr[offset10] > cb )
                                                        if ( ptr[offset11] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if ( ptr[offset8] > cb )
                                                    if ( ptr[offset9] > cb )
                                                        if ( ptr[offset10] > cb )
                                                            if ( ptr[offset11] > cb )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset6] > cb )
                                                if ( ptr[offset7] > cb )
                                                    if ( ptr[offset8] > cb )
                                                        if ( ptr[offset9] > cb )
                                                            if ( ptr[offset10] > cb )
                                                                if ( ptr[offset11] > cb )
                                                                {
                                                                }
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if ( ptr[offset12] < c_b )
                                    if ( ptr[offset7] < c_b )
                                        if ( ptr[offset8] < c_b )
                                            if ( ptr[offset9] < c_b )
                                                if ( ptr[offset10] < c_b )
                                                    if ( ptr[offset11] < c_b )
                                                        if ( ptr[offset13] < c_b )
                                                            if ( ptr[offset6] < c_b )
                                                            {
                                                            }
                                                            else if ( ptr[offset14] < c_b )
                                                                if ( ptr[offset15] < c_b )
                                                                {
                                                                }
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if ( ptr[offset12] > cb )
                                if ( ptr[offset13] > cb )
                                    if ( ptr[offset14] > cb )
                                        if ( ptr[offset15] > cb )
                                            if ( ptr[offset1] > cb )
                                                if ( ptr[offset3] > cb )
                                                {
                                                }
                                                else if ( ptr[offset10] > cb )
                                                    if ( ptr[offset11] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset8] > cb )
                                                if ( ptr[offset9] > cb )
                                                    if ( ptr[offset10] > cb )
                                                        if ( ptr[offset11] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset6] > cb )
                                            if ( ptr[offset7] > cb )
                                                if ( ptr[offset8] > cb )
                                                    if ( ptr[offset9] > cb )
                                                        if ( ptr[offset10] > cb )
                                                            if ( ptr[offset11] > cb )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if ( ptr[offset12] < c_b )
                                if ( ptr[offset7] < c_b )
                                    if ( ptr[offset8] < c_b )
                                        if ( ptr[offset9] < c_b )
                                            if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    if ( ptr[offset13] < c_b )
                                                        if ( ptr[offset14] < c_b )
                                                            if ( ptr[offset6] < c_b )
                                                            {
                                                            }
                                                            else if ( ptr[offset15] < c_b )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if ( ptr[offset4] < c_b )
                            if ( ptr[offset11] > cb )
                                if ( ptr[offset12] > cb )
                                    if ( ptr[offset13] > cb )
                                        if ( ptr[offset10] > cb )
                                            if ( ptr[offset14] > cb )
                                                if ( ptr[offset15] > cb )
                                                    if ( ptr[offset1] > cb )
                                                    {
                                                    }
                                                    else if ( ptr[offset8] > cb )
                                                        if ( ptr[offset9] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if ( ptr[offset6] > cb )
                                                    if ( ptr[offset7] > cb )
                                                        if ( ptr[offset8] > cb )
                                                            if ( ptr[offset9] > cb )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset5] > cb )
                                                if ( ptr[offset6] > cb )
                                                    if ( ptr[offset7] > cb )
                                                        if ( ptr[offset8] > cb )
                                                            if ( ptr[offset9] > cb )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset1] > cb )
                                            if ( ptr[offset3] > cb )
                                                if ( ptr[offset14] > cb )
                                                    if ( ptr[offset15] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if ( ptr[offset11] < c_b )
                                if ( ptr[offset7] < c_b )
                                    if ( ptr[offset8] < c_b )
                                        if ( ptr[offset9] < c_b )
                                            if ( ptr[offset10] < c_b )
                                                if ( ptr[offset6] < c_b )
                                                    if ( ptr[offset5] < c_b )
                                                        if ( ptr[offset3] < c_b )
                                                        {
                                                        }
                                                        else if ( ptr[offset12] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else if ( ptr[offset12] < c_b )
                                                        if ( ptr[offset13] < c_b )
                                                            if ( ptr[offset14] < c_b )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if ( ptr[offset12] < c_b )
                                                    if ( ptr[offset13] < c_b )
                                                        if ( ptr[offset14] < c_b )
                                                            if ( ptr[offset15] < c_b )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if ( ptr[offset11] > cb )
                            if ( ptr[offset12] > cb )
                                if ( ptr[offset13] > cb )
                                    if ( ptr[offset10] > cb )
                                        if ( ptr[offset14] > cb )
                                            if ( ptr[offset15] > cb )
                                                if ( ptr[offset1] > cb )
                                                {
                                                }
                                                else if ( ptr[offset8] > cb )
                                                    if ( ptr[offset9] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset6] > cb )
                                                if ( ptr[offset7] > cb )
                                                    if ( ptr[offset8] > cb )
                                                        if ( ptr[offset9] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset5] > cb )
                                            if ( ptr[offset6] > cb )
                                                if ( ptr[offset7] > cb )
                                                    if ( ptr[offset8] > cb )
                                                        if ( ptr[offset9] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset1] > cb )
                                        if ( ptr[offset3] > cb )
                                            if ( ptr[offset14] > cb )
                                                if ( ptr[offset15] > cb )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if ( ptr[offset11] < c_b )
                            if ( ptr[offset7] < c_b )
                                if ( ptr[offset8] < c_b )
                                    if ( ptr[offset9] < c_b )
                                        if ( ptr[offset10] < c_b )
                                            if ( ptr[offset12] < c_b )
                                                if ( ptr[offset13] < c_b )
                                                    if ( ptr[offset6] < c_b )
                                                        if ( ptr[offset5] < c_b )
                                                        {
                                                        }
                                                        else if ( ptr[offset14] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else if ( ptr[offset14] < c_b )
                                                        if ( ptr[offset15] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if ( ptr[offset2] < c_b )
                        if ( ptr[offset9] > cb )
                            if ( ptr[offset10] > cb )
                                if ( ptr[offset11] > cb )
                                    if ( ptr[offset8] > cb )
                                        if ( ptr[offset12] > cb )
                                            if ( ptr[offset13] > cb )
                                                if ( ptr[offset14] > cb )
                                                    if ( ptr[offset15] > cb )
                                                    {
                                                    }
                                                    else if ( ptr[offset6] > cb )
                                                        if ( ptr[offset7] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if ( ptr[offset5] > cb )
                                                    if ( ptr[offset6] > cb )
                                                        if ( ptr[offset7] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset4] > cb )
                                                if ( ptr[offset5] > cb )
                                                    if ( ptr[offset6] > cb )
                                                        if ( ptr[offset7] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset3] > cb )
                                            if ( ptr[offset4] > cb )
                                                if ( ptr[offset5] > cb )
                                                    if ( ptr[offset6] > cb )
                                                        if ( ptr[offset7] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset1] > cb )
                                        if ( ptr[offset12] > cb )
                                            if ( ptr[offset13] > cb )
                                                if ( ptr[offset14] > cb )
                                                    if ( ptr[offset15] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if ( ptr[offset9] < c_b )
                            if ( ptr[offset7] < c_b )
                                if ( ptr[offset8] < c_b )
                                    if ( ptr[offset6] < c_b )
                                        if ( ptr[offset5] < c_b )
                                            if ( ptr[offset4] < c_b )
                                                if ( ptr[offset3] < c_b )
                                                    if ( ptr[offset1] < c_b )
                                                    {
                                                    }
                                                    else if ( ptr[offset10] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else if ( ptr[offset10] < c_b )
                                                    if ( ptr[offset11] < c_b )
                                                        if ( ptr[offset12] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    if ( ptr[offset12] < c_b )
                                                        if ( ptr[offset13] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset10] < c_b )
                                            if ( ptr[offset11] < c_b )
                                                if ( ptr[offset12] < c_b )
                                                    if ( ptr[offset13] < c_b )
                                                        if ( ptr[offset14] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset10] < c_b )
                                        if ( ptr[offset11] < c_b )
                                            if ( ptr[offset12] < c_b )
                                                if ( ptr[offset13] < c_b )
                                                    if ( ptr[offset14] < c_b )
                                                        if ( ptr[offset15] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if ( ptr[offset9] > cb )
                        if ( ptr[offset10] > cb )
                            if ( ptr[offset11] > cb )
                                if ( ptr[offset8] > cb )
                                    if ( ptr[offset12] > cb )
                                        if ( ptr[offset13] > cb )
                                            if ( ptr[offset14] > cb )
                                                if ( ptr[offset15] > cb )
                                                {
                                                }
                                                else if ( ptr[offset6] > cb )
                                                    if ( ptr[offset7] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset5] > cb )
                                                if ( ptr[offset6] > cb )
                                                    if ( ptr[offset7] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset4] > cb )
                                            if ( ptr[offset5] > cb )
                                                if ( ptr[offset6] > cb )
                                                    if ( ptr[offset7] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset3] > cb )
                                        if ( ptr[offset4] > cb )
                                            if ( ptr[offset5] > cb )
                                                if ( ptr[offset6] > cb )
                                                    if ( ptr[offset7] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if ( ptr[offset1] > cb )
                                    if ( ptr[offset12] > cb )
                                        if ( ptr[offset13] > cb )
                                            if ( ptr[offset14] > cb )
                                                if ( ptr[offset15] > cb )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if ( ptr[offset9] < c_b )
                        if ( ptr[offset7] < c_b )
                            if ( ptr[offset8] < c_b )
                                if ( ptr[offset10] < c_b )
                                    if ( ptr[offset11] < c_b )
                                        if ( ptr[offset6] < c_b )
                                            if ( ptr[offset5] < c_b )
                                                if ( ptr[offset4] < c_b )
                                                    if ( ptr[offset3] < c_b )
                                                    {
                                                    }
                                                    else if ( ptr[offset12] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else if ( ptr[offset12] < c_b )
                                                    if ( ptr[offset13] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset12] < c_b )
                                                if ( ptr[offset13] < c_b )
                                                    if ( ptr[offset14] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset12] < c_b )
                                            if ( ptr[offset13] < c_b )
                                                if ( ptr[offset14] < c_b )
                                                    if ( ptr[offset15] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else
                        continue;
                else if ( ptr[offset0] < c_b )
                    if ( ptr[offset2] > cb )
                        if ( ptr[offset9] > cb )
                            if ( ptr[offset7] > cb )
                                if ( ptr[offset8] > cb )
                                    if ( ptr[offset6] > cb )
                                        if ( ptr[offset5] > cb )
                                            if ( ptr[offset4] > cb )
                                                if ( ptr[offset3] > cb )
                                                    if ( ptr[offset1] > cb )
                                                    {
                                                    }
                                                    else if ( ptr[offset10] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else if ( ptr[offset10] > cb )
                                                    if ( ptr[offset11] > cb )
                                                        if ( ptr[offset12] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset10] > cb )
                                                if ( ptr[offset11] > cb )
                                                    if ( ptr[offset12] > cb )
                                                        if ( ptr[offset13] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset10] > cb )
                                            if ( ptr[offset11] > cb )
                                                if ( ptr[offset12] > cb )
                                                    if ( ptr[offset13] > cb )
                                                        if ( ptr[offset14] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset10] > cb )
                                        if ( ptr[offset11] > cb )
                                            if ( ptr[offset12] > cb )
                                                if ( ptr[offset13] > cb )
                                                    if ( ptr[offset14] > cb )
                                                        if ( ptr[offset15] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if ( ptr[offset9] < c_b )
                            if ( ptr[offset10] < c_b )
                                if ( ptr[offset11] < c_b )
                                    if ( ptr[offset8] < c_b )
                                        if ( ptr[offset12] < c_b )
                                            if ( ptr[offset13] < c_b )
                                                if ( ptr[offset14] < c_b )
                                                    if ( ptr[offset15] < c_b )
                                                    {
                                                    }
                                                    else if ( ptr[offset6] < c_b )
                                                        if ( ptr[offset7] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if ( ptr[offset5] < c_b )
                                                    if ( ptr[offset6] < c_b )
                                                        if ( ptr[offset7] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset4] < c_b )
                                                if ( ptr[offset5] < c_b )
                                                    if ( ptr[offset6] < c_b )
                                                        if ( ptr[offset7] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset3] < c_b )
                                            if ( ptr[offset4] < c_b )
                                                if ( ptr[offset5] < c_b )
                                                    if ( ptr[offset6] < c_b )
                                                        if ( ptr[offset7] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset1] < c_b )
                                        if ( ptr[offset12] < c_b )
                                            if ( ptr[offset13] < c_b )
                                                if ( ptr[offset14] < c_b )
                                                    if ( ptr[offset15] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if ( ptr[offset2] < c_b )
                        if ( ptr[offset4] > cb )
                            if ( ptr[offset11] > cb )
                                if ( ptr[offset7] > cb )
                                    if ( ptr[offset8] > cb )
                                        if ( ptr[offset9] > cb )
                                            if ( ptr[offset10] > cb )
                                                if ( ptr[offset6] > cb )
                                                    if ( ptr[offset5] > cb )
                                                        if ( ptr[offset3] > cb )
                                                        {
                                                        }
                                                        else if ( ptr[offset12] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else if ( ptr[offset12] > cb )
                                                        if ( ptr[offset13] > cb )
                                                            if ( ptr[offset14] > cb )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if ( ptr[offset12] > cb )
                                                    if ( ptr[offset13] > cb )
                                                        if ( ptr[offset14] > cb )
                                                            if ( ptr[offset15] > cb )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if ( ptr[offset11] < c_b )
                                if ( ptr[offset12] < c_b )
                                    if ( ptr[offset13] < c_b )
                                        if ( ptr[offset10] < c_b )
                                            if ( ptr[offset14] < c_b )
                                                if ( ptr[offset15] < c_b )
                                                    if ( ptr[offset1] < c_b )
                                                    {
                                                    }
                                                    else if ( ptr[offset8] < c_b )
                                                        if ( ptr[offset9] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if ( ptr[offset6] < c_b )
                                                    if ( ptr[offset7] < c_b )
                                                        if ( ptr[offset8] < c_b )
                                                            if ( ptr[offset9] < c_b )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset5] < c_b )
                                                if ( ptr[offset6] < c_b )
                                                    if ( ptr[offset7] < c_b )
                                                        if ( ptr[offset8] < c_b )
                                                            if ( ptr[offset9] < c_b )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset1] < c_b )
                                            if ( ptr[offset3] < c_b )
                                                if ( ptr[offset14] < c_b )
                                                    if ( ptr[offset15] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if ( ptr[offset4] < c_b )
                            if ( ptr[offset5] > cb )
                                if ( ptr[offset12] > cb )
                                    if ( ptr[offset7] > cb )
                                        if ( ptr[offset8] > cb )
                                            if ( ptr[offset9] > cb )
                                                if ( ptr[offset10] > cb )
                                                    if ( ptr[offset11] > cb )
                                                        if ( ptr[offset13] > cb )
                                                            if ( ptr[offset6] > cb )
                                                            {
                                                            }
                                                            else if ( ptr[offset14] > cb )
                                                                if ( ptr[offset15] > cb )
                                                                {
                                                                }
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if ( ptr[offset12] < c_b )
                                    if ( ptr[offset13] < c_b )
                                        if ( ptr[offset14] < c_b )
                                            if ( ptr[offset15] < c_b )
                                                if ( ptr[offset1] < c_b )
                                                    if ( ptr[offset3] < c_b )
                                                    {
                                                    }
                                                    else if ( ptr[offset10] < c_b )
                                                        if ( ptr[offset11] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if ( ptr[offset8] < c_b )
                                                    if ( ptr[offset9] < c_b )
                                                        if ( ptr[offset10] < c_b )
                                                            if ( ptr[offset11] < c_b )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset6] < c_b )
                                                if ( ptr[offset7] < c_b )
                                                    if ( ptr[offset8] < c_b )
                                                        if ( ptr[offset9] < c_b )
                                                            if ( ptr[offset10] < c_b )
                                                                if ( ptr[offset11] < c_b )
                                                                {
                                                                }
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if ( ptr[offset5] < c_b )
                                if ( ptr[offset7] > cb )
                                    if ( ptr[offset14] > cb )
                                        if ( ptr[offset8] > cb )
                                            if ( ptr[offset9] > cb )
                                                if ( ptr[offset10] > cb )
                                                    if ( ptr[offset11] > cb )
                                                        if ( ptr[offset12] > cb )
                                                            if ( ptr[offset13] > cb )
                                                                if ( ptr[offset6] > cb )
                                                                {
                                                                }
                                                                else if ( ptr[offset15] > cb )
                                                                {
                                                                }
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset14] < c_b )
                                        if ( ptr[offset15] < c_b )
                                            if ( ptr[offset1] < c_b )
                                                if ( ptr[offset3] < c_b )
                                                    if ( ptr[offset6] < c_b )
                                                    {
                                                    }
                                                    else if ( ptr[offset13] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else if ( ptr[offset10] < c_b )
                                                    if ( ptr[offset11] < c_b )
                                                        if ( ptr[offset12] < c_b )
                                                            if ( ptr[offset13] < c_b )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset8] < c_b )
                                                if ( ptr[offset9] < c_b )
                                                    if ( ptr[offset10] < c_b )
                                                        if ( ptr[offset11] < c_b )
                                                            if ( ptr[offset12] < c_b )
                                                                if ( ptr[offset13] < c_b )
                                                                {
                                                                }
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if ( ptr[offset7] < c_b )
                                    if ( ptr[offset3] < c_b )
                                        if ( ptr[offset1] < c_b )
                                            if ( ptr[offset6] < c_b )
                                                if ( ptr[offset8] < c_b )
                                                {
                                                }
                                                else if ( ptr[offset15] < c_b )
                                                {
                                                }
                                                else
                                                    continue;
                                            else if ( ptr[offset13] < c_b )
                                                if ( ptr[offset14] < c_b )
                                                    if ( ptr[offset15] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset8] < c_b )
                                            if ( ptr[offset9] < c_b )
                                                if ( ptr[offset10] < c_b )
                                                    if ( ptr[offset6] < c_b )
                                                    {
                                                    }
                                                    else if ( ptr[offset11] < c_b )
                                                        if ( ptr[offset12] < c_b )
                                                            if ( ptr[offset13] < c_b )
                                                                if ( ptr[offset14] < c_b )
                                                                    if ( ptr[offset15] < c_b )
                                                                    {
                                                                    }
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset10] < c_b )
                                        if ( ptr[offset11] < c_b )
                                            if ( ptr[offset12] < c_b )
                                                if ( ptr[offset8] < c_b )
                                                    if ( ptr[offset9] < c_b )
                                                        if ( ptr[offset6] < c_b )
                                                        {
                                                        }
                                                        else if ( ptr[offset13] < c_b )
                                                            if ( ptr[offset14] < c_b )
                                                                if ( ptr[offset15] < c_b )
                                                                {
                                                                }
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else if ( ptr[offset1] < c_b )
                                                        if ( ptr[offset13] < c_b )
                                                            if ( ptr[offset14] < c_b )
                                                                if ( ptr[offset15] < c_b )
                                                                {
                                                                }
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if ( ptr[offset1] < c_b )
                                                    if ( ptr[offset13] < c_b )
                                                        if ( ptr[offset14] < c_b )
                                                            if ( ptr[offset15] < c_b )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if ( ptr[offset14] < c_b )
                                    if ( ptr[offset15] < c_b )
                                        if ( ptr[offset1] < c_b )
                                            if ( ptr[offset3] < c_b )
                                                if ( ptr[offset6] < c_b )
                                                {
                                                }
                                                else if ( ptr[offset13] < c_b )
                                                {
                                                }
                                                else
                                                    continue;
                                            else if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    if ( ptr[offset12] < c_b )
                                                        if ( ptr[offset13] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset8] < c_b )
                                            if ( ptr[offset9] < c_b )
                                                if ( ptr[offset10] < c_b )
                                                    if ( ptr[offset11] < c_b )
                                                        if ( ptr[offset12] < c_b )
                                                            if ( ptr[offset13] < c_b )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if ( ptr[offset12] > cb )
                                if ( ptr[offset7] > cb )
                                    if ( ptr[offset8] > cb )
                                        if ( ptr[offset9] > cb )
                                            if ( ptr[offset10] > cb )
                                                if ( ptr[offset11] > cb )
                                                    if ( ptr[offset13] > cb )
                                                        if ( ptr[offset14] > cb )
                                                            if ( ptr[offset6] > cb )
                                                            {
                                                            }
                                                            else if ( ptr[offset15] > cb )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if ( ptr[offset12] < c_b )
                                if ( ptr[offset13] < c_b )
                                    if ( ptr[offset14] < c_b )
                                        if ( ptr[offset15] < c_b )
                                            if ( ptr[offset1] < c_b )
                                                if ( ptr[offset3] < c_b )
                                                {
                                                }
                                                else if ( ptr[offset10] < c_b )
                                                    if ( ptr[offset11] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset8] < c_b )
                                                if ( ptr[offset9] < c_b )
                                                    if ( ptr[offset10] < c_b )
                                                        if ( ptr[offset11] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset6] < c_b )
                                            if ( ptr[offset7] < c_b )
                                                if ( ptr[offset8] < c_b )
                                                    if ( ptr[offset9] < c_b )
                                                        if ( ptr[offset10] < c_b )
                                                            if ( ptr[offset11] < c_b )
                                                            {
                                                            }
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if ( ptr[offset11] > cb )
                            if ( ptr[offset7] > cb )
                                if ( ptr[offset8] > cb )
                                    if ( ptr[offset9] > cb )
                                        if ( ptr[offset10] > cb )
                                            if ( ptr[offset12] > cb )
                                                if ( ptr[offset13] > cb )
                                                    if ( ptr[offset6] > cb )
                                                        if ( ptr[offset5] > cb )
                                                        {
                                                        }
                                                        else if ( ptr[offset14] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else if ( ptr[offset14] > cb )
                                                        if ( ptr[offset15] > cb )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if ( ptr[offset11] < c_b )
                            if ( ptr[offset12] < c_b )
                                if ( ptr[offset13] < c_b )
                                    if ( ptr[offset10] < c_b )
                                        if ( ptr[offset14] < c_b )
                                            if ( ptr[offset15] < c_b )
                                                if ( ptr[offset1] < c_b )
                                                {
                                                }
                                                else if ( ptr[offset8] < c_b )
                                                    if ( ptr[offset9] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset6] < c_b )
                                                if ( ptr[offset7] < c_b )
                                                    if ( ptr[offset8] < c_b )
                                                        if ( ptr[offset9] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset5] < c_b )
                                            if ( ptr[offset6] < c_b )
                                                if ( ptr[offset7] < c_b )
                                                    if ( ptr[offset8] < c_b )
                                                        if ( ptr[offset9] < c_b )
                                                        {
                                                        }
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset1] < c_b )
                                        if ( ptr[offset3] < c_b )
                                            if ( ptr[offset14] < c_b )
                                                if ( ptr[offset15] < c_b )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if ( ptr[offset9] > cb )
                        if ( ptr[offset7] > cb )
                            if ( ptr[offset8] > cb )
                                if ( ptr[offset10] > cb )
                                    if ( ptr[offset11] > cb )
                                        if ( ptr[offset6] > cb )
                                            if ( ptr[offset5] > cb )
                                                if ( ptr[offset4] > cb )
                                                    if ( ptr[offset3] > cb )
                                                    {
                                                    }
                                                    else if ( ptr[offset12] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else if ( ptr[offset12] > cb )
                                                    if ( ptr[offset13] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset12] > cb )
                                                if ( ptr[offset13] > cb )
                                                    if ( ptr[offset14] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset12] > cb )
                                            if ( ptr[offset13] > cb )
                                                if ( ptr[offset14] > cb )
                                                    if ( ptr[offset15] > cb )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if ( ptr[offset9] < c_b )
                        if ( ptr[offset10] < c_b )
                            if ( ptr[offset11] < c_b )
                                if ( ptr[offset8] < c_b )
                                    if ( ptr[offset12] < c_b )
                                        if ( ptr[offset13] < c_b )
                                            if ( ptr[offset14] < c_b )
                                                if ( ptr[offset15] < c_b )
                                                {
                                                }
                                                else if ( ptr[offset6] < c_b )
                                                    if ( ptr[offset7] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if ( ptr[offset5] < c_b )
                                                if ( ptr[offset6] < c_b )
                                                    if ( ptr[offset7] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset4] < c_b )
                                            if ( ptr[offset5] < c_b )
                                                if ( ptr[offset6] < c_b )
                                                    if ( ptr[offset7] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset3] < c_b )
                                        if ( ptr[offset4] < c_b )
                                            if ( ptr[offset5] < c_b )
                                                if ( ptr[offset6] < c_b )
                                                    if ( ptr[offset7] < c_b )
                                                    {
                                                    }
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if ( ptr[offset1] < c_b )
                                    if ( ptr[offset12] < c_b )
                                        if ( ptr[offset13] < c_b )
                                            if ( ptr[offset14] < c_b )
                                                if ( ptr[offset15] < c_b )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else
                        continue;
                else if ( ptr[offset7] > cb )
                    if ( ptr[offset8] > cb )
                        if ( ptr[offset9] > cb )
                            if ( ptr[offset6] > cb )
                                if ( ptr[offset5] > cb )
                                    if ( ptr[offset4] > cb )
                                        if ( ptr[offset3] > cb )
                                            if ( ptr[offset2] > cb )
                                                if ( ptr[offset1] > cb )
                                                {
                                                }
                                                else if ( ptr[offset10] > cb )
                                                {
                                                }
                                                else
                                                    continue;
                                            else if ( ptr[offset10] > cb )
                                                if ( ptr[offset11] > cb )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset10] > cb )
                                            if ( ptr[offset11] > cb )
                                                if ( ptr[offset12] > cb )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset10] > cb )
                                        if ( ptr[offset11] > cb )
                                            if ( ptr[offset12] > cb )
                                                if ( ptr[offset13] > cb )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if ( ptr[offset10] > cb )
                                    if ( ptr[offset11] > cb )
                                        if ( ptr[offset12] > cb )
                                            if ( ptr[offset13] > cb )
                                                if ( ptr[offset14] > cb )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if ( ptr[offset10] > cb )
                                if ( ptr[offset11] > cb )
                                    if ( ptr[offset12] > cb )
                                        if ( ptr[offset13] > cb )
                                            if ( ptr[offset14] > cb )
                                                if ( ptr[offset15] > cb )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else
                        continue;
                else if ( ptr[offset7] < c_b )
                    if ( ptr[offset8] < c_b )
                        if ( ptr[offset9] < c_b )
                            if ( ptr[offset6] < c_b )
                                if ( ptr[offset5] < c_b )
                                    if ( ptr[offset4] < c_b )
                                        if ( ptr[offset3] < c_b )
                                            if ( ptr[offset2] < c_b )
                                                if ( ptr[offset1] < c_b )
                                                {
                                                }
                                                else if ( ptr[offset10] < c_b )
                                                {
                                                }
                                                else
                                                    continue;
                                            else if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if ( ptr[offset10] < c_b )
                                            if ( ptr[offset11] < c_b )
                                                if ( ptr[offset12] < c_b )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if ( ptr[offset10] < c_b )
                                        if ( ptr[offset11] < c_b )
                                            if ( ptr[offset12] < c_b )
                                                if ( ptr[offset13] < c_b )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if ( ptr[offset10] < c_b )
                                    if ( ptr[offset11] < c_b )
                                        if ( ptr[offset12] < c_b )
                                            if ( ptr[offset13] < c_b )
                                                if ( ptr[offset14] < c_b )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if ( ptr[offset10] < c_b )
                                if ( ptr[offset11] < c_b )
                                    if ( ptr[offset12] < c_b )
                                        if ( ptr[offset13] < c_b )
                                            if ( ptr[offset14] < c_b )
                                                if ( ptr[offset15] < c_b )
                                                {
                                                }
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else
                        continue;
                else
                    continue;
            }
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
        }
    }
}

#endif // (defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64))

#if !( defined __i386__ || defined( _M_IX86 ) || defined __x86_64__ || defined( _M_X64 ) )

static void
OAST_9_16( InputArray _img, std::vector< KeyPoint >& keypoints, int threshold )
{
    AGAST_ALL( _img, keypoints, threshold, AgastDetector::OAST_9_16 );
}

#endif // (defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64))

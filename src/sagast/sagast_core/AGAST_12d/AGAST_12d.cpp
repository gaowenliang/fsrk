
#include "fsf/sagast/sagast.h"
#include "fsf/sagast/sagast_core/AGAST_ALL.h"
#include "fsf/sagast/score/sagast_score.hpp"
#include <iostream>

using namespace cv;

#if ( defined __i386__ || defined( _M_IX86 ) || defined __x86_64__ || defined( _M_X64 ) )

void
AgastDetector::calc_12d( InputArray _img, std::vector< KeyPoint >& keypoints, int threshold )
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

    short pixel_12d_[16];
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
    width = xsize;

    for ( y = 3; y < ysizeB; y++ )
    {
        x = 2;
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
                //                std::cout << " cb " << cb << "\n";
                getOffsets_12d( pixel_12d_, ( short )img.step, x, y );

                offset0  = pixel_12d_[0];
                offset1  = pixel_12d_[1];
                offset2  = pixel_12d_[2];
                offset3  = pixel_12d_[3];
                offset4  = pixel_12d_[4];
                offset5  = pixel_12d_[5];
                offset6  = pixel_12d_[6];
                offset7  = pixel_12d_[7];
                offset8  = pixel_12d_[8];
                offset9  = pixel_12d_[9];
                offset10 = pixel_12d_[10];
                offset11 = pixel_12d_[11];

                if ( ptr[offset0] > cb )
                    if ( ptr[offset5] > cb )
                        if ( ptr[offset2] > cb )
                            if ( ptr[offset9] > cb )
                                if ( ptr[offset1] > cb )
                                    if ( ptr[offset6] > cb )
                                        if ( ptr[offset3] > cb )
                                            if ( ptr[offset4] > cb )
                                                goto success_homogeneous;
                                            else if ( ptr[offset10] > cb )
                                                if ( ptr[offset11] > cb )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset8] > cb )
                                            if ( ptr[offset10] > cb )
                                                if ( ptr[offset11] > cb )
                                                    goto success_structured;
                                                else if ( ptr[offset4] > cb )
                                                    if ( ptr[offset7] > cb )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset11] > cb )
                                        if ( ptr[offset3] > cb )
                                            if ( ptr[offset4] > cb )
                                                goto success_homogeneous;
                                            else if ( ptr[offset10] > cb )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset8] > cb )
                                            if ( ptr[offset10] > cb )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset6] > cb )
                                    if ( ptr[offset7] > cb )
                                        if ( ptr[offset8] > cb )
                                            if ( ptr[offset4] > cb )
                                                if ( ptr[offset3] > cb )
                                                    goto success_structured;
                                                else if ( ptr[offset10] > cb )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else if ( ptr[offset10] > cb )
                                                if ( ptr[offset11] > cb )
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
                            else if ( ptr[offset3] > cb )
                                if ( ptr[offset4] > cb )
                                    if ( ptr[offset1] > cb )
                                        if ( ptr[offset6] > cb )
                                            goto success_homogeneous;
                                        else if ( ptr[offset11] > cb )
                                            goto success_homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset6] > cb )
                                        if ( ptr[offset7] > cb )
                                            if ( ptr[offset8] > cb )
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
                        else if ( ptr[offset9] > cb )
                            if ( ptr[offset7] > cb )
                                if ( ptr[offset8] > cb )
                                    if ( ptr[offset1] > cb )
                                        if ( ptr[offset10] > cb )
                                            if ( ptr[offset11] > cb )
                                                goto success_homogeneous;
                                            else if ( ptr[offset6] > cb )
                                                if ( ptr[offset4] > cb )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset6] > cb )
                                            if ( ptr[offset3] > cb )
                                                if ( ptr[offset4] > cb )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset6] > cb )
                                        if ( ptr[offset4] > cb )
                                            if ( ptr[offset3] > cb )
                                                goto success_homogeneous;
                                            else if ( ptr[offset10] > cb )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset10] > cb )
                                            if ( ptr[offset11] > cb )
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
                        else
                            goto homogeneous;
                    else if ( ptr[offset5] < c_b )
                        if ( ptr[offset9] > cb )
                            if ( ptr[offset3] < c_b )
                                if ( ptr[offset4] < c_b )
                                    if ( ptr[offset11] > cb )
                                        if ( ptr[offset1] > cb )
                                            if ( ptr[offset8] > cb )
                                                if ( ptr[offset10] > cb )
                                                    if ( ptr[offset2] > cb )
                                                        goto success_structured;
                                                    else if ( ptr[offset7] > cb )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto homogeneous;
                                            else if ( ptr[offset6] < c_b )
                                                if ( ptr[offset2] < c_b )
                                                    if ( ptr[offset7] < c_b )
                                                        if ( ptr[offset8] < c_b )
                                                            goto success_structured;
                                                        else
                                                            goto structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset6] > cb )
                                            if ( ptr[offset7] > cb )
                                                if ( ptr[offset8] > cb )
                                                    if ( ptr[offset10] > cb )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset6] < c_b )
                                            if ( ptr[offset2] < c_b )
                                                if ( ptr[offset7] < c_b )
                                                    if ( ptr[offset1] < c_b )
                                                        goto success_structured;
                                                    else if ( ptr[offset8] < c_b )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset2] < c_b )
                                        if ( ptr[offset7] < c_b )
                                            if ( ptr[offset1] < c_b )
                                                if ( ptr[offset6] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else if ( ptr[offset6] < c_b )
                                                if ( ptr[offset8] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset11] > cb )
                                    if ( ptr[offset8] > cb )
                                        if ( ptr[offset10] > cb )
                                            if ( ptr[offset1] > cb )
                                                if ( ptr[offset2] > cb )
                                                    goto success_structured;
                                                else if ( ptr[offset7] > cb )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else if ( ptr[offset6] > cb )
                                                if ( ptr[offset7] > cb )
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
                            else if ( ptr[offset11] > cb )
                                if ( ptr[offset10] > cb )
                                    if ( ptr[offset3] > cb )
                                        if ( ptr[offset1] > cb )
                                            if ( ptr[offset2] > cb )
                                                goto success_homogeneous;
                                            else if ( ptr[offset7] > cb )
                                                if ( ptr[offset8] > cb )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset6] > cb )
                                            if ( ptr[offset7] > cb )
                                                if ( ptr[offset8] > cb )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset8] > cb )
                                        if ( ptr[offset1] > cb )
                                            if ( ptr[offset2] > cb )
                                                goto success_homogeneous;
                                            else if ( ptr[offset7] > cb )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset6] > cb )
                                            if ( ptr[offset7] > cb )
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
                        else if ( ptr[offset9] < c_b )
                            if ( ptr[offset2] > cb )
                                if ( ptr[offset1] > cb )
                                    if ( ptr[offset4] > cb )
                                        if ( ptr[offset10] > cb )
                                            if ( ptr[offset3] > cb )
                                                if ( ptr[offset11] > cb )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset6] < c_b )
                                            if ( ptr[offset7] < c_b )
                                                if ( ptr[offset8] < c_b )
                                                    if ( ptr[offset11] < c_b )
                                                        if ( ptr[offset10] < c_b )
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
                                            goto homogeneous;
                                    else if ( ptr[offset6] < c_b )
                                        if ( ptr[offset7] < c_b )
                                            if ( ptr[offset8] < c_b )
                                                if ( ptr[offset10] < c_b )
                                                    if ( ptr[offset4] < c_b )
                                                        goto success_structured;
                                                    else if ( ptr[offset11] < c_b )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else if ( ptr[offset3] < c_b )
                                                    if ( ptr[offset4] < c_b )
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
                                else if ( ptr[offset6] < c_b )
                                    if ( ptr[offset7] < c_b )
                                        if ( ptr[offset8] < c_b )
                                            if ( ptr[offset4] < c_b )
                                                if ( ptr[offset3] < c_b )
                                                    goto success_structured;
                                                else if ( ptr[offset10] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
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
                            else if ( ptr[offset6] < c_b )
                                if ( ptr[offset7] < c_b )
                                    if ( ptr[offset8] < c_b )
                                        if ( ptr[offset4] < c_b )
                                            if ( ptr[offset3] < c_b )
                                                goto success_homogeneous;
                                            else if ( ptr[offset10] < c_b )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset10] < c_b )
                                            if ( ptr[offset11] < c_b )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset2] < c_b )
                                        if ( ptr[offset1] < c_b )
                                            if ( ptr[offset3] < c_b )
                                                if ( ptr[offset4] < c_b )
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
                            else
                                goto homogeneous;
                        else if ( ptr[offset2] > cb )
                            if ( ptr[offset1] > cb )
                                if ( ptr[offset3] > cb )
                                    if ( ptr[offset4] > cb )
                                        if ( ptr[offset10] > cb )
                                            if ( ptr[offset11] > cb )
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
                        else if ( ptr[offset2] < c_b )
                            if ( ptr[offset3] < c_b )
                                if ( ptr[offset4] < c_b )
                                    if ( ptr[offset7] < c_b )
                                        if ( ptr[offset1] < c_b )
                                            if ( ptr[offset6] < c_b )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset6] < c_b )
                                            if ( ptr[offset8] < c_b )
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
                        else
                            goto homogeneous;
                    else if ( ptr[offset2] > cb )
                        if ( ptr[offset10] > cb )
                            if ( ptr[offset11] > cb )
                                if ( ptr[offset9] > cb )
                                    if ( ptr[offset1] > cb )
                                        if ( ptr[offset3] > cb )
                                            goto success_homogeneous;
                                        else if ( ptr[offset8] > cb )
                                            goto success_homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset6] > cb )
                                        if ( ptr[offset7] > cb )
                                            if ( ptr[offset8] > cb )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset1] > cb )
                                    if ( ptr[offset3] > cb )
                                        if ( ptr[offset4] > cb )
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
                    else if ( ptr[offset9] > cb )
                        if ( ptr[offset7] > cb )
                            if ( ptr[offset8] > cb )
                                if ( ptr[offset10] > cb )
                                    if ( ptr[offset11] > cb )
                                        if ( ptr[offset1] > cb )
                                            goto success_homogeneous;
                                        else if ( ptr[offset6] > cb )
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
                    else
                        goto homogeneous;
                else if ( ptr[offset0] < c_b )
                    if ( ptr[offset2] > cb )
                        if ( ptr[offset5] > cb )
                            if ( ptr[offset7] > cb )
                                if ( ptr[offset6] > cb )
                                    if ( ptr[offset4] > cb )
                                        if ( ptr[offset3] > cb )
                                            if ( ptr[offset1] > cb )
                                                goto success_homogeneous;
                                            else if ( ptr[offset8] > cb )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset9] > cb )
                                            if ( ptr[offset8] > cb )
                                                if ( ptr[offset10] > cb )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset9] > cb )
                                        if ( ptr[offset8] > cb )
                                            if ( ptr[offset10] > cb )
                                                if ( ptr[offset11] > cb )
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
                            else if ( ptr[offset9] < c_b )
                                if ( ptr[offset8] < c_b )
                                    if ( ptr[offset10] < c_b )
                                        if ( ptr[offset11] < c_b )
                                            if ( ptr[offset7] < c_b )
                                                if ( ptr[offset1] < c_b )
                                                    goto success_structured;
                                                else if ( ptr[offset6] < c_b )
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
                            else
                                goto homogeneous;
                        else if ( ptr[offset9] < c_b )
                            if ( ptr[offset7] < c_b )
                                if ( ptr[offset8] < c_b )
                                    if ( ptr[offset5] < c_b )
                                        if ( ptr[offset1] < c_b )
                                            if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    goto success_structured;
                                                else if ( ptr[offset6] < c_b )
                                                    if ( ptr[offset4] < c_b )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto homogeneous;
                                            else if ( ptr[offset6] < c_b )
                                                if ( ptr[offset3] < c_b )
                                                    if ( ptr[offset4] < c_b )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset6] < c_b )
                                            if ( ptr[offset4] < c_b )
                                                if ( ptr[offset3] < c_b )
                                                    goto success_structured;
                                                else if ( ptr[offset10] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset10] < c_b )
                                        if ( ptr[offset11] < c_b )
                                            if ( ptr[offset1] < c_b )
                                                goto success_homogeneous;
                                            else if ( ptr[offset6] < c_b )
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
                        else
                            goto homogeneous;
                    else if ( ptr[offset2] < c_b )
                        if ( ptr[offset9] > cb )
                            if ( ptr[offset5] > cb )
                                if ( ptr[offset1] < c_b )
                                    if ( ptr[offset4] < c_b )
                                        if ( ptr[offset10] < c_b )
                                            if ( ptr[offset3] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset6] > cb )
                                            if ( ptr[offset7] > cb )
                                                if ( ptr[offset8] > cb )
                                                    if ( ptr[offset11] > cb )
                                                        if ( ptr[offset10] > cb )
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
                                            goto homogeneous;
                                    else if ( ptr[offset6] > cb )
                                        if ( ptr[offset7] > cb )
                                            if ( ptr[offset8] > cb )
                                                if ( ptr[offset10] > cb )
                                                    if ( ptr[offset4] > cb )
                                                        goto success_structured;
                                                    else if ( ptr[offset11] > cb )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else if ( ptr[offset3] > cb )
                                                    if ( ptr[offset4] > cb )
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
                                else if ( ptr[offset6] > cb )
                                    if ( ptr[offset7] > cb )
                                        if ( ptr[offset8] > cb )
                                            if ( ptr[offset4] > cb )
                                                if ( ptr[offset3] > cb )
                                                    goto success_structured;
                                                else if ( ptr[offset10] > cb )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else if ( ptr[offset10] > cb )
                                                if ( ptr[offset11] > cb )
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
                            else if ( ptr[offset3] < c_b )
                                if ( ptr[offset4] < c_b )
                                    if ( ptr[offset5] < c_b )
                                        if ( ptr[offset1] < c_b )
                                            if ( ptr[offset6] < c_b )
                                                goto success_homogeneous;
                                            else if ( ptr[offset11] < c_b )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset6] < c_b )
                                            if ( ptr[offset7] < c_b )
                                                if ( ptr[offset8] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset1] < c_b )
                                        if ( ptr[offset10] < c_b )
                                            if ( ptr[offset11] < c_b )
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
                        else if ( ptr[offset9] < c_b )
                            if ( ptr[offset5] < c_b )
                                if ( ptr[offset1] < c_b )
                                    if ( ptr[offset6] < c_b )
                                        if ( ptr[offset3] < c_b )
                                            if ( ptr[offset4] < c_b )
                                                goto success_homogeneous;
                                            else if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset8] < c_b )
                                            if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    goto success_structured;
                                                else if ( ptr[offset4] < c_b )
                                                    if ( ptr[offset7] < c_b )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset11] < c_b )
                                        if ( ptr[offset3] < c_b )
                                            if ( ptr[offset4] < c_b )
                                                goto success_homogeneous;
                                            else if ( ptr[offset10] < c_b )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset8] < c_b )
                                            if ( ptr[offset10] < c_b )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset6] < c_b )
                                    if ( ptr[offset7] < c_b )
                                        if ( ptr[offset8] < c_b )
                                            if ( ptr[offset4] < c_b )
                                                if ( ptr[offset3] < c_b )
                                                    goto success_structured;
                                                else if ( ptr[offset10] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
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
                            else if ( ptr[offset10] < c_b )
                                if ( ptr[offset11] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        if ( ptr[offset3] < c_b )
                                            goto success_homogeneous;
                                        else if ( ptr[offset8] < c_b )
                                            goto success_homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset6] < c_b )
                                        if ( ptr[offset7] < c_b )
                                            if ( ptr[offset8] < c_b )
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
                        else if ( ptr[offset3] < c_b )
                            if ( ptr[offset4] < c_b )
                                if ( ptr[offset5] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        if ( ptr[offset6] < c_b )
                                            goto success_homogeneous;
                                        else if ( ptr[offset11] < c_b )
                                            goto success_homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset6] < c_b )
                                        if ( ptr[offset7] < c_b )
                                            if ( ptr[offset8] < c_b )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset1] < c_b )
                                    if ( ptr[offset10] < c_b )
                                        if ( ptr[offset11] < c_b )
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
                    else if ( ptr[offset9] < c_b )
                        if ( ptr[offset7] < c_b )
                            if ( ptr[offset8] < c_b )
                                if ( ptr[offset5] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        if ( ptr[offset10] < c_b )
                                            if ( ptr[offset11] < c_b )
                                                goto success_homogeneous;
                                            else if ( ptr[offset6] < c_b )
                                                if ( ptr[offset4] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset6] < c_b )
                                            if ( ptr[offset3] < c_b )
                                                if ( ptr[offset4] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset6] < c_b )
                                        if ( ptr[offset4] < c_b )
                                            if ( ptr[offset3] < c_b )
                                                goto success_homogeneous;
                                            else if ( ptr[offset10] < c_b )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset10] < c_b )
                                            if ( ptr[offset11] < c_b )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset10] < c_b )
                                    if ( ptr[offset11] < c_b )
                                        if ( ptr[offset1] < c_b )
                                            goto success_homogeneous;
                                        else if ( ptr[offset6] < c_b )
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
                    else if ( ptr[offset5] > cb )
                        if ( ptr[offset9] > cb )
                            if ( ptr[offset6] > cb )
                                if ( ptr[offset7] > cb )
                                    if ( ptr[offset8] > cb )
                                        if ( ptr[offset4] > cb )
                                            if ( ptr[offset3] > cb )
                                                goto success_homogeneous;
                                            else if ( ptr[offset10] > cb )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else if ( ptr[offset10] > cb )
                                            if ( ptr[offset11] > cb )
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
                        else
                            goto homogeneous;
                    else
                        goto homogeneous;
                else if ( ptr[offset5] > cb )
                    if ( ptr[offset9] > cb )
                        if ( ptr[offset6] > cb )
                            if ( ptr[offset7] > cb )
                                if ( ptr[offset4] > cb )
                                    if ( ptr[offset3] > cb )
                                        if ( ptr[offset8] > cb )
                                            goto success_homogeneous;
                                        else if ( ptr[offset1] > cb )
                                            if ( ptr[offset2] > cb )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset8] > cb )
                                        if ( ptr[offset10] > cb )
                                            goto success_homogeneous;
                                        else
                                            goto homogeneous;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset11] > cb )
                                    if ( ptr[offset8] > cb )
                                        if ( ptr[offset10] > cb )
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
                    else if ( ptr[offset2] > cb )
                        if ( ptr[offset3] > cb )
                            if ( ptr[offset4] > cb )
                                if ( ptr[offset7] > cb )
                                    if ( ptr[offset1] > cb )
                                        if ( ptr[offset6] > cb )
                                            goto success_homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset6] > cb )
                                        if ( ptr[offset8] > cb )
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
                    else
                        goto homogeneous;
                else if ( ptr[offset5] < c_b )
                    if ( ptr[offset9] < c_b )
                        if ( ptr[offset6] < c_b )
                            if ( ptr[offset7] < c_b )
                                if ( ptr[offset4] < c_b )
                                    if ( ptr[offset3] < c_b )
                                        if ( ptr[offset8] < c_b )
                                            goto success_homogeneous;
                                        else if ( ptr[offset1] < c_b )
                                            if ( ptr[offset2] < c_b )
                                                goto success_homogeneous;
                                            else
                                                goto homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset8] < c_b )
                                        if ( ptr[offset10] < c_b )
                                            goto success_homogeneous;
                                        else
                                            goto homogeneous;
                                    else
                                        goto homogeneous;
                                else if ( ptr[offset11] < c_b )
                                    if ( ptr[offset8] < c_b )
                                        if ( ptr[offset10] < c_b )
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
                    else if ( ptr[offset2] < c_b )
                        if ( ptr[offset3] < c_b )
                            if ( ptr[offset4] < c_b )
                                if ( ptr[offset7] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        if ( ptr[offset6] < c_b )
                                            goto success_homogeneous;
                                        else
                                            goto homogeneous;
                                    else if ( ptr[offset6] < c_b )
                                        if ( ptr[offset8] < c_b )
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
                //                std::cout << " cb " << cb << "\n";
                getOffsets_12d( pixel_12d_, ( short )img.step, x, y );

                offset0  = pixel_12d_[0];
                offset1  = pixel_12d_[1];
                offset2  = pixel_12d_[2];
                offset3  = pixel_12d_[3];
                offset4  = pixel_12d_[4];
                offset5  = pixel_12d_[5];
                offset6  = pixel_12d_[6];
                offset7  = pixel_12d_[7];
                offset8  = pixel_12d_[8];
                offset9  = pixel_12d_[9];
                offset10 = pixel_12d_[10];
                offset11 = pixel_12d_[11];

                if ( ptr[offset0] > cb )
                    if ( ptr[offset5] > cb )
                        if ( ptr[offset2] > cb )
                            if ( ptr[offset9] > cb )
                                if ( ptr[offset1] > cb )
                                    if ( ptr[offset6] > cb )
                                        if ( ptr[offset3] > cb )
                                            if ( ptr[offset4] > cb )
                                            {
                                                goto success_structured;
                                            }
                                            else if ( ptr[offset10] > cb )
                                            {
                                                if ( ptr[offset11] > cb )
                                                {
                                                    goto success_structured;
                                                }
                                                else
                                                {
                                                    goto structured;
                                                }
                                            }
                                            else
                                            {
                                                goto structured;
                                            }
                                        else if ( ptr[offset8] > cb )
                                            if ( ptr[offset10] > cb )
                                                if ( ptr[offset11] > cb )
                                                {
                                                    goto success_structured;
                                                }
                                                else if ( ptr[offset4] > cb )
                                                {
                                                    if ( ptr[offset7] > cb )
                                                    {
                                                        goto success_structured;
                                                    }
                                                    else
                                                    {
                                                        goto structured;
                                                    }
                                                }
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset11] > cb )
                                        if ( ptr[offset3] > cb )
                                            if ( ptr[offset4] > cb )
                                                goto success_structured;
                                            else if ( ptr[offset10] > cb )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset8] > cb )
                                            if ( ptr[offset10] > cb )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset6] > cb )
                                    if ( ptr[offset7] > cb )
                                        if ( ptr[offset8] > cb )
                                            if ( ptr[offset4] > cb )
                                                if ( ptr[offset3] > cb )
                                                    goto success_structured;
                                                else if ( ptr[offset10] > cb )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else if ( ptr[offset10] > cb )
                                                if ( ptr[offset11] > cb )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else if ( ptr[offset3] > cb )
                                if ( ptr[offset4] > cb )
                                    if ( ptr[offset1] > cb )
                                        if ( ptr[offset6] > cb )
                                            goto success_structured;
                                        else if ( ptr[offset11] > cb )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset6] > cb )
                                        if ( ptr[offset7] > cb )
                                            if ( ptr[offset8] > cb )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else if ( ptr[offset9] > cb )
                            if ( ptr[offset7] > cb )
                                if ( ptr[offset8] > cb )
                                    if ( ptr[offset1] > cb )
                                        if ( ptr[offset10] > cb )
                                            if ( ptr[offset11] > cb )
                                                goto success_structured;
                                            else if ( ptr[offset6] > cb )
                                                if ( ptr[offset4] > cb )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset6] > cb )
                                            if ( ptr[offset3] > cb )
                                                if ( ptr[offset4] > cb )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset6] > cb )
                                        if ( ptr[offset4] > cb )
                                            if ( ptr[offset3] > cb )
                                                goto success_structured;
                                            else if ( ptr[offset10] > cb )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset10] > cb )
                                            if ( ptr[offset11] > cb )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto structured;
                    else if ( ptr[offset5] < c_b )
                        if ( ptr[offset9] > cb )
                            if ( ptr[offset3] < c_b )
                                if ( ptr[offset4] < c_b )
                                    if ( ptr[offset11] > cb )
                                        if ( ptr[offset1] > cb )
                                            if ( ptr[offset8] > cb )
                                                if ( ptr[offset10] > cb )
                                                    if ( ptr[offset2] > cb )
                                                        goto success_structured;
                                                    else if ( ptr[offset7] > cb )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto structured;
                                            else if ( ptr[offset6] < c_b )
                                                if ( ptr[offset2] < c_b )
                                                    if ( ptr[offset7] < c_b )
                                                        if ( ptr[offset8] < c_b )
                                                            goto success_structured;
                                                        else
                                                            goto structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset6] > cb )
                                            if ( ptr[offset7] > cb )
                                                if ( ptr[offset8] > cb )
                                                    if ( ptr[offset10] > cb )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset6] < c_b )
                                            if ( ptr[offset2] < c_b )
                                                if ( ptr[offset7] < c_b )
                                                    if ( ptr[offset1] < c_b )
                                                        goto success_structured;
                                                    else if ( ptr[offset8] < c_b )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset2] < c_b )
                                        if ( ptr[offset7] < c_b )
                                            if ( ptr[offset1] < c_b )
                                                if ( ptr[offset6] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else if ( ptr[offset6] < c_b )
                                                if ( ptr[offset8] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset11] > cb )
                                    if ( ptr[offset8] > cb )
                                        if ( ptr[offset10] > cb )
                                            if ( ptr[offset1] > cb )
                                                if ( ptr[offset2] > cb )
                                                    goto success_structured;
                                                else if ( ptr[offset7] > cb )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else if ( ptr[offset6] > cb )
                                                if ( ptr[offset7] > cb )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else if ( ptr[offset11] > cb )
                                if ( ptr[offset10] > cb )
                                    if ( ptr[offset3] > cb )
                                        if ( ptr[offset1] > cb )
                                            if ( ptr[offset2] > cb )
                                                goto success_structured;
                                            else if ( ptr[offset7] > cb )
                                                if ( ptr[offset8] > cb )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset6] > cb )
                                            if ( ptr[offset7] > cb )
                                                if ( ptr[offset8] > cb )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset8] > cb )
                                        if ( ptr[offset1] > cb )
                                            if ( ptr[offset2] > cb )
                                                goto success_structured;
                                            else if ( ptr[offset7] > cb )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset6] > cb )
                                            if ( ptr[offset7] > cb )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else if ( ptr[offset9] < c_b )
                            if ( ptr[offset2] > cb )
                                if ( ptr[offset1] > cb )
                                    if ( ptr[offset4] > cb )
                                        if ( ptr[offset10] > cb )
                                            if ( ptr[offset3] > cb )
                                                if ( ptr[offset11] > cb )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset6] < c_b )
                                            if ( ptr[offset7] < c_b )
                                                if ( ptr[offset8] < c_b )
                                                    if ( ptr[offset11] < c_b )
                                                        if ( ptr[offset10] < c_b )
                                                            goto success_structured;
                                                        else
                                                            goto structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset6] < c_b )
                                        if ( ptr[offset7] < c_b )
                                            if ( ptr[offset8] < c_b )
                                                if ( ptr[offset10] < c_b )
                                                    if ( ptr[offset4] < c_b )
                                                        goto success_structured;
                                                    else if ( ptr[offset11] < c_b )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else if ( ptr[offset3] < c_b )
                                                    if ( ptr[offset4] < c_b )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset6] < c_b )
                                    if ( ptr[offset7] < c_b )
                                        if ( ptr[offset8] < c_b )
                                            if ( ptr[offset4] < c_b )
                                                if ( ptr[offset3] < c_b )
                                                    goto success_structured;
                                                else if ( ptr[offset10] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else if ( ptr[offset6] < c_b )
                                if ( ptr[offset7] < c_b )
                                    if ( ptr[offset8] < c_b )
                                        if ( ptr[offset4] < c_b )
                                            if ( ptr[offset3] < c_b )
                                                goto success_structured;
                                            else if ( ptr[offset10] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset10] < c_b )
                                            if ( ptr[offset11] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset2] < c_b )
                                        if ( ptr[offset1] < c_b )
                                            if ( ptr[offset3] < c_b )
                                                if ( ptr[offset4] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else if ( ptr[offset2] > cb )
                            if ( ptr[offset1] > cb )
                                if ( ptr[offset3] > cb )
                                    if ( ptr[offset4] > cb )
                                        if ( ptr[offset10] > cb )
                                            if ( ptr[offset11] > cb )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else if ( ptr[offset2] < c_b )
                            if ( ptr[offset3] < c_b )
                                if ( ptr[offset4] < c_b )
                                    if ( ptr[offset7] < c_b )
                                        if ( ptr[offset1] < c_b )
                                            if ( ptr[offset6] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset6] < c_b )
                                            if ( ptr[offset8] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto homogeneous;
                    else if ( ptr[offset2] > cb )
                        if ( ptr[offset10] > cb )
                            if ( ptr[offset11] > cb )
                                if ( ptr[offset9] > cb )
                                    if ( ptr[offset1] > cb )
                                        if ( ptr[offset3] > cb )
                                            goto success_structured;
                                        else if ( ptr[offset8] > cb )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset6] > cb )
                                        if ( ptr[offset7] > cb )
                                            if ( ptr[offset8] > cb )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset1] > cb )
                                    if ( ptr[offset3] > cb )
                                        if ( ptr[offset4] > cb )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto structured;
                    else if ( ptr[offset9] > cb )
                        if ( ptr[offset7] > cb )
                            if ( ptr[offset8] > cb )
                                if ( ptr[offset10] > cb )
                                    if ( ptr[offset11] > cb )
                                        if ( ptr[offset1] > cb )
                                            goto success_structured;
                                        else if ( ptr[offset6] > cb )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto structured;
                    else
                        goto structured;
                else if ( ptr[offset0] < c_b )
                    if ( ptr[offset2] > cb )
                        if ( ptr[offset5] > cb )
                            if ( ptr[offset7] > cb )
                                if ( ptr[offset6] > cb )
                                    if ( ptr[offset4] > cb )
                                        if ( ptr[offset3] > cb )
                                            if ( ptr[offset1] > cb )
                                                goto success_structured;
                                            else if ( ptr[offset8] > cb )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset9] > cb )
                                            if ( ptr[offset8] > cb )
                                                if ( ptr[offset10] > cb )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset9] > cb )
                                        if ( ptr[offset8] > cb )
                                            if ( ptr[offset10] > cb )
                                                if ( ptr[offset11] > cb )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else if ( ptr[offset9] < c_b )
                                if ( ptr[offset8] < c_b )
                                    if ( ptr[offset10] < c_b )
                                        if ( ptr[offset11] < c_b )
                                            if ( ptr[offset7] < c_b )
                                                if ( ptr[offset1] < c_b )
                                                    goto success_structured;
                                                else if ( ptr[offset6] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else if ( ptr[offset9] < c_b )
                            if ( ptr[offset7] < c_b )
                                if ( ptr[offset8] < c_b )
                                    if ( ptr[offset5] < c_b )
                                        if ( ptr[offset1] < c_b )
                                            if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    goto success_structured;
                                                else if ( ptr[offset6] < c_b )
                                                    if ( ptr[offset4] < c_b )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto structured;
                                            else if ( ptr[offset6] < c_b )
                                                if ( ptr[offset3] < c_b )
                                                    if ( ptr[offset4] < c_b )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset6] < c_b )
                                            if ( ptr[offset4] < c_b )
                                                if ( ptr[offset3] < c_b )
                                                    goto success_structured;
                                                else if ( ptr[offset10] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset10] < c_b )
                                        if ( ptr[offset11] < c_b )
                                            if ( ptr[offset1] < c_b )
                                                goto success_structured;
                                            else if ( ptr[offset6] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto structured;
                    else if ( ptr[offset2] < c_b )
                        if ( ptr[offset9] > cb )
                            if ( ptr[offset5] > cb )
                                if ( ptr[offset1] < c_b )
                                    if ( ptr[offset4] < c_b )
                                        if ( ptr[offset10] < c_b )
                                            if ( ptr[offset3] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset6] > cb )
                                            if ( ptr[offset7] > cb )
                                                if ( ptr[offset8] > cb )
                                                    if ( ptr[offset11] > cb )
                                                        if ( ptr[offset10] > cb )
                                                            goto success_structured;
                                                        else
                                                            goto structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset6] > cb )
                                        if ( ptr[offset7] > cb )
                                            if ( ptr[offset8] > cb )
                                                if ( ptr[offset10] > cb )
                                                    if ( ptr[offset4] > cb )
                                                        goto success_structured;
                                                    else if ( ptr[offset11] > cb )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else if ( ptr[offset3] > cb )
                                                    if ( ptr[offset4] > cb )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset6] > cb )
                                    if ( ptr[offset7] > cb )
                                        if ( ptr[offset8] > cb )
                                            if ( ptr[offset4] > cb )
                                                if ( ptr[offset3] > cb )
                                                    goto success_structured;
                                                else if ( ptr[offset10] > cb )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else if ( ptr[offset10] > cb )
                                                if ( ptr[offset11] > cb )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else if ( ptr[offset3] < c_b )
                                if ( ptr[offset4] < c_b )
                                    if ( ptr[offset5] < c_b )
                                        if ( ptr[offset1] < c_b )
                                            if ( ptr[offset6] < c_b )
                                                goto success_structured;
                                            else if ( ptr[offset11] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset6] < c_b )
                                            if ( ptr[offset7] < c_b )
                                                if ( ptr[offset8] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset1] < c_b )
                                        if ( ptr[offset10] < c_b )
                                            if ( ptr[offset11] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else if ( ptr[offset9] < c_b )
                            if ( ptr[offset5] < c_b )
                                if ( ptr[offset1] < c_b )
                                    if ( ptr[offset6] < c_b )
                                        if ( ptr[offset3] < c_b )
                                            if ( ptr[offset4] < c_b )
                                                goto success_structured;
                                            else if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset8] < c_b )
                                            if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    goto success_structured;
                                                else if ( ptr[offset4] < c_b )
                                                    if ( ptr[offset7] < c_b )
                                                        goto success_structured;
                                                    else
                                                        goto structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset11] < c_b )
                                        if ( ptr[offset3] < c_b )
                                            if ( ptr[offset4] < c_b )
                                                goto success_structured;
                                            else if ( ptr[offset10] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset8] < c_b )
                                            if ( ptr[offset10] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset6] < c_b )
                                    if ( ptr[offset7] < c_b )
                                        if ( ptr[offset8] < c_b )
                                            if ( ptr[offset4] < c_b )
                                                if ( ptr[offset3] < c_b )
                                                    goto success_structured;
                                                else if ( ptr[offset10] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else if ( ptr[offset10] < c_b )
                                                if ( ptr[offset11] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else if ( ptr[offset10] < c_b )
                                if ( ptr[offset11] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        if ( ptr[offset3] < c_b )
                                            goto success_structured;
                                        else if ( ptr[offset8] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset6] < c_b )
                                        if ( ptr[offset7] < c_b )
                                            if ( ptr[offset8] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else if ( ptr[offset3] < c_b )
                            if ( ptr[offset4] < c_b )
                                if ( ptr[offset5] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        if ( ptr[offset6] < c_b )
                                            goto success_structured;
                                        else if ( ptr[offset11] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset6] < c_b )
                                        if ( ptr[offset7] < c_b )
                                            if ( ptr[offset8] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset1] < c_b )
                                    if ( ptr[offset10] < c_b )
                                        if ( ptr[offset11] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto structured;
                    else if ( ptr[offset9] < c_b )
                        if ( ptr[offset7] < c_b )
                            if ( ptr[offset8] < c_b )
                                if ( ptr[offset5] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        if ( ptr[offset10] < c_b )
                                            if ( ptr[offset11] < c_b )
                                                goto success_structured;
                                            else if ( ptr[offset6] < c_b )
                                                if ( ptr[offset4] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset6] < c_b )
                                            if ( ptr[offset3] < c_b )
                                                if ( ptr[offset4] < c_b )
                                                    goto success_structured;
                                                else
                                                    goto structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset6] < c_b )
                                        if ( ptr[offset4] < c_b )
                                            if ( ptr[offset3] < c_b )
                                                goto success_structured;
                                            else if ( ptr[offset10] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset10] < c_b )
                                            if ( ptr[offset11] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset10] < c_b )
                                    if ( ptr[offset11] < c_b )
                                        if ( ptr[offset1] < c_b )
                                            goto success_structured;
                                        else if ( ptr[offset6] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto structured;
                    else if ( ptr[offset5] > cb )
                        if ( ptr[offset9] > cb )
                            if ( ptr[offset6] > cb )
                                if ( ptr[offset7] > cb )
                                    if ( ptr[offset8] > cb )
                                        if ( ptr[offset4] > cb )
                                            if ( ptr[offset3] > cb )
                                                goto success_structured;
                                            else if ( ptr[offset10] > cb )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else if ( ptr[offset10] > cb )
                                            if ( ptr[offset11] > cb )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto homogeneous;
                    else
                        goto structured;
                else if ( ptr[offset5] > cb )
                    if ( ptr[offset9] > cb )
                        if ( ptr[offset6] > cb )
                            if ( ptr[offset7] > cb )
                                if ( ptr[offset4] > cb )
                                    if ( ptr[offset3] > cb )
                                        if ( ptr[offset8] > cb )
                                            goto success_structured;
                                        else if ( ptr[offset1] > cb )
                                            if ( ptr[offset2] > cb )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset8] > cb )
                                        if ( ptr[offset10] > cb )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset11] > cb )
                                    if ( ptr[offset8] > cb )
                                        if ( ptr[offset10] > cb )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto structured;
                    else if ( ptr[offset2] > cb )
                        if ( ptr[offset3] > cb )
                            if ( ptr[offset4] > cb )
                                if ( ptr[offset7] > cb )
                                    if ( ptr[offset1] > cb )
                                        if ( ptr[offset6] > cb )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset6] > cb )
                                        if ( ptr[offset8] > cb )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto structured;
                    else
                        goto structured;
                else if ( ptr[offset5] < c_b )
                    if ( ptr[offset9] < c_b )
                        if ( ptr[offset6] < c_b )
                            if ( ptr[offset7] < c_b )
                                if ( ptr[offset4] < c_b )
                                    if ( ptr[offset3] < c_b )
                                        if ( ptr[offset8] < c_b )
                                            goto success_structured;
                                        else if ( ptr[offset1] < c_b )
                                            if ( ptr[offset2] < c_b )
                                                goto success_structured;
                                            else
                                                goto structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset8] < c_b )
                                        if ( ptr[offset10] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else if ( ptr[offset11] < c_b )
                                    if ( ptr[offset8] < c_b )
                                        if ( ptr[offset10] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto structured;
                    else if ( ptr[offset2] < c_b )
                        if ( ptr[offset3] < c_b )
                            if ( ptr[offset4] < c_b )
                                if ( ptr[offset7] < c_b )
                                    if ( ptr[offset1] < c_b )
                                        if ( ptr[offset6] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else if ( ptr[offset6] < c_b )
                                        if ( ptr[offset8] < c_b )
                                            goto success_structured;
                                        else
                                            goto structured;
                                    else
                                        goto structured;
                                else
                                    goto structured;
                            else
                                goto structured;
                        else
                            goto structured;
                    else
                        goto structured;
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

    //    std::cout << "total " << total << "\n";
    //    std::cout << "keypoints " << keypoints.size( ) << "\n";
    //    std::cout << "keypoints " << keypoints.size( ) << "\n";
}

#endif // (defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64))

#if !( defined __i386__ || defined( _M_IX86 ) || defined __x86_64__ || defined( _M_X64 ) )

static void
calc_12d( InputArray _img, std::vector< KeyPoint >& keypoints, int threshold )
{
    AGAST_ALL( _img, keypoints, threshold, AgastDetector::AGAST_12d );
}

#endif // (defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64))

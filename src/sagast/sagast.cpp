/* This is AGAST and OAST, an optimal and accelerated corner detector
              based on the accelerated segment tests
   Below is the original copyright and the references */

/*
Copyright (C) 2010  Elmar Mair
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    *Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

    *Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

    *Neither the name of the University of Cambridge nor the names of
     its contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
The references are:
 * Adaptive and Generic Corner Detection Based on the Accelerated Segment Test,
   Elmar Mair and Gregory D. Hager and Darius Burschka
   and Michael Suppa and Gerhard Hirzinger ECCV 2010
   URL: http://www6.in.tum.de/Main/ResearchAgast
*/

#include "fsf/sagast/sagast.h"
#include "fsf/sagast/score/sagast_score.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>

namespace cv
{

class AgastDetector_Impl : public AgastDetector
{
    public:
    AgastDetector_Impl( int _threshold, bool _nonmaxSuppression, int _type )
    : threshold( _threshold )
    , nonmaxSuppression( _nonmaxSuppression )
    , type( ( short )_type )
    {
    }

    void detect( InputArray _image, std::vector< KeyPoint >& keypoints, InputArray _mask )
    {
        Mat mask = _mask.getMat( ), grayImage;
        UMat ugrayImage;
        _InputArray gray = _image;
        if ( _image.type( ) != CV_8U )
        {
            _OutputArray ogray
            = _image.isUMat( ) ? _OutputArray( ugrayImage ) : _OutputArray( grayImage );
            cvtColor( _image, ogray, COLOR_BGR2GRAY );
            gray = ogray;
        }
        keypoints.clear( );
        SAGAST2( gray, keypoints, threshold, nonmaxSuppression, type );
        KeyPointsFilter::runByPixelsMask( keypoints, mask );
    }

    void set( int prop, double value )
    {
        if ( prop == THRESHOLD )
            threshold = cvRound( value );
        else if ( prop == NONMAX_SUPPRESSION )
            nonmaxSuppression = value != 0;
        else
            CV_Error( Error::StsBadArg, "" );
    }

    double get( int prop ) const
    {
        if ( prop == THRESHOLD )
            return threshold;
        if ( prop == NONMAX_SUPPRESSION )
            return nonmaxSuppression;
        CV_Error( Error::StsBadArg, "" );
        return 0;
    }

    void setThreshold( int threshold_ ) { threshold = threshold_; }
    int getThreshold( ) const { return threshold; }

    void setNonmaxSuppression( bool f ) { nonmaxSuppression = f; }
    bool getNonmaxSuppression( ) const { return nonmaxSuppression; }

    void setType( int type_ ) { type = type_; }
    int getType( ) const { return type; }

    int threshold;
    bool nonmaxSuppression;
    int type;
};

Ptr< AgastDetector >
AgastDetector::create( int threshold, bool nonmaxSuppression, int type )
{
    return makePtr< AgastDetector_Impl >( threshold, nonmaxSuppression, type );
}

bool
AgastDetector::loadCamera( std::string cam_file )
{

    std::cout << "#INFO: camera config is " << cam_file << std::endl;
    cam = camera_model::CameraFactory::instance( )->generateCameraFromYamlFile( cam_file );

    std::cout << cam->parametersToString( ) << std::endl;
    image_center = cam->getPrinciple( );
    std::cout << "#INFO: LOADing camera config is DONE." << cam_file << std::endl;

    return true;
}
bool
AgastDetector::loadMask( std::string file )
{
    m_mask = cv::imread( file, cv::IMREAD_GRAYSCALE );

    if ( m_mask.empty( ) )
        return false;
    else
        return true;
}

bool
AgastDetector::loadMask( )
{
    m_mask = cv::Mat( cam->imageHeight( ), cam->imageWidth( ), CV_8UC1, cv::Scalar( 255 ) );

    return true;
}

void
AgastDetector::SAGAST2( InputArray _img, std::vector< KeyPoint >& keypoints, int threshold, bool nonmax_suppression, int type )
{
    std::vector< KeyPoint > kpts;

    // detect
    switch ( type )
    {
        case AgastDetector::SAGAST_8:
        {
            calc_8( _img, kpts, threshold );
            break;
        }
        case AgastDetector::SAGAST_12s:
        {
            calc_12s( _img, kpts, threshold );
            break;
        }
        case AgastDetector::SAGAST_12d:
        {
            calc_12d( _img, kpts, threshold );
            break;
        }
        case AgastDetector::SAGAST_16:
        {
            calc_16( _img, kpts, threshold );
            break;
        }
    }

    cv::Mat img = _img.getMat( );

    // score
    short pixel_[16];

    std::vector< KeyPoint >::iterator kpt;
    for ( kpt = kpts.begin( ); kpt != kpts.end( ); ++kpt )
    {
        switch ( type )
        {
            case AgastDetector::SAGAST_8:
            {
                getOffsets_8( pixel_, ( short )img.step, kpt->pt.x, kpt->pt.y );

                kpt->response = ( float )agastCornerScore< AgastDetector::SAGAST_8 >(
                &img.at< uchar >( ( int )kpt->pt.y, ( int )kpt->pt.x ), pixel_, threshold );

                break;
            }
            case AgastDetector::SAGAST_12s:
            {
                getOffsets_12s( pixel_, ( short )img.step, kpt->pt.x, kpt->pt.y );

                kpt->response = ( float )agastCornerScore< AgastDetector::SAGAST_12s >(
                &img.at< uchar >( ( int )kpt->pt.y, ( int )kpt->pt.x ), pixel_, threshold );
                break;
            }
            case AgastDetector::SAGAST_12d:
            {
                getOffsets_12d( pixel_, ( short )img.step, kpt->pt.x, kpt->pt.y );

                kpt->response = ( float )agastCornerScore< AgastDetector::SAGAST_12d >(
                &img.at< uchar >( ( int )kpt->pt.y, ( int )kpt->pt.x ), pixel_, threshold );

                break;
            }
            case AgastDetector::SAGAST_16:
            {
                getOffsets_16( pixel_, ( short )img.step, kpt->pt.x, kpt->pt.y );

                kpt->response = ( float )agastCornerScore< AgastDetector::SAGAST_16 >(
                &img.at< uchar >( ( int )kpt->pt.y, ( int )kpt->pt.x ), pixel_, threshold );

                break;
            }
        }
    }

    // suppression
    if ( nonmax_suppression )
    {
        size_t j;
        size_t curr_idx;
        size_t lastRow = 0, next_lastRow = 0;
        size_t num_Corners       = kpts.size( );
        size_t lastRowCorner_ind = 0, next_lastRowCorner_ind = 0;

        std::vector< int > nmsFlags;
        std::vector< KeyPoint >::iterator currCorner_nms;
        std::vector< KeyPoint >::const_iterator currCorner;

        currCorner = kpts.begin( );

        nmsFlags.resize( ( int )num_Corners );

        // set all flags to MAXIMUM
        for ( j         = 0; j < num_Corners; j++ )
            nmsFlags[j] = -1;

        for ( curr_idx = 0; curr_idx < num_Corners; curr_idx++ )
        {
            int t;
            // check above
            if ( lastRow + 1 < currCorner->pt.y )
            {
                lastRow           = next_lastRow;
                lastRowCorner_ind = next_lastRowCorner_ind;
            }
            if ( next_lastRow != currCorner->pt.y )
            {
                next_lastRow           = ( size_t )currCorner->pt.y;
                next_lastRowCorner_ind = curr_idx;
            }
            if ( lastRow + 1 == currCorner->pt.y )
            {
                // find the corner above the current one
                while ( ( kpts[lastRowCorner_ind].pt.x < currCorner->pt.x )
                        && ( kpts[lastRowCorner_ind].pt.y == lastRow ) )
                    lastRowCorner_ind++;

                if ( ( kpts[lastRowCorner_ind].pt.x == currCorner->pt.x )
                     && ( lastRowCorner_ind != curr_idx ) )
                {
                    size_t w = lastRowCorner_ind;
                    // find the maximum in this block
                    while ( nmsFlags[w] != -1 )
                        w = nmsFlags[w];

                    if ( kpts[curr_idx].response < kpts[w].response )
                        nmsFlags[curr_idx] = ( int )w;
                    else
                        nmsFlags[w] = ( int )curr_idx;
                }
            }

            // check left
            t = ( int )curr_idx - 1;
            if ( ( curr_idx != 0 ) && ( kpts[t].pt.y == currCorner->pt.y )
                 && ( kpts[t].pt.x + 1 == currCorner->pt.x ) )
            {
                int currCornerMaxAbove_ind = nmsFlags[curr_idx];
                // find the maximum in that area
                while ( nmsFlags[t] != -1 )
                    t = nmsFlags[t];
                // no maximum above
                if ( currCornerMaxAbove_ind == -1 )
                {
                    if ( ( size_t )t != curr_idx )
                    {
                        if ( kpts[curr_idx].response < kpts[t].response )
                            nmsFlags[curr_idx] = t;
                        else
                            nmsFlags[t] = ( int )curr_idx;
                    }
                }
                else // maximum above
                {
                    if ( t != currCornerMaxAbove_ind )
                    {
                        if ( kpts[currCornerMaxAbove_ind].response < kpts[t].response )
                        {
                            nmsFlags[currCornerMaxAbove_ind] = t;
                            nmsFlags[curr_idx]               = t;
                        }
                        else
                        {
                            nmsFlags[t]        = currCornerMaxAbove_ind;
                            nmsFlags[curr_idx] = currCornerMaxAbove_ind;
                        }
                    }
                }
            }
            ++currCorner;
        }

        // collecting maximum corners
        for ( curr_idx = 0; curr_idx < num_Corners; curr_idx++ )
        {
            if ( nmsFlags[curr_idx] == -1 )
                keypoints.push_back( kpts[curr_idx] );
        }
    }
    else
    {
        keypoints = kpts;
    }
}

} // END NAMESPACE CV

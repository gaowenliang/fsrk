#ifndef CV_UTILS_H
#define CV_UTILS_H

#include <opencv2/core.hpp>

namespace cv_utils
{
template< typename T >
static inline void
scalarToRawData_( const cv::Scalar& s, T* const buf, const int cn, const int unroll_to )
{
    int i = 0;
    for ( ; i < cn; i++ )
        buf[i] = cv::saturate_cast< T >( s.val[i] );
    for ( ; i < unroll_to; i++ )
        buf[i] = buf[i - cn];
}

void
scalarToData( const cv::Scalar& s, void* _buf, int type, int unroll_to );
}

#endif // CV_UTILS_H

#include "cv_utils.h"

void
cv_utils::scalarToData( const cv::Scalar& s, void* _buf, int type, int unroll_to )
{
    const int depth = CV_MAT_DEPTH( type ), cn = CV_MAT_CN( type );
    CV_Assert( cn <= 4 );
    switch ( depth )
    {
        case CV_8U:
            scalarToRawData_< uchar >( s, ( uchar* )_buf, cn, unroll_to );
            break;
        case CV_8S:
            scalarToRawData_< schar >( s, ( schar* )_buf, cn, unroll_to );
            break;
        case CV_16U:
            scalarToRawData_< ushort >( s, ( ushort* )_buf, cn, unroll_to );
            break;
        case CV_16S:
            scalarToRawData_< short >( s, ( short* )_buf, cn, unroll_to );
            break;
        case CV_32S:
            scalarToRawData_< int >( s, ( int* )_buf, cn, unroll_to );
            break;
        case CV_32F:
            scalarToRawData_< float >( s, ( float* )_buf, cn, unroll_to );
            break;
        case CV_64F:
            scalarToRawData_< double >( s, ( double* )_buf, cn, unroll_to );
            break;
        default:
            CV_Error( CV_StsUnsupportedFormat, "" );
    }
}

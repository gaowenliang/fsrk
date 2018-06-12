#include "fsrk/sagast/sagast.h"
#include "fsrk/sagast/score/sagast_score.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;

void
AgastDetector::buildOffsetsTable_12s( )
{
    std::cout << "start build AGAST Offsets Table.\n";

    int offset_size;
    if ( getType( ) == SAGAST_12s )
        offset_size = 12;

    int image_height = cam->imageHeight( );
    int image_width  = cam->imageWidth( );
    //    std::cout << image_height << " " << image_width << "\n";

    m_tableOffsets = cv::Mat( image_height, image_width, CV_8SC24, cv::Scalar( 0 ) );

    std::vector< Eigen::Vector2d > offset;
    offset.push_back( Eigen::Vector2d( -2, 0 ) );
    offset.push_back( Eigen::Vector2d( -2, -1 ) );
    offset.push_back( Eigen::Vector2d( -1, -2 ) );
    offset.push_back( Eigen::Vector2d( 0, -2 ) );
    offset.push_back( Eigen::Vector2d( 1, -2 ) );
    offset.push_back( Eigen::Vector2d( 2, -1 ) );
    offset.push_back( Eigen::Vector2d( 2, 0 ) );
    offset.push_back( Eigen::Vector2d( 2, 1 ) );
    offset.push_back( Eigen::Vector2d( 1, 2 ) );
    offset.push_back( Eigen::Vector2d( 0, 2 ) );
    offset.push_back( Eigen::Vector2d( -1, 2 ) );
    offset.push_back( Eigen::Vector2d( -2, 1 ) );

    for ( int row_index = 0; row_index < image_height; ++row_index )
        for ( int col_index = 0; col_index < image_width; ++col_index )
        {
            if ( m_mask.at< uchar >( row_index, col_index ) < 20 )
                continue;

            // ----------------------------------------
            // src pt ---------------------------------
            Eigen::Vector2d p_u00 = Eigen::Vector2d( image_center.x, image_center.y ); // 0
            Eigen::Vector3d P_center;
            cam->liftSphere( p_u00, P_center );
            P_center.normalize( );

            // ----------------------------------------
            // dst pt ---------------------------------
            Eigen::Vector2d angle = calcAngle( cam, Eigen::Vector2d( col_index, row_index ) );

            double center_phi   = angle( 1 );
            double center_theta = angle( 0 );
            Eigen::Vector2d angle_center( center_theta, center_phi );
            Eigen::Vector3d P_center2( cos( angle_center( 1 ) ) * sin( angle_center( 0 ) ),
                                       sin( angle_center( 1 ) ) * sin( angle_center( 0 ) ),
                                       cos( angle_center( 0 ) ) );
            P_center2.normalize( );

            // -----------------------------------------------
            // delta v, angle --------------------------------
            double delta_angle           = acos( P_center.dot( P_center2 ) );
            Eigen::Vector3d delta_vector = P_center.cross( P_center2 );
            delta_vector.normalize( );

            Eigen::Quaterniond q_v1v0( cos( 0.5 * delta_angle ),
                                       sin( 0.5 * delta_angle ) * delta_vector( 0 ),
                                       sin( 0.5 * delta_angle ) * delta_vector( 1 ),
                                       sin( 0.5 * delta_angle ) * delta_vector( 2 ) );

            Eigen::Quaterniond q_v1v1( cos( 0.5 * center_phi ),
                                       sin( 0.5 * center_phi ) * P_center2( 0 ),
                                       sin( 0.5 * center_phi ) * P_center2( 1 ),
                                       sin( 0.5 * center_phi ) * P_center2( 2 ) );

            // ----------------------------------------
            Eigen::Vector3d P_u00, offset_P_u00;
            Eigen::Vector2d offset_p_u00;

            std::vector< Eigen::Vector3d > P_u, offset_P_u;
            std::vector< Eigen::Vector2d > p_u, offset_p_u;
            P_u.resize( offset_size );
            offset_P_u.resize( offset_size );
            p_u.resize( offset_size );
            offset_p_u.resize( offset_size );

            cam->liftSphere( p_u00, P_u00 );
            offset_P_u00 = q_v1v1 * q_v1v0 * P_u00;
            cam->spaceToPlane( offset_P_u00, offset_p_u00 );

            cv::Vec24c offset_pt;
            for ( int i = 0; i < offset_size; ++i )
            {
                p_u[i] = p_u00 + offset[i];
                cam->liftSphere( p_u[i], P_u[i] );
                offset_P_u[i] = q_v1v1 * q_v1v0 * P_u[i];
                cam->spaceToPlane( offset_P_u[i], offset_p_u[i] );

                offset_pt[2 * i] = char( double2int( ( offset_p_u[i] - offset_p_u00 )( 0 ) ) );
                offset_pt[2 * i + 1] = char( double2int( ( offset_p_u[i] - offset_p_u00 )( 1 ) ) );
            }

            m_tableOffsets.at< cv::Vec24c >( row_index, col_index ) = offset_pt;
        }

    //    std::string featureTable = "/home/gao/ws/src/vins/config/dual/featureTable.bmp";
    //    cv::imwrite( featureTable, m_tableOffsets );

    std::cout << "[#INFO]: build AGAST Offsets Table Done!\n";
}

void
AgastDetector::getOffsets_12s( short pixel[16], short rowStride, int xx, int yy )
{
    // cv::Vec24c offset = m_tableOffsets.at< cv::Vec24c >( yy, xx );
    //
    // for ( int k = 0; k < 12; k++ )
    //{
    //    pixel[k] = short( offset[2 * k] ) + short( offset[2 * k + 1] ) * rowStride;
    //}

    char* p  = m_tableOffsets.ptr< char >( yy );
    int xx24 = xx * 24;

    // for ( int k = 0; k < 12; k++ )
    // {
    //     pixel[k] = short( p[2 * k + xx24] ) + short( p[2 * k + xx24 + 1] ) * rowStride;
    // }

    pixel[0]  = short( p[0 + xx24] ) + short( p[1 + xx24] ) * rowStride;
    pixel[1]  = short( p[2 + xx24] ) + short( p[3 + xx24] ) * rowStride;
    pixel[2]  = short( p[4 + xx24] ) + short( p[5 + xx24] ) * rowStride;
    pixel[3]  = short( p[6 + xx24] ) + short( p[7 + xx24] ) * rowStride;
    pixel[4]  = short( p[8 + xx24] ) + short( p[9 + xx24] ) * rowStride;
    pixel[5]  = short( p[10 + xx24] ) + short( p[11 + xx24] ) * rowStride;
    pixel[6]  = short( p[12 + xx24] ) + short( p[13 + xx24] ) * rowStride;
    pixel[7]  = short( p[14 + xx24] ) + short( p[15 + xx24] ) * rowStride;
    pixel[8]  = short( p[16 + xx24] ) + short( p[17 + xx24] ) * rowStride;
    pixel[9]  = short( p[18 + xx24] ) + short( p[19 + xx24] ) * rowStride;
    pixel[10] = short( p[20 + xx24] ) + short( p[21 + xx24] ) * rowStride;
    pixel[11] = short( p[22 + xx24] ) + short( p[23 + xx24] ) * rowStride;
}

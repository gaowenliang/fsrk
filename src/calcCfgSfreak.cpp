
#include "../include/fsrk/sfreak/sfreak.hpp"
#include <iostream>

int
main( )
{
    std::string file_data;
    std::string file_cam;
    std::string file_mask;

    file_data = "/home/gao/ws/src/fsf/cfg/dataFREAK_down";
    file_cam  = "/home/gao/ws/src/vins/config/dual/cam_down_camera_calib.yaml";
    file_mask = "/home/gao/ws/src/vins/config/dual/mask_down.jpg";

    cv::Ptr< cv::SFREAK > freak = cv::SFREAK::create( true, true, 30, 4 ); // 36
    freak->loadCamera( file_cam );
    freak->loadMask( file_mask );

    freak->buildPatternTable( );
    freak->savePatternTableToData( file_data );

    return 0;
}

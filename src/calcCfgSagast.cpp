
#include "../include/fsf/sagast/sagast.h"
#include <iostream>

int
main( )
{
    std::string file_data;
    std::string file_cam;
    std::string file_mask;
    cv::AgastDetector::SAGAST_TYPE type;

    file_data = "/home/gao/ws/src/fsf/cfg/dataSAGAST12d22";
    file_cam  = "/home/gao/ws/src/vins/config/dual/cam_down_camera_calib.yaml";
    file_mask = "/home/gao/ws/src/vins/config/dual/mask_down.jpg";
    type      = cv::AgastDetector::SAGAST_12d;

    cv::Ptr< cv::AgastDetector > agast12 = cv::AgastDetector::create( 10, true, type );

    agast12->loadMask( file_mask );
    agast12->loadCamera( file_cam );

    agast12->buildOffsetsTable( );
    agast12->saveOffsetsCfg( file_data );

    return 0;
}

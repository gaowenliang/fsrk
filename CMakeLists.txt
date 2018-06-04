cmake_minimum_required(VERSION 2.8.3)
project(fsrk)

set(CMAKE_VERBOSE_MAKEFILE "true")
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -g -fopenmp")

find_package(catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
    sensor_msgs
    cv_bridge
    camera_model
    vignetting_model
    message_filters
    code_utils
    )

find_package(OpenCV REQUIRED)
find_package(Eigen3  REQUIRED)

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES fsrk
#    CATKIN_DEPENDS roscpp
    )

include_directories("include")

include_directories(
    ${catkin_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIR}
    )

#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(agast_LIB_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/sagast/sagast.cpp
    ${PROJECT_SOURCE_DIR}/src/sagast/score/sagast_score.cpp
    ${PROJECT_SOURCE_DIR}/src/sagast/sagast_core/AGAST_8/AGAST_8.cpp
    ${PROJECT_SOURCE_DIR}/src/sagast/sagast_core/AGAST_8/OffsetsTable_8.cpp
    ${PROJECT_SOURCE_DIR}/src/sagast/sagast_core/AGAST_12d/AGAST_12d.cpp
    ${PROJECT_SOURCE_DIR}/src/sagast/sagast_core/AGAST_12d/OffsetsTable_12d.cpp
    ${PROJECT_SOURCE_DIR}/src/sagast/sagast_core/AGAST_12s/AGAST_12s.cpp
    ${PROJECT_SOURCE_DIR}/src/sagast/sagast_core/AGAST_12s/OffsetsTable_12s.cpp
    ${PROJECT_SOURCE_DIR}/src/sagast/sagast_core/OAST_16/OAST_16.cpp
    ${PROJECT_SOURCE_DIR}/src/sagast/sagast_core/OAST_16/OffsetsTable_16.cpp
    ${PROJECT_SOURCE_DIR}/src/sagast/offsets/getOffsets.cpp
)
set(sfreak_LIB_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/sfreak/sfreak_impl.cpp
    ${PROJECT_SOURCE_DIR}/src/sfreak/sfreak.cpp
    ${PROJECT_SOURCE_DIR}/src/shape/circle.cpp
    ${PROJECT_SOURCE_DIR}/src/shape/ellipse.cpp
    ${PROJECT_SOURCE_DIR}/src/shape/rectrot.cpp
    ${PROJECT_SOURCE_DIR}/src/shape/cv_utils.cpp
)

add_library(sagast     ${agast_LIB_SOURCE_FILES})
target_link_libraries(sagast     ${Boost_LIBRARIES}  ${catkin_LIBRARIES}  ${OpenCV_LIBS} )

add_executable(run_sagast    src/run_sagast.cpp    ${agast_LIB_SOURCE_FILES}    )
target_link_libraries(run_sagast dw ${catkin_LIBRARIES}  ${catkin_LIBRARIES}  ${OpenCV_LIBS})

add_executable(calcCfgSagast    src/calcCfgSagast.cpp    ${agast_LIB_SOURCE_FILES}    )
target_link_libraries(calcCfgSagast dw ${catkin_LIBRARIES}  ${catkin_LIBRARIES}  ${OpenCV_LIBS})

add_executable(run_sfreak    src/run_sfreak.cpp    ${sfreak_LIB_SOURCE_FILES}  ${agast_LIB_SOURCE_FILES})
target_link_libraries(run_sfreak dw ${catkin_LIBRARIES}  ${catkin_LIBRARIES}  ${OpenCV_LIBS})

add_executable(test_ell    src/test_ell.cpp    ${sfreak_LIB_SOURCE_FILES}  ${agast_LIB_SOURCE_FILES})
target_link_libraries(test_ell dw ${catkin_LIBRARIES}  ${catkin_LIBRARIES}  ${OpenCV_LIBS})



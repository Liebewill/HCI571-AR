# Main cmake file 

cmake_minimum_required(VERSION 2.6)

# Main project name is ARMaker
project(CameraCalibrationExample)

# Find packages
FIND_PACKAGE(OpenCV REQUIRED)

# Include dirs
include_directories(${OpenCV_INCLUDE_DIRS})

# Add all files to the configuration
file(GLOB CameraCalibrationExample_SRC
    "*.h"
    "HCI571X_CameraCalibrationExample.cpp"
)

# Create an executable
add_executable( CameraCalibrationExample ${CameraCalibrationExample_SRC} )

# Add link directories
link_directories(${OpenCV_LIB_DIR})

IF(NOT CMAKE_CFG_INTDIR)
 SET(CMAKE_CFG_INTDIR "Release")
ENDIF(NOT CMAKE_CFG_INTDIR)

IF(CMAKE_CFG_INTDIR MATCHES "Debug")
	link_directories(${OpenCV_LIB_DIR_DBG})
ELSE(CMAKE_CFG_INTDIR MATCHES "Debug")
	link_directories(${OpenCV_LIB_DIR_OPT})
ENDIF(CMAKE_CFG_INTDIR MATCHES "Debug")


# Add libraries
if(WIN32 AND NOT APPLE)
    target_link_libraries(CameraCalibrationExample  ${OpenCV_LIBS})
else()
    target_link_libraries(CameraCalibrationExample  ${OpenCV_LIBS})
endif()

# Set the project architecture for this example
set( PROJECT_ARCH "x86" )

# Main cmake file 

cmake_minimum_required(VERSION 2.6)

# Main project name is ARMaker
project(HCI571X_OpenCV_Load_Image)

# Find packages
FIND_PACKAGE(OpenGL)
FIND_PACKAGE(OpenSceneGraph)
FIND_PACKAGE(osg)
FIND_PACKAGE(osgDB)
FIND_PACKAGE(osgGA)
FIND_PACKAGE(osgViewer)
FIND_PACKAGE(osgUtil)
FIND_PACKAGE(OpenThreads)
FIND_PACKAGE(OpenCV REQUIRED)

# Include dirs
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

# Add all files to the configuration
file(GLOB HCI571X_OpenCV_Load_Image_SRC
    "*.h"
    "OpenCV-Load_Image.cpp"
)

# Create an executable
add_executable( HCI571X_OpenCV_Load_Image ${HCI571X_OpenCV_Load_Image_SRC} )

# Add link directories
link_directories(${OPENSCENEGRAPH_LIBRARY_DIR})
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
	set(OSG_LIBS
		optimized ${OSG_LIBRARY}
		optimized ${OSGGA_LIBRARY}
		optimized ${OSGDB_LIBRARY}
		optimized ${OSGVIEWER_LIBRARY}
		optimized ${OSGUTIL_LIBRARY}
		optimized ${OPENTHREADS_LIBRARY}
		
		debug ${OSG_LIBRARY_DEBUG}
		debug ${OSGGA_LIBRARY_DEBUG}
		debug ${OSGDB_LIBRARY_DEBUG}
		debug ${OSGVIEWER_LIBRARY_DEBUG}
		debug ${OSGUTIL_LIBRARY_DEBUG}
		debug ${OPENTHREADS_LIBRARY_DEBUG}
	)
    target_link_libraries(HCI571X_OpenCV_Load_Image ${OPENGL_LIBRARIES} ${OSG_LIBS} ${OpenCV_LIBS})
else()
	set(OSG_LIBS
		${OSG_LIBRARY}
		${OSGGA_LIBRARY}
		${OSGDB_LIBRARY}
		${OSGVIEWER_LIBRARY}
		${OSGUTIL_LIBRARY}
		${OPENTHREADS_LIBRARY}
	)
    target_link_libraries(HCI571X_OpenCV_Load_Image ${OPENGL_LIBRARIES} ${OSG_LIBS} ${OpenCV_LIBS})
endif()

# Set the project architecture for this example
set( PROJECT_ARCH "x86" )

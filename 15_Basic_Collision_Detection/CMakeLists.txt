# Main cmake file 

cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 OLD)

# Main project name is ARMaker
project(HCI571_Collision_Detection_Basic)

# Find packages
FIND_PACKAGE(OpenGL)
FIND_PACKAGE(OpenSceneGraph)
FIND_PACKAGE(osg)
FIND_PACKAGE(osgDB)
FIND_PACKAGE(osgGA)
FIND_PACKAGE(osgViewer)
FIND_PACKAGE(osgUtil)
FIND_PACKAGE(osgText)
FIND_PACKAGE(OpenThreads)
FIND_PACKAGE(OpenCV REQUIRED)

# Include dirs
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

# Add all files to the configuration
file(GLOB HCI571_Collision_Detection_Basic_SRC
    "*.h"
    "main.cpp"
)

# Create an executable
add_executable( HCI571_Collision_Detection_Basic ${HCI571_Collision_Detection_Basic_SRC} )

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
		optimized ${OSGTEXT_LIBRARY}
		
		debug ${OSG_LIBRARY_DEBUG}
		debug ${OSGGA_LIBRARY_DEBUG}
		debug ${OSGDB_LIBRARY_DEBUG}
		debug ${OSGVIEWER_LIBRARY_DEBUG}
		debug ${OSGUTIL_LIBRARY_DEBUG}
		debug ${OPENTHREADS_LIBRARY_DEBUG}
		debug ${OSGTEXT_LIBRARY_DEBUG}
	)
	
	
    target_link_libraries(HCI571X_Collision_Detection_Basic ${OPENGL_LIBRARIES} ${OSG_LIBS} ${OpenCV_LIBS} )
else()
	set(OSG_LIBS
		${OSG_LIBRARY}
		${OSGGA_LIBRARY}
		${OSGDB_LIBRARY}
		${OSGVIEWER_LIBRARY}
		${OSGUTIL_LIBRARY}
		${OPENTHREADS_LIBRARY}
        ${OSGTEXT_LIBRARY}
	)
	
	
    target_link_libraries(HCI571_Collision_Detection_Basic ${OPENGL_LIBRARIES} ${OSG_LIBS} ${OpenCV_LIBS} )
endif()



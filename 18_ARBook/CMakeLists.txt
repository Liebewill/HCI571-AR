# Main cmake file 

cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 OLD)

# Main project name is ARLens
project(HCI571_ARBook)

# Find packages
FIND_PACKAGE(OpenGL)
FIND_PACKAGE(OpenSceneGraph)
FIND_PACKAGE(osg)
FIND_PACKAGE(osgDB)
FIND_PACKAGE(osgGA)
FIND_PACKAGE(osgText)
FIND_PACKAGE(osgViewer)
FIND_PACKAGE(osgUtil)
FIND_PACKAGE(OpenThreads)
FIND_PACKAGE(OpenCV REQUIRED)

# Include dirs
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("./StateMachine")

# Add all files to the configuration
file(GLOB HCI571_ARBook_SRC
	"main.cpp"
	"ARPaddle.*"
	"ARPaddleCallback.*"
	"ARPaddleOperation.*"
	"ARPaddleStateParameters.*"
	"DistanceNodeVisitor.*"
	"ModelHandler.*"
	"MoveableObject.*"
	"MoveableObjectCallback.*"
	"ARPaddleTypes.*"
	"ARBook.*"
	"ARBookVisitor.*"

)

set(UTILS_SRC
    "PrerenderSubgraph.h"
	"PrerenderSubgraph.cpp"
	"ExampleGeometry.h"
	"ExampleGeometry.cpp"
	"KeyboardEventHandler.h"
	"KeyboardEventHandler.cpp"
)

set(VIDEO_SRC
	"VideoScreen.h"
	"VideoScreen.cpp"
	"GLSL_ShaderLoader.h"
	"GLSL_ShaderLoader.cpp"
	"HUD.h"
	"HUD.cpp*"
	"VideoRenderer.h"
	"VideoRenderer.cpp"
	"VideoTexCallback.h"
	"VideoTexCallback.cpp"
)

set(TRACKING_SRC
	"MarkerDetector.h"
	"MarkerDetector.cpp"
	"Marker.h"
	"Marker.cpp"
	"GeometryTypes.h"
	"GeometryTypes.cpp"
	"ARToolkitTypes.h"
	"ARTagNode.h"
	"ARTagNode.cpp"
)

set(Statemachine_SRC
	"./StateMachine/FiniteStateMachine.h"
	"./StateMachine/FiniteStateMachine.cpp"
	"./StateMachine/FiniteStateMachineBuilder.h"
	"./StateMachine/FiniteStateMachineBuilder.cpp"
	"./StateMachine/State.h"
	"./StateMachine/State.cpp"
	"./StateMachine/StateOperation.h"
	"./StateMachine/StateOperation.cpp"
	"./StateMachine/StateParameter.h"
	"./StateMachine/StateParameter.cpp"
	"./StateMachine/Transition.h"
	"./StateMachine/Transition.cpp"
	"./StateMachine/StateTypes.h"

)


#  SRC Groups
source_group(Marker_Tracking FILES ${TRACKING_SRC})
source_group(Video FILES ${VIDEO_SRC})
source_group(Utils FILES ${UTILS_SRC})
source_group(StateMachine FILES ${Statemachine_SRC})


# Create an executable
add_executable( HCI571_ARBook ${HCI571_ARBook_SRC} ${TRACKING_SRC} ${VIDEO_SRC} ${UTILS_SRC} ${Statemachine_SRC})

# Add link directories
link_directories(${OPENSCENEGRAPH_LIBRARY_DIR})

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
	
	set(OPENCV
		optimized opencv_highgui249.lib
		optimized opencv_imgproc249.lib
		optimized opencv_core249.lib
		optimized opencv_contrib249.lib
		optimized opencv_calib3d249.lib
	
		debug opencv_highgui249d.lib
		debug opencv_imgproc249d.lib
		debug opencv_core249d.lib
		debug opencv_contrib249d.lib
		debug opencv_calib3d249d.lib
	)
	
	
    target_link_libraries(HCI571_ARBook ${OPENGL_LIBRARIES} ${OSG_LIBS} ${OPENCV} )
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

	
	
    target_link_libraries(HCI571_ARBook ${OPENGL_LIBRARIES} ${OSG_LIBS} ${OpenCV_LIBS} )
endif()


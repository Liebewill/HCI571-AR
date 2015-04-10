# Main cmake file 

cmake_minimum_required(VERSION 2.6)

# Main project name is ARMaker
project(HCI571X-GLSL_Basic_Shader)

# Find packages
FIND_PACKAGE(OpenGL)
FIND_PACKAGE(OpenSceneGraph)
FIND_PACKAGE(osg)
FIND_PACKAGE(osgDB)
FIND_PACKAGE(osgGA)
FIND_PACKAGE(osgViewer)
FIND_PACKAGE(osgUtil)
FIND_PACKAGE(OpenThreads)

# Include dirs
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

# Add all files to the configuration
file(GLOB HCI571X-GLSL_Basic_Shader_SRC
    "*.h"
    "OSG_Basic_Shader_Example.cpp"
	"GLSL_ShaderLoader.cpp"
)

# Create an executable
add_executable( HCI571X-GLSL_Basic_Shader ${HCI571X-GLSL_Basic_Shader_SRC} )

# Add link directories
link_directories(${OPENSCENEGRAPH_LIBRARY_DIR})


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
    target_link_libraries(HCI571X-GLSL_Basic_Shader ${OPENGL_LIBRARIES} ${OSG_LIBS})
else()
	set(OSG_LIBS
		${OSG_LIBRARY}
		${OSGGA_LIBRARY}
		${OSGDB_LIBRARY}
		${OSGVIEWER_LIBRARY}
		${OSGUTIL_LIBRARY}
		${OPENTHREADS_LIBRARY}
	)
    target_link_libraries(HCI571X-GLSL_Basic_Shader ${OPENGL_LIBRARIES} ${OSG_LIBS})
endif()

# Set the project architecture for this example
set( PROJECT_ARCH "x86" )

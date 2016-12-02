CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4)

PROJECT(Ray_Tracer)

FIND_PACKAGE(glfw3 3.2 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)

FIND_LIBRARY(CGM_LIB   cgm)
FIND_LIBRARY(CGS_LIB   cgs)
FIND_LIBRARY(GLAUX_LIB glaux)

SET(my_libs_dir C:/Users/mateu/Documents/Projects/Libraries )

INCLUDE_DIRECTORIES(src ${my_libs_dir}/cgm/src ${my_libs_dir}/cgs/src ${my_libs_dir}/glaux/src C:/glfw/include  C:/glew-2.0.0/include ${OPENGL_INCLUDE_DIR})

LINK_DIRECTORIES(C:/glew-2.0.0/lib/Release/Win32 C:/glfw/lib)


SET(INCLUDE_FILES src/ray_tracer.hpp)
SET(SOURCE_FILES  src/ray_tracer.cpp)

ADD_EXECUTABLE(Ray_Tracer ${INCLUDE_FILES} ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(Ray_Tracer glfw3 ${OPENGL_gl_LIBRARY} glew32s  ${CGM_LIB} ${CGS_LIB} ${GLAUX_LIB})



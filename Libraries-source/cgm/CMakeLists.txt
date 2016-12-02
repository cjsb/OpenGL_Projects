CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4)
PROJECT(cgm)
INCLUDE_DIRECTORIES(src)

SET(INCLUDE_FILES src/vec2.hpp src/vec3.hpp src/vector_operations.hpp src/mat4.hpp src/matrix_transform.hpp)
SET(SOURCE_FILES  src/vec2.cpp src/vec3.cpp src/vector_operations.cpp src/mat4.cpp src/matrix_transform.cpp)

ADD_LIBRARY(cgm STATIC ${INCLUDE_FILES} ${SOURCE_FILES})
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4)
PROJECT(cgs)

INCLUDE_DIRECTORIES(src ../cgm/src)

SET(INCLUDE_FILES src/Camera.hpp src/Transform.hpp src/Euler.hpp)
SET(SOURCE_FILES  src/Camera.cpp src/Transform.cpp src/Euler.cpp)


ADD_LIBRARY(cgs STATIC ${INCLUDE_FILES} ${SOURCE_FILES})

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4)

PROJECT(gls)

find_package(OpenGL REQUIRED)

#assimp include 
FIND_PATH(ASSIMP_INCLUDE_DIR importer.hpp)

#SOIL include
FIND_PATH(SOIL_INCLUDE_DIR SOIL.h)

#cgm include
FIND_PATH(CGM_INCLUDE_DIR  vec3.hpp)

#glfw library
FIND_LIBRARY(GLFW_LIBRARY glfw3)
FIND_PATH(GLFW_INCLUDE_DIR glfw3.h)

#glew library
FIND_LIBRARY(GLEW_LIBRARY glew32s)
FIND_PATH(GLEW_INCLUDE_DIR glew.h)

include_directories(src  ${GLFW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${SOIL_INCLUDE_DIR} ${CGM_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR})

#link_directories(C:/glew-2.0.0/lib/Release/Win32 C:/glfw/lib )

SET(INCLUDE_FILES src/Shader.hpp src/Vertex1P1N1UV.hpp src/Texture.hpp src/Mesh.hpp src/Model.hpp)
SET(SOURCE_FILES  src/Shader.cpp  src/Texture.cpp src/Mesh.cpp src/Model.cpp)


ADD_LIBRARY(gls STATIC ${INCLUDE_FILES} ${SOURCE_FILES} )

target_include_directories(gls PUBLIC  ${OPENGL_INCLUDE_DIR})

target_link_libraries(gls ${GLFW_LIBRARY} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY})
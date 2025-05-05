cmake_minimum_required(VERSION 3.0)
project(Editor)

# Find OpenCV package
find_package(OpenCV REQUIRED)

#Include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Create executable
add_executable(Editor main.cpp)

# Add local libraries
add_library(inputHelper inputHelper.cpp inputHelper.h)
add_library(transformations transformations.cpp transformations.h)

# Link OpenCV libraries
target_link_libraries(Editor ${OpenCV_LIBS})
target_link_libraries(Editor inputHelper)
target_link_libraries(Editor transformations)
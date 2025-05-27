cmake_minimum_required(VERSION 3.0)
project(Editor)

# Find OpenCV package
find_package(OpenCV REQUIRED)
#find_package (PkgConfig REQUIRED)

#Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
#include_directories(${GTK_INCLUDE_DIRS})

# Create executable
add_executable(Editor 
    main.cpp 
    record.cpp record.h 
    inputHelper.cpp inputHelper.h 
    operations.cpp operations.h 
    inputs.h)

# Link OpenCV libraries
target_link_libraries(Editor ${OpenCV_LIBS})
# ${GTK_LIBS}
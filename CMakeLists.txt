cmake_minimum_required(VERSION 3.5)
project(SerialLib)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-Wall")
set (CMAKE_CXX_FLAGS_RELEASE "-O2")
add_library(${PROJECT_NAME} STATIC binarydata.cpp serializer.cpp)

#add_executable(SerialTest main.cpp)
#target_link_libraries(SerialTest, SerialLib)

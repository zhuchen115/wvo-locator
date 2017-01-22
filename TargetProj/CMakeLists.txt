cmake_minimum_required(VERSION 3.0)
project(pprobot)
find_package(OpenCV REQUIRED)
find_package(WiringPi REQUIRED)
find_package(Threads REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${WIRINGPI_INCLUDE_DIRS})

#set(CMAKE_INSTALL_PREFIX /opt)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(SHARED_LIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
#set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)


add_subdirectory(src)
#add_subdirectory(programs)





cmake_minimum_required(VERSION 3.0)
include_directories(${SHARED_LIB_INCLUDE_DIR})
set(AGOLIB_SRC Camera.cpp ColorFilter.cpp config.cpp ContourFinder.cpp math_func.cpp objectdimension.cpp)

#add_library(agoshared STATIC ${AGOLIB_SRC})

set(HWLIB_SRC spi_driver.cpp mice.cpp)
#add_library(hwshared STATIC ${HWLIB_SRC})

#target_link_libraries(agoshared)
#target_link_libraries(hwshared)
add_executable(recodemo recognizedemo.cpp ${AGOLIB_SRC})
target_link_libraries(recodemo ${OpenCV_LIBS})

add_executable(movedist irmain.cpp ${AGOLIB_SRC})
target_link_libraries(movedist ${OpenCV_LIBS})

add_executable(rbtmain robotentry.cpp ${AGOLIB_SRC} ${HWLIB_SRC})
target_link_libraries(rbtmain ${OpenCV_LIBS} ${WIRINGPI_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_executable(micetest mice_main.cpp mice.cpp config.cpp)
target_link_libraries(micetest ${CMAKE_THREAD_LIBS_INIT})

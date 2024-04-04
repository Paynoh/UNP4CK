cmake_minimum_required(VERSION 3.10)
project(UNP4CK)

find_package(FLTK REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")


include_directories(${FLTK_INCLUDE_DIRS})

add_executable(UNP4CK Main.cpp P4CK.cpp)
target_link_libraries(UNP4CK ${FLTK_LIBRARIES})

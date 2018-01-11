cmake_minimum_required(VERSION 3.5)

project(heos-cpp-api)

file(GLOB SOURCE *.cpp )
add_subdirectory (json)

add_library(heos-cpp-api SHARED ${SOURCE})
target_link_libraries (heos-cpp-api LINK_PUBLIC json)
target_include_directories(heos-cpp-api PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

  
   

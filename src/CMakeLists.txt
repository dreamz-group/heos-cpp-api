cmake_minimum_required(VERSION 3.5)

project(heos-cpp-api)

file(GLOB SOURCE *.cpp )
add_library(heos-cpp-api SHARED ${SOURCE})
target_include_directories(heos-cpp-api PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

  
   

cmake_minimum_required(VERSION 3.5)

project(json)

file(GLOB SOURCE "*.cpp" )
add_library(json SHARED ${SOURCE})

target_include_directories(json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


  
   

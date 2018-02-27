
add_executable (heos-cpp-api-test heosSocket.cpp)
add_executable (t j.cpp)

target_link_libraries (heos-cpp-api-test LINK_PUBLIC heos-cpp-api)
target_link_libraries (t LINK_PUBLIC json)

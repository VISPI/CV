cmake_minimum_required(VERSION 2.8)
project( alg.cpp )
find_package( OpenCV REQUIRED )
add_executable( alg alg.cpp )
target_link_libraries( alg  ${OpenCV_LIBS} )


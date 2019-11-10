cmake_minimum_required(VERSION 3.15)
project(Socket)

set(CMAKE_CXX_STANDARD 14)
find_package (Threads)
#target_link_libraries (Socket/lib ${CMAKE_THREAD_LIBS_INIT})
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(Socket main.cpp Request.cpp Request.h Fs.cpp Fs.h Response.cpp Response.h config.h utils.cpp utils.h my_thread.cpp my_thread.h http_server.cpp http_server.h)
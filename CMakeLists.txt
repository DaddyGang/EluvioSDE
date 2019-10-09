cmake_minimum_required( VERSION 3.5 )
cmake_policy( VERSION 3.5 )


project( EluvioSDE )

set(HEADER_FILES main.h threadpool.h)
set(SOURCES main.cpp)

add_executable( test main.cpp)


add_definitions(-std=c++14)

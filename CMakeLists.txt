project(containers)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 98)

set(CMAKE_CXX_FLAGS "-stdlib=libc++ -Wall -Wextra -Werror")


#Main sources
file(GLOB srcs "*.cpp")
file(GLOB containers "srcs/*.hpp")

#Utils headers
#System headers
include_directories(/usr/local/include)

add_executable(containers ${srcs} ${containers})
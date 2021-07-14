project(containers)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-D STANDAR -stdlib=libc++ -Wall -Wextra -Werror")
set(CMAKE_BUILD_TYPE Debug)

#Main sources
file(GLOB srcs "*.cpp")
file(GLOB containers "srcs/*.hpp")

#Utils headers
#System headers
include_directories(/usr/local/include)

add_executable(containers ${srcs} ${containers})
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++14 -pedantic -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(src/
                    ${Boost_INCLUDE_DIRS}
                    ${OpenCV_INCLUDE_DIRS})

set(SRC src/allrgb.cc
        src/transformer.cc)

add_executable(allrgb ${SRC})
target_link_libraries(allrgb ${Boost_LIBRARIES})
target_link_libraries(allrgb ${OpenCV_LIBRARIES})

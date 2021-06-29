cmake_minimum_required(VERSION 3.5)

project(Packeger_Extractor LANGUAGES CXX)
file (GLOB  SRCS_HEDS *.cc *.h )
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS system thread regex filesystem)

add_executable(${PROJECT_NAME} ${SRCS_HEDS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

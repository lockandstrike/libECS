cmake_minimum_required(VERSION 3.5)

project(libECS)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
include_directories(include)

file(GLOB SOURCES src/ECS/*.cpp)
file(GLOB HEADERS include/ECS/*.h)

add_library(ECS SHARED ${SOURCES})

install(TARGETS ECS DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/ECS)

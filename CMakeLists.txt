cmake_minimum_required(VERSION 3.14)

project(tankgame VERSION 0.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_subdirectory(mdfgame)
add_subdirectory(vendor)

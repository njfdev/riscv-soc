cmake_minimum_required(VERSION 3.16)
project(soc LANGUAGES CXX)

set(RTL_TOP_MODULE soc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(verilator REQUIRED)

add_executable(V${RTL_TOP_MODULE} tests/verilator/main.cpp)

file(GLOB RTL_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/rtl/*.sv")

verilate(V${RTL_TOP_MODULE}
  TOP_MODULE ${RTL_TOP_MODULE}
  PREFIX V${RTL_TOP_MODULE}
  SOURCES ${RTL_SOURCES}
  VERILATOR_ARGS --timing --trace #-Wall
)


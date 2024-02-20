cmake_minimum_required(VERSION 3.27)
project(rvemu)

set(CMAKE_CXX_STANDARD 23)

find_package(fmt)

add_library(
    RVEmuCore
    RVEmu.h
    DRAM.hpp
    DRAM.cpp
    Bus.hpp
    Bus.cpp
    CPU.hpp
    CPU.cpp
    Exception.h
    Exception.cpp
)

add_executable(
    rvemu
    main.cpp
)

target_link_libraries(RVEmuCore PRIVATE fmt::fmt-header-only)
target_link_libraries(rvemu PRIVATE RVEmuCore)

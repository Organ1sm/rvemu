cmake_minimum_required(VERSION 3.27)
project(rvemu)

set(CMAKE_CXX_STANDARD 23)

find_package(fmt)

include(FetchContent)
fetchcontent_declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG
        v3.4.0 # or a later release
)
fetchcontent_makeavailable(Catch2)

add_library(
    RVEmuCore
    RVEmu.h
    DRAM.hpp
    DRAM.cpp
    Bus.hpp
    Bus.cpp
    CPU.hpp
    CPU.cpp
    InstructionExecutor.hpp
    InstructionExecutor.cpp
)

add_executable(rvemu main.cpp)

add_executable(
    utilTest
    tests/testUtil.hpp
    tests/testUtil.cpp
)

target_link_libraries(RVEmuCore PRIVATE fmt::fmt-header-only)
target_link_libraries(rvemu PRIVATE RVEmuCore)
target_link_libraries(utilTest PRIVATE RVEmuCore Catch2::Catch2)

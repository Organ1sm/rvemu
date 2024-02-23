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
    Log.hpp
)

add_executable(rvemu main.cpp)

include(CTest)
include(Catch)
add_executable(
    unitTest
    tests/testUtil.hpp
    tests/testUtil.cpp
    tests/instructionsTest.cpp
)

target_link_libraries(RVEmuCore PRIVATE fmt::fmt-header-only)
target_link_libraries(rvemu PRIVATE RVEmuCore)
target_link_libraries(
    unitTest
    PRIVATE
        RVEmuCore
        Catch2::Catch2WithMain
)

catch_discover_tests(unitTest)

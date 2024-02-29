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
    CSR.hpp
    CSR.cpp
)

add_executable(rvemu main.cpp)

target_link_libraries(RVEmuCore PRIVATE fmt::fmt-header-only)
target_link_libraries(rvemu PRIVATE RVEmuCore)

function(add_test_target test_name)
    target_link_libraries(
        ${test_name}
        PRIVATE
            RVEmuCore
            Catch2::Catch2WithMain
    )
    catch_discover_tests(${test_name})
endfunction()

include(CTest)
include(Catch)
add_executable(
    instsTest
    tests/testUtil.hpp
    tests/testUtil.cpp
    tests/InstsTest.cpp
)
add_executable(csrTest tests/CSRTest.cpp)
add_executable(cpuTest tests/CPUTest.cpp)
add_executable(busTest tests/BUSTest.cpp)
add_executable(dramTest tests/DRAMTest.cpp)

add_test_target(instsTest)
add_test_target(cpuTest)
add_test_target(busTest)
add_test_target(csrTest)
add_test_target(dramTest)

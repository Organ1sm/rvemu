cmake_minimum_required(VERSION 3.27)
project(rvemu)

set(CMAKE_CXX_STANDARD 23)
add_link_options("-fuse-ld=mold")

find_package(fmt)

include(FetchContent)
fetchcontent_declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG
        v3.4.0 # or a later release
)
fetchcontent_makeavailable(Catch2)

set(instsHeaders
    src/instructions/InstFormat.hpp
    src/instructions/Iformat.hpp
    src/instructions/Rformat.hpp
    src/instructions/Uformat.hpp
    src/instructions/Jformat.hpp
    src/instructions/Fence.hpp
    src/instructions/Load.hpp
    src/instructions/Store.hpp
    src/instructions/Branch.hpp
    src/instructions/System.hpp
)

set(componentsHeaders
    src/BitsManipulation.hpp
    src/Cpu.hpp
    src/Csr.hpp
    src/Emulator.hpp
    src/Memory.hpp
    src/RVEmu.hpp
    src/Registers.hpp
)

# To show up headers in IDE we need to make a target.
add_library(
    EmulatorHeaders
    INTERFACE
        ${componentsHeaders}
        ${instsHeaders}
)

set(instructions
    src/instructions/InstFormat.cpp
    src/instructions/Iformat.cpp
    src/instructions/Rformat.cpp
    src/instructions/Uformat.cpp
    src/instructions/Jformat.cpp
    src/instructions/Store.cpp
    src/instructions/Load.cpp
    src/instructions/Branch.cpp
    src/instructions/System.cpp
)

set(components
    src/BitsManipulation.cpp
    src/Cpu.cpp
    src/Csr.cpp
    src/Emulator.cpp
    src/Memory.cpp
    src/Registers.cpp
)

add_library(
    emulator
    STATIC
    ${instructions}
    ${components}
)

add_executable(newRVEMU src/main.cpp)

target_link_libraries(
    emulator
    PUBLIC
        EmulatorHeaders
    PRIVATE
        fmt::fmt-header-only
)
target_link_libraries(newRVEMU emulator)

function(add_test_target test_name)
    target_link_libraries(
        ${test_name}
        PRIVATE
            emulator
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

add_test_target(instsTest)


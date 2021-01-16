cmake_minimum_required(VERSION 3.8)

project("reversible_data_hiding")

option(BUILD_TESTS "Build tests" ON)
option(BUILD_BENCHMARKS "Build benchmarks" ON)

# Boost - to parse command line params
set(BOOST_ROOT "C:\\Users\\madrat\\Desktop\\projects\\libraries\\boost_1_75_0")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}\\lib64-msvc-14.2")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system)

set(CMAKE_CXX_STANDARD 20)

message(STATUS "##################### GLOBAL OPTIONS #####################")
message(STATUS "BUILD_TESTS        : ${BUILD_TESTS}")
message(STATUS "BOOST_ROOT         : ${BOOST_ROOT}")
message(STATUS "BOOST_LIBRARYDIR   : ${BOOST_LIBRARYDIR}")
message(STATUS "CMAKE_CXX_STANDARD : ${CMAKE_CXX_STANDARD}")

include_directories(
    ${Boost_INCLUDE_DIRS}
    "libraries"
    "src"
)

add_subdirectory(src)
add_subdirectory(libraries/googletest)

if (${BUILD_TESTS} MATCHES ON)
    enable_testing()
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(tests)
endif()

if (${BUILD_BENCHMARKS} MATCHES ON)
    enable_testing()
    add_subdirectory(libraries/benchmark)
    add_subdirectory(benchmarks)
endif()

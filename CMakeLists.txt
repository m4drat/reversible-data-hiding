cmake_minimum_required(VERSION 3.8)

project("reversible_data_hiding")

# Boost - to parse command line params
set(BOOST_ROOT "C:\\Users\\madrat\\Desktop\\projects\\libraries\\boost_1_75_0")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}\\lib64-msvc-14.2")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system)

# Add source to this project's executable.
add_executable(${PROJECT_NAME} "src/main.cpp"  "include/img_matrix.h" "src/img_matrix.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${Boost_INCLUDE_DIRS}
    "libraries"
    "include"
)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} "${BOOST_LIBRARYDIR}/libboost_program_options-vc142-mt-gd-x64-1_75.lib")
# TODO: Add tests and install targets if needed.

set(BINARY ${CMAKE_PROJECT_NAME})

# Compile executable
add_executable(${BINARY}_run "main.cpp" "bmp_image.h" "bmp_image.cpp" "image_matrix.cpp" "image_matrix.h" "image_matrix-impl.h" "types.h" "utils.h" "encryptor.cpp" "encryptor.h" "options.h" "options.cpp")
set_property(TARGET ${BINARY}_run PROPERTY CXX_STANDARD 20)
target_compile_features(${BINARY}_run PRIVATE cxx_std_20)
if (CMAKE_BUILD_TYPE MATCHES ".*(D|d)ebug$")
    target_link_libraries(${BINARY}_run 
        ${Boost_LIBRARIES} 
        "${BOOST_LIBRARYDIR}/libboost_program_options-vc142-mt-gd-x64-1_75.lib" 
        "${BOOST_LIBRARYDIR}/libboost_thread-vc142-mt-gd-x64-1_75.lib"
        "${BOOST_LIBRARYDIR}/libboost_date_time-vc142-mt-gd-x64-1_75.lib"
        "${BOOST_LIBRARYDIR}/libboost_chrono-vc142-mt-gd-x64-1_75.lib")
else()
    target_link_libraries(${BINARY}_run 
        ${Boost_LIBRARIES} 
        "${BOOST_LIBRARYDIR}/libboost_program_options-vc142-mt-x64-1_75.lib" 
        "${BOOST_LIBRARYDIR}/libboost_thread-vc142-mt-x64-1_75.lib"
        "${BOOST_LIBRARYDIR}/libboost_date_time-vc142-mt-x64-1_75.lib"
        "${BOOST_LIBRARYDIR}/libboost_chrono-vc142-mt-x64-1_75.lib")
endif()

# Static library to use with tests
add_library(${BINARY}_lib STATIC "main.cpp" "bmp_image.h" "bmp_image.cpp" "image_matrix.cpp" "image_matrix.h" "image_matrix-impl.h" "types.h" "utils.h" "encryptor.cpp" "encryptor.h" "options.h" "options.cpp")
set_property(TARGET ${BINARY}_lib PROPERTY CXX_STANDARD 20)
target_compile_features(${BINARY}_lib PRIVATE cxx_std_20)

if (CMAKE_BUILD_TYPE MATCHES ".*(D|d)ebug$")
    target_link_libraries(${BINARY}_lib 
        ${Boost_LIBRARIES} 
        "${BOOST_LIBRARYDIR}/libboost_program_options-vc142-mt-gd-x64-1_75.lib" 
        "${BOOST_LIBRARYDIR}/libboost_thread-vc142-mt-gd-x64-1_75.lib"
        "${BOOST_LIBRARYDIR}/libboost_date_time-vc142-mt-gd-x64-1_75.lib"
        "${BOOST_LIBRARYDIR}/libboost_chrono-vc142-mt-gd-x64-1_75.lib")
else()
    target_link_libraries(${BINARY}_lib 
        ${Boost_LIBRARIES} 
        "${BOOST_LIBRARYDIR}/libboost_program_options-vc142-mt-x64-1_75.lib" 
        "${BOOST_LIBRARYDIR}/libboost_thread-vc142-mt-x64-1_75.lib"
        "${BOOST_LIBRARYDIR}/libboost_date_time-vc142-mt-x64-1_75.lib"
        "${BOOST_LIBRARYDIR}/libboost_chrono-vc142-mt-x64-1_75.lib")
endif()

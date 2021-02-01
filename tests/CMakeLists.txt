set(BINARY ${CMAKE_PROJECT_NAME}_test)

add_executable(${BINARY} "test_main.cpp" "test_image_matrix.cpp" "test_encryptor.cpp" "test_rlc_encoder.cpp")
set_property(TARGET ${BINARY} PROPERTY CXX_STANDARD 20)
target_compile_features(${BINARY} PRIVATE cxx_std_20)

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)

add_test(NAME ${BINARY} COMMAND ${BINARY})

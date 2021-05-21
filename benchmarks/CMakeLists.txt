set(BINARY ${CMAKE_PROJECT_NAME}_benchmark)

add_executable(${BINARY} "bench_main.cpp" "bench_encrypt.cpp" "bench_load.cpp" "bench_embed.cpp" "bench_extractor.cpp" "params_generator.h" "reference_images.h")
set_property(TARGET ${BINARY} PROPERTY CXX_STANDARD 20)
target_compile_features(${BINARY} PRIVATE cxx_std_20)

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib benchmark::benchmark)

# add_test(NAME ${BINARY} COMMAND ${BINARY})

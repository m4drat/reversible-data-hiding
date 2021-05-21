#include "benchmark/include/benchmark/benchmark.h"

#include "utils.h"
#include "image/bmp_image.h"
#include "encryptor/encryptor.h"

using namespace rdh;

static void Loader_Load_512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(rdh::BmpImage("..\\..\\..\\..\\images\\original\\airplane.bmp"));
    }
}
BENCHMARK(Loader_Load_512x512_bench)->Unit(benchmark::kMillisecond);

static void Loader_Load_1024x1024_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(rdh::BmpImage("..\\..\\..\\..\\images\\original\\liberty1024x1024.bmp"));
    }
}
BENCHMARK(Loader_Load_1024x1024_bench)->Unit(benchmark::kMillisecond);

static void Loader_Load_2048x2048_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(rdh::BmpImage("..\\..\\..\\..\\images\\original\\man2048x2048.bmp"));
    }
}
BENCHMARK(Loader_Load_2048x2048_bench)->Unit(benchmark::kMillisecond);

static void Loader_Load_4096x4096_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(rdh::BmpImage("..\\..\\..\\..\\images\\original\\man4096x4096.bmp"));
    }
}
BENCHMARK(Loader_Load_4096x4096_bench)->Unit(benchmark::kMillisecond);

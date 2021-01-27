#include "benchmark/include/benchmark/benchmark.h"

#include "utils.h"
#include "image/bmp_image.h"
#include "encryptor/encryptor.h"

using namespace rdh;

static void Load512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(rdh::BmpImage("..\\..\\..\\..\\images\\airplane.bmp"));
    }
}
BENCHMARK(Load512x512_bench)->Unit(benchmark::kMillisecond);

static void Load514x514_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(rdh::BmpImage("..\\..\\..\\..\\images\\boat514x514.bmp"));
    }
}
BENCHMARK(Load514x514_bench)->Unit(benchmark::kMillisecond);

static void Load1024x1024_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(rdh::BmpImage("..\\..\\..\\..\\images\\liberty1024x1024.bmp"));
    }
}
BENCHMARK(Load1024x1024_bench)->Unit(benchmark::kMillisecond);

static void Load2048x2048_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(rdh::BmpImage("..\\..\\..\\..\\images\\man2048x2048.bmp"));
    }
}
BENCHMARK(Load2048x2048_bench)->Unit(benchmark::kMillisecond);

static void Load4096x4096_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(rdh::BmpImage("..\\..\\..\\..\\images\\man4096x4096.bmp"));
    }
}
BENCHMARK(Load4096x4096_bench)->Unit(benchmark::kMillisecond);

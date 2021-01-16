#include "benchmark/include/benchmark/benchmark.h"

#include "options.h"
#include "utils.h"
#include "bmp_image.h"
#include "encryptor.h"

using namespace rdh;

static void EncryptAirplane_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadKeyFile<uint8_t>("..\\..\\..\\..\\example_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\airplane.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EncryptAirplane_bench);

static void EncryptCrowd_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadKeyFile<uint8_t>("..\\..\\..\\..\\example_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\crowd.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EncryptCrowd_bench);

static void EncryptMan_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadKeyFile<uint8_t>("..\\..\\..\\..\\example_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\man.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EncryptMan_bench);

static void EncryptBoat_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadKeyFile<uint8_t>("..\\..\\..\\..\\example_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\boat.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EncryptBoat_bench);

#include "benchmark/include/benchmark/benchmark.h"

#include "utils.h"
#include "image/bmp_image.h"
#include "encryptor/encryptor.h"

using namespace rdh;

static void EncryptAirplane_512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\airplane.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EncryptAirplane_512x512_bench)->Unit(benchmark::kMillisecond);

static void EncryptCrowd_512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\crowd.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EncryptCrowd_512x512_bench)->Unit(benchmark::kMillisecond);

static void EncryptMan_512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\man.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EncryptMan_512x512_bench)->Unit(benchmark::kMillisecond);

static void EncryptBoat_512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\boat.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EncryptBoat_512x512_bench)->Unit(benchmark::kMillisecond);

static void EncryptLiberty_1024x1024_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\liberty1024x1024.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EncryptLiberty_1024x1024_bench)->Unit(benchmark::kMillisecond);

static void EncryptMan_2048x2048_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\man2048x2048.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EncryptMan_2048x2048_bench)->Unit(benchmark::kMillisecond);

static void EncryptMan_4096x4096_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\man4096x4096.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EncryptMan_4096x4096_bench)->Unit(benchmark::kMillisecond);


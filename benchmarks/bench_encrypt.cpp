#include "benchmark/include/benchmark/benchmark.h"

#include "utils.h"
#include "image/bmp_image.h"
#include "encryptor/encryptor.h"

using namespace rdh;

static void Encryptor_Encrypt_Airplane_512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\airplane.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(Encryptor_Encrypt_Airplane_512x512_bench)->Unit(benchmark::kMillisecond);

static void Encryptor_Encrypt_Crowd_512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\crowd.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(Encryptor_Encrypt_Crowd_512x512_bench)->Unit(benchmark::kMillisecond);

static void Encryptor_Encrypt_Man_512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\man.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(Encryptor_Encrypt_Man_512x512_bench)->Unit(benchmark::kMillisecond);

static void Encryptor_Encrypt_Boat_512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\boat.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(Encryptor_Encrypt_Boat_512x512_bench)->Unit(benchmark::kMillisecond);

static void Encryptor_Encrypt_Liberty_1024x1024_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\liberty1024x1024.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(Encryptor_Encrypt_Liberty_1024x1024_bench)->Unit(benchmark::kMillisecond);

static void Encryptor_Encrypt_Man_2048x2048_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\man2048x2048.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(Encryptor_Encrypt_Man_2048x2048_bench)->Unit(benchmark::kMillisecond);

static void Encryptor_Encrypt_Man_4096x4096_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\original\\man4096x4096.bmp");
        state.ResumeTiming();
        Encryptor::Encrypt(image, encryptionKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(Encryptor_Encrypt_Man_4096x4096_bench)->Unit(benchmark::kMillisecond);


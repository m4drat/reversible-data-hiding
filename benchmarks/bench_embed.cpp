#include "benchmark/include/benchmark/benchmark.h"

#include "utils.h"
#include "embedder/embedder.h"

using namespace rdh;

static void EmbedAirplane_512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\airplane.bmp");
        state.ResumeTiming();
        Embedder::Embed(image, dataToEmbed, dataEmbedKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EmbedAirplane_512x512_bench)->Unit(benchmark::kMillisecond);

static void EmbedCrowd_512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\crowd.bmp");
        state.ResumeTiming();
        Embedder::Embed(image, dataToEmbed, dataEmbedKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EmbedCrowd_512x512_bench)->Unit(benchmark::kMillisecond);

static void EmbedMan_512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\man.bmp");
        state.ResumeTiming();
        Embedder::Embed(image, dataToEmbed, dataEmbedKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EmbedMan_512x512_bench)->Unit(benchmark::kMillisecond);

static void EmbedBoat_512x512_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\boat.bmp");
        state.ResumeTiming();
        Embedder::Embed(image, dataToEmbed, dataEmbedKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EmbedBoat_512x512_bench)->Unit(benchmark::kMillisecond);

static void EmbedLiberty_1024x1024_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\liberty1024x1024.bmp");
        state.ResumeTiming();
        Embedder::Embed(image, dataToEmbed, dataEmbedKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EmbedLiberty_1024x1024_bench)->Unit(benchmark::kMillisecond);

static void EmbedMan_2048x2048_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\man2048x2048.bmp");
        state.ResumeTiming();
        Embedder::Embed(image, dataToEmbed, dataEmbedKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EmbedMan_2048x2048_bench)->Unit(benchmark::kMillisecond);

static void EmbedMan_4096x4096_bench(benchmark::State& state) {
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\man4096x4096.bmp");
        state.ResumeTiming();
        Embedder::Embed(image, dataToEmbed, dataEmbedKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(EmbedMan_4096x4096_bench)->Unit(benchmark::kMillisecond);


#include "benchmark/include/benchmark/benchmark.h"

#include "utils.h"
#include "extractor/extractor.h"

using namespace rdh;

static void ExtractDataLena_512x512_bench(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted-marked\\lena_gray-embed.bmp");
        state.ResumeTiming();

        //Extractor::ExtractData(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits);
    }
}
BENCHMARK(ExtractDataLena_512x512_bench)->Unit(benchmark::kMillisecond);;

static void ExtractDataAirplane_512x512_bench(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted-marked\\airplane-embed.bmp");
        state.ResumeTiming();

        //Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")

    }
}
BENCHMARK(ExtractDataAirplane_512x512_bench)->Unit(benchmark::kMillisecond);;

static void ExtractDataCrowd_512x512_bench(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted-marked\\crowd-embed.bmp");
        state.ResumeTiming();

        //Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(ExtractDataCrowd_512x512_bench)->Unit(benchmark::kMillisecond);;

static void ExtractDataMan_512x512_bench(benchmark::State& state) 
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted-marked\\man-embed.bmp");
        state.ResumeTiming();
        
        //Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(ExtractDataMan_512x512_bench)->Unit(benchmark::kMillisecond);;

static void ExtractDataBoat_512x512_bench(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted-marked\\boat-embed.bmp");
        state.ResumeTiming();

        //Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
    }
}
BENCHMARK(ExtractDataBoat_512x512_bench)->Unit(benchmark::kMillisecond);;

//static void EmbedLiberty_1024x1024_bench(benchmark::State& state) {
//    for (auto _ : state)
//    {
//        state.PauseTiming();
//        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
//        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
//        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\liberty1024x1024-enc.bmp");
//        rdh::Consts::Instance().UpdateThreshold(state.range(0));
//        state.ResumeTiming();
//        Embedder::Embed(image, dataToEmbed, dataEmbedKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
//    }
//}
//BENCHMARK(EmbedLiberty_1024x1024_bench)->Unit(benchmark::kMillisecond)->DenseRange(10, 24, 1);
//
//static void EmbedMan_2048x2048_bench(benchmark::State& state) {
//    for (auto _ : state)
//    {
//        state.PauseTiming();
//        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
//        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
//        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\man2048x2048-enc.bmp");
//        rdh::Consts::Instance().UpdateThreshold(state.range(0));
//        state.ResumeTiming();
//        Embedder::Embed(image, dataToEmbed, dataEmbedKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
//    }
//}
//BENCHMARK(EmbedMan_2048x2048_bench)->Unit(benchmark::kMillisecond)->DenseRange(10, 24, 1);
//
//static void EmbedMan_4096x4096_bench(benchmark::State& state) {
//    for (auto _ : state)
//    {
//        state.PauseTiming();
//        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
//        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
//        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\man4096x4096-enc.bmp");
//        rdh::Consts::Instance().UpdateThreshold(state.range(0));
//        state.ResumeTiming();
//        Embedder::Embed(image, dataToEmbed, dataEmbedKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
//    }
//}
//BENCHMARK(EmbedMan_4096x4096_bench)->Unit(benchmark::kMillisecond)->DenseRange(10, 24, 1);

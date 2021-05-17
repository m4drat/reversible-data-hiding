#include "benchmark/include/benchmark/benchmark.h"

#include "utils.h"
#include "embedder/embedder.h"

using namespace rdh;

static void CustomArguments(benchmark::internal::Benchmark* b)
{
    for (uint16_t threshold : { 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 })
    {
        for (uint16_t alpha : { 4, 5, 6 })
        {
            for (uint16_t lambda : { 60, 80, 100, 150, 200, 300, 400 })
            {
                for (uint16_t lsbLayers : { 1, 2, 3 })
                {
                    b->Args({ threshold, alpha, lambda, lsbLayers });
                }
            }
        }
    }
}

static void EmbedLena_512x512_bench(benchmark::State& state)
{
    double tMax = 0.0f;
    uint32_t maxUserDataBits = 0;

    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\lena_gray-enc.bmp");
        
        rdh::Consts::Instance().UpdateThreshold(state.range(0));
        rdh::Consts::Instance().UpdateAlpha(state.range(1));
        rdh::Consts::Instance().UpdateLambda(state.range(2));
        rdh::Consts::Instance().UpdateLsbLayers(state.range(3));

        state.ResumeTiming();

        try
        {
            Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
        }
        catch (const std::exception& e)
        {
            state.SkipWithError(e.what());
            break;
        }
    }
 
    state.counters["MaxEmbeddingRate"] = tMax;
    state.counters["maxUserDataBits"] = maxUserDataBits;
}
BENCHMARK(EmbedLena_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);;

static void EmbedAirplane_512x512_bench(benchmark::State& state)
{
    double tMax = 0.0f;
    uint32_t maxUserDataBits = 0;

    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\airplane-enc.bmp");

        rdh::Consts::Instance().UpdateThreshold(state.range(0));
        rdh::Consts::Instance().UpdateAlpha(state.range(1));
        rdh::Consts::Instance().UpdateLambda(state.range(2));
        rdh::Consts::Instance().UpdateLsbLayers(state.range(3));

        state.ResumeTiming();

        try
        {
            Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
        }
        catch (const std::exception& e)
        {
            state.SkipWithError(e.what());
            break;
        }
    }

    state.counters["MaxEmbeddingRate"] = tMax;
    state.counters["maxUserDataBits"] = maxUserDataBits;
}
BENCHMARK(EmbedAirplane_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);;

static void EmbedCrowd_512x512_bench(benchmark::State& state)
{
    double tMax = 0.0f;
    uint32_t maxUserDataBits = 0;

    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\crowd-enc.bmp");

        rdh::Consts::Instance().UpdateThreshold(state.range(0));
        rdh::Consts::Instance().UpdateAlpha(state.range(1));
        rdh::Consts::Instance().UpdateLambda(state.range(2));
        rdh::Consts::Instance().UpdateLsbLayers(state.range(3));

        state.ResumeTiming();

        try
        {
            Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
        }
        catch (const std::exception& e)
        {
            state.SkipWithError(e.what());
            break;
        }
    }

    state.counters["MaxEmbeddingRate"] = tMax;
}
BENCHMARK(EmbedCrowd_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);;

static void EmbedMan_512x512_bench(benchmark::State& state) 
{
    double tMax = 0.0f;
    uint32_t maxUserDataBits = 0;

    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\man-enc.bmp");

        rdh::Consts::Instance().UpdateThreshold(state.range(0));
        rdh::Consts::Instance().UpdateAlpha(state.range(1));
        rdh::Consts::Instance().UpdateLambda(state.range(2));
        rdh::Consts::Instance().UpdateLsbLayers(state.range(3));

        state.ResumeTiming();

        try
        {
            Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
        }
        catch (const std::exception& e)
        {
            state.SkipWithError(e.what());
            break;
        }
    }

    state.counters["MaxEmbeddingRate"] = tMax;
    state.counters["maxUserDataBits"] = maxUserDataBits;
}
BENCHMARK(EmbedMan_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);;

static void EmbedBoat_512x512_bench(benchmark::State& state)
{
    double tMax = 0.0f;
    uint32_t maxUserDataBits = 0;

    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\boat-enc.bmp");

        rdh::Consts::Instance().UpdateThreshold(state.range(0));
        rdh::Consts::Instance().UpdateAlpha(state.range(1));
        rdh::Consts::Instance().UpdateLambda(state.range(2));
        rdh::Consts::Instance().UpdateLsbLayers(state.range(3));

        state.ResumeTiming();

        try
        {
            Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
        }
        catch (const std::exception& e)
        {
            state.SkipWithError(e.what());
            break;
        }
    }

    state.counters["MaxEmbeddingRate"] = tMax;
    state.counters["maxUserDataBits"] = maxUserDataBits;
}
BENCHMARK(EmbedBoat_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);;

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

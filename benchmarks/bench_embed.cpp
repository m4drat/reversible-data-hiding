#include "benchmark/include/benchmark/benchmark.h"

#include <iostream>

#include "params_generator.h"
#include "reference_images.h"

#include "utils.h"
#include "embedder/embedder.h"
#include "image/image_quality.h"

using namespace rdh;

static void Embedder_Embed_Lena_512x512_bench(benchmark::State& state)
{
    double tMax = 0.0f;
    uint32_t maxUserDataBits = 0;
    double psnr = 0;
    double ssim = 0;

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
            Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits);
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
BENCHMARK(Embedder_Embed_Lena_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);

static void Embedder_Embed_Airplane_512x512_bench(benchmark::State& state)
{
    double tMax = 0.0f;
    uint32_t maxUserDataBits = 0;
    double psnr = 0;
    double ssim = 0;

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
            Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits);
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
BENCHMARK(Embedder_Embed_Airplane_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);

static void Embedder_Embed_Crowd_512x512_bench(benchmark::State& state)
{
    double tMax = 0.0f;
    uint32_t maxUserDataBits = 0;
    double psnr = 0;
    double ssim = 0;

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
            Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits);
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
BENCHMARK(Embedder_Embed_Crowd_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);

static void Embedder_Embed_Man_512x512_bench(benchmark::State& state)
{
    double tMax = 0.0f;
    uint32_t maxUserDataBits = 0;
    double psnr = 0;
    double ssim = 0;

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
            Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits);
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
BENCHMARK(Embedder_Embed_Man_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);

static void Embedder_Embed_Boat_512x512_bench(benchmark::State& state)
{
    double tMax = 0.0f;
    uint32_t maxUserDataBits = 0;
    double psnr = 0;
    double ssim = 0;

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
            Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits);
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
BENCHMARK(Embedder_Embed_Boat_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);

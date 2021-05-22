#include "benchmark/include/benchmark/benchmark.h"

#include "params_generator.h"
#include "reference_images.h"

#include "utils.h"
#include "embedder/embedder.h"
#include "extractor/extractor.h"
#include "image/image_quality.h"

using namespace rdh;

static void Extractor_ExtractData_Boat_512x512_bench(benchmark::State& state)
{
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

        Embedder::Embed(image, dataToEmbed, dataEmbedKey, std::nullopt, std::nullopt);
        
        state.ResumeTiming();

        try
        {
            Extractor::ExtractData(image, "", dataEmbedKey);
        }
        catch (const std::exception& e)
        {
            state.SkipWithError(e.what());
            break;
        }
    }
}
BENCHMARK(Extractor_ExtractData_Boat_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);

static void Extractor_RecoverImage_Boat_512x512_bench(benchmark::State& state)
{
    rdh::BmpImage image(0, 0);

    for (auto _ : state)
    {
        state.PauseTiming();

        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        image = rdh::BmpImage("..\\..\\..\\..\\images\\encrypted\\boat-enc.bmp");

        rdh::Consts::Instance().UpdateThreshold(state.range(0));
        rdh::Consts::Instance().UpdateAlpha(state.range(1));
        rdh::Consts::Instance().UpdateLambda(state.range(2));
        rdh::Consts::Instance().UpdateLsbLayers(state.range(3));

        Embedder::Embed(image, dataToEmbed, dataEmbedKey, std::nullopt, std::nullopt);

        state.ResumeTiming();

        try
        {
            Extractor::RecoverImage(image, "", encryptionKey);
        }
        catch (const std::exception& e)
        {
            state.SkipWithError(e.what());
            break;
        }
    }

    state.counters["PSNR"] = rdh::ImageQuality::CalculatePSNR(References::GetInstance().GetBoat(), image);
    state.counters["SSIM"] = rdh::ImageQuality::CalculateSSIM(References::GetInstance().GetBoat(), image);
}
BENCHMARK(Extractor_RecoverImage_Boat_512x512_bench)->Iterations(3)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);

static void Extractor_RecoverImageExtractData_Boat_512x512_bench(benchmark::State& state)
{
    rdh::BmpImage image(0, 0);

    for (auto _ : state)
    {
        state.PauseTiming();

        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
        image = rdh::BmpImage("..\\..\\..\\..\\images\\encrypted\\boat-enc.bmp");

        rdh::Consts::Instance().UpdateThreshold(state.range(0));
        rdh::Consts::Instance().UpdateAlpha(state.range(1));
        rdh::Consts::Instance().UpdateLambda(state.range(2));
        rdh::Consts::Instance().UpdateLsbLayers(state.range(3));

        Embedder::Embed(image, dataToEmbed, dataEmbedKey, std::nullopt, std::nullopt);

        state.ResumeTiming();

        try
        {
            Extractor::RecoverImageAndExract(image, "", "", dataEmbedKey, encryptionKey);
        }
        catch (const std::exception& e)
        {
            state.SkipWithError(e.what());
            break;
        }
    }

    state.counters["PSNR"] = rdh::ImageQuality::CalculatePSNR(References::GetInstance().GetBoat(), image);
    state.counters["SSIM"] = rdh::ImageQuality::CalculateSSIM(References::GetInstance().GetBoat(), image);
}
BENCHMARK(Extractor_RecoverImageExtractData_Boat_512x512_bench)->Iterations(3)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);

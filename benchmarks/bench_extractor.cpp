//#include "benchmark/include/benchmark/benchmark.h"
//
//#include "params_generator.h"
//
//#include "utils.h"
//#include "embedder/embedder.h"
//#include "extractor/extractor.h"
//
//using namespace rdh;
//
//static void ExtractDataLena_512x512_bench(benchmark::State& state)
//{
//    for (auto _ : state)
//    {
//        state.PauseTiming();
//
//        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
//        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
//        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\lena_gray-enc.bmp");
//
//        rdh::Consts::Instance().UpdateThreshold(state.range(0));
//        rdh::Consts::Instance().UpdateAlpha(state.range(1));
//        rdh::Consts::Instance().UpdateLambda(state.range(2));
//        rdh::Consts::Instance().UpdateLsbLayers(state.range(3));
//
//        Embedder::Embed(image, dataToEmbed, dataEmbedKey, std::nullopt, std::nullopt); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
//
//        state.ResumeTiming();
//
//        Extractor::ExtractData(image, "", dataEmbedKey);
//    }
//}
//BENCHMARK(ExtractDataLena_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);
//
//static void RecoverImageLena_512x512_bench(benchmark::State& state)
//{
//    for (auto _ : state)
//    {
//        state.PauseTiming();
//
//        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
//        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
//        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
//        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\lena_gray-enc.bmp");
//
//        rdh::Consts::Instance().UpdateThreshold(state.range(0));
//        rdh::Consts::Instance().UpdateAlpha(state.range(1));
//        rdh::Consts::Instance().UpdateLambda(state.range(2));
//        rdh::Consts::Instance().UpdateLsbLayers(state.range(3));
//
//        Embedder::Embed(image, dataToEmbed, dataEmbedKey, std::nullopt, std::nullopt); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
//
//        state.ResumeTiming();
//
//        Extractor::RecoverImage(image, "", encryptionKey);
//    }
//}
//BENCHMARK(RecoverImageLena_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);
//
//static void RecoverImageExtractDataLena_512x512_bench(benchmark::State& state)
//{
//    for (auto _ : state)
//    {
//        state.PauseTiming();
//
//        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
//        std::vector<uint8_t> encryptionKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_encrypt_key.bin");
//        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
//        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\lena_gray-enc.bmp");
//
//        rdh::Consts::Instance().UpdateThreshold(state.range(0));
//        rdh::Consts::Instance().UpdateAlpha(state.range(1));
//        rdh::Consts::Instance().UpdateLambda(state.range(2));
//        rdh::Consts::Instance().UpdateLsbLayers(state.range(3));
//
//        Embedder::Embed(image, dataToEmbed, dataEmbedKey, std::nullopt, std::nullopt); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
//
//        state.ResumeTiming();
//
//        Extractor::RecoverImageAndExract(image, "", "", dataEmbedKey, encryptionKey);
//    }
//}
//BENCHMARK(RecoverImageLena_512x512_bench)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);
//
//static void ExtractDataAirplane_512x512_bench(benchmark::State& state)
//{
//    for (auto _ : state)
//    {
//        state.PauseTiming();
//        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
//        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted-marked\\airplane-embed.bmp");
//        state.ResumeTiming();
//
//        //Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
//
//    }
//}
//BENCHMARK(ExtractDataAirplane_512x512_bench)->Unit(benchmark::kMillisecond);
//
//static void ExtractDataCrowd_512x512_bench(benchmark::State& state)
//{
//    for (auto _ : state)
//    {
//        state.PauseTiming();
//        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
//        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted-marked\\crowd-embed.bmp");
//        state.ResumeTiming();
//
//        //Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
//    }
//}
//BENCHMARK(ExtractDataCrowd_512x512_bench)->Unit(benchmark::kMillisecond);
//
//static void ExtractDataMan_512x512_bench(benchmark::State& state) 
//{
//    for (auto _ : state)
//    {
//        state.PauseTiming();
//        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
//        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted-marked\\man-embed.bmp");
//        state.ResumeTiming();
//        
//        //Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
//    }
//}
//BENCHMARK(ExtractDataMan_512x512_bench)->Unit(benchmark::kMillisecond);
//
//static void ExtractDataBoat_512x512_bench(benchmark::State& state)
//{
//    for (auto _ : state)
//    {
//        state.PauseTiming();
//        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
//        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted-marked\\boat-embed.bmp");
//        state.ResumeTiming();
//
//        //Embedder::Embed(image, dataToEmbed, dataEmbedKey, tMax, maxUserDataBits); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
//    }
//}
//BENCHMARK(ExtractDataBoat_512x512_bench)->Unit(benchmark::kMillisecond);
//
////static void EmbedLiberty_1024x1024_bench(benchmark::State& state) {
////    for (auto _ : state)
////    {
////        state.PauseTiming();
////        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
////        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
////        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\liberty1024x1024-enc.bmp");
////        rdh::Consts::Instance().UpdateThreshold(state.range(0));
////        state.ResumeTiming();
////        Embedder::Embed(image, dataToEmbed, dataEmbedKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
////    }
////}
////BENCHMARK(EmbedLiberty_1024x1024_bench)->Unit(benchmark::kMillisecond)->DenseRange(10, 24, 1);
////
////static void EmbedMan_2048x2048_bench(benchmark::State& state) {
////    for (auto _ : state)
////    {
////        state.PauseTiming();
////        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
////        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
////        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\man2048x2048-enc.bmp");
////        rdh::Consts::Instance().UpdateThreshold(state.range(0));
////        state.ResumeTiming();
////        Embedder::Embed(image, dataToEmbed, dataEmbedKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
////    }
////}
////BENCHMARK(EmbedMan_2048x2048_bench)->Unit(benchmark::kMillisecond)->DenseRange(10, 24, 1);
////
////static void EmbedMan_4096x4096_bench(benchmark::State& state) {
////    for (auto _ : state)
////    {
////        state.PauseTiming();
////        std::vector<uint8_t> dataEmbedKey = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_embed_key.bin");
////        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>("..\\..\\..\\..\\example_data_to_embed.bin");
////        rdh::BmpImage image("..\\..\\..\\..\\images\\encrypted\\man4096x4096-enc.bmp");
////        rdh::Consts::Instance().UpdateThreshold(state.range(0));
////        state.ResumeTiming();
////        Embedder::Embed(image, dataToEmbed, dataEmbedKey); // .Save("..\\..\\..\\..\\images\\benchmark-test.bmp")
////    }
////}
////BENCHMARK(EmbedMan_4096x4096_bench)->Unit(benchmark::kMillisecond)->DenseRange(10, 24, 1);

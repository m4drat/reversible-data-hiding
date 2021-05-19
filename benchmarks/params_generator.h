#pragma once

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
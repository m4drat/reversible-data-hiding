#include "benchmark/include/benchmark/benchmark.h"

#include <boost/log/trivial.hpp>

#include "logging.h"
#include "reference_images.h"

int main(int argc, char** argv)
{
    /* Initialize logger in a way that it will write nothing to the stdout */
    rdh::log::InitLogger(boost::log::trivial::severity_level::fatal);

    References::GetInstance();

    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}
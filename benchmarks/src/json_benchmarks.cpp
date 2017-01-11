#include "benchmark/benchmark.h"

#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

void parse_json_file(benchmark::State& state, const std::string& filename)
{
    std::string input;
    {
        // read file into string stream
        std::stringstream istr;
        std::ifstream input_file(filename);
        istr << input_file.rdbuf();
        input = istr.str();
    }
    const auto filesize = input.size();

    while (state.KeepRunning())
    {
        json j = json::parse(input);
    }
    state.SetBytesProcessed(state.iterations() * filesize);
}

void dump_json_file(benchmark::State& state, const std::string& filename, bool pretty)
{
    const size_t width = pretty ? 4 : 0;

    json j;
    {
        // parse the input file
        std::ifstream input_file(filename);
        json j;
        j << input_file;
    }

    while (state.KeepRunning())
    {
        std::stringstream ss;
        ss << std::setw(width) << j;
    }

    state.SetBytesProcessed(state.iterations() * j.dump(pretty).size());
    state.SetLabel(pretty ? "pretty" : "normal");
}

int main(int argc, char** argv)
{
    // parse files
    for (const auto& filename :
{
    "data/jeopardy/jeopardy.json",
    "data/nativejson-benchmark/canada.json",
    "data/nativejson-benchmark/citm_catalog.json",
    "data/nativejson-benchmark/twitter.json",
    "data/numbers/floats.json",
    "data/numbers/signed_ints.json",
    "data/numbers/unsigned_ints.json"
})
    {
        std::string benchmark_name = std::string("parse ") + filename;
        benchmark::RegisterBenchmark(benchmark_name.c_str(), &parse_json_file, filename);
    }

    // dump files
    for (const auto& filename :
{
    "data/jeopardy/jeopardy.json",
    "data/numbers/floats.json",
    "data/numbers/signed_ints.json"
})
    {
        std::string benchmark_name = std::string("dump ") + filename;
        benchmark::RegisterBenchmark(benchmark_name.c_str(), &dump_json_file, filename, false);
        benchmark::RegisterBenchmark(benchmark_name.c_str(), &dump_json_file, filename, true);
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}

#include <celero/Celero.h>
#include "json.hpp"
#include <fstream>
#include <sstream>

using nlohmann::json;

CELERO_MAIN

class ParsingFixture : public celero::TestFixture
{
	public:
		ParsingFixture()
		{}

		virtual std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
		{
			std::vector<std::pair<int64_t, uint64_t>> problemSpace;

			for(size_t i = 0; i < files.size(); ++i)
			{
				problemSpace.push_back(std::make_pair(i, 0));
			}

			return problemSpace;
		}

		/// Before each run, build a vector of random integers.
		virtual void setUp(int64_t experimentValue)
		{
			std::string filename = files[experimentValue];

	        // read file into string stream
	        std::ifstream input_file(filename);
	        ss << input_file.rdbuf();
	        input_file.close();
		}

		std::stringstream ss;

	private:
		std::vector<std::string> files = {
			"/Users/niels/Documents/repositories/json/benchmarks/files/jeopardy/jeopardy.json",
			"/Users/niels/Documents/repositories/json/benchmarks/files/nativejson-benchmark/canada.json",
			"/Users/niels/Documents/repositories/json/benchmarks/files/nativejson-benchmark/citm_catalog.json",
			"/Users/niels/Documents/repositories/json/benchmarks/files/nativejson-benchmark/twitter.json",
			"/Users/niels/Documents/repositories/json/benchmarks/files/numbers/floats.json",
			"/Users/niels/Documents/repositories/json/benchmarks/files/numbers/signed_ints.json",
			"/Users/niels/Documents/repositories/json/benchmarks/files/numbers/unsigned_ints.json"
		};
};

// For a baseline, I'll choose Bubble Sort.
BASELINE_F(Parsing, ReadLines, ParsingFixture, 5, 5)
{
	std::string content;
	content = ss.str();

    ss.clear();
    ss.seekg(0);
}

BENCHMARK_F(Parsing, ParseJSON, ParsingFixture, 5, 5)
{
    //std::ifstream file(filename);
	json j = json::parse(ss);
    ss.clear();
    ss.seekg(0);
}

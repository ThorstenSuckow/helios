#include <benchmark/benchmark.h>
#include <cstdint>
#include <memory>

import helios.util;

struct GuidPimpl {
    private:
        struct GuidImpl {
            uint64_t value;
        };

        explicit GuidPimpl(const uint64_t value)
            : pImpl(std::make_unique<GuidImpl>()) {
            this->pImpl->value = value;
        }


        std::unique_ptr<GuidImpl> pImpl;

    public:
        const uint64_t value() const {
            return this->pImpl->value;
        }

        static GuidPimpl generate() {
            static std::atomic<uint64_t> counter{ 1 };
            return GuidPimpl(counter.fetch_add(1, std::memory_order_relaxed));
        }

        ~GuidPimpl() = default;
};



static void BM_GuidGenerate(benchmark::State& state) {

    for (auto _ : state) {
         benchmark::DoNotOptimize(helios::util::Guid::generate());
    }
}

BENCHMARK(BM_GuidGenerate)->UseRealTime();

// Define another benchmark
static void BM_GuidPimplGenerate(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(GuidPimpl::generate());
    }

}
BENCHMARK(BM_GuidPimplGenerate)->UseRealTime();

BENCHMARK_MAIN();
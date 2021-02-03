//
// Created by justnik on 03.02.2021.
//

#include "benchmark/include/benchmark/benchmark.h"
#include "../../includes/lru.h"

static void BM_Put(benchmark::State &state) {
    lru<uint32_t> l(20000);
    uint32_t i = 0;
    for (auto _ : state) {
        // This code gets timed
        l.put(i++);
    }
}

BENCHMARK(BM_Put);

static void BM_Has(benchmark::State &state) {
    lru<uint32_t> l(20000);
    uint32_t i = 0;
    for (auto _ : state) {
        // This code gets timed
        l.has(i++);
    }
}

BENCHMARK(BM_Has);
// Run the benchmark
BENCHMARK_MAIN();

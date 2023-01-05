#ifdef BENCH

#include <benchmark/benchmark.h>

#include <iostream>
#include <string>

#include "tetravex.hh"
#include "utils.hh"

static void Tetravex_n(benchmark::State& state, int n)
{
  std::vector<std::string> file_in = get_file_list(n);

  for (auto _ : state)
  {
    for (auto& file : file_in)
    {
      Tetravex game(file);
      solve(game);
    }
  }

  state.SetItemsProcessed(state.iterations() * file_in.size());
}

static void BM_Tetravex_s2(benchmark::State& state)
{
  Tetravex_n(state, 2);
}

static void BM_Tetravex_s4(benchmark::State& state)
{
  Tetravex_n(state, 4);
}

static void BM_Tetravex_s6(benchmark::State& state)
{
  Tetravex_n(state, 6);
}


BENCHMARK(BM_Tetravex_s2)->Unit(benchmark::kSecond);
BENCHMARK(BM_Tetravex_s4)->Unit(benchmark::kSecond);
BENCHMARK(BM_Tetravex_s6)->Unit(benchmark::kSecond);

BENCHMARK_MAIN();

#endif
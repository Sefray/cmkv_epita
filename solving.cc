#include "tetravex.hh"

#include <iostream>
#include <math.h>
#include <thread>
#include <tuple>
#include <vector>

std::vector<int> get_histogram(const Tetravex& game)
{
  std::vector<int> histo(10, 0);

  for (int y = 0; y < game.size; y++)
    for (int x = 0; x < game.size; x++)
    {
      Tile& tile = game.get_tile(x, y);
      histo[tile.value[NORTH]]++;
      histo[tile.value[EAST]]++;
      histo[tile.value[SOUTH]]++;
      histo[tile.value[WEST]]++;
    }

  return histo;
}

std::tuple<int, int, enum Direction> get_tile_position_with_value(const Tetravex& game, int value)
{
  for (int y = 0; y < game.size; y++)
    for (int x = 0; x < game.size; x++)
    {
      Tile& tile = game.get_tile(x, y);
      if (tile.value[NORTH] == value)
        return std::make_tuple(x, y, NORTH);
      if (tile.value[EAST] == value)
        return std::make_tuple(x, y, EAST);
      if (tile.value[SOUTH] == value)
        return std::make_tuple(x, y, SOUTH);
      if (tile.value[WEST] == value)
        return std::make_tuple(x, y, WEST);
    }

  return std::make_tuple(-1, -1, NORTH);
}


std::vector<std::pair<int, enum Direction>> get_unique_values(const Tetravex& game)
{
  std::vector<int>                            histo = get_histogram(game);
  std::vector<std::pair<int, enum Direction>> unique_values;

  for (int i = 0; i < 10; i++)
    if (histo[i] == 1)
    {
      auto position = get_tile_position_with_value(game, i);
      unique_values.push_back(std::make_pair(i, std::get<2>(position)));
    }

  return unique_values;
}

std::vector<std::tuple<int, int>> get_all_moves(Tetravex& game)
{
  std::vector<std::tuple<int, int>> moves;

  auto unique_values = get_unique_values(game);

  for (int ta = 0; ta < game.size * game.size; ta++)
  {
    if (game.tiles[ta].is_locked)
      continue;

    for (int tb = ta + 1; tb < game.size * game.size; tb++)
    {
      if (game.tiles[tb].is_locked)
        continue;

      moves.push_back(std::make_tuple(ta, tb));
    }
  }

  return moves;
}


void metropolis_hasting_with_recuit(Tetravex& game, bool verbose, float Tinit, float Tmin, float cooling_rate)
{
  std::mt19937 mt;
  mt.seed(std::hash<std::thread::id>()(std::this_thread::get_id()));

  static std::uniform_real_distribution<float> uni_real(0.0f, 1.0f);

  // Compute once the unique values
  auto unique_values = get_unique_values(game);
  auto moves         = get_all_moves(game);

  if (verbose)
  {
    std::cout << "All moves : " << std::endl;
    for (auto& move : moves)
      std::cout << std::get<0>(move) << " <-> " << std::get<1>(move) << std::endl;
  }

  float current_error = get_error(game, unique_values);
  if (current_error == 0)
    return;

  float T = Tinit;
  if (verbose)
  {
    std::cout << "Initial temperature : " << T << std::endl;
    std::cout << "Min temperature : " << Tmin << std::endl;
    std::cout << "Cooling rate : " << cooling_rate << std::endl;
  }

  for (int i = 0;; i++)
  {
    if (verbose)
      game.display();

    // Get the move to apply
    const auto& [ta, tb] = get_move(game, moves, mt);

    // Apply the move
    game.swap_tiles(ta, tb);
    if (verbose)
      std::cout << "Swap tiles " << ta << " and " << tb << std::endl;

    // Get the new error rate
    float new_error = get_error(game, unique_values);
    if (new_error == 0)
    {
      std::cout << "Solution found in " << i << " iterations" << std::endl;
      if (verbose)
      {
        game.display();
      }
      return;
    }

    float new_score     = new_error;
    float current_score = current_error;

    if (new_score - current_score < 0)
    {
      current_error = new_error;
      if (verbose)
        std::cout << "Move applied" << std::endl;
    }
    else
    {
      float alpha      = std::exp(-(new_score - current_score) / T);
      float acceptance = std::min(alpha, 1.0f);
      float r          = uni_real(mt);

      if (verbose)
      {
        std::cout << "Iteration " << i << " : Temperature : " << T << std::endl;
        std::cout << "Current_error : " << current_error << ", new_error : " << new_error << std::endl;
        std::cout << "Alpha : " << alpha << " Acceptance : " << acceptance << std::endl;
      }

      if (r < acceptance)
      {
        current_error = new_error;
        if (verbose)
          std::cout << "Move applied" << std::endl;
      }
      else
      {
        game.swap_tiles(ta, tb);
        if (verbose)
          std::cout << "Move rejected" << std::endl;
      }
    }

    // std::cout << "Iteration " << i << " : Temperature : " << T << ", error : " << current_error << std::endl;
    T = std::max(T * (1 - cooling_rate), Tmin);
  }

  std::cout << "No solution found" << std::endl;
}


void solve(Tetravex& game, bool verbose)
{
  metropolis_hasting_with_recuit(game, verbose, 250.f, 0.5f, 0.01f);
}
#include "tetravex.hh"

#include <iostream>
#include <math.h>
#include <tuple>
#include <vector>

std::vector<int> get_histogram(Tetravex& game)
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

std::tuple<int, int, enum Direction> get_tile_position_with_value(Tetravex& game, int value)
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


std::vector<std::pair<int, enum Direction>> get_unique_values(Tetravex& game)
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

float get_error_move(Tetravex& game, std::vector<std::tuple<int, int>>& moves, std::tuple<int, int, float>& move)
{
  auto distributions = get_distributions(game, moves);

  int index = -1;
  for (auto& m : moves)
  {
    index++;
    if (std::get<0>(m) == std::get<0>(move) && std::get<1>(m) == std::get<1>(move))
      break;
  }

  return distributions[index];
}

float get_scores_factor(float current_error, float new_error)
{
  return 1 / std::exp(new_error - current_error);
}

float get_acceptance_probability(Tetravex& game, std::vector<std::tuple<int, int>> moves, float current_error,
                                 float new_error, std::tuple<int, int, float>& move)
{
  float score_factor = get_scores_factor(current_error, new_error);

  float proba_move_to_new_position = std::get<2>(move);
  float proba_move_to_old_position = get_error_move(game, moves, move);

  float alpha = score_factor * proba_move_to_old_position / proba_move_to_new_position;

  // std::cout << "proba_move_to_new_position : " << proba_move_to_new_position
  //           << ", proba_move_to_old_position : " << proba_move_to_old_position << std::endl;
  // std::cout << "alpha : " << alpha << std::endl;

  float acceptance = std::min(alpha, 1.0f);

  return acceptance;
}

void solve(Tetravex& game, int max_iterations, bool verbose)
{
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

  for (int i = 0; i < max_iterations; i++)
  {
    if (verbose)
      game.display();

    // Get the move to apply
    auto move = get_move(game, moves);

    // Apply the move
    game.swap_tiles(std::get<0>(move), std::get<1>(move));
    if (verbose)
      std::cout << "Swap tiles " << std::get<0>(move) << " and " << std::get<1>(move) << std::endl;

    // Get the new error rate
    float new_error = get_error(game, unique_values);
    if (new_error == 0)
    {
      if (verbose)
      {
        std::cout << "Solution found in " << i << " iterations" << std::endl;
        game.display();
      }
      return;
    }

    // Compute acceptance probability
    float acceptance_probability = get_acceptance_probability(game, moves, current_error, new_error, move);

    // Generate a random number between 0 and 1
    float random_number = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    if (verbose)
    {
      std::cout << "Iteration " << i << " : current error : " << new_error << ", previous error : " << current_error
                << ", acceptance probability : " << acceptance_probability << std::endl;
    }

    if (random_number <= acceptance_probability)
    {
      current_error = new_error;
      if (verbose)
        std::cout << "Move applied" << std::endl;
    }
    else
      game.swap_tiles(std::get<0>(move), std::get<1>(move));
  }
}
#include "tetravex.hh"

#include <iostream>

void display_horizontal_line(int size)
{
  for (int i = 0; i < size; ++i)
  {
    std::cout << "+";
    std::cout << "-----";
  }
  std::cout << "+" << std::endl;
}

void display_line_north_south(Tetravex& game, int line, int direction)
{
  for (int i = 0; i < game.size; ++i)
  {
    Tile& tile = game.get_tile(i, line);
    std::cout << "|  " << tile.value[direction] << "  ";
  }
  std::cout << "|" << std::endl;
}

void display_line_west_east(Tetravex& game, int line)
{
  for (int i = 0; i < game.size; ++i)
  {
    Tile& tile = game.get_tile(i, line);
    std::cout << "| " << tile.value[WEST] << " " << tile.value[EAST] << " ";
  }
  std::cout << "|" << std::endl;
}

void display_line(Tetravex& game, int line)
{
  display_line_north_south(game, line, NORTH);
  display_line_west_east(game, line);
  display_line_north_south(game, line, SOUTH);
}

void Tetravex::display()
{
  for (int line = 0; line < size; line++)
  {
    display_horizontal_line(size);
    display_line(*this, line);
  }
  display_horizontal_line(size);
}
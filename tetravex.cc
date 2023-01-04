#include "tetravex.hh"

#include <fstream>
#include <iostream>
#include <sstream>

/**
 * @brief Get the index of the char after the last slash last
 *
 * @param str The string in which we want to find the last slash
 * @return int The position after the last slash in the string or 0 if there is no slash
 */
int get_after_slash_last_position(std::string str)
{
  int pos = str.find_last_of('/');
  if (pos == std::string::npos)
    return 0;
  return pos + 1;
}

Tetravex::Tetravex(std::string filename)
{
  int slash_pos = get_after_slash_last_position(filename);
  int size      = filename[slash_pos + 1] - '0';
  if (size < 2 || size > 6)
  {
    std::cout << "Invalid size : " << size << " for file : " << filename << std::endl;
    return;
  }

  this->size = size;

  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cout << "Unable to open file" << std::endl;
    return;
  }

  tiles = new Tile[size * size];
  std::string line;
  int         i = 0;
  while (std::getline(file, line))
  {
    Tile& tile = tiles[i++];
    if (line.empty())
      continue;

    tile.is_locked = line.back() == '@';
    for (int i = 0; i < 4; ++i)
      tile.value[i] = line[i] - '0';
  }

  file.close();

  if (i != size * size)
  {
    delete[] tiles;
    std::cout << "Invalid file" << std::endl;
    return;
  }
}

void Tetravex::write(std::string file_out)
{
  std::ofstream file(file_out);
  if (!file.is_open())
  {
    std::cout << "Unable to open file" << std::endl;
    return;
  }

  for (int i = 0; i < size * size; ++i)
  {
    Tile& tile = tiles[i];
    for (int j = 0; j < 4; ++j)
      file << tile.value[j];
    file << std::endl;
  }

  file.close();
}

Tile& Tetravex::get_tile(int x, int y)
{
  return tiles[x + y * size];
}
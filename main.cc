#include <iostream>
#include <string>

#include "tetravex.hh"

#ifndef BENCH
#ifndef TEST

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " in.txt out.txt" << std::endl;
    return 1;
  }

  std::string in_file  = argv[1];
  std::string out_file = argv[2];

  Tetravex game(in_file);
  game.display();

  game.resolve();

  game.write(out_file);
}

#endif
#endif
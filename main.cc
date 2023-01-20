#include <iostream>
#include <string>

#include "tetravex.hh"

#ifndef BENCH
#ifndef TEST

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " in.txt [out.txt]" << std::endl;
    return 1;
  }

  bool verbose = argc == 2;

  std::string in_file = argv[1];

  Tetravex game(in_file);
  solve(game, verbose);

  if (verbose)
    return 0;

  std::string out_file = argv[2];
  game.write(out_file);
}

#endif
#endif
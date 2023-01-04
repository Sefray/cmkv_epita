#ifdef TEST

#include "tetravex.hh"

#include <criterion/criterion.h>

Test(tetravex, load)
{
  Tetravex game("data/input/s2-02.txt");
  cr_assert_eq(game.size, 2);
  cr_assert(game.tiles[0].is_locked);
  cr_assert_eq(game.tiles[0].value[NORTH], 0);
  cr_assert_eq(game.tiles[1].value[WEST], 2);
  cr_assert_eq(game.tiles[2].value[EAST], 2);
  cr_assert_eq(game.tiles[3].value[SOUTH], 5);
}

#endif
#ifdef TEST

#include <iostream>

#include "tetravex.hh"
#include "utils.hh"

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

Test(tetravex, error_count_s2_02)
{
  Tetravex game("data/input/s2-02.txt");
  cr_assert_eq(get_error_count(game), 2);
}

Test(tetravex, error_count_s5_04)
{
  Tetravex game("data/input/s5-04.txt");
  cr_assert_eq(get_error_count(game), 29);
}

void test_full(int n)
{
  auto file_list = get_file_list(n);

  int passed = 0;
  for (int i = 0; i < file_list.size(); i++)
  {
    Tetravex game(file_list[i]);

    // std::cout << "Solving..." << std::endl; 
    // game.display();

    solve(game);
    auto err = get_error_count(game);
    // cr_expect_eq(err, 0, "Error for File: %s", file_list[i].c_str());
    if (err == 0)
    {
      passed++;
      // game.display();
      // std::cout << "Passed" << std::endl;
    }
  }

  cr_assert_eq(passed, file_list.size(), "Passed %d over %d", passed, file_list.size());
}

Test(tetravex, s2_full)
{
  test_full(2);
}

Test(tetravex, s3_full)
{
  test_full(3);
}

Test(tetravex, s4_full)
{
  test_full(4);
}

Test(tetravex, s5_full)
{
  test_full(5);
}

Test(tetravex, s6_full)
{
  test_full(6);
}

#endif
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "scoundrel.h"

int main(int argc, char *argv[]) {
  const int seed = rand();
  ScoundrelGame game(seed);
  for (int i = 0; i < 4; ++i) {
    const Card card = game.get_room()->at(i);
    std::cout << card_to_string(card) << "  ";
  }
  printf("\n");

  game.run_away();

  for (int i = 0; i < 4; ++i) {
    const Card card = game.get_room()->at(i);
    std::cout << card_to_string(card) << "  ";
  }
  printf("\n");
  return 0;
}

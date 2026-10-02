#pragma once

#include <block.h>

#include <vector>

struct Strucutre {
  /* data */
  int w = 0;
  int h = 0;

  std::vector<Block> mapData;
  void create(int w, int h);

  Block& getBlockUnsafe(int x, int y);
  Block* getBloackSafe(int x, int y);
};

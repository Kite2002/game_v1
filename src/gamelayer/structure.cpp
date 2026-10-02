#include <asserts.h>

#include "strucutre.h"

void Strucutre::create(int w, int h) {
  // reset all data
  *this = {};
  mapData.resize(w * h);

  this->w = w;
  this->h = h;

  for (auto& e : mapData) {
    e = {};
  }  // clear all block data
}

Block& Strucutre::getBlockUnsafe(int x, int y) {
  permaAssertCommentDevelopement(mapData.size() == w * h,
                                 "Map data not initialized");

  permaAssertCommentDevelopement(x >= 0 && y >= 0 && x < w && y < h,
                                 "getBlockUnsafe out of bounds error");

  return mapData[x + y * w];
}

Block* Strucutre::getBloackSafe(int x, int y) {
  permaAssertCommentDevelopement(mapData.size() == w * h,
                                 "Map data not initalized");

  if (x < 0 || y < 0 || x >= w || y >= h) {
    return nullptr;
  }
  return &mapData[x + y * w];
}

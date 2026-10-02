#pragma once
#include <block.h>

#include <fstream>
#include <vector>

bool saveBlockDataToFile(std::vector<Block> blocks, int w, int h,
                         const char* fileName);
bool loadBlockDataFromFile(std::vector<Block>& block, int& w, int& h,
                           const char* fileName);
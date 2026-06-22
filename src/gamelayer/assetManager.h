#pragma once
#include <raylib.h>

struct AssetManager {
  Texture2D dirt = {};
  Texture2D textures = {};
  Texture2D frame = {};
  Texture2D treeLog = {};
  Texture2D texturesWithWalls = {};
  void loadAll();
};
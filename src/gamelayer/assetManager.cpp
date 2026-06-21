#include "assetManager.h"

void AssetManager::loadAll() {
  // Load all procedural textures
  dirt = LoadTexture(RESOURCES_PATH "dirt.png");
  textures = LoadTexture(RESOURCES_PATH "textures.png");
  frame = LoadTexture(RESOURCES_PATH "frame.png");
  treeLog = LoadTexture(RESOURCES_PATH "treetextures.png");

  // Add more textures here as needed
  // stone = CreateTerrariaStyleStoneTexture();
  // grass = CreateTerrariaStyleGrassTexture();
}
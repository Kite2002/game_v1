#include "assetManager.h"

void AssetManager::loadAll() {
  // Load all procedural textures
  dirt = LoadTexture(RESOURCES_PATH "dirt.png");
  textures = LoadTexture(RESOURCES_PATH "textures.png");
  // Add more textures here as needed
  // stone = CreateTerrariaStyleStoneTexture();
  // grass = CreateTerrariaStyleGrassTexture();
}
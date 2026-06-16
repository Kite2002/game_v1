#include <Textures.h>
#include <assetManager.h>
#include <gameMain.h>
#include <gameMap.h>
#include <raylib.h>
#include <textureManager.h>

#include <iostream>

struct GameData {
  float posX = 100;
  float posY = 100;

  GameMap gameMap;

  Camera2D camera;
} gameData;

TextureManager textureManager;
AssetManager assetManager;

bool initGame() {
  assetManager.loadAll();

  gameData.gameMap.create(30, 10);

  gameData.gameMap.getBlockUnsafe(0, 1).type = Block::dirt;
  gameData.gameMap.getBlockUnsafe(0, 2).type = Block::bookShelf;
  gameData.gameMap.getBlockUnsafe(0, 3).type = Block::copperBlock;
  gameData.gameMap.getBlockUnsafe(0, 4).type = Block::grass;

  gameData.camera.target = {0, 0};
  gameData.camera.rotation = 0.0f;
  gameData.camera.zoom = 100.0f;

  return true;
}

bool updateGame() {
  float deltaTime = GetFrameTime();

  // if delta time gets bigger than 5 frames per second keep the frame at that
  if (deltaTime > 1.f / 5) {
    deltaTime = 1 / 5.f;
  }

  gameData.camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

  ClearBackground({75, 75, 150, 255});
  BeginMode2D(gameData.camera);

#pragma region camera movement

  if (IsKeyDown(KEY_A)) gameData.camera.target.x -= 7.f * deltaTime;
  if (IsKeyDown(KEY_D)) gameData.camera.target.x += 7.f * deltaTime;
  if (IsKeyDown(KEY_W)) gameData.camera.target.y -= 7.f * deltaTime;
  if (IsKeyDown(KEY_S)) gameData.camera.target.y += 7.f * deltaTime;
#pragma endregion

  for (int y = 0; y < gameData.gameMap.h; y++) {
    for (int x = 0; x < gameData.gameMap.w; x++) {
      auto& b = gameData.gameMap.getBlockUnsafe(x, y);

      if (b.type != Block::air) {
        float size = 1;
        float posx = x * size;
        float posy = y * size;

        Rectangle textureUV;
        textureUV.width = 32;
        textureUV.height = 32;
        textureUV.x = b.type * 32;
        textureUV.y = 0;

        DrawTexturePro(assetManager.textures, textureUV,
                       {posx, posy, size, size}, {0, 0}, 0.0f, WHITE);
      }
    }
  }
  DrawRectangle(gameData.camera.target.x, gameData.camera.target.y, 1, 1, RED);

  return true;
}

void closeGame() {
  textureManager.UnloadAll();
  gameData = {};
  std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}
#include <Textures.h>
#include <assetManager.h>
#include <gameMain.h>
#include <gameMap.h>
#include <helpers.h>
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

  gameData.gameMap.create(20, 20);

  printf("tilesPerRow = %d\n", assetManager.textures.width / 32);
  printf("tilesPerCol = %d\n", assetManager.textures.height / 32);
  for (int y = 0; y < gameData.gameMap.h; y++)
    for (int x = 0; x < gameData.gameMap.w; x++) {
      float s = (std::sin(x) + 1.f) / 2.f;
      float s2 = (std::sin(x * 0.5) + 1.f) / 2.f;

      if (gameData.gameMap.h - (gameData.gameMap.h * 0.3 * s) -
              gameData.gameMap.h * 0.5 - (gameData.gameMap.h * 0.2 * s2)

          < y) {
        gameData.gameMap.getBlockUnsafe(x, y).type = Block::bholu;
      } else {
        gameData.gameMap.getBlockUnsafe(x, y).type = Block::bonePlatform;
      }
    }

  gameData.camera.target = {0, 0};
  gameData.camera.rotation = 0.0f;
  gameData.camera.zoom = 30.0f;

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

  Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
  int blockX = (int)floor(worldPos.x);
  int blockY = (int)floor(worldPos.y);

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    auto b = gameData.gameMap.getBloackSafe(blockX, blockY);
    if (b) {
      *b = {};
    }
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    auto b = gameData.gameMap.getBloackSafe(blockX, blockY);
    if (b) {
      b->type = Block::gold;
    }
  }

  for (int y = 0; y < gameData.gameMap.h; y++) {
    for (int x = 0; x < gameData.gameMap.w; x++) {
      auto& b = gameData.gameMap.getBlockUnsafe(x, y);

      if (b.type != Block::air) {
        float size = 1;
        float posx = x * size;
        float posy = y * size;

        DrawTexturePro(assetManager.textures,
                       getTextureAtlas(b.type, 0, 32, 32),
                       {posx, posy, size, size}, {0, 0}, 0.0f, WHITE);
      }
    }
  }
  DrawRectangle(gameData.camera.target.x, gameData.camera.target.y, 1, 1, RED);
  DrawTexturePro(
      assetManager.frame,
      {0, 0, (float)assetManager.frame.width, (float)assetManager.frame.height},
      {(float)blockX, (float)blockY, 1, 1}, {0, 0}, 0.0f, WHITE);

  return true;
}

void closeGame() {
  textureManager.UnloadAll();
  gameData = {};
  std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}
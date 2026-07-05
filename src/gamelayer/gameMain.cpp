#include "gameMain.h"

#include <Textures.h>
#include <imgui.h>
#include <raylib.h>
#include <raymath.h>
#include <rlImGui.h>
#include <textureManager.h>

#include <iostream>

#include "assetManager.h"
#include "gameMap.h"
#include "helpers.h"
#include "randomStuff.h"

struct GameData {
  float posX = 100;
  float posY = 100;

  GameMap gameMap;
  GameMap backGroundMap;

  int selectedBlock = 0;
  Camera2D camera;
} gameData;

TextureManager textureManager;
AssetManager assetManager;

bool initGame() {
#pragma region imgui
  rlImGuiSetup(true);

#pragma endregion
  assetManager.loadAll();

  gameData.gameMap.create(700, 500);
  gameData.backGroundMap.create(700, 500);

  printf("tilesPerRow = %d\n", assetManager.texturesWithWalls.width / 32);
  printf("tilesPerCol = %d\n", assetManager.texturesWithWalls.height / 32);
  // for (int y = 0; y < gameData.gameMap.h; y++)
  //   for (int x = 0; x < gameData.gameMap.w; x++) {
  //     float s = (std::sin(x) + 1.f) / 2.f;
  //     float s2 = (std::sin(x * 0.5) + 1.f) / 2.f;

  //     if (gameData.gameMap.h - (gameData.gameMap.h * 0.3 * s) -
  //             gameData.gameMap.h * 0.5 - (gameData.gameMap.h * 0.2 * s2)

  //         < y) {
  //       gameData.gameMap.getBlockUnsafe(x, y).type = Block::woodPlank;
  //     } else {
  //       gameData.gameMap.getBlockUnsafe(x, y).type = Block::bonePlatform;
  //     }
  //   }

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

  if (IsKeyDown(KEY_A)) gameData.camera.target.x -= 9.f * deltaTime;
  if (IsKeyDown(KEY_D)) gameData.camera.target.x += 9.f * deltaTime;
  if (IsKeyDown(KEY_W)) gameData.camera.target.y -= 9.f * deltaTime;
  if (IsKeyDown(KEY_S)) gameData.camera.target.y += 9.f * deltaTime;
#pragma endregion

  Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
  int blockX = (int)floor(worldPos.x);
  int blockY = (int)floor(worldPos.y);

  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    auto b = gameData.gameMap.getBloackSafe(blockX, blockY);

    if (b && b->type != 0) {
      printf("%d", b->type);
      *b = {};
    } else {
      auto bg = gameData.backGroundMap.getBloackSafe(blockX, blockY);
      if (bg) {
        *bg = {};
      }
    }
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    if (gameData.selectedBlock >= Block::dirtWall) {
      auto bg = gameData.backGroundMap.getBloackSafe(blockX, blockY);
      if (bg) {
        // Ensure we cast to the actual type of the field to avoid
        // invalid static_cast from int to Block (enum class or typedef)
        bg->type = static_cast<decltype(bg->type)>(gameData.selectedBlock);
      }
    } else {
      auto b = gameData.gameMap.getBloackSafe(blockX, blockY);
      if (b) {
        // Ensure we cast to the actual type of the field to avoid
        // invalid static_cast from int to Block (enum class or typedef)
        b->type = static_cast<decltype(b->type)>(gameData.selectedBlock);
      }
    }
  }

  Vector2 topLeftView = GetScreenToWorld2D({0, 0}, gameData.camera);
  Vector2 bottomRightView = GetScreenToWorld2D(
      {(float)GetScreenWidth(), (float)GetScreenHeight()}, gameData.camera);

  int startXView = (int)floor(topLeftView.x - 1);
  int endXView = (int)ceilf(bottomRightView.x + 1);

  int startYView = (int)floor(topLeftView.y - 1);
  int endYView = (int)ceilf(bottomRightView.y + 1);

  startXView = Clamp(startXView, 0, gameData.gameMap.w - 1);
  endXView = Clamp(endXView, 0, gameData.gameMap.w - 1);

  startYView = Clamp(startXView, 0, gameData.gameMap.h - 1);
  endYView = Clamp(endXView, 0, gameData.gameMap.h - 1);

#pragma region bg_rendering
  for (int y = startYView; y < endYView; y++) {
    for (int x = startXView; x < endXView; x++) {
      auto& b = gameData.backGroundMap.getBlockUnsafe(x, y);

      if (b.type != Block::air && b.type >= Block::dirtWall) {
        float size = 1;
        float posx = x * size;
        float posy = y * size;
        std::ranlux24_base prng(x * y);
        int atlasY = getRandomInt(prng, 0, 3);
        printf("%d\n", atlasY);
        DrawTexturePro(assetManager.texturesWithWalls,
                       getTextureAtlas(b.type, atlasY, 32, 32),
                       {posx, posy, size, size}, {0, 0}, 0.0f, WHITE);
      }
    }
  }
  DrawRectangle(gameData.camera.target.x, gameData.camera.target.y, 1, 1, RED);

#pragma endregion

#pragma region foreground_rendering
  for (int y = startYView; y < endYView; y++) {
    for (int x = startXView; x < endXView; x++) {
      auto& b = gameData.gameMap.getBlockUnsafe(x, y);

      if (b.type != Block::air) {
        float size = 1;
        float posx = x * size;
        float posy = y * size;

        if (b.type != Block::air) {
          float size = 1;
          float posx = x * size;
          float posy = y * size;

          // Determine neighbor types safely (use safe accessor that returns
          // nullptr on OOB)
          auto leftPtr = gameData.gameMap.getBloackSafe(x - 1, y);
          auto rightPtr = gameData.gameMap.getBloackSafe(x + 1, y);
          auto topPtr = gameData.gameMap.getBloackSafe(x, y - 1);
          auto bottomPtr = gameData.gameMap.getBloackSafe(x, y + 1);
          using T = decltype(b.type);

          T leftType = leftPtr ? leftPtr->type : Block::air;
          T rightType = rightPtr ? rightPtr->type : Block::air;
          T topType = topPtr ? topPtr->type : Block::air;
          T bottomType = bottomPtr ? bottomPtr->type : Block::air;

          Texture& tex = (b.type == Block::woodLog)
                             ? assetManager.treeLog
                             : assetManager.texturesWithWalls;

          // Compute atlas index / coordinates in a single place
          Rectangle src;
          std::ranlux24_base prng(x * y);
          int atlasY = getRandomInt(prng, 0, 3);
          printf("%d\n", atlasY);
          if (b.type == Block::woodLog) {
            int atlasIndex;
            if (bottomType != Block::woodLog && topType != Block::woodLog &&
                leftType != Block::leaves && rightType != Block::leaves) {
              atlasIndex = 7;
            } else if (topType == Block::leaves) {
              atlasIndex = 5;
            } else if (leftType == Block::leaves &&
                       rightType == Block::leaves) {
              atlasIndex = 1;
            } else if (leftType == Block::leaves) {
              atlasIndex = 3;
            } else if (rightType == Block::leaves) {
              atlasIndex = 2;
            } else if (topType != Block::woodLog) {
              atlasIndex = 6;
            } else if (bottomType != Block::woodLog) {
              atlasIndex = 4;
            } else {
              atlasIndex = 0;
            }
            src = getTextureAtlas(atlasIndex, atlasY, 32, 32);
          } else {
            src = getTextureAtlas(b.type, atlasY, 32, 32);
          }

          DrawTexturePro(tex, src, {posx, posy, size, size}, {0, 0}, 0.0f,
                         WHITE);
        }
      }
    }
  }
  DrawRectangle(gameData.camera.target.x, gameData.camera.target.y, 1, 1, RED);
  DrawTexturePro(
      assetManager.frame,
      {0, 0, (float)assetManager.frame.width, (float)assetManager.frame.height},
      {(float)blockX, (float)blockY, 1, 1}, {0, 0}, 0.0f,
      gameData.gameMap.getBloackSafe(blockX, blockY) ? WHITE : RED);
  EndMode2D();
#pragma endregion

  DrawTexturePro(
      assetManager.frame,
      {0, 0, (float)assetManager.frame.width, (float)assetManager.frame.height},
      {(float)blockX, (float)blockY, 1, 1}, {0, 0}, 0.0f,
      gameData.gameMap.getBloackSafe(blockX, blockY) ? WHITE : RED);
  EndMode2D();

#pragma region imgui
  ImGui::Begin("Block Picker");
  ImGui::Text("FPS: %d", GetFPS());
  for (size_t i = 1; i < Block::BLOCKS_COUNT; i++) {
    ImGui::PushID(i);

    Rectangle src = getTextureAtlas(i, 0, 32, 32);
    ImVec2 uv0 = {src.x / assetManager.texturesWithWalls.width,
                  src.y / assetManager.texturesWithWalls.height};
    ImVec2 uv1 = {(src.x + src.width) / assetManager.texturesWithWalls.width,
                  (src.y + src.height) / assetManager.texturesWithWalls.height};
    if (ImGui::ImageButton(
            (ImTextureID)(intptr_t)assetManager.texturesWithWalls.id,
            ImVec2(32.0f, 32.0f), uv0, uv1, -1, ImVec4(0, 0, 0, 0),
            ImVec4(1, 1, 1, 1))) {
      gameData.selectedBlock = i;
    }
    if ((i - 1) % 6 != 5) {  // 0-indexed, so button 6 is index 5
      ImGui::SameLine(0.0f, 4.0f);
    }

    ImGui::PopID();
  }
  ImGui::End();

#pragma endregion
  return true;
}

void closeGame() {
  rlImGuiShutdown();  // cleans up ImGui

  textureManager.UnloadAll();
  gameData = {};
  std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}
#include <raylib.h>
#include <gameMain.h>
#include <iostream>

struct GameData {
    float posX = 100;
    float posY = 100;
}gameData;

bool initGame() {
    return true;
}

bool updateGame() {
    Color c;
    c.r = 0;
    c.g = 255;
    c.b = 200;
    c.a = 255;

    int playerHeight = 20;
    int playerWidth = 20;
    float deltaTime = GetFrameTime();

    // if delta time gets bigger than 5 frames per second keep the frame at that
    if (deltaTime > 1.f / 5) { deltaTime = 1 / 5.f; }

    // move player 200 pixels per second
    if (IsKeyDown(KEY_A)) { gameData.posX -= 200.f * deltaTime; }
    if (IsKeyDown(KEY_D)) { gameData.posX += 200.f * deltaTime; }
    if (IsKeyDown(KEY_W)) { gameData.posY -= 200.f * deltaTime; }
    if (IsKeyDown(KEY_S)) { gameData.posY += 200.f * deltaTime; }

    DrawRectangle(gameData.posX, gameData.posY, playerHeight, playerWidth, c);
    return true;
}

void closeGame() {
    gameData = {};
    std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}
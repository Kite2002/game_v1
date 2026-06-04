#include <raylib.h>
#include <gameMain.h>
#include <iostream>
#include <textureManager.h>

Texture2D CreateTerrariaStyleDirtTexture()
{
    const int TILE_SIZE = 16;

    Color A = { 101, 67, 33, 255 };   // Base dirt
    Color B = { 84, 52, 26, 255 };    // Dark dirt
    Color C = { 140, 110, 70, 255 };  // Stone speck

    const char* dirt[TILE_SIZE] =
    {
        "AABBAAAABBAAAABB",
        "ABBBBBAABBBBBAAA",
        "BBBCAAABBBCAAABB",
        "BAAABBBBBAAABBBB",
        "ABBBAAAABBBAAAAB",
        "BBBBAABBBBBAABBB",
        "AAABBBAAAABBBAAA",
        "BBBAAABBBBAAABBB",
        "ABBBAABBABBBAABB",
        "BBAABBABBBAABBAB",
        "AAABBBAAAABBBAAA",
        "BBBBAABBBBBAABBB",
        "ABBBAAAABBBAAAAB",
        "BAAABBBBBAAABBBB",
        "BBBCAAABBBCAAABB",
        "ABBBBBAABBBBBAAA"
    };

    Image img = GenImageColor(TILE_SIZE, TILE_SIZE, BLANK);

    for (int y = 0; y < TILE_SIZE; y++)
    {
        for (int x = 0; x < TILE_SIZE; x++)
        {
            Color pixelColor = A;

            switch (dirt[y][x])
            {
            case 'A': pixelColor = A; break;
            case 'B': pixelColor = B; break;
            case 'C': pixelColor = C; break;
            }

            ImageDrawPixel(&img, x, y, pixelColor);
        }
    }

    Texture2D texture = LoadTextureFromImage(img);
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);

    UnloadImage(img);

    return texture;
}

struct GameData {
    float posX = 100;
    float posY = 100;
    TextureManager textureManager;
}gameData;

bool initGame() {
    // Load all procedural textures
    gameData.textureManager.AddTexture("dirt", CreateTerrariaStyleDirtTexture());
    // Add more textures here as needed
    // gameData.textureManager.AddTexture("stone", CreateTerrariaStyleStoneTexture());
    // gameData.textureManager.AddTexture("grass", CreateTerrariaStyleGrassTexture());
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

    Texture2D dirtTexture = gameData.textureManager.GetTexture("dirt");
    DrawTextureEx(
        dirtTexture,
        {
            gameData.posX,
            gameData.posY
        },
        0.0f,
        3.0f, // 16x16 -> 48x48
        WHITE
    );

    // DrawRectangle(gameData.posX, gameData.posY, playerHeight, playerWidth, c);
    return true;
}

void closeGame() {
    gameData.textureManager.UnloadAll();
    gameData = {};
    std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}
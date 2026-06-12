#include <raylib.h>
#include <Textures.h>

Texture2D CreateDirtTexture()
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

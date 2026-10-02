#pragma once
#include <cstdint>

struct Block {
  enum {
    air = 0,
    dirt,
    grassBlock,
    stone,
    grass,
    sand,
    sandRuby,
    sandStone,
    woodPlank,
    stoneBricks,
    clay,
    woodLog,
    leaves,
    copper,
    iron,
    gold,
    copperBlock,
    ironBlock,
    goldBlock,
    bricks,
    snow,
    ice,
    rubyBlock,
    platform,
    workBench,
    glass,
    furnace,
    painting,
    sappling,
    snowBlueRuby,
    blueRubyBlock,
    door,
    jar,
    table,
    wordrobe,
    bookShelf,
    snowBricks,
    iceTable,
    iceWordrobe,
    iceBookShelf,
    icePlatform,
    sandTable,
    sandWordrobe,
    sandBookShelf,
    sandPlatform,
    woodenChest,
    iceChest,
    sandChest,
    boneChest,
    boneBricks,
    boneBench,
    boneWordrobe,
    boneBookShelf,
    bonePlatform,
    dirtWall,
    stoneWall,
    woodWall,
    sandStoneWall,
    brickWall,
    glassWall,
    copperBlockWall,
    silverBlockWall,
    goldBlockWall,
    snowWall,
    sandWall,
    stoneBricksWall,
    rubyBlockWall,
    heroglyphWall,
    blueRubyWall,
    plankedWall,
    snowBrickWall,
    boneBrickWall,

    BLOCKS_COUNT,

  };

  static constexpr const char* Names[] = {
      "air",           "dirt",         "grassBlock",    "stone",
      "grass",         "sand",         "sandRuby",      "sandStone",
      "woodPlank",     "stoneBricks",  "clay",          "woodLog",
      "leaves",        "copper",       "iron",          "gold",
      "copperBlock",   "ironBlock",    "goldBlock",     "bricks",
      "snow",          "ice",          "rubyBlock",     "platform",
      "workBench",     "glass",        "furnace",       "painting",
      "sappling",      "snowBlueRuby", "blueRubyBlock", "door",
      "jar",           "table",        "wordrobe",      "bookShelf",
      "snowBricks",    "iceTable",     "iceWordrobe",   "iceBookShelf",
      "icePlatform",   "sandTable",    "sandWordrobe",  "sandBookShelf",
      "sandPlatform",  "woodenChest",  "iceChest",      "sandChest",
      "boneChest",     "boneBricks",   "boneBench",     "boneWordrobe",
      "boneBookShelf", "bonePlatform", "bholu"};

  static constexpr const char* GetName(std::uint16_t type) {
    return Names[type];
  }
  std::uint16_t type = 0;

  void sanitize() {
    if (type >= BLOCKS_COUNT) {
      type = 0;
    }
  }
};

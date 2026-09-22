#include "worldGen.h"

#include <FastNoiseSIMD.h>

#include <memory>

#include "randomStuff.h"

void generateWorld(GameMap& gameMap, int seed) {
  const int w = 900;
  const int h = 500;

  gameMap.create(w, h);

  std::ranlux24 rng(seed++);

  std::unique_ptr<FastNoiseSIMD> dirtNoiseGen(
      FastNoiseSIMD::NewFastNoiseSIMD());
  std::unique_ptr<FastNoiseSIMD> stoneNoiseGen(
      FastNoiseSIMD::NewFastNoiseSIMD());

  dirtNoiseGen->SetSeed(seed++);
  stoneNoiseGen->SetSeed(seed + 2);

  dirtNoiseGen->SetNoiseType(FastNoiseSIMD::NoiseType::ValueFractal);
  dirtNoiseGen->SetFractalOctaves(5);
  dirtNoiseGen->SetFrequency(0.01);

  stoneNoiseGen->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
  stoneNoiseGen->SetFractalOctaves(7);
  stoneNoiseGen->SetFrequency(0.01);

  float* dirtNoise = FastNoiseSIMD::GetEmptySet(w);
  float* stoneNoise = FastNoiseSIMD::GetEmptySet(w);

  dirtNoiseGen->FillNoiseSet(dirtNoise, 0, 0, 0, w, 1, 1);
  stoneNoiseGen->FillNoiseSet(stoneNoise, 0, 0, 0, w, 1, 1);

  // dirtnoise and stone noise contains numnber between -1 and 1 convert
  // to 0 and 1
  for (int i = 0; i < w; i++) {
    dirtNoise[i] = pow((dirtNoise[i] + 1) / 2, 1.2);
    stoneNoise[i] = pow((stoneNoise[i] + 1) / 2, 0.1);
  }
  std::ranlux24_base prng(seed);
  int counter = getRandomInt(prng, 2, 4);
  for (int i = 0; i < counter; i++) {
    std::ranlux24_base nprng(seed + i + 2);
    int hillcoord = getRandomInt(nprng, 58, w / 2);

    if (i % 2 == 0) {
      hillcoord = getRandomInt(nprng, w / 2, w);
    }

    float radius = getRandomFloat(nprng, 50, 70);
    float height = getRandomFloat(nprng, 0.5f, 0.8f);

    for (int i = hillcoord - radius; i < hillcoord + radius; i++) {
      float distance = std::abs(i - hillcoord) / radius;

      // Smooth falloff: 1 at center, 0 at edges
      float falloff = 1.0f - distance;
      falloff = falloff * falloff * (3.0f - 2.0f * falloff);

      dirtNoise[i] += height * falloff;
    }
  }

  // GenerateWorld with Noise

  int dirtOffsetStart = -10;
  int dirtOffsetEnd = 60;
  int stoneOffsetStart = 0;
  int stoneOffsetEnd = 170;

  for (int x = 0; x < w; x++) {
    int stoneHeight =
        stoneOffsetStart + (stoneOffsetEnd - stoneOffsetStart) * stoneNoise[x];
    int dirtHeight =
        dirtOffsetStart + (dirtOffsetEnd - dirtOffsetStart) * dirtNoise[x];

    dirtHeight = stoneHeight - dirtHeight;

    for (int y = 0; y < h; y++) {
      Block b;

      if (y > dirtHeight) {
        b.type = Block::dirt;
      }
      if (y == dirtHeight) {
        b.type = Block::grassBlock;
      }
      if (y >= stoneHeight) {
        b.type = Block::stone;
      }

      gameMap.getBlockUnsafe(x, y) = b;
    }
  }

  // clear state after use
  FastNoiseSIMD::FreeNoiseSet(dirtNoise);
  FastNoiseSIMD::FreeNoiseSet(stoneNoise);
}
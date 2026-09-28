#include "worldGen.h"

#include <FastNoiseSIMD.h>
#include <raymath.h>

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
  std::unique_ptr<FastNoiseSIMD> caveNoiseGen1(
      FastNoiseSIMD::NewFastNoiseSIMD());
  std::unique_ptr<FastNoiseSIMD> caveNoiseGen2(
      FastNoiseSIMD::NewFastNoiseSIMD());

  dirtNoiseGen->SetSeed(seed++);
  stoneNoiseGen->SetSeed(seed + 2);
  caveNoiseGen1->SetSeed(seed++);
  caveNoiseGen2->SetSeed(seed + 2);

  dirtNoiseGen->SetNoiseType(FastNoiseSIMD::NoiseType::ValueFractal);
  dirtNoiseGen->SetFractalOctaves(5);
  dirtNoiseGen->SetFrequency(0.01);

  stoneNoiseGen->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
  stoneNoiseGen->SetFractalOctaves(7);
  stoneNoiseGen->SetFrequency(0.01);

  caveNoiseGen1->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
  caveNoiseGen1->SetFractalOctaves(1);
  caveNoiseGen1->SetFrequency(0.02);

  caveNoiseGen2->SetNoiseType(FastNoiseSIMD::NoiseType::PerlinFractal);
  caveNoiseGen2->SetFractalOctaves(1);
  caveNoiseGen2->SetFrequency(0.02);

  float* dirtNoise = FastNoiseSIMD::GetEmptySet(w);
  float* stoneNoise = FastNoiseSIMD::GetEmptySet(w);

  float* caveNoise1 = FastNoiseSIMD::GetEmptySet(w * h);
  float* caveNoise2 = FastNoiseSIMD::GetEmptySet(w * h);

  caveNoiseGen1->FillNoiseSet(caveNoise1, 0, 0, 0, h, w, 1);
  caveNoiseGen2->FillNoiseSet(caveNoise2, 0, 0, 0, h, w, 1);

  dirtNoiseGen->FillNoiseSet(dirtNoise, 0, 0, 0, w, 1, 1);
  stoneNoiseGen->FillNoiseSet(stoneNoise, 0, 0, 0, w, 1, 1);

  // dirtnoise and stone noise contains numnber between -1 and 1 convert
  // to 0 and 1
  for (int i = 0; i < w; i++) {
    dirtNoise[i] = pow((dirtNoise[i] + 1) / 2, 1.2);
    stoneNoise[i] = pow((stoneNoise[i] + 1) / 2, 0.1);
  }

  auto getCaveNoise1 = [&](int x, int y) { return caveNoise1[x + y * w]; };
  auto getCaveNoise2 = [&](int x, int y) { return caveNoise2[x + y * w]; };
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

  // Generate Desert with noise
  int deserCoord = getRandomInt(prng, 100, w - 250);

  int desertRadius = getRandomInt(prng, 60, 70);

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

    bool isDesert = false;
    if (x < deserCoord + desertRadius && x > deserCoord - desertRadius) {
      isDesert = true;
    }

    dirtHeight = stoneHeight - dirtHeight;

    for (int y = 0; y < h; y++) {
      Block b;

      if (y > dirtHeight) {
        b.type = Block::dirt;

        if (isDesert) {
          b.type = Block::sand;
        }
      }
      if (y == dirtHeight) {
        b.type = Block::grassBlock;

        if (isDesert) {
          b.type = Block::sand;
        }
      }
      if (y >= stoneHeight) {
        b.type = Block::stone;

        if (isDesert) {
          b.type = Block::sandStone;
        }
      }

      if (isDesert) {
        int distanceFromMidDesert = std::abs(x - deserCoord);
        float desertDistance =
            1.0f - (float)distanceFromMidDesert / (float)(desertRadius);
        if (desertDistance < 0.0f) desertDistance = 0.0f;
        if (desertDistance > 1.0f) desertDistance = 1.0f;

        // extend stone deeper under the desert center
        int desertYStart = stoneHeight + 10;

        int desertYEnd = stoneHeight + 20;

        desertDistance = Clamp(desertDistance, 0.0f, 1.0f);
        desertDistance = pow(desertDistance, 0.6);

        int triangleStoneY =
            desertYStart + static_cast<int>(desertDistance * desertYEnd);

        if (y > triangleStoneY) {
          b.type = Block::stone;
        }
      }
      if (getCaveNoise1(x, y) > 0.6 || getCaveNoise2(x, y) > 0.8) {
        b.type = Block::air;
      }
      gameMap.getBlockUnsafe(x, y) = b;
    }
  }

  // clear state after use
  FastNoiseSIMD::FreeNoiseSet(dirtNoise);
  FastNoiseSIMD::FreeNoiseSet(stoneNoise);
  FastNoiseSIMD::FreeNoiseSet(caveNoise1);
  FastNoiseSIMD::FreeNoiseSet(caveNoise2);
}
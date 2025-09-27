#pragma once
#include <cmath>
#include <raylib.h>

class NPC {
private:
  float calmness;
  float negativity;
  float patience;
  float creativity;

  float artPiecePrice;
  float baseArtPiecePrice;

  Texture2D characterTextureSheet;

public:
  NPC();
  ~NPC() {};
  void initialize();
  float getCalmness() { return calmness; };

  void generateBasePrice(float pixelCount) {
    float basePrice = 150.0f;
    float maxPrice = 550.0f;
    float growthFactor = 0.00001787f;

    artPiecePrice = basePrice + (maxPrice - basePrice) *
                                    (1.0f - expf(-growthFactor * pixelCount));
    baseArtPiecePrice = artPiecePrice;
  }

  float getBaseArtPiecePrice() { return baseArtPiecePrice; }
  float getArtPiecePrice() { return artPiecePrice; }
  void setArtPiecePrice(float newArtPiecePrice) {
    artPiecePrice = newArtPiecePrice;
  }

  float getNpcConversationStat(){return (calmness + negativity + patience) / 3;};

  void render();
};

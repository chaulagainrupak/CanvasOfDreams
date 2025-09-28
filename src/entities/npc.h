#pragma once
#include "../core/definitions.h"
#include <cmath>
#include <memory>
#include <raylib.h>

class ConversationHandler;

class NPC {
private:
  float calmness;
  float negativity;
  float patience;
  float creativity;

  float artPiecePrice;
  float baseArtPiecePrice;

  Texture2D characterTextureSheet;
  ArtPieceStats artPieceStats;

  std::unique_ptr<ConversationHandler> conversationHandler;

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

  void setArtPieceStatus(ArtPieceStats pieceStats);

  ConversationHandler &getConversationHandler() { return *conversationHandler; }

  float getNpcConversationStat() {
    return (calmness + negativity + patience) / 3;
  };

  void render();
};

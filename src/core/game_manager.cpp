#include "game_manager.h"
#include "game_state.h"

#include "../entities/brush.h"
#include "../entities/npc.h"
#include "../entities/player.h"
#include "../rendering/canvas.h"
#include "../ui/auction_interface.h"
#include "../ui/studio_interface.h"
#include "definitions.h"

#include <chrono>
#include <cmath>
#include <memory>
#include <raylib.h>
#include <unordered_map>

GameManager::GameManager() : currentState(GameState::GAME) {}

GameManager::~GameManager() {}

void GameManager::initialize() {
  player = std::make_unique<Player>();
  brush = std::make_unique<Brush>(Brush(20));
  canvas = std::make_unique<Canvas>(Canvas(560, 460, brush.get()));

  studioInterface = std::make_unique<StudioInterface>(
      this, brush.get(), canvas.get(), player.get());

  auctionInterface =
      std::make_unique<AuctionInterface>(this, canvas.get(), player.get());

  NPC currentNpc = NPC();
  currentNpc.initialize();
  this->setNPC(currentNpc);
  auctionInterface->setNPC(&currentNpc);

  lastNpcTime = std::chrono::steady_clock::now();

  loadGlobalAssets();
}

void GameManager::update() {
  switch (currentState) {
  case GameState::GAME:
    canvas->update();
    brush->update();
    studioInterface->update();
    break;

  case GameState::SPLASH:
  case GameState::CUTSCENE:
  case GameState::AUCTION:

    auctionInterface->update();
    break;
  case GameState::SHOP:
  case GameState::PAUSED:
    break;
  }
}

void GameManager::render() {
  switch (currentState) {
  case GameState::GAME:
    studioInterface->render();
    canvas->render();
    brush->render();
    break;
  case GameState::SPLASH:
  case GameState::CUTSCENE:

  case GameState::AUCTION:
    auctionInterface->render();
    canvas->render();
    break;

  case GameState::SHOP:
  case GameState::PAUSED:
    break;
  }
}

void GameManager::changeState(GameState newState) { currentState = newState; }

void GameManager::loadGlobalAssets() {
  gameFont = LoadFontEx("./res/font.otf", 64, 0, 0);
  cashTexture = LoadTexture("./res/cash.png");
  bgTexture = LoadTexture("./res/bg.png");
}

void GameManager::unloadGlobalAssets() {
  UnloadFont(gameFont);
  UnloadTexture(cashTexture);
  UnloadTexture(bgTexture);
}

void GameManager::cleanup() { unloadGlobalAssets(); }

void GameManager::rateArtPiece() {
  Image img = LoadImageFromTexture(getArtPieceCanvas().canvasTexture.texture);
  Color *pixels = (Color *)img.data;

  std::unordered_map<int, int> colorFrequency;
  int totalColoredPixels = 0;

  int pixelCount = img.width * img.height;
  for (int i = 0; i < pixelCount; i++) {
    Color pixel = pixels[i];
    if (pixel.a == 0)
      continue; // Skip transparent

    totalColoredPixels++;

    float minDistance = 1e9;
    int bestMatchIndex = -1;

    for (int c = 0; c <= 74; c++) {
      const ColorStats &color = colorStatsArray[c];
      float avgR = (color.colorRedRange[0] + color.colorRedRange[1]) / 2.0f;
      float avgG = (color.colorGreenRange[0] + color.colorGreenRange[1]) / 2.0f;
      float avgB = (color.colorBlueRange[0] + color.colorBlueRange[1]) / 2.0f;

      float dist = std::sqrt((pixel.r - avgR) * (pixel.r - avgR) +
                             (pixel.g - avgG) * (pixel.g - avgG) +
                             (pixel.b - avgB) * (pixel.b - avgB));
      if (dist < minDistance) {
        minDistance = dist;
        bestMatchIndex = c;
      }
    }

    if (bestMatchIndex != -1) {
      colorFrequency[bestMatchIndex]++;
    }
  }

  currentNpc.generateBasePrice(totalColoredPixels);

  for (auto &[index, freq] : colorFrequency) {
    TraceLog(LOG_INFO, "Color %s appears %d times",
             colorStatsArray[index].colorName, freq);
  }
  TraceLog(LOG_INFO, "Total colored pixels: %i", totalColoredPixels);

  ArtPieceStats artStats = {};
  int colorCount = static_cast<int>(colorFrequency.size());

  float sumSqrtFreq = 0.0f;
  for (auto &[colorIndex, freq] : colorFrequency) {
    sumSqrtFreq += std::sqrt(static_cast<float>(freq));
  }

  for (auto &[colorIndex, freq] : colorFrequency) {
    float weight = std::sqrt(static_cast<float>(freq)) / sumSqrtFreq;
    const ColorStats &color = colorStatsArray[colorIndex];

    artStats.calmness += weight * color.calmness;
    artStats.negativity += weight * color.negativity;
    artStats.patience += weight * color.patience;
    artStats.warmth += weight * color.warmth;
    artStats.energy += weight * color.energy;
    artStats.creativity += weight * color.creativity;
    artStats.happiness += weight * color.happiness;
    artStats.sadness += weight * color.sadness;
    artStats.dominance += weight * color.dominance;
  }

  artStats.calmness /= colorCount;
  artStats.negativity /= colorCount;
  artStats.patience /= colorCount;
  artStats.warmth /= colorCount;
  artStats.energy /= colorCount;
  artStats.creativity /= colorCount;
  artStats.happiness /= colorCount;
  artStats.sadness /= colorCount;
  artStats.dominance /= colorCount;

  TraceLog(LOG_INFO,
           "Art Stats:\nCalmness: %.2f\nNegativity: %.2f\nPatience: "
           "%.2f\nWarmth: %.2f\n"
           "Energy: %.2f\nCreativity: %.2f\nHappiness: %.2f\nSadness: "
           "%.2f\nDominance: %.2f",
           artStats.calmness, artStats.negativity, artStats.patience,
           artStats.warmth, artStats.energy, artStats.creativity,
           artStats.happiness, artStats.sadness, artStats.dominance);

  float basePrice = currentNpc.getBaseArtPiecePrice();

  float combinedEffect = 0.0f;
  combinedEffect += artStats.creativity * 0.15f;
  combinedEffect += artStats.happiness * 0.10f;
  combinedEffect += artStats.negativity * -0.12f;
  combinedEffect += artStats.sadness * -0.08f;
  combinedEffect += artStats.energy * 0.05f;
  combinedEffect += artStats.calmness * 0.03f;
  combinedEffect += artStats.patience * 0.02f;
  combinedEffect += artStats.warmth * 0.02f;
  combinedEffect += artStats.dominance * 0.04f;

  float finalPrice = basePrice;

  if (colorCount <= 2) {
    // 2 colors punishment with negativity + sadness
    float penalty = 0.1f;
    finalPrice = basePrice * penalty * (1.0f + artStats.negativity) *
                 (1.0f + artStats.sadness);

  } else if (colorCount <= 4) {
    // 3–4 colors check frequency balance
    int maxFreq = 0;
    int minFreq = 0;
    bool first = true;

    for (auto &[colorIndex, freq] : colorFrequency) {
      if (first) {
        maxFreq = minFreq = freq;
        first = false;
      } else {
        maxFreq = std::max(maxFreq, freq);
        minFreq = std::min(minFreq, freq);
      }
    }

    bool balanced =
        (maxFreq > 0) && ((maxFreq - minFreq) / (float)maxFreq < 0.2f);

    if (balanced) {
      // Balanced
      float penalty = 0.4f;
      finalPrice = basePrice * penalty;
    } else {
      finalPrice = basePrice + basePrice * combinedEffect;
    }

  } else {
    // ≥ 5 colors → scale down slowly by negativity
    float negativityPenalty = std::max(0.5f, 1.0f + artStats.negativity * 0.3f +
                                                 artStats.sadness * -0.08f);
    finalPrice = (basePrice + basePrice * combinedEffect) * negativityPenalty;
  }

  // Clamp so price is never free
  if (finalPrice < 1.0f)
    finalPrice = 1.0f;

  currentNpc.setArtPiecePrice(finalPrice);

  TraceLog(LOG_INFO, "Base Price: $%.2f", basePrice);
  // TraceLog(LOG_INFO, "Diversity Factor: %.2f (colors: %d)", diversityFactor,
  //          colorCount);
  TraceLog(LOG_INFO, "Final NPC Offer: $%.2f", finalPrice);

  UnloadImage(img);
}

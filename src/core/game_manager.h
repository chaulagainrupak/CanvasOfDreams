#pragma once
#include "../entities/npc.h"
#include "conversation_handler.h"
#include "game_state.h"
#include "raylib.h"
#include <chrono>
#include <memory>

class Player;
class Brush;
class Canvas;
class BaseInterface;
class StudioInterface;
class AuctionInterface;

class GameManager {
private:
  GameState currentState;
  std::unique_ptr<Player> player;
  std::unique_ptr<Brush> brush;
  std::unique_ptr<Canvas> canvas;
  std::unique_ptr<StudioInterface> studioInterface;
  std::unique_ptr<AuctionInterface> auctionInterface;
  std::unique_ptr<ConversationHandler> conversationHandler;

  NPC currentNpc;
  std::chrono::steady_clock::time_point lastNpcTime;
  const float npcCooldown = 150.0f; // 2.5 min

public:
  Font gameFont{};
  Texture2D cashTexture{};
  Texture2D bgTexture{};

  GameManager();
  ~GameManager();

  void initialize();
  void update();
  void render();
  void cleanup();

  void loadGlobalAssets();
  void unloadGlobalAssets();

  void changeState(GameState newState);
  GameState getCurrentState() const { return currentState; }

  void setNPC(NPC npc) { currentNpc = npc; }
  NPC &getNPC() { return currentNpc; };
  void setLastNpcTime(std::chrono::steady_clock::time_point newTime) {
    lastNpcTime = newTime;
  }
  std::chrono::steady_clock::time_point getLastNpcTime() { return lastNpcTime; }
  float getNpcCooldown() { return npcCooldown; }

  Canvas &getArtPieceCanvas() { return *canvas; }
  void rateArtPiece();
};

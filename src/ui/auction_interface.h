#pragma once
#include "../rendering/button.h"
#include "base_interface.h"
#include <vector>

class Canvas;
class Player;
class NPC;

class AuctionInterface : public BaseInterface {
private:
  std::vector<Button> buttons;
  Canvas *canvas;
  Player *player;
  NPC *currentNpc;

  void setupButtons();

public:
  AuctionInterface(GameManager *gm, Canvas *canvas, Player *player);
  ~AuctionInterface() = default;

  void update() override;
  void render() override;
  void unload() override;

  void setNPC(NPC *npc) { currentNpc = npc; }
};

#pragma once
#include "../core/game_state.h"
#include "raylib.h"

class GameManager;

class BaseInterface {
protected:
  GameManager *gameManager;

public:
  BaseInterface(GameManager *gm) : gameManager(gm) {}

  virtual ~BaseInterface() = default;

  virtual void update() = 0;
  virtual void render() = 0;
  virtual void unload() = 0;
};

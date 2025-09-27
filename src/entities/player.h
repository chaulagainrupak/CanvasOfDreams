#pragma once
#include "raylib.h"

class Player {
public:
  Player();
  ~Player();

  float getBalance() { return balance; }

private:
  float balance = 500;
};

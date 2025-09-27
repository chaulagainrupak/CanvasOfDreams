#include "npc.h"
#include <raylib.h>

NPC::NPC() { characterTextureSheet = LoadTexture("./res/characters.png"); }

void NPC::initialize() {
  calmness = GetRandomValue(10, 99) / 100.0f;
  negativity = -GetRandomValue(10, 99) / 100.0f;
  patience = GetRandomValue(20, 99) / 100.0f;
};

void NPC::render() {
  DrawTextureRec(characterTextureSheet, Rectangle{0, 0, 64, 64}, {616, 80},
                 WHITE);
}

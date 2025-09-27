#include "npc.h"

void NPC::initialize() {
  calmness = GetRandomValue(10, 99) / 100.0f;
  negativity = -GetRandomValue(10, 99) / 100.0f;
  patience = GetRandomValue(10, 99) / 100.0f;
};

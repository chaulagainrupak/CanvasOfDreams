#pragma once
#include <vector>

struct ColorStats {
  const char *colorName;
  int colorRedRange[2];
  int colorGreenRange[2];
  int colorBlueRange[2];
  float calmness;
  float negativity;
  float patience;
  float warmth;
  float energy;
  float creativity;
  float happiness;
  float sadness;
  float dominance;
};

struct ArtPieceStats {
  float calmness;
  float negativity;
  float patience;
  float warmth;
  float energy;
  float creativity;
  float happiness;
  float sadness;
  float dominance;
};

struct DialogueOption {
  const char *dialogueText;

  // Stats for evaluating response
  float calmness;
  float negativity;
  float patience;
  float warmth;
  float energy;
  float creativity;
  float happiness;
  float sadness;
  float dominance;
};

// External arrays
extern ColorStats colorStatsArray[74];
extern std::vector<DialogueOption> npcDialogues;
extern std::vector<DialogueOption> playerOptions;

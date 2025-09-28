#pragma once

#include "../entities/npc.h"
#include "definitions.h"
#include <raylib.h>
#include <vector>

enum CurrentSpeaker { PLAYER, BUYER };

class ConversationHandler {
public:
  ConversationHandler(std::vector<DialogueOption> &npcOpts,
                      std::vector<DialogueOption> &playerOpts);

  void generateFirstDialogue(ArtPieceStats *pieceStas);

private:
  NPC *currentNpc;
  ArtPieceStats *artPieceStats;

  DialogueOption currentDialogue;
  bool isFirstDialogue;
  std::vector<DialogueOption> &npcOpts;
  std::vector<DialogueOption> &playerOpts;
};

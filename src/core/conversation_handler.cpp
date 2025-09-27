#include "conversation_handler.h"
#include "definitions.h"
#include <raylib.h>

ConversationHandler::ConversationHandler(
    std::vector<DialogueOption> &npcOptions,
    std::vector<DialogueOption> &playerOptions, NPC *activeNpc)
    : npcOpts(npcOptions), playerOpts(playerOptions), currentNpc(activeNpc) {
  isFirstDialogue = true;
}

void ConversationHandler::setArtPieceStatus(ArtPieceStats *pieceStats) {

  // Only generate dialogue if first time
  if (!isFirstDialogue) {
    TraceLog(LOG_INFO, "[Art Selector] Using existing dialogue: %s",
             currentDialogue.dialogueText ? currentDialogue.dialogueText
                                          : "(null)");
    return;
  }

  TraceLog(LOG_INFO, "TF? %b", isFirstDialogue);
  artPieceStats = pieceStats;

  std::vector<const DialogueOption *> possibleDialogues;
  float artWeight = 0.7f;
  float npcWeight = 0.3f;

  struct Stat {
    float calmness, negativity, patience, warmth, energy, creativity, happiness,
        sadness, dominance;
  } targetStat;

  targetStat.calmness = artPieceStats->calmness * artWeight +
                        currentNpc->getCalmness() * npcWeight;
  targetStat.negativity = artPieceStats->negativity * artWeight +
                          currentNpc->getNpcConversationStat() * npcWeight;
  targetStat.patience = artPieceStats->patience * artWeight +
                        currentNpc->getNpcConversationStat() * npcWeight;
  targetStat.warmth = artPieceStats->warmth * artWeight;
  targetStat.energy = artPieceStats->energy * artWeight;
  targetStat.creativity = artPieceStats->creativity * artWeight;
  targetStat.happiness = artPieceStats->happiness * artWeight;
  targetStat.sadness = artPieceStats->sadness * artWeight;
  targetStat.dominance = artPieceStats->dominance * artWeight;

  float bestScore = 1e9f;
  const DialogueOption *bestOption = nullptr;

  for (const DialogueOption &option : npcOpts) {
    float score = 0.0f;
    score += std::pow(option.calmness - targetStat.calmness, 2);
    score += std::pow(option.negativity - targetStat.negativity, 2);
    score += std::pow(option.patience - targetStat.patience, 2);
    score += std::pow(option.warmth - targetStat.warmth, 2);
    score += std::pow(option.energy - targetStat.energy, 2);
    score += std::pow(option.creativity - targetStat.creativity, 2);
    score += std::pow(option.happiness - targetStat.happiness, 2);
    score += std::pow(option.sadness - targetStat.sadness, 2);
    score += std::pow(option.dominance - targetStat.dominance, 2);

    score = std::sqrt(score);

    // Keep all below a threshold or just track the best
    if (score < bestScore) {
      bestScore = score;
      bestOption = &option;
    }
  }

  if (bestOption) {
    currentDialogue = *bestOption;
    isFirstDialogue = false;

    TraceLog(LOG_INFO, "[Art Selector] Chosen Dialogue: %s",
             currentDialogue.dialogueText);
  } else {
    TraceLog(LOG_WARNING, "No suitable dialogue found for this art piece.");
  }
}

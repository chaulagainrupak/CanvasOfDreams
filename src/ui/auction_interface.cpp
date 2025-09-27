#include "auction_interface.h"
#include "../core/game_manager.h"
#include "../entities/player.h"
#include "../rendering/canvas.h"
#include "../entities/npc.h"
#include <raylib.h>

AuctionInterface::AuctionInterface(GameManager *gm, Canvas *canvas,
                                   Player *player)
    : BaseInterface(gm), canvas(canvas), player(player) {
  setupButtons();
}

void AuctionInterface::update() {
  for (auto &button : buttons) {
    button.checkClick();
  }
}

void AuctionInterface::render() {
  DrawTexture(gameManager->bgTexture, 0, 0, WHITE);
  DrawTexture(gameManager->cashTexture, 624, 16, WHITE);
  DrawTextEx(gameManager->gameFont, TextFormat("Balance: $%.0f", player->getBalance()),
             Vector2{696, 32}, 32, 0, BLACK);




  for (auto &button : buttons) {
    button.render();
  }

  DrawText(TextFormat("Buyer's calmness level in %: %f", gameManager->getNPC().getCalmness()), 100, 600, 30, BLACK);

}

void AuctionInterface::unload() {
  for (auto &button : buttons) {
    button.cleanup();
  }
}

void AuctionInterface::setupButtons() {
  buttons.emplace_back(
      Button(Vector2{192, 64}, Vector2{400, 540},
             LoadTexture("./res/cancelbutton.png"),
             [this]() { this->gameManager->changeState(GameState::GAME); }));
}

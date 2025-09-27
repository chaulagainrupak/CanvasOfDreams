#include "../core/game_manager.h"
#include "../entities/brush.h"
#include "../entities/player.h"
#include "../rendering/canvas.h"
#include "studio_interface.h"
#include <raylib.h>

StudioInterface::StudioInterface(GameManager *gm, Brush *brush, Canvas *canvas,
                                 Player *player)
    : BaseInterface(gm), activeBrush(brush), canvas(canvas), player(player) {

  setupButtons();
}

void StudioInterface::setupButtons() {
  buttons.emplace_back(
      Button(Vector2{64, 64}, Vector2{624, 80}, LoadTexture("./res/minus.png"),
             [this]() { this->activeBrush->incrementBrushSize(-1); }));

  buttons.emplace_back(
      Button(Vector2{64, 64}, Vector2{696, 80}, LoadTexture("./res/plus.png"),
             [this]() { this->activeBrush->incrementBrushSize(1); }));

  buttons.emplace_back(
      Button(Vector2{64, 64}, Vector2{768, 80},
             LoadTexture("./res/arrow_left.png"), [this]() {
               this->activeBrush->brushType = static_cast<BrushTypes>(
                   (this->activeBrush->brushType - 1) % NUM_BRUSH_TYPES);
             }));

  buttons.emplace_back(Button(Vector2{64, 64}, Vector2{848, 80},
                              LoadTexture("./res/buttonBG.png"), []() {}));

  buttons.emplace_back(
      Button(Vector2{64, 64}, Vector2{928, 80},
             LoadTexture("./res/arrow_right.png"), [this]() {
               this->activeBrush->brushType = static_cast<BrushTypes>(
                   (this->activeBrush->brushType + 1) % NUM_BRUSH_TYPES);
             }));

  buttons.emplace_back(Button(Vector2{64, 64}, Vector2{624, 176},
                              LoadTexture("./res/pencil.png"), [this]() {
                                this->activeBrush->brushMode = PENCIL;
                                this->activeBrush->brushColor =
                                    this->activeBrush->previousBurshColor;
                              }));

  buttons.emplace_back(Button(Vector2{64, 64}, Vector2{704, 176},
                              LoadTexture("./res/eraser.png"), [this]() {
                                this->activeBrush->brushMode = ERASER;
                                this->activeBrush->previousBurshColor =
                                    this->activeBrush->brushColor;
                                this->activeBrush->brushColor = {255, 255, 255,
                                                                 192};
                              }));

  buttons.emplace_back(Button(Vector2{64, 64}, Vector2{784, 176},
                              LoadTexture("./res/spray.png"), [this]() {
                                this->activeBrush->brushMode = SPRAY;
                                this->activeBrush->brushColor =
                                    this->activeBrush->previousBurshColor;
                              }));

  // This disgusting code is to load a single image and make color swatches not
  // the most beautiful but works... This is my first game dwag... PS i dont
  // have a degree

  Image baseSwatch = LoadImage("./res/colorswatch.png");

  Image whiteSwatch = ImageCopy(baseSwatch);
  Image blackSwatch = ImageCopy(baseSwatch);
  Image redSwatch = ImageCopy(baseSwatch);
  Image greenSwatch = ImageCopy(baseSwatch);
  Image blueSwatch = ImageCopy(baseSwatch);
  Image yellowSwatch = ImageCopy(baseSwatch);

  ImageColorTint(&whiteSwatch, WHITE);
  ImageColorTint(&blackSwatch, BLACK);
  ImageColorTint(&redSwatch, RED);
  ImageColorTint(&greenSwatch, GREEN);
  ImageColorTint(&blueSwatch, BLUE);
  ImageColorTint(&yellowSwatch, YELLOW);

  buttons.emplace_back(
      Button({64, 64}, {624, 256}, LoadTextureFromImage(whiteSwatch), [this]() {
        if (this->activeBrush->brushMode == ERASER)
          this->activeBrush->brushMode = PENCIL;
        this->activeBrush->brushColor = WHITE;
        this->activeBrush->previousBurshColor = WHITE;
      }));

  buttons.emplace_back(
      Button({64, 64}, {696, 256}, LoadTextureFromImage(blackSwatch), [this]() {
        if (this->activeBrush->brushMode == ERASER)
          this->activeBrush->brushMode = PENCIL;
        this->activeBrush->brushColor = BLACK;
        this->activeBrush->previousBurshColor = BLACK;
      }));

  buttons.emplace_back(
      Button({64, 64}, {768, 256}, LoadTextureFromImage(redSwatch), [this]() {
        if (this->activeBrush->brushMode == ERASER)
          this->activeBrush->brushMode = PENCIL;
        this->activeBrush->brushColor = RED;
        this->activeBrush->previousBurshColor = RED;
      }));

  buttons.emplace_back(
      Button({64, 64}, {840, 256}, LoadTextureFromImage(greenSwatch), [this]() {
        if (this->activeBrush->brushMode == ERASER)
          this->activeBrush->brushMode = PENCIL;
        this->activeBrush->brushColor = GREEN;
        this->activeBrush->previousBurshColor = GREEN;
      }));

  buttons.emplace_back(
      Button({64, 64}, {912, 256}, LoadTextureFromImage(blueSwatch), [this]() {
        if (this->activeBrush->brushMode == ERASER)
          this->activeBrush->brushMode = PENCIL;
        this->activeBrush->brushColor = BLUE;
        this->activeBrush->previousBurshColor = BLUE;
      }));

  buttons.emplace_back(Button({64, 64}, {984, 256},
                              LoadTextureFromImage(yellowSwatch), [this]() {
                                if (this->activeBrush->brushMode == ERASER)
                                  this->activeBrush->brushMode = PENCIL;
                                this->activeBrush->brushColor = YELLOW;
                                this->activeBrush->previousBurshColor = YELLOW;
                              }));

  buttons.emplace_back(Button(
      Vector2{192, 64}, Vector2{400, 540}, LoadTexture("./res/sellbutton.png"),
      [this]() {
        gameManager->changeState(GameState::AUCTION);

        // NPC generation check
        {
          auto now = std::chrono::steady_clock::now();
          float secondsSinceLast =
              std::chrono::duration<float>(now - gameManager->getLastNpcTime())
                  .count();

          if (secondsSinceLast >= gameManager->getNpcCooldown()) {
            NPC newNPC = NPC();
            newNPC.initialize();
            gameManager->setNPC(newNPC);
            gameManager->setLastNpcTime(now);
          }

          gameManager->rateArtPiece();
        };
      }));

  buttons.emplace_back(Button(Vector2{192, 64}, Vector2{192, 540},
                              LoadTexture("./res/cancelbutton.png"), [this]() {
                                this->canvas->clear(Color{0, 0, 0, 0});
                              }));
}

void StudioInterface::update() {
  Vector2 mousePos = GetMousePosition();

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    canvas->drawToCanvas(mousePos);
  }

  for (auto &button : buttons) {
    button.checkClick();
  }
}

void StudioInterface::render() {
  DrawTexture(gameManager->bgTexture, 0, 0, WHITE);
  DrawTexture(gameManager->cashTexture, 624, 16, WHITE);
  DrawTextEx(gameManager->gameFont,
             TextFormat("Balance: $%.0f", player->getBalance()),
             Vector2{696, 32}, 32, 0, BLACK);

  DrawTextEx(gameManager->gameFont,
             TextFormat("Brush Size: %.0fpx", activeBrush->width),
             Vector2{624, 152}, 16, 0, BLACK);
  DrawTextEx(gameManager->gameFont, "Brush Type: TAB to switch",
             Vector2{768, 152}, 16, 0, BLACK);

  DrawRectangle(624, 336, 64, 64, activeBrush->brushColor);
  for (auto &button : buttons) {
    button.render();
  }

  // Overlay for active brush type :)
  switch (activeBrush->brushMode) {
  case PENCIL:
    DrawRectangle(624, 176, 64, 64, Fade(BLACK, 0.4f));
    break;
  case ERASER:
    DrawRectangle(704, 176, 64, 64, Fade(BLACK, 0.4f));
    break;
  case SPRAY:
    DrawRectangle(784, 176, 64, 64, Fade(BLACK, 0.4f));
    break;
  }

  switch (activeBrush->brushType) {
  case CIRCLE:
    DrawCircle(848 + 32, 80 + 32, 16, BLACK);
    break;
  case SQUARE:
    DrawRectangle(848 + 16, 80 + 16, 32, 32, BLACK);
    break;
  default:
    DrawCircle(848 + 32, 80 + 32, 16, BLACK);
    break;
  }
}

void StudioInterface::unload() {
  UnloadTexture(gameManager->bgTexture);
  UnloadTexture(gameManager->cashTexture);
  UnloadFont(gameManager->gameFont);
  for (auto &button : buttons) {
    button.cleanup();
  }
}

StudioInterface::~StudioInterface() {}

#include "brush.h"
#include <raylib.h>

Brush::Brush(int w) {
  width = w;
  brushType = CIRCLE;
  brushMode = PENCIL;

  brushColor = Color{112, 31, 126, 255};
  previousBurshColor = brushColor;
}

void Brush::update() {

  if (IsKeyDown(KEY_ONE)) {
    brushColor.r += 5;
  }
  if (IsKeyDown(KEY_TWO)) {
    brushColor.g += 5;
  }
  if (IsKeyDown(KEY_THREE)) {
    brushColor.b += 5;
  }

  width += GetMouseWheelMove();
  if (IsKeyDown(KEY_EQUAL))
    width++;
  if (IsKeyDown(KEY_MINUS))
    width--;
  if (width < 1)
    width = 1;
  if (width > 150)
    width = 150;
  if (IsKeyPressed(KEY_TAB)) {
    brushType = static_cast<BrushTypes>((brushType + 1) % NUM_BRUSH_TYPES);
  }
}

void Brush::incrementBrushSize(float amount) {
  width += amount;
  if (width < 1)
    width = 1;
  if (width > 150)
    width = 150;
}

void Brush::render() {
  Vector2 mousePos = GetMousePosition();

  switch (brushType) {
  case CIRCLE:
    DrawCircle(mousePos.x, mousePos.y, width, Fade(brushColor, 0.3f));
    break;

  case SQUARE:
    DrawRectangle(mousePos.x - width / 2, mousePos.y - width / 2, width, width,
                  Fade(brushColor, 0.3f));
    break;

  default:
    DrawCircle(mousePos.x, mousePos.y, width, Fade(brushColor, 0.3f));
    break;
  }
}

void Brush::setType(BrushTypes type) { brushType = type; }

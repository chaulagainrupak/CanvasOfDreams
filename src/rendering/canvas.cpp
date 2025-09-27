#include "canvas.h"
#include <raylib.h>

Canvas::Canvas(float w, float h, Brush *brush) {
  width = w;
  height = h;
  activeBrush = brush;
  canvasTexture = LoadRenderTexture(width, height);
  canvasOffset = {312 - width / 2.0f, 32};

  BeginTextureMode(canvasTexture);
  ClearBackground({0, 0, 0, 0});
  EndTextureMode();
}

Canvas::~Canvas() {}

void Canvas::unload() { UnloadRenderTexture(canvasTexture); }

void Canvas::update() {
  if (IsKeyPressed(KEY_R))
    clear({0, 0, 0, 0});

  if (IsKeyPressed(KEY_Q))
    exportImage();
}

void Canvas::clear(Color color) {
  BeginTextureMode(canvasTexture);
  ClearBackground(color);
  EndTextureMode();
}

void Canvas::exportImage() {

  if (!DirectoryExists("./exports")) {
    MakeDirectory("./exports");
  }
  Image canvasImage = LoadImageFromTexture(canvasTexture.texture);
  ImageFlipVertical(&canvasImage);
  ExportImage(canvasImage, "./exports/image.png");
  UnloadImage(canvasImage);
}

void Canvas::drawToCanvas(Vector2 pos) {
  if (!activeBrush) {
    return;
  }

  if (pos.x > canvasOffset.x + width || pos.y > canvasOffset.y + height) {
    return;
  }

  BeginTextureMode(canvasTexture);

  // SPRAY BOTTLE HANDELING
  if (activeBrush->brushMode == SPRAY) {
    int cx = static_cast<int>(pos.x - canvasOffset.x);
    int cy = static_cast<int>(pos.y - canvasOffset.y);
    int radius = static_cast<int>(activeBrush->width);

    int amountOfPoints = static_cast<int>(0.1f * (4 * radius * radius));

    // this just gets a random point int a circle and then we place a pixel on
    // it
    for (int i = 0; i < amountOfPoints; i++) {
      int x = GetRandomValue(cx - radius, cx + radius);
      int y = GetRandomValue(cy - radius, cy + radius);

      int dx = x - cx;
      int dy = y - cy;

      if (dx * dx + dy * dy <= radius * radius) {
        DrawPixel(x, y, activeBrush->brushColor);
      }
    }

    EndTextureMode();
    return;
  }

  switch (activeBrush->brushType) {
  case CIRCLE:
    DrawCircle(pos.x - canvasOffset.x, pos.y - canvasOffset.y,
               activeBrush->width, activeBrush->brushColor);
    break;

  case SQUARE:
    DrawRectangle(pos.x - activeBrush->width / 2 - canvasOffset.x,
                  pos.y - activeBrush->width / 2 - canvasOffset.y,
                  activeBrush->width, activeBrush->width,
                  activeBrush->brushColor);
    break;

  default:
    DrawCircle(pos.x - canvasOffset.x, pos.y - canvasOffset.y,
               activeBrush->width, activeBrush->brushColor);
    break;
  }

  EndTextureMode();
}

void Canvas::updateWidth(float newWidth) {
  width += newWidth;
  resizeCanvas(width, height);
}

void Canvas::updateHeight(float newHeight) {
  height += newHeight;
  resizeCanvas(width, height);
}

void Canvas::resizeCanvas(float newWidth, float newHeight) {
  unload();

  width = newWidth;
  height = newHeight;

  canvasTexture = LoadRenderTexture(width, height);

  BeginTextureMode(canvasTexture);
  ClearBackground({0, 0, 0, 0});
  EndTextureMode();
}

void Canvas::render(Color outlineColor, float outlineThickness) {
  Rectangle src = {0, 0, (float)canvasTexture.texture.width,
                   -(float)canvasTexture.texture.height};
  Vector2 shadowOffset = {10, 10};

  // Shadow horizontal
  DrawRectangle(canvasOffset.x + width, canvasOffset.y + shadowOffset.y,
                shadowOffset.x, height, Fade(BLACK, 0.3f));
  // Shadow vertical
  DrawRectangle(canvasOffset.x + shadowOffset.x, canvasOffset.y + height,
                width - shadowOffset.x, shadowOffset.y, Fade(BLACK, 0.3f));
  // Canvas
  DrawTextureRec(canvasTexture.texture, src, canvasOffset, WHITE);

  // Outline
  DrawRectangleLinesEx(
      {canvasOffset.x, canvasOffset.y, (float)width, (float)height},
      outlineThickness, outlineColor);
}

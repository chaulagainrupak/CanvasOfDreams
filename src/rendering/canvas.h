#pragma once
#include "../entities/brush.h"
#include "raylib.h"

class Canvas {
public:
  float width, height;
  RenderTexture2D canvasTexture;
  Vector2 canvasOffset;
  Brush *activeBrush;

  Canvas(float w, float h, Brush *Brush);
  ~Canvas();

  void unload();
  void clear(Color color);
  void update();
  void drawToCanvas(Vector2 pos);
  void exportImage();
  void updateWidth(float newWidth);
  void updateHeight(float newHeight);

  void resizeCanvas(float newWidth, float newHeight);
  void render(Color outlineColor = BLACK, float outlineThickness = 2.0f);
};

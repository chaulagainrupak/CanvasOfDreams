#pragma once
#include "raylib.h"

enum BrushTypes { CIRCLE = 0, SQUARE, NUM_BRUSH_TYPES };
enum BrushModes {PENCIL, ERASER, SPRAY};

class Brush {
public:
  float width;
  BrushTypes brushType;
  BrushModes brushMode;
  Color brushColor;
  Color previousBurshColor;
  Brush(int w);

  void update();
  void render();

  void incrementBrushSize(float amount);
  void setType(BrushTypes type);
};

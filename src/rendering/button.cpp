#include "button.h"

Button::Button() {}

Button::Button(Vector2 dim, Vector2 pos, Texture2D bTexture,
               std::function<void()> onClickFunction) {
  dimensions = dim;
  position = pos;
  buttonTexture = bTexture;
  onClick = onClickFunction;
}

Button::~Button() {};

void Button::render() {
  DrawTextureEx(buttonTexture, position, this->rotation, 1, WHITE);
};

void Button::cleanup() { UnloadTexture(buttonTexture); }

void Button::checkClick() {

  Vector2 mousePos = GetMousePosition();
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (mousePos.x >= position.x && mousePos.x <= position.x + dimensions.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + dimensions.y) {
      if (onClick)
        onClick();
    }
  }
}

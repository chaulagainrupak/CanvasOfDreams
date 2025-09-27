#pragma once
#include "raylib.h"
#include <functional>

class Button {
private:
    Texture2D buttonTexture;
    Vector2 dimensions;
    Vector2 position;
    float rotation;
    std::function<void()> onClick;

public:
    Button();
    Button(Vector2 dimensions, Vector2 position, Texture2D buttonTexture,
           std::function<void()> onClickFunction);
    ~Button();

    void render();
    void cleanup();
    void checkClick();
    bool isClicked(Vector2 mousePos) const;
};

#pragma once
#include "base_interface.h"
#include "../rendering/button.h"


class Brush;
class Canvas;
class Player;

class StudioInterface : public BaseInterface {
private:
    Brush* activeBrush;
    Canvas* canvas;
    Player* player;

    std::vector<Button> buttons;

    void setupButtons();

public:
    StudioInterface(GameManager* gm, Brush* brush, Canvas* canvas, Player* player);
    ~StudioInterface();

    void update() override;
    void render() override;
    void unload() override;
};

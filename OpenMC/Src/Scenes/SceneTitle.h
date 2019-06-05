#pragma once
#include "../Camera/Camera.h"
#include "../SpriteRenderer/Sprite.h"
#include "../SpriteRenderer/SpriteRenderer.h"
#include "Scene.h"

class SceneTitle final : public Scene {
public:
    virtual void Start() override;
    virtual void Update() override;

    void initBlocks();
    void cursorPosCallback(double xpos, double ypos);
    void mouseButtonCallback(int button, int action, int mods);

    //bool cursorOnButton;

private:
    SpriteRenderer* renderer;
    Camera* camera;
    Window* window;

    enum MenuItem { Null = 0, MenuStart, MenuLoad, MenuSettings, MenuExit };
    MenuItem menuItem;

    int currentTime;
};


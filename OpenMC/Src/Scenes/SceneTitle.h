#pragma once
#include "../SpriteRenderer/SpriteRenderer.h"
#include "../Systems/Graphics.h"
#include "Scene.h"

class SceneTitle final : public Scene {
public:
    virtual void Start() override;
    virtual void Update() override;
    void setUpGraphics();
    static void cursorPosCallback(double xpos, double ypos);
    static void mouseButtonCallback(int button, int action, int mods);

    //bool cursorOnButton;

private:
    Graphics *graphics;

    SpriteRenderer* renderer;
    Camera* camera;
    Window* window;
    int currentTime;
};


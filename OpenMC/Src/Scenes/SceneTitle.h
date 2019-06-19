#pragma once
#include "../Camera/Camera.h"
#include "../SpriteRenderer/SpriteRenderer.h"
#include "Scene.h"

class SceneTitle final : public Scene {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void Terminate() override;

    void initBlocks();
    void cursorPosCallback(double xpos, double ypos);
    void mouseButtonCallback(int button, int action, int mods);

private:
    SpriteRenderer* renderer;
    Camera* camera;

    enum MenuItem { Null = 0, MenuStart, MenuLoad, MenuSettings, MenuExit };
    MenuItem menuItem;
    const glm::vec4 YELLOW = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    const glm::vec4 WHITE = glm::vec4(1.0f);

    int loadGame = 0;
};


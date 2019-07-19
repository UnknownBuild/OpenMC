#pragma once
#include "../Camera/Camera.h"
#include "../SpriteRenderer/SpriteRenderer.h"
#include "../World/World.h"
#include "../World/Characters/Player.h"
#include "Scene.h"

class SceneLoad final : public Scene {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void Terminate() override;

    void cursorPosCallback(double xpos, double ypos);
    void mouseButtonCallback(int button, int action, int mods);
    void keyCallback(int key, int scancode, int action, int mods);

private:
    SpriteRenderer* renderer;
    Camera* camera;
    World* world;
    Player* player;

    enum MenuItem { Null = 0, MainMenu, Help, Back };
    MenuItem menuItem;
    const glm::vec4 YELLOW = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    const glm::vec4 WHITE = glm::vec4(1.0f);
    int status; //0-playing  1-paused   2-help
    bool keys[100];
};

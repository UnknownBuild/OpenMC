#pragma once
#include "../Camera/Camera.h"
#include "../SpriteRenderer/SpriteRenderer.h"
#include "../World/Characters/Player.h"
#include "../World/World.h"
#include "Scene.h"

class SceneGame final : public Scene {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void Terminate() override;

    void cursorPosCallback(double xpos, double ypos);
    void mouseButtonCallback(int button, int action, int mods);
    glm::vec3 caculateLookingAt();
    glm::vec3 getNewBlockPosition();

private:
    SpriteRenderer* renderer;
    Camera* camera;
    World* world;
    Player* player;

    glm::vec3 lookingAt;
    glm::vec3 position;
    vector<BlockId> blockType;

    // test
    glm::vec3 testColor = glm::vec3(2, 2, 2);
};

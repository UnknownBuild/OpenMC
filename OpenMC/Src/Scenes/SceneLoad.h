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

private:
    SpriteRenderer* renderer;
    Camera* camera;
    World* world;
    Player* player;
};

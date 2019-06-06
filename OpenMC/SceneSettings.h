#pragma once
#include "Scene.h"
#include "../Camera/Camera.h"
#include "../SpriteRenderer/SpriteRenderer.h"

class SceneSettings final : public Scene {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void Terminate() override;

private:
    SpriteRenderer* renderer;
    Camera* camera;

    // test
    glm::vec3 testColor = glm::vec3(0.5, 0.5, 0.5);
};

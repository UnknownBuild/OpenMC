#pragma once
#include "../Camera/Camera.h"
#include "../SpriteRenderer/SpriteRenderer.h"
#include "Scene.h"

class SceneGame final : public Scene {
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

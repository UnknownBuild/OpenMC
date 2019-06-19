#pragma once
#include "Character.h"
#include "../../Collision/Collision.h"
#include "../../Gravity/Gravity.h"

class Player
{
public:
    Player();
    ~Player();
    void Update();
    void setPostion(glm::vec3 pos);
    void processInput();
    void updateDeltaTime();
    void updateVelocity();

    void setFrontAndRight(glm::vec3 Front, glm::vec3 Right);
    void Bind(Window* window) {
        this->window = window;
    }

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Right;

private:
    Window* window;
    Collision* collision;
    Gravity* gravity;

    enum State { Ground, Air };
    State state;
    // 重力
    bool isGravity;
    // 移动
    float FrontVelocity;
    float BackVelocity;
    float RightVelocity;
    float LeftVelocity;
    float acceleration;
    const float SPEED = 5.0f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};

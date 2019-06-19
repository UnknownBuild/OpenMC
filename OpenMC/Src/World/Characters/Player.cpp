#include "Player.h"

Player::Player()
{
    this->gravity = new Gravity();
    this->collision = new Collision();
    this->isGravity = false;
    this->FrontVelocity = 0.0f;
    this->BackVelocity = 0.0f;
    this->RightVelocity = 0.0f;
    this->LeftVelocity = 0.0f;
    this->acceleration = 20.0f;
    this->state = Air;
}

Player::~Player()
{

}

void Player::Update()
{
    updateDeltaTime();
    processInput();
    updateVelocity();

    // 碰撞检测处理
    if (this->collision->checkNegativeX(Position)) {
        Position.x = ceil(Position.x);
    }
    if (this->collision->checkPositiveX(Position)) {
        Position.x = floor(Position.x);
    }
    if (this->collision->checkNegativeZ(Position)) {
        Position.z = ceil(Position.z);
    }
    if (this->collision->checkPositiveZ(Position)) {
        Position.z = floor(Position.z);
    }
    // 地板检测
    if (this->collision->checkDown(Position) && this->state == Air && this->gravity->getVelocity() < 0) {
        this->state = Ground;
    }

    if (!this->collision->checkDown(Position)) {
        this->state = Air;
    }

    if (this->isGravity && this->state == Air) {
        Position += glm::vec3(0.0f, 1.0f, 0.0f) * this->gravity->UpdateVelocity(deltaTime);
    }
    // 头顶检测
    if (this->collision->checkUp(Position)) {
        Position.y = floor(Position.y);
        this->gravity->setVelocity(0.0f);
    }
}

void Player::updateDeltaTime() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Player::updateVelocity() {
    Position += glm::vec3(Front.x, 0.0f, Front.z) * this->FrontVelocity * deltaTime;
    Position -= glm::vec3(Front.x, 0.0f, Front.z) * this->BackVelocity * deltaTime;
    Position += glm::vec3(Right.x, 0.0f, Right.z) * this->RightVelocity * deltaTime;
    Position -= glm::vec3(Right.x, 0.0f, Right.z) * this->LeftVelocity * deltaTime;

    this->FrontVelocity = this->FrontVelocity - this->acceleration * deltaTime;
    if (this->FrontVelocity < 0.0f) this->FrontVelocity = 0.0f;
    this->BackVelocity = this->BackVelocity - this->acceleration * deltaTime;
    if (this->BackVelocity < 0.0f) this->BackVelocity = 0.0f;
    this->RightVelocity = this->RightVelocity - this->acceleration * deltaTime;
    if (this->RightVelocity < 0.0f) this->RightVelocity = 0.0f;
    this->LeftVelocity = this->LeftVelocity - this->acceleration * deltaTime;
    if (this->LeftVelocity < 0.0f) this->LeftVelocity = 0.0f;
}

void Player::setFrontAndRight(glm::vec3 Front, glm::vec3 Right)
{
    this->Front = Front;
    this->Right = Right;
}

void Player::setPostion(glm::vec3 pos) {
    this->Position = pos;
}

void Player::processInput() {
    if (window->GetKey(GLFW_KEY_W) == GLFW_PRESS) {
        this->FrontVelocity = SPEED;
        this->BackVelocity = 0.0f;
    }
    if (window->GetKey(GLFW_KEY_S) == GLFW_PRESS) {
        this->BackVelocity = SPEED;
        this->FrontVelocity = 0.0f;
    }
    if (window->GetKey(GLFW_KEY_A) == GLFW_PRESS) {
        this->LeftVelocity = SPEED;
        this->RightVelocity = 0.0f;
    }
    if (window->GetKey(GLFW_KEY_D) == GLFW_PRESS) {
        this->RightVelocity = SPEED;
        this->LeftVelocity = 0.0f;
    }
    if (window->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
        this->state = Air;
        this->gravity->setVelocity(8.0f);
        this->isGravity = true;
    }
    /*if (window->GetKey(GLFW_KEY_Q) == GLFW_PRESS)
        Position += Up;
    if (window->GetKey(GLFW_KEY_E) == GLFW_PRESS)
        Position -= Up;
    if (window->GetKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        Position -= Up;*/
    
}

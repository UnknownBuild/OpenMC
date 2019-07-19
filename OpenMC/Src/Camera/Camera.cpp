#include"Camera.h"
#define M_PI       3.14159265358979323846

Camera::Camera() {
    this->Position = glm::vec3(0.0f, 3.0f, 10.0f);
    this->Front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    this->Up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->Yaw = YAW;
    this->Pitch = PITCH;
    this->MovementSpeed = SPEED;
    this->MouseSensitivity = SENSITIVITY;
    this->Zoom = ZOOM;
    this->freedomView = false;
    this->gravity = new Gravity();
    this->collision = new Collision();
    this->isGravity = false;
    this->FrontVelocity = 0.0f;
    this->BackVelocity = 0.0f;
    this->RightVelocity = 0.0f;
    this->LeftVelocity = 0.0f;
    this->acceleration = 20.0f;
    this->perspective = First;
    this->state = Air;
    this->isPerspective = false;
}

void Camera::InitView() {
    this->Front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->Up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->Yaw = YAW;
    this->Pitch = PITCH;
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::Update() {
    this->updateDeltaTime();
    this->processInput();
}

void Camera::SetLookPostion(glm::vec3 pos, glm::vec3 look) {
    this->Position = pos;
    this->Front = glm::normalize(look - this->Position);
    this->setAngle();
}

void Camera::SetViewPostion(glm::vec3 pos, glm::vec3 front, glm::vec3 up) {
    this->Position = pos;
    this->Front = glm::normalize(front);
    this->Up = glm::normalize(up);
    this->setAngle();
}

// 设置摄像机角度
void Camera::setAngle() {
    glm::vec3 front = glm::normalize(this->Front);
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
    this->Yaw = atan2(front.z, front.x);
    this->Yaw = ((this->Yaw) / M_PI) * 180.0f;
    this->Pitch = asin(front.y);
    this->Pitch = ((this->Pitch) / M_PI) * 180.0f;
    if (this->Pitch > 89.0f) this->Pitch = 89.0f;
    if (this->Pitch < -89.0f) this->Pitch = -89.0f;
}

void Camera::TransitionTo(glm::vec3 target, float p) {
    if (p < 0.05) {
        this->oldPostion = this->Position;
    } else {
        float current = UtilTool::scaleValue(p);
        this->Position = this->oldPostion - (this->oldPostion - target) * current;
    }
}

void Camera::updateDeltaTime() {
    if (lastFrame == 0) lastFrame = glfwGetTime();
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    if (deltaTime > 0.05) deltaTime = 0.05;
    lastFrame = currentFrame;
}

void Camera::OpenFreeView() {
    this->freedomView = true;
}

void Camera::CloseFreeView() {
    this->freedomView = false;
    this->firstMouse = true;
}

void Camera::processInput() {
    // 自由视角下键盘控制摄像机方向
    // if (this->freedomView) {
    //     if (window->GetKey(GLFW_KEY_W) == GLFW_PRESS) {
    //         this->FrontVelocity = SPEED;
    //         this->BackVelocity = 0.0f;
    //     }
    //     if (window->GetKey(GLFW_KEY_S) == GLFW_PRESS) {
    //         this->BackVelocity = SPEED;
    //         this->FrontVelocity = 0.0f;
    //     }
    //     if (window->GetKey(GLFW_KEY_A) == GLFW_PRESS) {
    //         this->LeftVelocity = SPEED;
    //         this->RightVelocity = 0.0f;
    //     }
    //     if (window->GetKey(GLFW_KEY_D) == GLFW_PRESS){
    //         this->RightVelocity = SPEED;
    //         this->LeftVelocity = 0.0f;
    //     }
    //     if (window->GetKey(GLFW_KEY_Q) == GLFW_PRESS)
    //         Position += Up;
    //     if (window->GetKey(GLFW_KEY_E) == GLFW_PRESS)
    //         Position -= Up;
    //     if (window->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
    //         this->state = Air;
    //         this->gravity->setVelocity(8.0f);
    //         this->isGravity = true;
    //     }
    //     if (window->GetKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    //         Position -= Up;
    // }
    // 切换模式
    if (window->GetKey(GLFW_KEY_1) == GLFW_PRESS) {
        if (keys[KEY_CHANGE] == false) {
            if (this->freedomView) {
                // 释放鼠标
                window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                this->firstMouse = true;
            } else {
                // 捕获鼠标
                window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            this->freedomView = !this->freedomView;
            keys[KEY_CHANGE] = true;
        }
    } else {
        keys[KEY_CHANGE] = false;
    }

    if (window->GetKey(GLFW_KEY_V) == GLFW_PRESS) {
        this->isPerspective = true;
    }
    else {
        if (this->isPerspective) {
            if (this->perspective == First) this->perspective = Third;
            else this->perspective = First;
            this->isPerspective = false;
        }
    }
    // if (window->GetKey(GLFW_KEY_2) == GLFW_PRESS) {
    //     if (keys[KEY_GRAVITY] == false) {
    //         this->isGravity = !this->isGravity;
    //         keys[KEY_GRAVITY] = true;
    //     }
    //     else {
    //         keys[KEY_GRAVITY] = false;
    //     }
    // }
}

void Camera::MouseCallback(double xpos, double ypos) {
    if (!freedomView) return;
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        // printf("first: %lf, %lf\n", xpos, ypos);
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    // printf("move: %lf, %lf\n", xpos, ypos);
    // printf("offset: %f, %f\n", xoffset, yoffset);

    lastX = xpos;
    lastY = ypos;

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::ScrollCallback( double xoffset, double yoffset) {
    /*if (!freedomView) return;
    if (Zoom >= 1.0f && Zoom <= 45.0f)
        Zoom -= yoffset;
    if (Zoom <= 1.0f)
        Zoom = 1.0f;
    if (Zoom >= 45.0f)
        Zoom = 45.0f;*/
}

#pragma once
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "../Helpers/UtilTool.h"
#include "../Systems/Input.h"
#include "../Gravity/Gravity.h"
#include "../Collision/Collision.h"

// 默认参数
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 8.0f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

enum CameraKey { KEY_CHANGE, KEY_GRAVITY };

class Camera {
public:
    Camera();

    void Bind(Window* window) {
        this->window = window;
    }

    template<typename unsigned int ID>
    void Bind(Input<ID>* input) {
        input->OnCursorPosChanged += std::bind(&Camera::MouseCallback, this, std::placeholders::_1, std::placeholders::_2);
        input->OnScrollChanged += std::bind(&Camera::ScrollCallback, this, std::placeholders::_1, std::placeholders::_2);
    }

    void Update();

    // Camera 属性
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Look;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler 角度
    float Yaw;
    float Pitch;
    // 移动灵敏度
    float MovementSpeed;
    // 鼠标灵敏度
    float MouseSensitivity;
    // 滚轮缩放
    float Zoom;

    // 重力
    bool isGravity;

    // 获取视图矩阵
    glm::mat4 GetViewMatrix();
    // 处理鼠标移动
    void MouseCallback(double xpos, double ypos);
    // 处理滚轮滚动
   void ScrollCallback(double xoffset, double yoffset);
    // 设置固定视角
    void SetLookPostion(glm::vec3 pos, glm::vec3 look = glm::vec3(0, 0, 0));
    // 设置自由视角
    void SetViewPostion(glm::vec3 pos, glm::vec3 front, glm::vec3 up = glm::vec3(0, 1, 0));
    // 平滑过渡摄像机位置
    void TransitionTo(glm::vec3 target, float p);

private:
    Window* window;
    Gravity* gravity;
    Collision* collision;

    void setAngle();
    glm::vec3 oldPostion;
    bool keys[100];

    void processInput();
    void updateDeltaTime();

    bool freedomView = false;

    float lastX;
    float lastY;
    bool firstMouse = true;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};

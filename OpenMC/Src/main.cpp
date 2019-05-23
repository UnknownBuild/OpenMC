#include <functional>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Helpers/Singleton.h"
#include "Systems/Window.h"
// test
#include <glm/glm.hpp>
#include "ResourceManager/ResourceManager.h"
#include "SpriteRenderer/SpriteRenderer.h"
#include "Camera/Camera.h"

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = Singleton<Window>::GetInstance()->Create("OpenMC", 800, 600, false);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // V-Sync

    if (!gladLoadGL()) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // test begin
    // 初始化模型
    ResourceManager::LoadModel("Resources/Models/Garden/Garden.obj", "garden");
    ResourceManager::LoadModel("Resources/Models/Table/Table.obj", "table");
    // 初始化纹理
    ResourceManager::LoadTexture("Resources/Textures/awesomeface.png", false, "face");

  
    // 初始化渲染管理器
    SpriteRenderer* Renderer = new SpriteRenderer();
    Renderer->SetWindowSize(800, 600);
    // 初始化摄像头
    Camera* camera = new Camera(window);
    camera->SetLookPostion(glm::vec3(20, 20, 30));
    // test end

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        camera->Update();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, 800, 600);

        // test begin
     
        Renderer->SetLight(glm::vec3(1, 0.8, 0.9), glm::vec3(-3, -10, 0), camera->Position);

        Renderer->SetView(glm::perspective((float)glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f),
          camera->GetViewMatrix());

        Renderer->RenderText("NB ShowShow", glm::vec2(30, 30), 1.0);

        Renderer->DrawSprite(ResourceManager::GetModel("garden"), glm::vec3(0, -4, 0));

        Renderer->DrawSprite(ResourceManager::GetModel("table"), glm::vec3(0, 4, 0));

        Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec3(4, 4, 0));

        // test end

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

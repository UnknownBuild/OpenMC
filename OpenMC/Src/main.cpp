﻿#include <functional>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scenes/SceneManager.h"
#include "Scenes/SceneMenu.h"
#include "Helpers/Config.h"
#include "Helpers/Singleton.h"
#include "Systems/Window.h"
// test
#include <glm/glm.hpp>
#include "ResourceManager/ResourceManager.h"
#include "SpriteRenderer/SpriteRenderer.h"
#include "Camera/Camera.h"
#include "World/Database/BlockManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

int main() {
    Config* config = Singleton<Config>::GetInstance();
    config->Load();

    Window* window = Singleton<Window>::GetInstance();
    window->InitGLFW();
    window->CreateWindow("OpenMC", config->Width, config->Height, config->IsFullScreen);
    window->InitGLAD();
    window->InitImGui();

    SceneManager* sceneManager = Singleton<SceneManager>::GetInstance();
    sceneManager->Goto(new SceneMenu());
    // sceneManager->Goto(NULL);
    sceneManager->Run(window);

    // test begin
    Singleton<BlockManager>::GetInstance()->Load();
    // 初始化模型
    // ResourceManager::LoadModel("Resources/Models/Garden/Garden.obj", "garden");
    // ResourceManager::LoadModel("Resources/Models/Table/Table.obj", "table");
    // 初始化纹理
    ResourceManager::LoadTexture("Resources/Textures/blocks/grass_block_side.png", "grass");
    ResourceManager::LoadTexture("Resources/Textures/blocks/grass_block_top.png", "grass_top");

    ResourceManager::LoadTexture("Resources/Textures/blocks/oak_planks.png", "oak_planks");

    ResourceManager::LoadTexture("Resources/Textures/blocks/crafting_table_side.png", "tabel_side");
    ResourceManager::LoadTexture("Resources/Textures/blocks/crafting_table_top.png", "tabel_top");
    ResourceManager::LoadTexture("Resources/Textures/blocks/crafting_table_front.png", "tabel_front");

    ResourceManager::LoadTexture("Resources/Textures/blocks/furnace_front.png", "furnace_front");
    ResourceManager::LoadTexture("Resources/Textures/blocks/furnace_front_on.png", "furnace_front_on");
    ResourceManager::LoadTexture("Resources/Textures/blocks/furnace_side.png", "furnace_side");
    ResourceManager::LoadTexture("Resources/Textures/blocks/furnace_top.png", "furnace_top");


    ResourceManager::LoadTexture("Resources/Textures/blocks/cracked_stone_bricks.png", "stone");
    ResourceManager::LoadTexture("Resources/Textures/blocks/sand.png", "sand");

    ResourceManager::LoadTexture("Resources/Textures/blocks/torch.png", "torch");
    ResourceManager::LoadTexture("Resources/Textures/blocks/glass.png", "glass");

    ResourceManager::LoadTexture("Resources/Textures/blocks/dandelion.png", "dandelion");

    // 初始化渲染管理器
    SpriteRenderer* Renderer = new SpriteRenderer();
    // 初始化摄像头
    Camera* camera = new Camera(window->GetWindow());
    camera->SetLookPostion(glm::vec3(5, 5, 10));

    glm::vec3 testColor = glm::vec3(0.5, 0.5 ,0.5);

    Texture2D* torchTexture = ResourceManager::LoadTextureSplit("Resources/Textures/blocks/fire_0.png", 32);

    int frame = 0;


    // test end

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 多重缓冲
    glEnable(GL_MULTISAMPLE);
    // Gamma 校正
    // glEnable(GL_FRAMEBUFFER_SRGB);
    while (!window->IsClose()) {
        frame++;
        if (frame > 32) frame = 0;

        glfwPollEvents();
        camera->Update();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, window->GetWidth(), window->GetHeight());
        Renderer->SetWindowSize(window->GetWidth(), window->GetHeight());

        // test begin

        Renderer->SetLight(glm::vec3(-0.2f, -1.0f, -0.3f));

        Renderer->ClearPointLight();

        Renderer->AddPointLight(glm::vec3(5, 0, -3), glm::vec3(0.3), glm::vec3(0.7, 0, 0), glm::vec3(0.3, 0, 0), 100);

        Renderer->AddPointLight(glm::vec3(-3, 0, 5), glm::vec3(0.3), glm::vec3(0.7, 0, 0), glm::vec3(0.3, 0, 0), 100);

        Renderer->SetView(glm::perspective((float)glm::radians(camera->Zoom), window->GetWidth() / (float) window->GetHeight(), 0.1f, 100.0f),
            camera->GetViewMatrix(), camera->Position);

        Renderer->RenderText("NB ShowShow", glm::vec2(30, 30), 1.0);

        // 渲染火把
        glm::vec3 torchPosition[] = {
            glm::vec3(5, 0, -3)
        };
        Renderer->DrawBlock({ ResourceManager::GetTexture("torch") }, {}, 5, torchPosition, 2);

        // 渲染火
        glm::vec3 firePosition[] = {
            glm::vec3(-3, 0, 5)
        };
        Renderer->DrawBlock({ torchTexture[frame] }, {}, 6, firePosition, 1);

        // 渲染草方块
        glm::vec3 grassPosition[1000] = {
            glm::vec3(1, 0, 1),
            glm::vec3(2, 0, 2),
            glm::vec3(2, 0, 1),
            glm::vec3(3, 0, 2),
            glm::vec3(3, 0, 1),
        };
        int grassCount = 5;
        for (int i = -15; i < 15; i++) {
            for (int j = -15; j < 15; j++) {
                grassPosition[grassCount++] = glm::vec3(i, -1, j);
            }
        }
        Renderer->DrawBlock({ ResourceManager::GetTexture("grass") },
            { glm::vec4(0.567, 0.732, 0.366, 1) ,  glm::vec4(0.6, 0.45, 0.37, 1) }, 3, grassPosition, grassCount);

        //// 渲染圆石
        glm::vec3 stonePosition[] = {
            glm::vec3(3, 0, 3),
            glm::vec3(3, 0, 4),
            glm::vec3(3, 1, 4),
            glm::vec3(4, 0, 4),
        };
        Renderer->DrawBlock({}, { glm::vec4(0.8, 0.8, 0.8, 1) }, 2, stonePosition, 4);

        //// 渲染蒲公英
        glm::vec3 dandelionPosition[] = {
            glm::vec3(-1, 0, -1),
            glm::vec3(-2, 0, 0),
        };
        Renderer->DrawBlock({ ResourceManager::GetTexture("dandelion") }, {}, 4, dandelionPosition, 4);


        //// 渲染沙子
        glm::vec3 sandPosition[] = {
            glm::vec3(1, 0, -2),
            glm::vec3(1, 0, -1),
            glm::vec3(1, 1, 0),
            glm::vec3(2, 0, 0),
        };
        Renderer->DrawBlock({ ResourceManager::GetTexture("sand") }, {}, 0, sandPosition, 4);

        // 渲染橡木
        glm::vec3 oakPostions[] = {
            glm::vec3(-3, 0, 3),
            glm::vec3(-3, 0, 4),
            glm::vec3(-3, 1, 4),
            glm::vec3(-4, 0, 4),
            glm::vec3(-4, 0, -3),
            glm::vec3(-4, 1, -3),
            glm::vec3(-4, 2, -3),
            glm::vec3(-3, 0, -3),
            glm::vec3(-3, 1, -3),
            glm::vec3(-3, 2, -3),
            glm::vec3(-5, 0, -3),
            glm::vec3(-5, 1, -3),
            glm::vec3(-5, 2, -3),
            glm::vec3(-4, 0, -2),
            glm::vec3(-4, 2, -2),
            glm::vec3(-3, 0, -2),
            glm::vec3(-3, 1, -2),
            glm::vec3(-3, 2, -2),
            glm::vec3(-5, 0, -2),
            glm::vec3(-5, 1, -2),
            glm::vec3(-5, 2, -2),
            glm::vec3(-4, 0, -1),
            glm::vec3(-4, 2, -1),
            glm::vec3(-3, 0, -1),
            glm::vec3(-3, 1, -1),
            glm::vec3(-3, 2, -1),
            glm::vec3(-5, 0, -1),
            glm::vec3(-5, 1, -1),
            glm::vec3(-5, 2, -1),
        };
        Renderer->DrawBlock({ ResourceManager::GetTexture("oak_planks") }, {},0, oakPostions, 29);

        //// 渲染岩石
        glm::vec3 stonePosition2[] = {
            glm::vec3(3, 0, -3),
            glm::vec3(3, 0, -4),
            glm::vec3(3, 1, -4),
            glm::vec3(4, 0, -4),
        };
        Renderer->DrawBlock({ ResourceManager::GetTexture("stone") }, {}, 0, stonePosition2, 4);

        //// 渲染玻璃
        glm::vec3 glassPosition[] = {
            glm::vec3(7, 0, -3),
            glm::vec3(7, 0, -4),
            glm::vec3(7, 1, -4),
            glm::vec3(8, 0, -4),
        };
        Renderer->DrawBlock({ ResourceManager::GetTexture("glass") }, {}, 0, glassPosition, 4);


        //// 渲染工作台
        glm::vec3 tablePosition[] = {
            glm::vec3(0, 1, 0),
        };
        Renderer->DrawBlock({
            ResourceManager::GetTexture("tabel_front"),
            ResourceManager::GetTexture("tabel_side"),
            ResourceManager::GetTexture("tabel_side"),
            ResourceManager::GetTexture("tabel_side"),
            ResourceManager::GetTexture("tabel_top"),
            ResourceManager::GetTexture("oak_planks"),
            }, {},
            7, tablePosition, 2);

        //// 渲染火炉
        glm::vec3 furnacePosition[] = {
            glm::vec3(3, 2, 3),
        };
        Renderer->DrawBlock({
            ResourceManager::GetTexture("furnace_front_on"),
            ResourceManager::GetTexture("furnace_side"),
            ResourceManager::GetTexture("furnace_side"),
            ResourceManager::GetTexture("furnace_side"),
            ResourceManager::GetTexture("furnace_top"),
            ResourceManager::GetTexture("furnace_top"),
            }, {},
            7, furnacePosition, 2);



        // 渲染2D纹理
        Renderer->DrawTexture(ResourceManager::GetTexture("tabel_top"), glm::vec2(100, 100), 10);

        {
            ImGui::Begin("Application", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SliderFloat("testColorX", &testColor.x, 0, 1);
            ImGui::SliderFloat("testColorY", &testColor.y, 0, 1);
            ImGui::SliderFloat("testColorZ", &testColor.z, 0, 1);
            ImGui::End();
        }

        // test end

        ImGui::Render();

        Renderer->RenderSkyBox();

        int display_w, display_h;
        glfwMakeContextCurrent(window->GetWindow());
        glfwGetFramebufferSize(window->GetWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwMakeContextCurrent(window->GetWindow());
        glfwSwapBuffers(window->GetWindow());

        static bool flag;
        if (!flag) {
            // Singleton<Window>::GetInstance()->Dialog("Hello Zhenly", "A error occurs.");
            flag = true;
        }
    }
    window->DestroyImGui();
    window->DestroyWindow();
}

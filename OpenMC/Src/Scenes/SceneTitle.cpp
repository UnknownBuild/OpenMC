﻿#include <ctime>
#include <imgui.h>
#include <iostream>

#include "../Helpers/EnvPath.h"
#include "SceneGame.h"
#include "SceneLoad.h"
#include "SceneSettings.h"
#include "SceneManager.h"
#include "SceneTitle.h"

bool cursorOnButton;

void SceneTitle::Start() {
    // 初始化输入层
    Input<0> * input = Singleton<Input<0>>::GetInstance();
    input->Clear();
    Input<0>::OnCursorPosChanged += std::bind(&SceneTitle::cursorPosCallback, this, std::placeholders::_1, std::placeholders::_2);
    Input<0>::OnMouseButtonClick += std::bind(&SceneTitle::mouseButtonCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    // 初始化摄像机
    camera = Singleton<Camera>::GetInstance();
    camera->InitView();
    // 初始化渲染器
    renderer = Singleton<SpriteRenderer>::GetInstance();
    renderer->ClearBlock();
    renderer->SetLight(glm::vec3(-0.2f, -1.0f, -0.3f));
    // 不显示框
    renderer->HideShowBlock();
    // 初始化资源
    ResourceManager::LoadTexture(EnvPath::GameTitleImage, "title");

    ResourceManager::LoadModel("Resources/Models/Duck/duck.obj", "duck");
    ResourceManager::LoadModel("Resources/Models/JJMonster2/jj2.obj", "jjm2");

    time_t t = time(NULL);
    initBlocks();
    std::cout << time(NULL) - t << std::endl;
}

void SceneTitle::initBlocks() {
    srand(static_cast<unsigned int>(time(0)));
    vector<glm::vec3> grassPosition;
    for (int i = -30; i < 30; i++) {
        for (int j = -30; j < 30; j++) {
            grassPosition.push_back(glm::vec3(i, 0, j));
        }
    }
    vector<glm::vec3> sea_lanternPosition = {
        glm::vec3(0, 1, 0),
        glm::vec3(0, 2, 0),
    };
    vector<glm::vec3> oakPosition = {
        glm::vec3(1, 1, 1),
        glm::vec3(1, 2, 1),
        glm::vec3(1, 3, 1),
    };
    vector<glm::vec3> leavesPosition = {
        glm::vec3(1, 4, 1),
        glm::vec3(2, 4, 1),
        glm::vec3(0, 4, 1),
        glm::vec3(1, 4, 0),
        glm::vec3(1, 4, 2),
        glm::vec3(2, 4, 0),
        glm::vec3(2, 4, 2),
        glm::vec3(0, 4, 0),
        glm::vec3(0, 4, 2),
        glm::vec3(1, 5, 1),
        glm::vec3(1, 5, 0),
        glm::vec3(1, 5, 2),
        glm::vec3(2, 5, 0),
    };
    vector<glm::vec3> dandelionPosition;
    for (int i = 0; i < 50; i++) {
        int x = rand() % 41 - 20;
        int z = rand() % 41 - 20;
        dandelionPosition.push_back(glm::vec3(x, 1, z));
    }
    vector<glm::vec3> mushroomPosition;
    for (int i = 0; i < 50; i++) {
        int x = rand() % 41 - 20;
        int z = rand() % 41 - 20;
        mushroomPosition.push_back(glm::vec3(x, 1, z));
    }
    vector<glm::vec3> blue_orchidPosition;
    for (int i = 0; i < 50; i++) {
        int x = rand() % 41 - 20;
        int z = rand() % 41 - 20;
        blue_orchidPosition.push_back(glm::vec3(x, 1, z));
    }

    renderer->DrawBlock(BlockId::GrassBlock, grassPosition, 0);
    renderer->DrawBlock(BlockId::Dandelion, dandelionPosition, 0);
    renderer->DrawBlock(BlockId::BlueOrchid, blue_orchidPosition, 0);
    renderer->DrawBlock(BlockId::BrownMushroom, mushroomPosition, 0);
    renderer->DrawBlock(BlockId::OakLog, oakPosition, 0);
    renderer->DrawBlock(BlockId::OakLeaves, leavesPosition, 0);
    this->loadGame = 0;


    this->sceneExiting = false;
    renderer->aoThread = std::thread([&]() {
        while (!sceneExiting) {
            renderer->UpdateLight();
            for (int i = 0; i < 10; i++) {
                Sleep(100);
                if (sceneExiting) break;
            }
            if (sceneExiting) break;
        }
        });
}

void SceneTitle::Terminate() {
    this->sceneExiting = true;
    renderer->aoThread.join();
}

void SceneTitle::cursorPosCallback(double xpos, double ypos) {
    auto size = window->GetWindowSize();
    if (xpos >= 50 && xpos <= 240 && size.Height - ypos >= 225 && size.Height - ypos <= 275) {
        menuItem = MenuStart;
    } else if (xpos >= 50 && xpos <= 200 && size.Height - ypos >= 175 && size.Height - ypos <= 225) {
        menuItem = MenuLoad;
    } else if (xpos >= 50 && xpos <= 330 && size.Height - ypos >= 125 && size.Height - ypos <= 175) {
        menuItem = MenuSettings;
    } else if (xpos >= 50 && xpos <= 185 && size.Height - ypos >= 75 && size.Height - ypos <= 125) {
        menuItem = MenuExit;
    } else {
        menuItem = Null;
    }
}

void SceneTitle::mouseButtonCallback(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        SceneManager* sceneManager = Singleton<SceneManager>::GetInstance();
        switch (menuItem) {
        case MenuStart:
            this->loadGame = 1;
            break;
        case MenuLoad:
            this->loadGame = 5;
            break;
        case MenuSettings:
            this->loadGame = 3;
            break;
        case MenuExit:
            sceneManager->Goto(nullptr);
            break;
        }
    }
}

void SceneTitle::Update() {
    auto size = window->GetWindowSize();
    if (size.Height == 0) {
        return;
    }
    // 加载动画
    if (this->loadGame == 1) {
        this->loadGame = 2;
        return;
    }
    else if (this->loadGame == 2) {
        SceneManager* sceneManager = Singleton<SceneManager>::GetInstance();
        sceneManager->Goto(new SceneGame());
        return;
    }
    else if (this->loadGame == 3) {
        this->loadGame = 4;
        return;
    }
    else if (this->loadGame == 4) {
        SceneManager* sceneManager = Singleton<SceneManager>::GetInstance();
        sceneManager->Goto(new SceneSettings());
        return;
    }
    else if (this->loadGame == 5) {
        this->loadGame = 6;
        return;
    }
    else if (this->loadGame == 6) {
        SceneManager* sceneManager = Singleton<SceneManager>::GetInstance();
        sceneManager->Goto(new SceneLoad());
        return;
    }

    // 更新摄像机和渲染器
    float camPosX = sin(glfwGetTime() / 4) * 15;
    float camPosZ = cos(glfwGetTime() / 4) * 15;
    camera->SetLookPostion(glm::vec3(camPosX, 5, camPosZ), glm::vec3(1.0f, 2.0f, 1.0f));
    camera->Update();
    renderer->SetWindowSize(size.Width, size.Height);
    renderer->SetView(glm::perspective(( float) glm::radians(camera->Zoom), size.Width / ( float) size.Height, 0.1f, 100.0f),
        camera->GetViewMatrix(), camera->Position, camera->Front);

    // 渲染模型
    renderer->DrawSprite(ResourceManager::GetModel("duck"), glm::vec3(3, 0.5, 3), glm::vec3(2.0), 0, false);
    renderer->DrawSprite(ResourceManager::GetModel("jjm2"), glm::vec3(4, 0.5, 4), glm::vec3(3.4), 3.14, true);

    // 渲染FPS
    renderer->RenderText(std::to_string(static_cast<int>(ImGui::GetIO().Framerate)) + " FPS", glm::vec2(10, size.Height - 20), 0.4);

    // 渲染标题
    renderer->DrawTexture(ResourceManager::GetTexture("title"), glm::vec2(size.Width / 2, size.Height - 75), 2.0f);
    // 渲染菜单
    renderer->RenderText("Start", glm::vec2(50, 250), 1.0f, menuItem == MenuStart ? YELLOW : WHITE);
    renderer->RenderText("Load", glm::vec2(50, 200), 1.0f, menuItem == MenuLoad ? YELLOW : WHITE);
    renderer->RenderText("Settings", glm::vec2(50, 150), 1.0f, menuItem == MenuSettings ? YELLOW : WHITE);
    renderer->RenderText("Exit", glm::vec2(50, 100), 1.0f, menuItem == MenuExit ? YELLOW : WHITE);
    // 渲染天空盒
    renderer->RenderSkyBox();
    // 渲染方块
    renderer->RenderBlock(false);
}

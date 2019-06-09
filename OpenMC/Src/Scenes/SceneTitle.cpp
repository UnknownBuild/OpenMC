#include <ctime>
#include <imgui.h>

#include "../Helpers/EnvPath.h"
#include "SceneGame.h"
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
    // 初始化渲染器
    renderer = Singleton<SpriteRenderer>::GetInstance();
    renderer->SetLight(glm::vec3(-0.2f, -1.0f, -0.3f));
    // 初始化资源
    ResourceManager::LoadTexture(EnvPath::GameTitleImage, "title");
    initBlocks();
}

void SceneTitle::initBlocks() {
    srand(static_cast<unsigned int>(time(0)));
    vector<glm::vec3> grassPosition;
    for (int i = -30; i < 30; i++) {
        for (int j = -30; j < 30; j++) {
            grassPosition.push_back(glm::vec3(i, -1, j));
        }
    }
    vector<glm::vec3> sea_lanternPosition = {
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0),
    };
    vector<glm::vec3> oakPosition = {
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 2, 0),
    };
    vector<glm::vec3> leavesPosition = {
        glm::vec3(0, 3, 0),
        glm::vec3(1, 3, 0),
        glm::vec3(-1, 3, 0),
        glm::vec3(0, 3, -1),
        glm::vec3(0, 3, 1),
        glm::vec3(1, 3, -1),
        glm::vec3(1, 3, 1),
        glm::vec3(-1, 3, -1),
        glm::vec3(-1, 3, 1),
        glm::vec3(0, 4, 0),
        glm::vec3(0, 4, -1),
        glm::vec3(0, 4, 1),
        glm::vec3(1, 4, -1),
    };
    vector<glm::vec3> dandelionPosition;
    for (int i = 0; i < 50; i++) {
        int x = rand() % 41 - 20;
        int z = rand() % 41 - 20;
        dandelionPosition.push_back(glm::vec3(x, 0, z));
    }
    vector<glm::vec3> mushroomPosition;
    for (int i = 0; i < 50; i++) {
        int x = rand() % 41 - 20;
        int z = rand() % 41 - 20;
        mushroomPosition.push_back(glm::vec3(x, 0, z));
    }
    vector<glm::vec3> blue_orchidPosition;
    for (int i = 0; i < 50; i++) {
        int x = rand() % 41 - 20;
        int z = rand() % 41 - 20;
        blue_orchidPosition.push_back(glm::vec3(x, 0, z));
    }

    renderer->DrawBlock(BlockId::GrassBlock, grassPosition, 0);
    renderer->DrawBlock(BlockId::Dandelion, dandelionPosition, 0);
    renderer->DrawBlock(BlockId::BlueOrchid, blue_orchidPosition, 0);
    renderer->DrawBlock(BlockId::BrownMushroom, mushroomPosition, 0);
    renderer->DrawBlock(BlockId::OakLog, oakPosition, 0);
    renderer->DrawBlock(BlockId::OakLeaves, leavesPosition, 0);
}

void SceneTitle::cursorPosCallback(double xpos, double ypos) {
    auto size = window->GetWindowSize();
    if (xpos >= 50 && xpos <= 240 && size.Height - ypos >= 250 && size.Height - ypos <= 300) {
        menuItem = MenuStart;
    } else if (xpos >= 50 && xpos <= 200 && size.Height - ypos >= 200 && size.Height - ypos <= 250) {
        menuItem = MenuLoad;
    } else if (xpos >= 50 && xpos <= 330 && size.Height - ypos >= 150 && size.Height - ypos <= 200) {
        menuItem = MenuSettings;
    } else if (xpos >= 50 && xpos <= 185 && size.Height - ypos >= 100 && size.Height - ypos <= 150) {
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
            sceneManager->Goto(new SceneGame());
            break;
        case MenuLoad:
            break;
        case MenuSettings:
            sceneManager->Goto(new SceneSettings());
            break;
        case MenuExit:
            sceneManager->Goto(nullptr);
            break;
        }
    }
}

void SceneTitle::Update() {
    auto size = window->GetWindowSize();

    // 更新摄像机和渲染器
    float camPosX = sin(glfwGetTime() / 4) * 15;
    float camPosZ = cos(glfwGetTime() / 4) * 15;
    camera->SetLookPostion(glm::vec3(camPosX, 5, camPosZ), glm::vec3(0.0f, 2.0f, 0.0f));
    camera->Update();
    renderer->SetWindowSize(size.Width, size.Height);
    renderer->SetView(glm::perspective(( float) glm::radians(camera->Zoom), size.Width / ( float) size.Height, 0.1f, 100.0f),
        camera->GetViewMatrix(), camera->Position, camera->Front);

    // 渲染FPS
    renderer->RenderText(std::to_string(static_cast<int>(ImGui::GetIO().Framerate)) + " FPS", glm::vec2(10, size.Height - 20), 0.4);

    // 渲染标题
    renderer->DrawTexture(ResourceManager::GetTexture("title"), glm::vec2(size.Width / 2 - 200, size.Height - 150), 2.0f);
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

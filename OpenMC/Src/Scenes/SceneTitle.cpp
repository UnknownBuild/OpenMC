#include <ctime>
#include <imgui.h>

#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneTitle.h"

bool cursorOnButton;

void SceneTitle::Start() {
    window = Singleton<Window>::GetInstance();
    camera = Singleton<Camera>::GetInstance();
    renderer = Singleton<SpriteRenderer>::GetInstance();

    // 初始化
    Input<0>* input = Singleton<Input<0>>::GetInstance();
    input->Clear();
    Input<0>::OnCursorPosChanged += cursorPosCallback;
    Input<0>::OnMouseButtonClick += mouseButtonCallback;

    renderer->SetLight(glm::vec3(-0.2f, -1.0f, -0.3f));
    renderer->UpdateLight();
    currentTime = glfwGetTime();
    cursorOnButton = false;

    srand((int)time(0));
    setUpGraphics();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}

void SceneTitle::setUpGraphics(){
    // 草方块
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
    for (int i = 0; i < 50; i ++) {
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

    graphics = new Graphics();

    // add Sprite
    graphics->AddBlocks(BlockId::GrassBlock, grassPosition, 0, true);
    graphics->AddBlocks(BlockId::Dandelion, dandelionPosition, 0, true);
    graphics->AddBlocks(BlockId::BlueOrchid, blue_orchidPosition, 0, true);
    graphics->AddBlocks(BlockId::BrownMushroom, mushroomPosition, 0, true);
    graphics->AddBlocks(BlockId::OakLog, oakPosition, 0, true);
    graphics->AddBlocks(BlockId::OakLeaves, leavesPosition, 0, true);
    graphics->Add2DSprite("Resources/Textures/gui/minecraft_title.png", "minecraft_title", 320, 600, 2);

    //graphics->Update();
}

void SceneTitle::cursorPosCallback(double xpos, double ypos) {
    if (xpos >= 390 && xpos <= 600 && ypos >= 625 && ypos <= 677) {
        cursorOnButton = true;
    }
    else {
        cursorOnButton = false;
    }
}

void SceneTitle::mouseButtonCallback(int button, int action, int mods) {
    if (cursorOnButton && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "start game" << std::endl;
        SceneManager* sceneManager = Singleton<SceneManager>::GetInstance();
        sceneManager->Goto(new SceneGame());
    }
}

void SceneTitle::Update() {
    auto size = window->GetWindowSize();

    // render test
    float camPosX = sin(glfwGetTime() / 4) * 15;
    float camPosZ = cos(glfwGetTime() / 4) * 15;
    camera->SetLookPostion(glm::vec3(camPosX, 5, camPosZ), glm::vec3(0.0f, 2.0f, 0.0f));

    camera->Update();

    renderer->SetWindowSize(size.Width, size.Height);

    renderer->SetView(glm::perspective((float)glm::radians(camera->Zoom), size.Width / (float)size.Height, 0.1f, 100.0f),
        camera->GetViewMatrix(), camera->Position, camera->Front);
    // 渲染文字
    renderer->RenderText(to_string(ImGui::GetIO().Framerate).substr(0, 5) + " FPS", glm::vec2(10, 10), 0.4);
    if (cursorOnButton) {
        renderer->RenderText("Start", glm::vec2(390, 95), 1.2);
    }
    else {
        renderer->RenderText("Start", glm::vec2(400, 100), 1);
    }

    // 渲染天空盒
    graphics->Update();
    renderer->RenderSkyBox();
    renderer->RenderBlock(false);
}

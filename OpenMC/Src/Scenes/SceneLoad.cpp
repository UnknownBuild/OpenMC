#include <imgui.h>

#include "SceneLoad.h"
#include "SceneManager.h"
#include "SceneTitle.h"

void SceneLoad::Start() {
    // 初始化输入层
    Input<0> * input = Singleton<Input<0>>::GetInstance();
    input->Clear();
    Input<0>::OnCursorPosChanged += std::bind(&SceneLoad::cursorPosCallback, this, std::placeholders::_1, std::placeholders::_2);
    Input<0>::OnMouseButtonClick += std::bind(&SceneLoad::mouseButtonCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    Input<0>::OnKeyClick += std::bind(&SceneLoad::keyCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    player = Singleton<Player>::GetInstance();
    player->setPostion(glm::vec3(10, 26, 10));
    // 初始化摄像机
    camera = Singleton<Camera>::GetInstance();
    camera->Bind(input);
    camera->isGravity = false;
    camera->SetLookPostion(player->Position, glm::vec3(0, 10, 3));
    camera->OpenFreeView();
    window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 初始化摄像机
    //camera = Singleton<Camera>::GetInstance();
    //camera->Bind(input);
    //camera->isGravity = true;
    //camera->SetLookPostion(glm::vec3(10, 26, 10), glm::vec3(0.0));
    // 初始化渲染器
    renderer = Singleton<SpriteRenderer>::GetInstance();
    renderer->ClearBlock();
    // 初始化世界
    world = Singleton<World>::GetInstance();
    if (!world->Init("test")) {
        window->Dialog("World Error", "Failed to initializate world.");
    }
    world->Draw(renderer);
    camera->InitFrame();


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

void SceneLoad::Update() {
    auto size = window->GetWindowSize();
    if (size.Height == 0) {
        return;
    }

    camera->Update();
    player->setFrontAndRight(camera->Front, camera->Right);
    player->Update();
    // 渲染模型
    if (camera->perspective == Camera::Perspective::Third) {
        renderer->DrawSprite(ResourceManager::GetModel("jjm2"), player->Position + glm::vec3(0, -1.5f, 0), glm::vec3(3.4), -glm::radians(camera->Yaw) - glm::radians(90.0f), true);
        camera->Position = player->Position - glm::vec3(player->Front.x * 3, player->Front.y * 3, player->Front.z * 3);
    }
    else if (camera->perspective == Camera::Perspective::First) {
        camera->Position = player->Position;
    }

    renderer->SetWindowSize(size.Width, size.Height);
    renderer->SetView(glm::perspective(( float) glm::radians(camera->Zoom), size.Width / ( float) size.Height, 0.1f, 256.0f),
        camera->GetViewMatrix(), camera->Position, camera->Front);

    world->SetPosition(camera->Position);
    world->Draw(renderer);

    // 渲染FPS
    renderer->RenderText(std::to_string(static_cast<int>(ImGui::GetIO().Framerate)) + " FPS", glm::vec2(10, size.Height - 20), 0.4);
    std::stringstream ss2;
    ss2 << camera->Position.x << ", " << camera->Position.y << ", " << camera->Position.z;
    renderer->RenderText("position: " + ss2.str(), glm::vec2(size.Width - 400, size.Height - 40), 0.4);

    // 渲染暂停菜单
    if (status == 1) {
        renderer->RenderText("MainMenu", glm::vec2(50, 350), 1.0f, menuItem == MainMenu ? YELLOW : WHITE);
        renderer->RenderText("Help", glm::vec2(50, 250), 1.0f, menuItem == Help ? YELLOW : WHITE);
        renderer->RenderText("Back", glm::vec2(50, 150), 1.0f, menuItem == Back ? YELLOW : WHITE);
    }
    if (status == 2) {
        renderer->RenderText("Move:             W S A D", glm::vec2(50, 650), 0.8f, WHITE);
        renderer->RenderText("Jump:             Space", glm::vec2(50, 550), 0.8f, WHITE);
        renderer->RenderText("Create Block: left mouse button", glm::vec2(50, 450), 0.8f, WHITE);
        renderer->RenderText("Break Block:  right mouse button", glm::vec2(50, 350), 0.8f, WHITE);
        renderer->RenderText("Change Block: mouse wheel", glm::vec2(50, 250), 0.8f, WHITE);
        renderer->RenderText("Back", glm::vec2(50, 150), 1.0f, menuItem == Back ? YELLOW : WHITE);
    }

    // 渲染天空盒
    renderer->RenderSkyBox();
    // 渲染方块
    renderer->RenderBlock(false);

    camera->Update();
}

void SceneLoad::Terminate() {
    sceneExiting = true;
    renderer->aoThread.join();
}

void SceneLoad::cursorPosCallback(double xpos, double ypos) {
    auto size = window->GetWindowSize();
    if (xpos >= 50 && xpos <= 350 && size.Height - ypos >= 325 && size.Height - ypos <= 375) {
        menuItem = MainMenu;
    }
    else if (xpos >= 50 && xpos <= 350 && size.Height - ypos >= 225 && size.Height - ypos <= 275) {
        menuItem = Help;
    }
    else if (xpos >= 50 && xpos <= 200 && size.Height - ypos >= 125 && size.Height - ypos <= 175) {
        menuItem = Back;
    }
    else {
        menuItem = Null;
    }
}

void SceneLoad::mouseButtonCallback(int button, int action, int mods) {
    if (status == 0) {
        
    }
    else if (status == 1) {
        SceneManager* sceneManager = Singleton<SceneManager>::GetInstance();
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            switch (menuItem) {
            case MainMenu:
                sceneManager->Goto(new SceneTitle());
                break;
            case Help:
                status = 2;
                break;
            case Back:
                camera->OpenFreeView();
                window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                status = 0;
                break;
            }
        }

    }
    else if (status == 2) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            if (menuItem == Back) {
                status = 1;
            }
        }
    }
}

void SceneLoad::keyCallback(int key, int scancode, int action, int mods) {
    if (window->GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (keys[KEY_CHANGE] == false) {
            switch (status) {
            case 0:
                status = 1;
                camera->CloseFreeView();
                window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                break;
            case 1:
                status = 0;
                camera->OpenFreeView();
                window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                break;
            case 2:
                status = 1;
                break;
            }
            keys[KEY_CHANGE] = true;
        }
    }
    else {
        keys[KEY_CHANGE] = false;
    }
}

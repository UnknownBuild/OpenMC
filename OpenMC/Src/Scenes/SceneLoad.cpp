#include <imgui.h>

#include "SceneLoad.h"

void SceneLoad::Start() {
    // 初始化输入层
    Input<0> * input = Singleton<Input<0>>::GetInstance();
    input->Clear();
    player = Singleton<Player>::GetInstance();
    player->setPostion(glm::vec3(10, 26, 10));
    // 初始化摄像机
    camera = Singleton<Camera>::GetInstance();
    camera->Bind(input);
    camera->isGravity = false;
    camera->SetLookPostion(player->Position, glm::vec3(0, 10, 3));

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

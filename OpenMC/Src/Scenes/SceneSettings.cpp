#include "SceneSettings.h"
#include <ctime>
#include <imgui.h>
#include "SceneManager.h"
#include "SceneTitle.h"
#include <windows.h>

void SceneSettings::Start() {
    Input<0>* input = Singleton<Input<0>>::GetInstance();
    input->Clear();
    Input<0>::OnCursorPosChanged += std::bind(&SceneSettings::cursorPosCallback, this, std::placeholders::_1, std::placeholders::_2);
    Input<0>::OnMouseButtonClick += std::bind(&SceneSettings::mouseButtonCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    // 初始化摄像机
    camera = Singleton<Camera>::GetInstance();
    // 加载设置
    config = Config();
    if (!config.Load()) {
        cout << "load config fail!" << endl;
    }
    isFullScreen = config.IsFullScreen;
    GLint height = config.Height, width = config.Width;
    sightDistance = config.SightDistance;
    // 初始化窗口大小选项参数
    size_array = vector<glm::vec2>();
    size_array.push_back(glm::vec2(1024, 768));
    size_array.push_back(glm::vec2(1920, 1080));
    size_array.push_back(glm::vec2(9999, 9999));
    for (int i = 0, len = size_array.size(); i < len; i++) {
        if (size_array[i].x == width && size_array[i].y == height) {
            windowSizeIndex = i;
            break;
        }
    }
    // 初始化渲染器
    renderer = Singleton<SpriteRenderer>::GetInstance();
    renderer->ClearBlock();
    renderer->SetLight(glm::vec3(-0.2f, -1.0f, -0.3f));
    // 初始化资源
    ResourceManager::LoadTexture(EnvPath::GameTitleImage, "title");

    // 渲染场景
    time_t t = time(NULL);
    srand(static_cast<unsigned int>(time(0)));
    vector<glm::vec3> grassPosition;
    for (int i = -30; i < 30; i++) {
        for (int j = -30; j < 30; j++) {
            grassPosition.push_back(glm::vec3(i, 0, j));
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

    this->loadScene = 0;

    std::cout << time(NULL) - t << std::endl;

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

void SceneSettings::Update() {
    auto size = window->GetWindowSize();
    if (size.Height == 0) {
        return;
    }

    if (this->loadScene == 1) {
        this->loadScene = 2;
        return;
    }
    else if (this->loadScene == 2) {
        SceneManager* sceneManager = Singleton<SceneManager>::GetInstance();
        sceneManager->Goto(new SceneTitle());
        return;
    }

    // 更新摄像机和渲染器
    float camPosX = sin(glfwGetTime() / 4) * 15;
    float camPosZ = cos(glfwGetTime() / 4) * 15;
    camera->SetLookPostion(glm::vec3(camPosX, 5, camPosZ), glm::vec3(0.0f, 2.0f, 0.0f));
    camera->Update();
    renderer->SetWindowSize(size.Width, size.Height);
    renderer->SetView(glm::perspective((float)glm::radians(camera->Zoom), size.Width / (float)size.Height, 0.1f, 100.0f),
        camera->GetViewMatrix(), camera->Position, camera->Front);

    // 渲染FPS
    renderer->RenderText(std::to_string(static_cast<int>(ImGui::GetIO().Framerate)) + " FPS", glm::vec2(10, size.Height - 20), 0.4);

    // 渲染设置
    GLfloat mid_width = size.Width / 2, mid_height = size.Height / 2;
    // 分辨率
    string size_string = to_string(int(size_array[windowSizeIndex].x)) + "X" + to_string(int(size_array[windowSizeIndex].y));
    renderer->RenderText("resolution:", glm::vec2(mid_width - 100, mid_height + 180), 0.75f, WHITE);
    renderer->RenderText("<<", glm::vec2(mid_width - 200, mid_height + 100), 1.0f, settingItem == Index_Decrease ? YELLOW : WHITE);
    renderer->RenderText(size_string, glm::vec2(mid_width - 80, mid_height + 100), 0.75f, WHITE);
    renderer->RenderText(">>", glm::vec2(mid_width + 200, mid_height + 100), 1.0f, settingItem == Index_Increase ? YELLOW : WHITE);
    // 视距
    renderer->RenderText("sight distance:", glm::vec2(mid_width - 150, mid_height), 0.75f, WHITE);
    renderer->RenderText("<<", glm::vec2(mid_width - 200, mid_height-50), 1.0f, settingItem == Sight_Distance_Decrease ? YELLOW : WHITE);
    renderer->RenderText(to_string(sightDistance), glm::vec2(mid_width, mid_height-50), 0.75f, WHITE);
    renderer->RenderText(">>", glm::vec2(mid_width + 200, mid_height-50), 1.0f, settingItem == Sight_Distance_Increase ? YELLOW : WHITE);
    // 全屏
    string fullscreen_string = isFullScreen ? "Yes" : "No";
    renderer->RenderText("full screen:", glm::vec2(mid_width - 100, mid_height - 120), 0.75f, WHITE);
    renderer->RenderText(fullscreen_string, glm::vec2(mid_width - 10, mid_height - 170), 0.75f, WHITE);
    renderer->RenderText(">>", glm::vec2(mid_width + 200, mid_height - 170), 1.0f, settingItem == FullScreen_Change ? YELLOW : WHITE);
    // 离开按键
    renderer->RenderText("Exit", glm::vec2(50, size.Height-100), 1.0f, settingItem == Setting_Exit ? YELLOW : WHITE);
    // 保存按键
    renderer->RenderText("Save", glm::vec2(mid_width - 50, 100), 1.0f, settingItem == Setting_Save ? YELLOW : WHITE);

    // 渲染天空盒
    renderer->RenderSkyBox();
    // 渲染方块
    renderer->RenderBlock(false);
}

void SceneSettings::cursorPosCallback(double xpos, double ypos) {
    auto size = window->GetWindowSize();
    GLfloat mid_width = size.Width / 2, mid_height = size.Height / 2;
    if (xpos >= mid_width - 200 && xpos <= mid_width - 150 && size.Height - ypos >= mid_height + 100 && size.Height - ypos <= mid_height + 200) {
        settingItem = Index_Decrease;
    }
    else if (xpos >= mid_width + 200 && xpos <= mid_width + 250 && size.Height - ypos >= mid_height + 100 && size.Height - ypos <= mid_height + 200) {
        settingItem = Index_Increase;
    } else if (xpos >= mid_width - 200 && xpos <= mid_width - 150 && size.Height - ypos >= mid_height - 50 && size.Height - ypos <= mid_height + 50) {
        settingItem = Sight_Distance_Decrease;
    }
    else if (xpos >= mid_width + 200 && xpos <= mid_width + 250 && size.Height - ypos >= mid_height - 50 && size.Height - ypos <= mid_height + 50) {
        settingItem = Sight_Distance_Increase;
    }
    else if (xpos >= mid_width + 200 && xpos <= mid_width + 250 && size.Height - ypos >= mid_height - 170 && size.Height - ypos <= mid_height - 70) {
        settingItem = FullScreen_Change;
    }
    else if (xpos >= 50 && xpos <= 150 && size.Height - ypos >= size.Height - 100 && size.Height - ypos <= size.Height) {
        settingItem = Setting_Exit;
    }
    else if (xpos >= mid_width - 50 && xpos <= mid_width + 50 && size.Height - ypos >= 100 && size.Height - ypos <= 200) {
        settingItem = Setting_Save;
    }
    else {
        settingItem = Null;
    }

}

void SceneSettings::mouseButtonCallback(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        SceneManager* sceneManager = Singleton<SceneManager>::GetInstance();
        switch (settingItem) {
        case Index_Decrease:
            if(windowSizeIndex > 0) windowSizeIndex--;
            break;
        case Index_Increase:
            if (windowSizeIndex < size_array.size()-1) windowSizeIndex++;
            break;
        case Sight_Distance_Increase:
            if (sightDistance < Max_Distance) sightDistance++;
            break;
        case Sight_Distance_Decrease:
            if (sightDistance > Min_Distance) sightDistance--;
            break;
        case FullScreen_Change:
            isFullScreen = !isFullScreen;
            break;
        case Setting_Exit:
            this->loadScene = 1;
            break;
        case Setting_Save:
            this->config.Save(isFullScreen, size_array[windowSizeIndex].x, size_array[windowSizeIndex].y, sightDistance);
            window->SetWindowAttribute(size_array[windowSizeIndex].x, size_array[windowSizeIndex].y, isFullScreen);
            break;
        case Null:
            break;
        default:
            break;
        }
    }
}

void SceneSettings::Terminate() {
    this->sceneExiting = true;
    renderer->aoThread.join();
}

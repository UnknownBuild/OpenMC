#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scenes/SceneManager.h"
#include "Scenes/SceneTitle.h"
#include "Helpers/Config.h"
#include "Helpers/Singleton.h"
#include "Systems/Input.h"
#include "Systems/Window.h"
#include "World/Map/MapManager.h"

int main() {
    Singleton<MapManager>::GetInstance()->Init("test", 1);

    // 初始化配置文件
    Config* config = Singleton<Config>::GetInstance();
    config->Load();

    // 初始化游戏窗体
    Window* window = Singleton<Window>::GetInstance();
    window->InitGLFW();
    window->CreateWindow("OpenMC", config->Width, config->Height, config->IsFullScreen);
    window->InitGLAD();
    window->InitImGui();

    // 初始化输入层
    Input<0> * input = Singleton<Input<0>>::GetInstance();
    input->Bind(window);

    // 初始化摄像头
    Camera* camera = Singleton<Camera>::GetInstance();
    camera->Bind(window);

    // 初始化方块管理
    Singleton<BlockManager>::GetInstance()->Load();

    // 初始化场景管理
    SceneManager* sceneManager = Singleton<SceneManager>::GetInstance();
    sceneManager->Goto(new SceneTitle());

    // 初始化OpenGL
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    // 执行场景
    sceneManager->Run(window);

    // 销毁窗体
    window->DestroyImGui();
    window->DestroyWindow();
}

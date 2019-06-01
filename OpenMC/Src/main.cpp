#include <functional>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scenes/SceneManager.h"
#include "Scenes/SceneMenu.h"
#include "Helpers/Config.h"
#include "Helpers/Singleton.h"
#include "Systems/Input.h"
#include "Systems/Window.h"
// test
#include <glm/glm.hpp>
#include "World/Map/MapGenerator.h"
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

    // 初始化摄像头
    Camera* camera = new Camera(window->GetWindow());
    camera->SetLookPostion(glm::vec3(5, 5, 10));

    Input<0>* input = Singleton<Input<0>>::GetInstance();
    Input<0>::OnCursorPosChanged += camera->MouseCallback;
    Input<0>::OnScrollChanged += camera->ScrollCallback;
    input->Bind(window);

    MapGenerator gen(10086);
    Chunk* chunk = gen.GenChunk(0, 0);

    SceneManager* sceneManager = Singleton<SceneManager>::GetInstance();
    // sceneManager->Goto(new SceneMenu());
    sceneManager->Goto(NULL);
    sceneManager->Run(window);

    // test begin
    Singleton<BlockManager>::GetInstance()->Load();


    // 初始化渲染管理器
    SpriteRenderer* Renderer = new SpriteRenderer();

    glm::vec3 testColor = glm::vec3(0.5, 0.5 ,0.5);

    // 草方块
    vector<glm::vec3> grassPosition = {
        glm::vec3(1, 0, 1),
        glm::vec3(2, 0, 2),
        glm::vec3(2, 0, 1),
        glm::vec3(3, 0, 2),
        glm::vec3(3, 0, 1),

        glm::vec3(5, 0, 7),
        glm::vec3(5, 0, 8),
        glm::vec3(5, 0, 9),
        glm::vec3(5, 0, 10),
        glm::vec3(6, 0, 7),

        glm::vec3(6, 0, 10),
        glm::vec3(7, 0, 7),
        glm::vec3(7, 0, 10),
        glm::vec3(8, 0, 7),
        glm::vec3(8, 0, 8),

        glm::vec3(8, 0, 9),
        glm::vec3(8, 0, 10)

    };
    int grassCount = 17;
    for (int i = -40; i < 40; i++) {
        for (int j = -40; j < 40; j++) {
            grassPosition.push_back(glm::vec3(i, -1, j));
        }
    }

    vector<glm::vec3> torchPosition = {
        glm::vec3(5, 0, -3)
    };

    vector<glm::vec3> waterPosition = {
        glm::vec3(6, 0, 8),
        glm::vec3(6, 0, 9),
        glm::vec3(7, 0, 8),
        glm::vec3(7, 0, 9),
    };

    vector<glm::vec3> sea_lanternPosition = {
        glm::vec3(10, 0, 12),
        glm::vec3(10, 1, 12),
    };
    vector<glm::vec3> dandelionPosition = {
        glm::vec3(-1, 0, -1),
        glm::vec3(-2, 0, 0),
    };
    vector<glm::vec3> brown_mushroomPosition = {
        glm::vec3(12, 0, 10),
    };
    vector<glm::vec3> horn_coralPosition = {
        glm::vec3(12, 0, 12),
    };
    vector<glm::vec3> dirtPosition = {
        glm::vec3(12, 0, 16),
        glm::vec3(12, 1, 16),
    };
    vector<glm::vec3> cobble_stonePosition = {
        glm::vec3(12, 0, 18),
        glm::vec3(12, 1, 18),
    };
    vector<glm::vec3> grassTopPosition = {
        glm::vec3(1, 0, 6),
        glm::vec3(1, 0, 7),
        glm::vec3(0, 0, 7),
        glm::vec3(1, 0, 8),
        glm::vec3(2, 0, 8),
    };
    vector<glm::vec3> sandPosition = {
        glm::vec3(1, 0, -2),
        glm::vec3(1, 0, -1),
        glm::vec3(1, 1, 0),
        glm::vec3(2, 0, 0),
    };
    vector<glm::vec3> oakPostions = {
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
    vector<glm::vec3> blue_orchidPosition = {
        glm::vec3(12, 0, 11),
    };
    vector<glm::vec3> stonePosition2 = {
        glm::vec3(3, 0, -3),
        glm::vec3(3, 0, -4),
        glm::vec3(3, 1, -4),
        glm::vec3(4, 0, -4),
    };
    vector<glm::vec3> glassPosition = {
        glm::vec3(7, 0, -3),
        glm::vec3(7, 0, -4),
        glm::vec3(7, 1, -4),
        glm::vec3(8, 0, -4),
    };
    vector<glm::vec3> tablePosition = {
        glm::vec3(0, 1, 0),
    };
    vector<glm::vec3> furnacePosition = {
        glm::vec3(3, 2, 3),
    };
    vector<glm::vec3> oakPosition = {
        glm::vec3(0, 0, 9),
        glm::vec3(0, 1, 9),
        glm::vec3(0, 2, 9),
        glm::vec3(0, 3, 9),
    };
    vector<glm::vec3> cactusPosition = {
        glm::vec3(10, 0, 10),
        glm::vec3(10, 1, 10),
        glm::vec3(10, 2, 10),
        glm::vec3(10, 3, 10),
        glm::vec3(10, 4, 10),
    };
    vector<glm::vec3> leavePosition = {
        glm::vec3(0, 4, 9),
        glm::vec3(1, 4, 9),
        glm::vec3(-1, 4, 9),
        glm::vec3(0, 4, 8),
        glm::vec3(0, 4, 10),
        glm::vec3(1, 4, 8),
        glm::vec3(1, 4, 10),
        glm::vec3(-1, 4, 8),
        glm::vec3(-1, 4, 10),
        glm::vec3(0, 5, 9),
        glm::vec3(0, 5, 8),
        glm::vec3(0, 5, 10),
        glm::vec3(1, 5, 8),
    };
    vector<glm::vec3> doorPosition = {
        glm::vec3(6, 0, 6),
    };
    vector<glm::vec3> oakDoorPosition = {
        glm::vec3(6, 0, 4),
    };
    vector<glm::vec3> colorGlassPosition = {
        glm::vec3(4, 0, 7),
        glm::vec3(4, 0, 8),
    };

    vector<glm::vec3> stonePosition = {
            glm::vec3(3, 0, 3),
            glm::vec3(3, 0, 4),
            glm::vec3(3, 1, 4),
            glm::vec3(4, 0, 4) };

    Renderer->SetLight(glm::vec3(-0.2f, -1.0f, -0.3f));

    Renderer->ClearPointLight();

    Renderer->AddPointLight(glm::vec3(5, 0, -3), glm::vec3(0.3), glm::vec3(0.7, 0, 0), glm::vec3(0.3, 0, 0), 100);

    Renderer->AddPointLight(glm::vec3(-3, 0, 5), glm::vec3(0.3), glm::vec3(0.7, 0, 0), glm::vec3(0.3, 0, 0), 100);

    vector<glm::vec3> firePosition = { glm::vec3(-3, 0, 5) };
    // 渲染火
    Renderer->DrawBlock(BlockId::Fire, firePosition);
    // 渲染草
    Renderer->DrawBlock(BlockId::Grass, grassPosition);
    // 渲染圆石
    Renderer->DrawBlock(BlockId::Stone, stonePosition);

    // 渲染火把
    Renderer->DrawBlock(BlockId::Torch, torchPosition);

    // 渲染草
    Renderer->DrawBlock(BlockId::GrassBlock, grassPosition);

    // 渲染水面
    Renderer->DrawBlock(BlockId::Water, waterPosition);

    // 渲染海晶灯
    Renderer->DrawBlock(BlockId::SeaLantern, sea_lanternPosition);

    // 渲染圆石
    Renderer->DrawBlock(BlockId::Stone, stonePosition);

    // 渲染煤
    Renderer->DrawBlock(BlockId::Dirt, dirtPosition);

    // 渲染鹅卵石
    Renderer->DrawBlock(BlockId::CobbleStone, cobble_stonePosition);

    //// 渲染蒲公英
    Renderer->DrawBlock(BlockId::Dandelion, dandelionPosition);

    //// 渲染蘑菇
    Renderer->DrawBlock(BlockId::BrownMushroom, brown_mushroomPosition);

    //// 渲染 blue flower
    Renderer->DrawBlock(BlockId::BlueOrchid, blue_orchidPosition);

    //// 渲染 horn_coral
    Renderer->DrawBlock(BlockId::HornCoral, horn_coralPosition);

    //// 渲染草
    Renderer->DrawBlock(BlockId::Grass, grassTopPosition);

    //// 渲染沙子
    Renderer->DrawBlock(BlockId::Sand, sandPosition);

    // 渲染橡木
    Renderer->DrawBlock(BlockId::OakPlanks, oakPostions);

    //// 渲染岩石
    Renderer->DrawBlock(BlockId::CrackedStoneBricks, stonePosition2);

    //// 渲染玻璃
    Renderer->DrawBlock(BlockId::Glass, glassPosition);

    //// 渲染工作台
    Renderer->DrawBlock(BlockId::CraftingTable, tablePosition, 1);

    //// 渲染火炉
    Renderer->DrawBlock(BlockId::Furnace, furnacePosition, 2);

    //// 渲染树干
    Renderer->DrawBlock(BlockId::OakLog, oakPosition);

    // 渲染仙人掌
    Renderer->DrawBlock(BlockId::Cactus, cactusPosition);

    //// 渲染树叶
    Renderer->DrawBlock(BlockId::OakLeaves, leavePosition);

    // 渲染门
    Renderer->DrawBlock(BlockId::IronDoor, doorPosition, 2);

    // 渲染门
    Renderer->DrawBlock(BlockId::OakDoor, oakDoorPosition, 3);

    // 渲染玻璃
    Renderer->DrawBlock(BlockId::BlueStainedGlassPane, colorGlassPosition, 0);


    // 更新光照
    Renderer->UpdateLight();
    // test end

    int currentTime = glfwGetTime();

    // 面剔除
    // glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    // 渲染半透明纹理
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 多重缓冲
    glEnable(GL_MULTISAMPLE);
    // Gamma 校正
    // glEnable(GL_FRAMEBUFFER_SRGB);
    while (!window->IsClose()) {

        glfwPollEvents();
        camera->Update();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto size = window->GetWindowSize();
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, size.first, size.second);

        // test begin
        Renderer->SetWindowSize(size.first, size.second);

        Renderer->SetView(glm::perspective((float)glm::radians(camera->Zoom), size.first/(float)size.second, 0.1f, 100.0f),
            camera->GetViewMatrix(), camera->Position);

        // 渲染天空盒
        Renderer->RenderSkyBox();

        // 渲染文字
        Renderer->RenderText(to_string(ImGui::GetIO().Framerate).substr(0, 5) + " FPS", glm::vec2(10, 10), 0.4);

        if (glfwGetTime() - currentTime > 1.0) {
            currentTime = glfwGetTime();
            Renderer->UpdateLight();
        }

        // 渲染方块
        Renderer->RenderBlock(false);

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

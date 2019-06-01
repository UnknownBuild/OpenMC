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

void cursorPosCallbackA(double xOffset, double yOffset) {
    std::cout << "Hello" << std::endl;
}

void cursorPosCallbackB(double xOffset, double yOffset) {
    std::cout << "Hi" << std::endl;
}

void mouseButtonCallbackA(int button, int action, int mods) {
    std::cout << "mouse button A" << std::endl;
}

void scrollCallbackA(double xoffset, double yoffset) {
    std::cout << "scroll A" << std::endl;
}

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
    //Input<0>::OnCursorPosChanged += cursorPosCallbackA;
    //Input<0>::OnCursorPosChanged += cursorPosCallbackB;
    Input<0>::OnCursorPosChanged += camera->MouseCallback;
    //Input<0>::OnMouseButtonClick += mouseButtonCallbackA;
    //Input<0>::OnScrollChanged += scrollCallbackA;
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
    ResourceManager::LoadTexture("Resources/Textures/blocks/grass.png", "grassTop");

    ResourceManager::LoadTexture("Resources/Textures/blocks/oak_leaves.png", "oak_leaves");
    ResourceManager::LoadTexture("Resources/Textures/blocks/oak_log.png", "oak_log");
    ResourceManager::LoadTexture("Resources/Textures/blocks/oak_log_top.png", "oak_log_top");



    ResourceManager::LoadTexture("Resources/Textures/blocks/dandelion.png", "dandelion");

    // brown蘑菇
    ResourceManager::LoadTexture("Resources/Textures/blocks/brown_mushroom.png", "brown_mushroom");
    // blue flower
    ResourceManager::LoadTexture("Resources/Textures/blocks/blue_orchid.png", "blue_orchid");
    // horn_coral
    ResourceManager::LoadTexture("Resources/Textures/blocks/horn_coral.png", "horn_coral");
    ResourceManager::LoadTexture("Resources/Textures/blocks/horn_coral_block.png", "horn_coral_block");
    ResourceManager::LoadTexture("Resources/Textures/blocks/horn_coral_fan.png", "horn_coral_fan");

    ResourceManager::LoadTexture("Resources/Textures/blocks/iron_door_top.png", "iron_door_top");
    ResourceManager::LoadTexture("Resources/Textures/blocks/iron_door_bottom.png", "iron_door_bottom");

    ResourceManager::LoadTexture("Resources/Textures/blocks/oak_door_top.png", "oak_door_top");
    ResourceManager::LoadTexture("Resources/Textures/blocks/oak_door_bottom.png", "oak_door_bottom");

    ResourceManager::LoadTexture("Resources/Textures/blocks/blue_stained_glass.png", "blue_stained_glass");
    ResourceManager::LoadTexture("Resources/Textures/blocks/blue_stained_glass_pane_top.png", "blue_stained_glass_pane_top");

    // 仙人掌
    ResourceManager::LoadTexture("Resources/Textures/blocks/cactus_side.png", "cactus_side");
    ResourceManager::LoadTexture("Resources/Textures/blocks/cactus_top.png", "cactus_top");
    ResourceManager::LoadTexture("Resources/Textures/blocks/cactus_bottom.png", "cactus_bottom");
    // 蛋糕
    ResourceManager::LoadTexture("Resources/Textures/blocks/cake_bottom.png", "cake_bottom");
    ResourceManager::LoadTexture("Resources/Textures/blocks/cake_inner.png", "cake_inner");
    ResourceManager::LoadTexture("Resources/Textures/blocks/cake_side.png", "cake_side");
    ResourceManager::LoadTexture("Resources/Textures/blocks/cake_top.png", "cake_top");



    // 初始化渲染管理器
    SpriteRenderer* Renderer = new SpriteRenderer();

    glm::vec3 testColor = glm::vec3(0.5, 0.5 ,0.5);

    vector<Texture2D> torchTexture = ResourceManager::LoadSplitTexture("Resources/Textures/blocks/fire_0.png", "fire");

    vector<Texture2D> waterTexture = ResourceManager::LoadSplitTexture("Resources/Textures/blocks/water_still.png", "water");
    // 海晶灯
    vector<Texture2D> sea_lanternTexture = ResourceManager::LoadSplitTexture("Resources/Textures/blocks/sea_lantern.png", "sea_lantern");


    int frame = 0;



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

    glm::vec3 torchPosition[] = {
        glm::vec3(5, 0, -3)
    };

    glm::vec3 waterPosition[] = {
        glm::vec3(6, 0, 8),
        glm::vec3(6, 0, 9),
        glm::vec3(7, 0, 8),
        glm::vec3(7, 0, 9),
    };

    glm::vec3 sea_lanternPosition[] = {
        glm::vec3(10, 0, 12),
        glm::vec3(10, 1, 12),
    };
    glm::vec3 dandelionPosition[] = {
        glm::vec3(-1, 0, -1),
        glm::vec3(-2, 0, 0),
    };
    glm::vec3 brown_mushroomPosition[] = {
        glm::vec3(12, 0, 10),
    };
    glm::vec3 horn_coralPosition[] = {
        glm::vec3(12, 0, 12),
    };
    glm::vec3 grassTopPosition[] = {
        glm::vec3(1, 0, 6),
        glm::vec3(1, 0, 7),
        glm::vec3(0, 0, 7),
        glm::vec3(1, 0, 8),
        glm::vec3(2, 0, 8),
    };
    glm::vec3 sandPosition[] = {
        glm::vec3(1, 0, -2),
        glm::vec3(1, 0, -1),
        glm::vec3(1, 1, 0),
        glm::vec3(2, 0, 0),
    };
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
    glm::vec3 blue_orchidPosition[] = {
        glm::vec3(12, 0, 11),
    };
    glm::vec3 stonePosition2[] = {
        glm::vec3(3, 0, -3),
        glm::vec3(3, 0, -4),
        glm::vec3(3, 1, -4),
        glm::vec3(4, 0, -4),
    };
    glm::vec3 glassPosition[] = {
        glm::vec3(7, 0, -3),
        glm::vec3(7, 0, -4),
        glm::vec3(7, 1, -4),
        glm::vec3(8, 0, -4),
    };
    glm::vec3 tablePosition[] = {
        glm::vec3(0, 1, 0),
    };
    glm::vec3 furnacePosition[] = {
        glm::vec3(3, 2, 3),
    };
    glm::vec3 oakPosition[] = {
        glm::vec3(0, 0, 9),
        glm::vec3(0, 1, 9),
        glm::vec3(0, 2, 9),
        glm::vec3(0, 3, 9),
    };
    glm::vec3 cactusPosition[] = {
        glm::vec3(10, 0, 10),
        glm::vec3(10, 1, 10),
        glm::vec3(10, 2, 10),
        glm::vec3(10, 3, 10),
        glm::vec3(10, 4, 10),
    };
    glm::vec3 leavePosition[] = {
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
    glm::vec3 doorPosition[] = {
        glm::vec3(6, 0, 6),
    };
    glm::vec3 oakDoorPosition[] = {
        glm::vec3(6, 0, 4),
    };
    glm::vec3 colorGlassPosition[] = {
        glm::vec3(4, 0, 7),
        glm::vec3(4, 0, 8),
    };

    Renderer->SetLight(glm::vec3(-0.2f, -1.0f, -0.3f));

    Renderer->ClearPointLight();

    Renderer->AddPointLight(glm::vec3(5, 0, -3), glm::vec3(0.3), glm::vec3(0.7, 0, 0), glm::vec3(0.3, 0, 0), 100);

    Renderer->AddPointLight(glm::vec3(-3, 0, 5), glm::vec3(0.3), glm::vec3(0.7, 0, 0), glm::vec3(0.3, 0, 0), 100);

    vector<glm::vec3> firePosition = { glm::vec3(-3, 0, 5) };
    // 渲染火
    Renderer->DrawBlock(BlockId::Fire, firePosition);
    // 渲染草
    Renderer->DrawBlock(BlockId::Grass, grassPosition);

    vector<glm::vec3>stonePosition = {
            glm::vec3(3, 0, 3),
            glm::vec3(3, 0, 4),
            glm::vec3(3, 1, 4),
            glm::vec3(4, 0, 4) };
    // 渲染圆石
    Renderer->DrawBlock(BlockId::Stone, stonePosition);
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
        frame++;
        if (frame > 31 * 5) frame = 0;

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

        // 渲染火把
        Renderer->DrawBlock({ ResourceManager::GetTexture("torch") }, { glm::vec4(1.0, 0.5, 0.2, 1.0) }, RenderType::TorchTexture, torchPosition, 2);

        // 渲染水面
        Renderer->DrawBlock( waterTexture, {glm::vec4(0.26, 0.38 ,0.45, 0.35)}, RenderType::LiquidTexture, waterPosition, 4, 0, frame / 5);

        // 渲染海晶灯
        Renderer->DrawBlock(sea_lanternTexture, { }, RenderType::OneTexture, sea_lanternPosition, 2, 0, (frame / 5) % 5);

        // 渲染蒲公英
        Renderer->DrawBlock({ ResourceManager::GetTexture("dandelion") }, { glm::vec4(1) }, RenderType::CenterCrossTexture, dandelionPosition, 2);

        //// 渲染蘑菇
        Renderer->DrawBlock({ ResourceManager::GetTexture("brown_mushroom") }, { glm::vec4(1) }, RenderType::CenterCrossTexture, brown_mushroomPosition, 1);

        //// 渲染 blue flower
        Renderer->DrawBlock({ ResourceManager::GetTexture("blue_orchid") }, { glm::vec4(1) }, RenderType::CenterCrossTexture, blue_orchidPosition, 1);

        //// 渲染 horn_coral
        Renderer->DrawBlock({ ResourceManager::GetTexture("horn_coral") }, { glm::vec4(1) }, RenderType::CenterCrossTexture, horn_coralPosition, 1);

        //// 渲染草
        Renderer->DrawBlock({ ResourceManager::GetTexture("grassTop") }, {glm::vec4(0.62, 0.92, 0.4, 1)}, RenderType::CenterCrossTexture, grassTopPosition, 3);

        //// 渲染沙子
        Renderer->DrawBlock({ ResourceManager::GetTexture("sand") }, {}, RenderType::OneTexture, sandPosition, 4);

        // 渲染橡木
        Renderer->DrawBlock({ ResourceManager::GetTexture("oak_planks") }, {}, RenderType::OneTexture, oakPostions, 29);

        //// 渲染岩石
        Renderer->DrawBlock({ ResourceManager::GetTexture("stone") }, {}, RenderType::OneTexture, stonePosition2, 4);


        //// 渲染玻璃
        Renderer->DrawBlock({ ResourceManager::GetTexture("glass") }, {}, RenderType::OneTexture, glassPosition, 4);


        //// 渲染工作台
        Renderer->DrawBlock({
            ResourceManager::GetTexture("tabel_front"),
            ResourceManager::GetTexture("tabel_side"),
            ResourceManager::GetTexture("tabel_side"),
            ResourceManager::GetTexture("tabel_side"),
            ResourceManager::GetTexture("tabel_top"),
            ResourceManager::GetTexture("oak_planks"),
            }, {},
            RenderType::CustomTexture, tablePosition, 2);

        //// 渲染火炉
        Renderer->DrawBlock({
            ResourceManager::GetTexture("furnace_front_on"),
            ResourceManager::GetTexture("furnace_side"),
            ResourceManager::GetTexture("furnace_top"),
            }, {},
            RenderType::DirCustomTexture, furnacePosition, 2, 2);


        //// 渲染树干
        Renderer->DrawBlock({
            ResourceManager::GetTexture("oak_log"),
            ResourceManager::GetTexture("oak_log"),
            ResourceManager::GetTexture("oak_log_top"),
            }, {},
            RenderType::ThreeTexture, oakPosition, 4);

        // 渲染仙人掌
        Renderer->DrawBlock({
            ResourceManager::GetTexture("cactus_top"),
            ResourceManager::GetTexture("cactus_side"),
            ResourceManager::GetTexture("cactus_bottom"),
            }, {
                glm::vec4(0.0, 0.0625, 0.0, 1.0)
            },
            RenderType::OffsetTexture, cactusPosition, 5);


        //// 渲染树叶
        Renderer->DrawBlock({ ResourceManager::GetTexture("oak_leaves") }, { glm::vec4(0.5, 0.67, 0.43, 1) }, RenderType::OneTexture, leavePosition, 13);


        // 渲染门
        Renderer->DrawBlock({
            ResourceManager::GetTexture("iron_door_top"),
            ResourceManager::GetTexture("iron_door_bottom")
            }, { glm::vec4(0.675, 0.691, 0.675, 1.0) },
            RenderType::DoorTexture, doorPosition, 2, 1);

        // 渲染门
        Renderer->DrawBlock({
            ResourceManager::GetTexture("oak_door_top"),
            ResourceManager::GetTexture("oak_door_bottom")
            }, { glm::vec4(0.53, 0.38, 0.18, 1.0) },
            RenderType::DoorTexture, oakDoorPosition, 2, 0);

        // 渲染玻璃
        Renderer->DrawBlock({
            ResourceManager::GetTexture("blue_stained_glass")
            }, { glm::vec4(0.2, 0.5,0.7, 0.35) },
            RenderType::GlassTexture, colorGlassPosition, 2, 0);


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

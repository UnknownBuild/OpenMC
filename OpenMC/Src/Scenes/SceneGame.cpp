#include <imgui.h>

#include "SceneGame.h"

void SceneGame::Start() {
    // 初始化输入层
    Input<0> * input = Singleton<Input<0>>::GetInstance();
    input->Clear();
    // 初始化摄像机
    camera = Singleton<Camera>::GetInstance();
    camera->Bind(input);
    // 初始化渲染器
    renderer = Singleton<SpriteRenderer>::GetInstance();
    renderer->ClearBlock();
    // 初始化世界
    world = Singleton<World>::GetInstance();
    if (!world->Init("test")) {
        window->Dialog("World Error", "Failed to initializate world.");
    }

    // test begin
    vector<glm::vec3> grassPosition = {
        glm::vec3(1, 1, 1),
        glm::vec3(2, 1, 2),
        glm::vec3(2, 1, 1),
        glm::vec3(3, 1, 2),
        glm::vec3(3, 1, 1),

        glm::vec3(5, 1, 7),
        glm::vec3(5, 1, 8),
        glm::vec3(5, 1, 9),
        glm::vec3(5, 1, 10),
        glm::vec3(6, 1, 7),

        glm::vec3(6, 1, 10),
        glm::vec3(7, 1, 7),
        glm::vec3(7, 1, 10),
        glm::vec3(8, 1, 7),
        glm::vec3(8, 1, 8),

        glm::vec3(8, 1, 9),
        glm::vec3(8, 1, 10)

    };
    int grassCount = 17;
    for (int i = -40; i < 40; i++) {
        for (int j = -40; j < 40; j++) {
            grassPosition.push_back(glm::vec3(i, 0, j));
        }
    }
    vector<glm::vec3> torchPosition = {
        glm::vec3(5, 1, -3)
    };
    vector<glm::vec3> waterPosition = {
        glm::vec3(6, 1, 8),
        glm::vec3(6, 1, 9),
        glm::vec3(7, 1, 8),
        glm::vec3(7, 1, 9),
    };
    vector<glm::vec3> sea_lanternPosition = {
        glm::vec3(10, 1, 12),
        glm::vec3(10, 2, 12),
    };
    vector<glm::vec3> dandelionPosition = {
        glm::vec3(-1, 1, -1),
        glm::vec3(-2, 1, 0),
    };
    vector<glm::vec3> brown_mushroomPosition = {
        glm::vec3(12, 1, 10),
    };
    vector<glm::vec3> horn_coralPosition = {
        glm::vec3(12, 1, 12),
    };
    vector<glm::vec3> dirtPosition = {
        glm::vec3(12, 1, 16),
        glm::vec3(12, 2, 16),
    };
    vector<glm::vec3> cobble_stonePosition = {
        glm::vec3(12, 1, 18),
        glm::vec3(12, 2, 18),
    };
    vector<glm::vec3> grassTopPosition = {
        glm::vec3(1, 1, 6),
        glm::vec3(1, 1, 7),
        glm::vec3(0, 1, 7),
        glm::vec3(1, 1, 8),
        glm::vec3(2, 1, 8),
    };
    vector<glm::vec3> sandPosition = {
        glm::vec3(1, 1, -2),
        glm::vec3(1, 1, -1),
        glm::vec3(2, 2, 0),
        glm::vec3(1, 1, 0),
        glm::vec3(2, 1, 0),
    };
    vector<glm::vec3> oakPostions = {
        glm::vec3(-3, 1, 2),
        glm::vec3(-3, 1, 3),
        glm::vec3(-3, 2, 3),
        glm::vec3(-4, 1, 3),
        glm::vec3(-4, 1, -4),
        glm::vec3(-4, 2, -4),
        glm::vec3(-4, 3, -4),
        glm::vec3(-3, 1, -4),
        glm::vec3(-3, 2, -4),
        glm::vec3(-3, 3, -4),
        glm::vec3(-5, 1, -4),
        glm::vec3(-5, 2, -4),
        glm::vec3(-5, 3, -4),
        glm::vec3(-4, 1, -3),
        glm::vec3(-4, 3, -3),
        glm::vec3(-3, 1, -3),
        glm::vec3(-3, 2, -3),
        glm::vec3(-3, 3, -3),
        glm::vec3(-5, 1, -3),
        glm::vec3(-5, 2, -3),
        glm::vec3(-5, 3, -3),
        glm::vec3(-4, 1, -2),
        glm::vec3(-4, 3, -2),
        glm::vec3(-3, 1, -2),
        glm::vec3(-3, 2, -2),
        glm::vec3(-3, 3, -2),
        glm::vec3(-5, 1, -2),
        glm::vec3(-5, 2, -2),
        glm::vec3(-5, 3, -2),
    };
    vector<glm::vec3> blue_orchidPosition = {
        glm::vec3(12, 1, 11),
    };
    vector<glm::vec3> stonePosition2 = {
        glm::vec3(3, 1, -3),
        glm::vec3(3, 1, -4),
        glm::vec3(3, 2, -4),
        glm::vec3(4, 1, -4),
    };
    vector<glm::vec3> glassPosition = {
        glm::vec3(7, 1, -3),
        glm::vec3(7, 1, -4),
        glm::vec3(7, 2, -4),
        glm::vec3(8, 1, -4),
    };
    vector<glm::vec3> tablePosition = {
        glm::vec3(1, 2, 0),
    };
    vector<glm::vec3> furnacePosition = {
        glm::vec3(3, 3, 3),
    };
    vector<glm::vec3> oakPosition = {
        glm::vec3(1, 1, 9),
        glm::vec3(1, 2, 9),
        glm::vec3(1, 3, 9),
        glm::vec3(1, 4, 9),
    };
    vector<glm::vec3> cactusPosition = {
        glm::vec3(10, 1, 10),
        glm::vec3(10, 2, 10),
        glm::vec3(10, 3, 10),
        glm::vec3(10, 4, 10),
        glm::vec3(10, 5, 10),
    };
    vector<glm::vec3> leavePosition = {
        glm::vec3(1, 5, 9),
        glm::vec3(2, 5, 9),
        glm::vec3(0, 5, 9),
        glm::vec3(1, 5, 8),
        glm::vec3(1, 5, 10),
        glm::vec3(2, 5, 8),
        glm::vec3(2, 5, 10),
        glm::vec3(0, 5, 8),
        glm::vec3(0, 5, 10),
        glm::vec3(1, 6, 9),
        glm::vec3(1, 6, 8),
        glm::vec3(1, 6, 10),
        glm::vec3(2, 6, 8),
    };
    vector<glm::vec3> doorPosition = {
        glm::vec3(6, 1, 6),
    };
    vector<glm::vec3> oakDoorPosition = {
        glm::vec3(6, 1, 4),
    };
    vector<glm::vec3> colorGlassPosition = {
        glm::vec3(4, 1, 7),
        glm::vec3(4, 1, 8),
    };
    vector<glm::vec3> stonePosition = {
            glm::vec3(3, 1, 3),
            glm::vec3(3, 1, 4),
            glm::vec3(3, 2, 4),
            glm::vec3(4, 1, 4) };

    vector<glm::vec3> beetrootPostion = {
        glm::vec3(13, 1, 13),
    };

    renderer->SetLight(glm::vec3(0.5f, -0.3f, 0.4f));
    renderer->ClearPointLight();
    renderer->AddPointLight(glm::vec3(5, 1, -3), glm::vec3(0.3), glm::vec3(0.7, 0, 0), glm::vec3(0.3, 0, 0), 40);
    renderer->AddPointLight(glm::vec3(-3, 1, 5), glm::vec3(0.3), glm::vec3(0.7, 0, 0), glm::vec3(0.3, 0, 0), 40);

    vector<glm::vec3> firePosition = { glm::vec3(-3, 1, 5) };
    // 渲染火
    renderer->DrawBlock(BlockId::Fire, firePosition);
    // 渲染圆石
    renderer->DrawBlock(BlockId::Stone, stonePosition);
    // 渲染火把
    renderer->DrawBlock(BlockId::Torch, torchPosition);
    // 渲染草
    renderer->DrawBlock(BlockId::GrassBlock, grassPosition);
    // 渲染水面
    renderer->DrawBlock(BlockId::Water, waterPosition);
    // 渲染海晶灯
    renderer->DrawBlock(BlockId::SeaLantern, sea_lanternPosition);
    // 渲染圆石
    renderer->DrawBlock(BlockId::Stone, stonePosition);
    // renderer->DrawBlock(BlockId::Stone, stones);
    // 渲染煤
    renderer->DrawBlock(BlockId::Dirt, dirtPosition);
    // 渲染鹅卵石
    renderer->DrawBlock(BlockId::CobbleStone, cobble_stonePosition);
    // 渲染蒲公英
    renderer->DrawBlock(BlockId::Dandelion, dandelionPosition);
    // 渲染蘑菇
    renderer->DrawBlock(BlockId::BrownMushroom, brown_mushroomPosition);
    // 渲染 blue flower
    renderer->DrawBlock(BlockId::BlueOrchid, blue_orchidPosition);
    // 渲染 horn_coral
    renderer->DrawBlock(BlockId::HornCoral, horn_coralPosition);
    // 渲染草
    renderer->DrawBlock(BlockId::Grass, grassTopPosition);
    // 渲染沙子
    renderer->DrawBlock(BlockId::Sand, sandPosition);
    // 渲染橡木
    renderer->DrawBlock(BlockId::OakPlanks, oakPostions);
    // 渲染岩石
    renderer->DrawBlock(BlockId::CrackedStoneBricks, stonePosition2);
    // 渲染玻璃
    renderer->DrawBlock(BlockId::Glass, glassPosition);
    // 渲染工作台
    renderer->DrawBlock(BlockId::CraftingTable, tablePosition, 1);
    // 渲染火炉
    renderer->DrawBlock(BlockId::Furnace, furnacePosition, 2);
    // 渲染树干
    renderer->DrawBlock(BlockId::OakLog, oakPosition);
    // 渲染仙人掌
    renderer->DrawBlock(BlockId::Cactus, cactusPosition);
    // 渲染树叶
    renderer->DrawBlock(BlockId::OakLeaves, leavePosition);
    // 渲染门
    renderer->DrawBlock(BlockId::IronDoor, doorPosition, 2);
    // 渲染门
    renderer->DrawBlock(BlockId::OakDoor, oakDoorPosition, 3);
    // 渲染玻璃
    renderer->DrawBlock(BlockId::BlueStainedGlassPane, colorGlassPosition, 0);
    // 渲染胡萝卜
    renderer->DrawBlock(BlockId::Beetroot, beetrootPostion, 0);
    // 更新光照
    renderer->UpdateLight();

    // glEnable(GL_FRAMEBUFFER_SRGB);
}

void SceneGame::Update() {
    auto size = window->GetWindowSize();

    camera->Update();
    renderer->SetWindowSize(size.Width, size.Height);
    renderer->SetView(glm::perspective(( float) glm::radians(camera->Zoom), size.Width / ( float) size.Height, 0.1f, 256.0f),
        camera->GetViewMatrix(), camera->Position, camera->Front);

    // 渲染FPS
    renderer->RenderText(std::to_string(static_cast<int>(ImGui::GetIO().Framerate)) + " FPS", glm::vec2(10, size.Height - 20), 0.4);

    // 渲染天空盒
    renderer->RenderSkyBox();
    // 渲染方块
    renderer->RenderBlock(false);

    {
        ImGui::Begin("Application", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SliderFloat("testColorX", &testColor.x, 2, 5);
        ImGui::SliderFloat("testColorY", &testColor.y, 2, 5);
        ImGui::SliderFloat("testColorZ", &testColor.z, 2, 5);
        ImGui::End();
    }
}

void SceneGame::Terminate() {
}

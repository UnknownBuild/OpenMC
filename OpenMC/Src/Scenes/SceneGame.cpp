﻿#include <imgui.h>

#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneTitle.h"

glm::vec3 normal[6] = { glm::vec3(0, 1, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1),
                        glm::vec3(1, 0, 0), glm::vec3(0, 0, -1), glm::vec3(-1, 0, 0) };

void SceneGame::Start() {
    window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // 初始化输入层
    Input<0> * input = Singleton<Input<0>>::GetInstance();
    input->Clear();
    Input<0>::OnCursorPosChanged += std::bind(&SceneGame::cursorPosCallback, this, std::placeholders::_1, std::placeholders::_2);
    Input<0>::OnMouseButtonClick += std::bind(&SceneGame::mouseButtonCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    // 初始化人物
    player = Singleton<Player>::GetInstance();
    player->setPostion(glm::vec3(0, 10, 0));
    // 初始化摄像机
    camera = Singleton<Camera>::GetInstance();
    camera->Bind(input);
    camera->isGravity = false;
    camera->SetLookPostion(player->Position, glm::vec3(0, 10, 3));
    camera->OpenFreeView();
    Input<0>::OnKeyClick += std::bind(&SceneGame::keyCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    Input<0>::OnScrollChanged += std::bind(&SceneGame::ScrollCallback, this, std::placeholders::_1, std::placeholders::_2);
    // 初始化渲染器
    renderer = Singleton<SpriteRenderer>::GetInstance();
    renderer->ClearBlock();
    //
    buildingHelper = new BuildingHelper();
    // 初始化资源
    ResourceManager::LoadTexture(EnvPath::FocusImage, "focus");
    // 初始化模型
    ResourceManager::LoadModel("Resources/Models/JJMonster2/jj2.obj", "jjm2");
    ResourceManager::LoadTexture(EnvPath::GrassBlockImage, "GrassBlock");
    ResourceManager::LoadTexture(EnvPath::SandImage, "Sand");
    ResourceManager::LoadTexture(EnvPath::OakPlanksImage, "OakPlanks");
    ResourceManager::LoadTexture(EnvPath::OakLogImage, "OakLog");
    ResourceManager::LoadTexture(EnvPath::GlassImage, "Glass");
    ResourceManager::LoadTexture(EnvPath::SpruceLeavesImage, "OakLeaves");
    ResourceManager::LoadTexture(EnvPath::CraftingTableImage, "CraftingTable");
    ResourceManager::LoadTexture(EnvPath::LitFurnaceImage, "Furnace");
    ResourceManager::LoadTexture(EnvPath::MelonImage, "Melon");
    ResourceManager::LoadTexture(EnvPath::PumpkinImage, "Pumpkin");
    ResourceManager::LoadTexture(EnvPath::StoneImage, "CrackedStoneBricks");
    ResourceManager::LoadTexture(EnvPath::CobblestoneImage, "CobbleStone");
    ResourceManager::LoadTexture(EnvPath::QuartzImage, "Quartz");
    ResourceManager::LoadTexture(EnvPath::FocusImage, "focus"); 
    // 初始化世界
    world = Singleton<World>::GetInstance();
    if (!world->Init("test")) {
        window->Dialog("World Error", "Failed to initializate world.");
    }
    // world->Draw(renderer);

    // test begin
    vector<glm::vec3> grassPosition = {
        glm::vec3(1, 2, 1),
        glm::vec3(2, 2, 2),
        glm::vec3(2, 2, 1),
        glm::vec3(3, 2, 2),
        glm::vec3(3, 2, 1),

        glm::vec3(5, 2, 7),
        glm::vec3(5, 2, 8),
        glm::vec3(5, 2, 9),
        glm::vec3(5, 2, 10),
        glm::vec3(6, 2, 7),

        glm::vec3(6, 2, 10),
        glm::vec3(7, 2, 7),
        glm::vec3(7, 2, 10),
        glm::vec3(8, 2, 7),
        glm::vec3(8, 2, 8),

        glm::vec3(8, 2, 9),
        glm::vec3(8, 2, 10)

    };
    int grassCount = 17;
    for (int i = -60; i < 60; i++) {
        for (int j = -60; j < 60; j++) {
            grassPosition.push_back(glm::vec3(i, 1, j));
            grassPosition.push_back(glm::vec3(i, 0, j));
        }
    }
    vector<glm::vec3> torchPosition = {
        glm::vec3(5, 2, -3)
    };
    vector<glm::vec3> waterPosition = {
        glm::vec3(6, 2, 8),
        glm::vec3(6, 2, 9),
        glm::vec3(7, 2, 8),
        glm::vec3(7, 2, 9),
    };
    vector<glm::vec3> sea_lanternPosition = {
        glm::vec3(10, 2, 12),
        glm::vec3(10, 3, 12),
    };
    vector<glm::vec3> dandelionPosition = {
        glm::vec3(-1, 2, -1),
        glm::vec3(-2, 2, 0),
    };
    vector<glm::vec3> brown_mushroomPosition = {
        glm::vec3(12, 2, 10),
    };
    vector<glm::vec3> horn_coralPosition = {
        glm::vec3(12, 2, 12),
    };
    vector<glm::vec3> dirtPosition = {
        glm::vec3(12, 2, 16),
        glm::vec3(12, 3, 16),
    };
    vector<glm::vec3> cobble_stonePosition = {
        glm::vec3(12, 2, 18),
        glm::vec3(12, 3, 18),
    };
    vector<glm::vec3> grassTopPosition = {
        glm::vec3(1, 2, 6),
        glm::vec3(1, 2, 7),
        glm::vec3(0, 2, 7),
        glm::vec3(1, 2, 8),
        glm::vec3(2, 2, 8),
    };
    vector<glm::vec3> sandPosition = {
        glm::vec3(1, 2, -2),
        glm::vec3(1, 2, -1),
        glm::vec3(2, 1, 0),
        glm::vec3(1, 2, 0),
        glm::vec3(2, 2, 0),
    };
    vector<glm::vec3> oakPostions = {
        glm::vec3(-3, 2, 2),
        glm::vec3(-3, 2, 3),
        glm::vec3(-3, 3, 3),
        glm::vec3(-4, 2, 3),
        glm::vec3(-4, 2, -4),
        glm::vec3(-4, 3, -4),
        glm::vec3(-4, 4, -4),
        glm::vec3(-3, 2, -4),
        glm::vec3(-3, 3, -4),
        glm::vec3(-3, 4, -4),
        glm::vec3(-5, 2, -4),
        glm::vec3(-5, 3, -4),
        glm::vec3(-5, 4, -4),
        glm::vec3(-4, 2, -3),
        glm::vec3(-4, 4, -3),
        glm::vec3(-3, 2, -3),
        glm::vec3(-3, 3, -3),
        glm::vec3(-3, 4, -3),
        glm::vec3(-5, 2, -3),
        glm::vec3(-5, 3, -3),
        glm::vec3(-5, 4, -3),
        glm::vec3(-4, 2, -2),
        glm::vec3(-4, 4, -2),
        glm::vec3(-3, 2, -2),
        glm::vec3(-3, 3, -2),
        glm::vec3(-3, 4, -2),
        glm::vec3(-5, 2, -2),
        glm::vec3(-5, 3, -2),
        glm::vec3(-5, 4, -2),
    };
    vector<glm::vec3> blue_orchidPosition = {
        glm::vec3(12, 2, 11),
    };
    vector<glm::vec3> stonePosition2 = {
        glm::vec3(3, 2, -3),
        glm::vec3(3, 2, -4),
        glm::vec3(3, 3, -4),
        glm::vec3(4, 2, -4),
    };
    vector<glm::vec3> glassPosition = {
        glm::vec3(7, 2, -3),
        glm::vec3(7, 2, -4),
        glm::vec3(7, 3, -4),
        glm::vec3(8, 2, -4),
    };
    vector<glm::vec3> tablePosition = {
        glm::vec3(1, 3, 0),
    };
    vector<glm::vec3> furnacePosition = {
        glm::vec3(3, 4, 3),
    };
    vector<glm::vec3> oakPosition = {
        glm::vec3(1, 2, 9),
        glm::vec3(1, 3, 9),
        glm::vec3(1, 4, 9),
        glm::vec3(1, 5, 9),
    };
    vector<glm::vec3> cactusPosition = {
        glm::vec3(10, 3, 10),
        glm::vec3(10, 4, 10),
        glm::vec3(10, 5, 10),
        glm::vec3(10, 6, 10),
        glm::vec3(10, 7, 10),
    };
    vector<glm::vec3> leavePosition = {
        glm::vec3(1, 6, 9),
        glm::vec3(2, 6, 9),
        glm::vec3(0, 6, 9),
        glm::vec3(1, 6, 8),
        glm::vec3(1, 6, 10),
        glm::vec3(2, 6, 8),
        glm::vec3(2, 6, 10),
        glm::vec3(0, 6, 8),
        glm::vec3(0, 6, 10),
        glm::vec3(1, 7, 9),
        glm::vec3(1, 7, 8),
        glm::vec3(1, 7, 10),
        glm::vec3(2, 7, 8),
    };
    vector<glm::vec3> doorPosition = {
        glm::vec3(6, 2, 6),
    };
    vector<glm::vec3> oakDoorPosition = {
        glm::vec3(6, 2, 4),
    };
    vector<glm::vec3> colorGlassPosition = {
        glm::vec3(4, 2, 7),
        glm::vec3(4, 2, 8),
    };
    vector<glm::vec3> stonePosition = {
            glm::vec3(3, 2, 3),
            glm::vec3(3, 2, 4),
            glm::vec3(3, 3, 4),
            glm::vec3(4, 2, 4) };

    vector<glm::vec3> beetrootPostion = {
        glm::vec3(13, 2, 13),
    };

    renderer->SetLight(glm::vec3(0.5f, -0.3f, 0.4f));
    renderer->ClearPointLight();
    renderer->AddPointLight(glm::vec3(5, 2, -3), glm::vec3(0.3), glm::vec3(0.7, 0, 0), glm::vec3(0.3, 0, 0), 40);
    renderer->AddPointLight(glm::vec3(-3, 2, 5), glm::vec3(0.3), glm::vec3(0.7, 0, 0), glm::vec3(0.3, 0, 0), 40);

    vector<glm::vec3> firePosition = { glm::vec3(-3, 2, 5) };
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
    // renderer->DrawBlock(BlockId::Stone, stonePosition); 
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

    renderer->RemoveBlock(glm::vec3(-5, 1, -4));
    renderer->DrawBlock(BlockId::CraftingTable, glm::vec3(-5, 1, -8), 2);

    //renderer->UpdateLight();

    renderer->SetShowBlock(glm::vec3(1, 2, 0));

    // glEnable(GL_FRAMEBUFFER_SRGB);

    lookingAt = camera->Front;
    position = glm::vec3((int)camera->Position.x, (int)camera->Position.y, (int)camera->Position.z);
    blockType.push_back(BlockId::GrassBlock);
    blockType.push_back(BlockId::CobbleStone);
    blockType.push_back(BlockId::OakPlanks);
    blockType.push_back(BlockId::OakLeaves);
    blockType.push_back(BlockId::CrackedStoneBricks);
    blockType.push_back(BlockId::OakLog);
    blockType.push_back(BlockId::Sand);
    blockType.push_back(BlockId::CraftingTable);
    blockType.push_back(BlockId::Furnace);
    blockType.push_back(BlockId::Pumpkin);
    blockType.push_back(BlockId::Melon);
    blockType.push_back(BlockId::Glass);
    blockType.push_back(BlockId::Quartz);
    status = 0;
    current_index = 0;
    newBlockDirection = 0;
    newBlockPosition = glm::vec3(0, 0, 0);

    for (int i = 1; i <= 4; i++) {
        buildingHelper->buildTree(glm::vec3(35, 2, -35 + i * 4), i);
    }
    buildingHelper->buildTree(glm::vec3(30, 2, -35), 10);
    buildingHelper->buildHouse(glm::vec3(20, 2, -20));

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

void SceneGame::Update() {
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
    else if(camera->perspective == Camera::Perspective::First){
        camera->Position = player->Position;
    }

    renderer->SetWindowSize(size.Width, size.Height);
    renderer->SetView(glm::perspective(( float) glm::radians(camera->Zoom), size.Width / ( float) size.Height, 0.1f, 256.0f),
        camera->GetViewMatrix(), camera->Position, camera->Front);

    position = glm::vec3((int)camera->Position.x, (int)camera->Position.y, (int)camera->Position.z);
    lookingAt = caculateLookingAt();
    // newBlockPosition = getNewBlockPosition();
    updateNewBlockPosition();
    if (renderer->GetBlock(lookingAt).Id != BlockId::Air) {
        
        renderer->SetShowBlock(lookingAt, newBlockDirection);
    }
    else {
        renderer->HideShowBlock();
    }

    // 渲染FPS
    renderer->RenderText(std::to_string(static_cast<int>(ImGui::GetIO().Framerate)) + " FPS", glm::vec2(10, size.Height - 20), 0.4);

    std::stringstream ss;
    ss << (int)this->lookingAt.x << ", " << (int)this->lookingAt.y << ", " << (int)this->lookingAt.z;
    // 渲染Looking At
    renderer->RenderText("looking: " + ss.str(), glm::vec2(size.Width - 400, size.Height - 20), 0.4);
    std::stringstream ss2;
    ss2 << this->position.x << ", " << this->position.y << ", " << this->position.z;
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
    

    // 渲染准星
    renderer->DrawTexture(ResourceManager::GetTexture("focus"), glm::vec2(size.Width / 2, size.Height / 2), 0.4f);

    // 渲染低配背包
    showBlockPicture();

    // 渲染天空盒
    renderer->RenderSkyBox();
    // 渲染方块
    renderer->RenderBlock(false);

    /*{
        ImGui::Begin("Application", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SliderFloat("testColorX", &testColor.x, 2, 5);
        ImGui::SliderFloat("testColorY", &testColor.y, 2, 5);
        ImGui::SliderFloat("testColorZ", &testColor.z, 2, 5);
        ImGui::End();
    }*/
}

glm::vec3 SceneGame::caculateLookingAt() {
    glm::vec3 result = camera->Position;
    for (int i = 0; i < 10; i++) {
        result.x += camera->Front.x * 1.0;
        result.y += camera->Front.y * 1.0;
        result.z += camera->Front.z * 1.0;
        BlockData block = renderer->GetBlock(glm::vec3(round(result.x), round(result.y), round(result.z)));

        if (block.Id != BlockId::Air) {
            break;
        }
    }

    result.x = round(result.x);
    result.y = round(result.y);
    result.z = round(result.z);
    return result;
}

glm::vec3 SceneGame::getIntPosition(glm::vec3 pos) {
    glm::vec3 result;
    result.x = pos.x > 0 ? floor(pos.x) : floor(pos.x) - 1.0;
    result.y = pos.y > 0 ? floor(pos.x) : floor(pos.y) - 1.0;
    result.z = pos.z > 0 ? floor(pos.z) : floor(pos.z) - 1.0;
    return result;
}

void SceneGame::showBlockPicture() {
    auto size = window->GetWindowSize();
    switch (blockType[current_index])
    {
    case BlockId::GrassBlock:
        renderer->DrawTexture(ResourceManager::GetTexture("GrassBlock"), glm::vec2(size.Width - 120, 120), 0.3f);
        break;
    case BlockId::CobbleStone:
        renderer->DrawTexture(ResourceManager::GetTexture("CobbleStone"), glm::vec2(size.Width - 120, 120), 0.3f);
        break;
    case BlockId::CrackedStoneBricks:
        renderer->DrawTexture(ResourceManager::GetTexture("CrackedStoneBricks"), glm::vec2(size.Width - 120, 120), 0.3f);
        break;
    case BlockId::OakLog:
        renderer->DrawTexture(ResourceManager::GetTexture("OakLog"), glm::vec2(size.Width - 120, 120), 0.3f);
        break;
    case BlockId::Sand:
        renderer->DrawTexture(ResourceManager::GetTexture("Sand"), glm::vec2(size.Width - 120, 120), 0.3f);
        break;
    case BlockId::CraftingTable:
        renderer->DrawTexture(ResourceManager::GetTexture("CraftingTable"), glm::vec2(size.Width - 120, 120), 0.3f);
        break;
    case BlockId::Furnace:
        renderer->DrawTexture(ResourceManager::GetTexture("Furnace"), glm::vec2(size.Width - 120, 120), 0.3f);
        break;
    case BlockId::OakPlanks:
        renderer->DrawTexture(ResourceManager::GetTexture("OakPlanks"), glm::vec2(size.Width - 120, 120), 0.3f);
        break;
    case BlockId::OakLeaves:
        renderer->DrawTexture(ResourceManager::GetTexture("OakLeaves"), glm::vec2(size.Width - 120, 120), 0.3f);
        break;
    case BlockId::Pumpkin:
        renderer->DrawTexture(ResourceManager::GetTexture("Pumpkin"), glm::vec2(size.Width - 120, 120), 0.3f);
        break;
    case BlockId::Melon:
        renderer->DrawTexture(ResourceManager::GetTexture("Melon"), glm::vec2(size.Width - 120, 120), 0.3f);
        break;
    case BlockId::Glass:
        renderer->DrawTexture(ResourceManager::GetTexture("Glass"), glm::vec2(size.Width - 120, 120), 0.3f);
        break;
    case BlockId::Quartz:
        renderer->DrawTexture(ResourceManager::GetTexture("Quartz"), glm::vec2(size.Width - 120, 120), 0.3f);
        break;
    default:
        break;
    }
    renderer->RenderText("block type", glm::vec2(size.Width - 170, 60), 0.3);
}

glm::vec3 SceneGame::getNewBlockPosition() {
    glm::vec3 result = lookingAt;
    glm::vec3 direction = lookingAt - camera->Position;
    result.x = round(result.x);
    result.y = round(result.y);
    result.z = round(result.z);

    glm::vec3 angle = direction * normal[0];
    float maxViewCos = angle.x + angle.y + angle.z;
    int index = 0;
    for (int i = 1; i < 6; i++) {
        angle = direction * normal[i];
        float viewCos = angle.x + angle.y + angle.z;
        if (viewCos > maxViewCos) {
            maxViewCos = viewCos;
            index = i;
        }
    }

    newBlockDirection = index;
    return result - normal[index];
}

void SceneGame::updateNewBlockPosition() {
    glm::vec3 pos = camera->Position;
    glm::vec3 front = camera->Front;
    glm::vec3 temp;
    float border[6] = { lookingAt.y + 0.5, lookingAt.y - 0.5, lookingAt.z + 0.5, lookingAt.x + 0.5, lookingAt.z - 0.5, lookingAt.x - 0.5 };
    int direction = 0;

    float t, x, y, z, b;
    if (front.x != 0) {
        b = front.x < 0 ? border[3] : border[5];
        t = (b - pos.x) / front.x;
        y = pos.y + front.y * t;
        z = pos.z + front.z * t;
        if (y >= border[1] && y <= border[0] && z >= border[4] && z <= border[2]) {
            direction = front.x < 0 ? 3 : 5;
        }
    }
    if (front.y != 0) {
        b = front.y < 0 ? border[0] : border[1];
        t = (b - pos.y) / front.y;
        x = pos.x + front.x * t;
        z = pos.z + front.z * t;
        if (x >= border[5] && x <= border[3] && z >= border[4] && z <= border[2]) {
            direction = front.y < 0 ? 0 : 1;
        }
    }
    if (front.z != 0) {
        b = front.z < 0 ? border[2] : border[4];
        t = (b - pos.z) / front.z;
        x = pos.x + front.x * t;
        y = pos.y + front.y * t;
        if (x >= border[5] && x <= border[3] && y >= border[1] && y <= border[0]) {
            direction = front.z < 0 ? 2 : 4;
        }
    }

    newBlockDirection = direction;
    if (newBlockDirection == 3) {
        newBlockDirection = 5;
    }
    else if (newBlockDirection == 5) {
        newBlockDirection = 3;
    }
    newBlockPosition = lookingAt + normal[direction];
}

void SceneGame::Terminate() {
    this->sceneExiting = true;
    renderer->aoThread.join();
}

void SceneGame::cursorPosCallback(double xpos, double ypos) {
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

void SceneGame::ScrollCallback(double xoffset, double yoffset) {
    if (yoffset < 0) {
        current_index++;
        current_index %= blockType.size();
    }
    else {
        current_index--;
        current_index = current_index < 0 ? current_index + blockType.size() : current_index;
    }
}

void SceneGame::mouseButtonCallback(int button, int action, int mods) {
    if (status == 0) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            if (renderer->GetBlock(lookingAt).Id != BlockId::Air) {
                renderer->DrawBlock(blockType[current_index], newBlockPosition);
            }
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            renderer->RemoveBlock(lookingAt);
        }
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

void SceneGame::keyCallback(int key, int scancode, int action, int mods) {
    /*if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        current_index++;
        current_index %= blockType.size();
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        current_index--;
        current_index = current_index < 0 ? current_index + blockType.size() : current_index;
    }*/
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

#include <imgui.h>

#include "SceneGame.h"

glm::vec3 normal[6] = { glm::vec3(0, 1, 0), glm::vec3(0, -1, 0), glm::vec3(-1, 0, 0),
                        glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, -1) };

void SceneGame::Start() {
    // 初始化输入层
    Input<0> * input = Singleton<Input<0>>::GetInstance();
    input->Clear();
    Input<0>::OnCursorPosChanged += std::bind(&SceneGame::cursorPosCallback, this, std::placeholders::_1, std::placeholders::_2);
    Input<0>::OnMouseButtonClick += std::bind(&SceneGame::mouseButtonCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    Input<0>::OnKeyClick += std::bind(&SceneGame::keyCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    // 初始化摄像机
    camera = Singleton<Camera>::GetInstance();
    camera->Bind(input);
    camera->isGravity = true;
    camera->SetLookPostion(glm::vec3(10, 5, 10), glm::vec3(0.0));
    // 初始化渲染器
    renderer = Singleton<SpriteRenderer>::GetInstance();
    renderer->ClearBlock();
    //
    buildingHelper = new BuildingHelper();
    // 初始化资源
    ResourceManager::LoadTexture(EnvPath::FocusImage, "focus");
    ResourceManager::LoadTexture(EnvPath::GrassBlockImage, "GrassBlock");
    ResourceManager::LoadTexture(EnvPath::SandImage, "Sand");
    ResourceManager::LoadTexture(EnvPath::OakPlanksImage, "OakPlanks");
    ResourceManager::LoadTexture(EnvPath::OakLogImage, "OakLog");
    ResourceManager::LoadTexture(EnvPath::GlassImage, "GlassBlock");
    ResourceManager::LoadTexture(EnvPath::SpruceLeavesImage, "OakLeaves");
    ResourceManager::LoadTexture(EnvPath::CraftingTableImage, "CraftingTable");
    ResourceManager::LoadTexture(EnvPath::LitFurnaceImage, "Furnace");
    ResourceManager::LoadTexture(EnvPath::MelonImage, "Melon");
    ResourceManager::LoadTexture(EnvPath::PumpkinImage, "Pumpkin");
    ResourceManager::LoadTexture(EnvPath::StoneImage, "CrackedStoneBricks");
    ResourceManager::LoadTexture(EnvPath::CobblestoneImage, "CobbleStone");
    ResourceManager::LoadTexture(EnvPath::FocusImage, "focus"); 
    // 初始化世界
    world = Singleton<World>::GetInstance();
    if (!world->Init("test")) {
        window->Dialog("World Error", "Failed to initializate world.");
    }
    //world->Draw(renderer);

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
    renderer->DrawBlock(BlockId::CraftingTable, glm::vec3(-5, 1, -8),2);

    renderer->UpdateLight();

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
    current_index = 0;

    for (int i = 1; i <= 4; i++) {
        buildingHelper->buildTree(glm::vec3(35, 1, -35 + i * 4), i);
    }
    buildingHelper->buildTree(glm::vec3(30, 1, -35), 10);
}

void SceneGame::Update() {
    auto size = window->GetWindowSize();

    camera->Update();
    renderer->SetWindowSize(size.Width, size.Height);
    renderer->SetView(glm::perspective(( float) glm::radians(camera->Zoom), size.Width / ( float) size.Height, 0.1f, 256.0f),
        camera->GetViewMatrix(), camera->Position, camera->Front);

    position = glm::vec3((int)camera->Position.x, (int)camera->Position.y, (int)camera->Position.z);
    lookingAt = caculateLookingAt();
    if (renderer->GetBlock(lookingAt).Id != BlockId::Air) {
        renderer->SetShowBlock(lookingAt);
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
    default:
        break;
    }
    renderer->RenderText("block type", glm::vec2(size.Width - 170, 60), 0.3);
}

glm::vec3 SceneGame::getNewBlockPosition() {
    glm::vec3 result = lookingAt;
    result.x = round(result.x);
    result.y = round(result.y);
    result.z = round(result.z);

    glm::vec3 angle = camera->Front * normal[0];
    float maxViewCos = angle.x + angle.y + angle.z;
    int index = 0;
    for (int i = 1; i < 6; i++) {
        angle = camera->Front * normal[i];
        float viewCos = angle.x + angle.y + angle.z;
        if (viewCos > maxViewCos) {
            maxViewCos = viewCos;
            index = i;
        }
    }
    
    return result - normal[index];
}

void SceneGame::Terminate() {
}

void SceneGame::cursorPosCallback(double xpos, double ypos) {
    
}

void SceneGame::mouseButtonCallback(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (renderer->GetBlock(lookingAt).Id != BlockId::Air) {
            renderer->DrawBlock(blockType[current_index], getNewBlockPosition());
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        renderer->RemoveBlock(lookingAt);
    }
}

void SceneGame::keyCallback(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        current_index++;
        current_index %= blockType.size();
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        current_index--;
        current_index = current_index < 0 ? current_index + blockType.size() : current_index;
    }
}

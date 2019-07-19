#include <imgui.h>

#include "SceneLoad.h"
#include "SceneManager.h"
#include "SceneTitle.h"

glm::vec3 normals[6] = { glm::vec3(0, 1, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1),
                        glm::vec3(1, 0, 0), glm::vec3(0, 0, -1), glm::vec3(-1, 0, 0) };

void SceneLoad::Start() {
    // 初始化输入层
    Input<0> * input = Singleton<Input<0>>::GetInstance();
    input->Clear();
    Input<0>::OnCursorPosChanged += std::bind(&SceneLoad::cursorPosCallback, this, std::placeholders::_1, std::placeholders::_2);
    Input<0>::OnMouseButtonClick += std::bind(&SceneLoad::mouseButtonCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    Input<0>::OnKeyClick += std::bind(&SceneLoad::keyCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    Input<0>::OnScrollChanged += std::bind(&SceneLoad::ScrollCallback, this, std::placeholders::_1, std::placeholders::_2);
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
    world->Draw(renderer);

    // Init data
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

    // calculate camera data
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

    // 渲染准星
    renderer->DrawTexture(ResourceManager::GetTexture("focus"), glm::vec2(size.Width / 2, size.Height / 2), 0.4f);
    // 渲染低配背包
    showBlockPicture();

    // 渲染天空盒
    renderer->RenderSkyBox();
    // 渲染方块
    renderer->RenderBlock(false);

    camera->Update();
}

glm::vec3 SceneLoad::caculateLookingAt() {
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

void SceneLoad::showBlockPicture() {
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

void SceneLoad::updateNewBlockPosition() {
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
    newBlockPosition = lookingAt + normals[direction];
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

void SceneLoad::ScrollCallback(double xoffset, double yoffset) {
    if (yoffset < 0) {
        current_index++;
        current_index %= blockType.size();
    }
    else {
        current_index--;
        current_index = current_index < 0 ? current_index + blockType.size() : current_index;
    }
}

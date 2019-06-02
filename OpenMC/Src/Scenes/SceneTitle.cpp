#include "SceneTitle.h"

void SceneTitle::Start() {
    window = Singleton<Window>::GetInstance();
    camera = Camera::getCameraInst();
    Renderer = Singleton<SpriteRenderer>::GetInstance();
    Renderer->SetLight(glm::vec3(-0.2f, -1.0f, -0.3f));
    Renderer->UpdateLight();
    currentTime = glfwGetTime();

    srand((int)time(0));
    setUpGraphics();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    // 渲染半透明纹理
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 多重缓冲
    glEnable(GL_MULTISAMPLE);
}

void SceneTitle::setUpGraphics(){
    // 草方块
    vector<glm::vec3> grassPosition;
    for (int i = -30; i < 30; i++) {
        for (int j = -30; j < 30; j++) {
            grassPosition.push_back(glm::vec3(i, -1, j));
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
        glm::vec3(0, 3, 0),
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
    for (int i = 0; i < 50; i ++) {
        int x = rand() % 41 - 20;
        int z = rand() % 41 - 20;
        dandelionPosition.push_back(glm::vec3(x, 0, z));
    }
    vector<glm::vec3> mushroomPosition;
    for (int i = 0; i < 50; i++) {
        int x = rand() % 41 - 20;
        int z = rand() % 41 - 20;
        mushroomPosition.push_back(glm::vec3(x, 0, z));
    }
    vector<glm::vec3> blue_orchidPosition;
    for (int i = 0; i < 50; i++) {
        int x = rand() % 41 - 20;
        int z = rand() % 41 - 20;
        blue_orchidPosition.push_back(glm::vec3(x, 0, z));
    }

    graphics = new Graphics();

    // add Sprite
    graphics->AddBlocks(BlockId::GrassBlock, grassPosition, 0, true);
    graphics->AddBlocks(BlockId::Dandelion, dandelionPosition, 0, true);
    graphics->AddBlocks(BlockId::BlueOrchid, blue_orchidPosition, 0, true);
    graphics->AddBlocks(BlockId::BrownMushroom, mushroomPosition, 0, true);
    graphics->AddBlocks(BlockId::OakLog, oakPosition, 0, true);
    graphics->AddBlocks(BlockId::OakLeaves, leavesPosition, 0, true);
    graphics->Add2DSprite("Resources/Textures/gui/minecraft_title.png", "minecraft_title", 320, 600, 2);

    //graphics->Update();
}

void SceneTitle::Update() {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    auto size = window->GetWindowSize();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, size.first, size.second);

    // render test
    float camPosX = sin(glfwGetTime() / 2) * 15;
    float camPosZ = cos(glfwGetTime() / 2) * 15;
    camera->SetLookPostion(glm::vec3(camPosX, 5, camPosZ), glm::vec3(0.0f, 2.0f, 0.0f));

    camera->Update();

    Renderer->SetWindowSize(size.first, size.second);

    Renderer->SetView(glm::perspective((float)glm::radians(camera->Zoom), size.first / (float)size.second, 0.1f, 100.0f),
        camera->GetViewMatrix(), camera->Position);
    // 渲染文字
    Renderer->RenderText(to_string(ImGui::GetIO().Framerate).substr(0, 5) + " FPS", glm::vec2(10, 10), 0.4);

    // 渲染天空盒
    Renderer->RenderSkyBox();

    graphics->Update();

    Renderer->RenderBlock(false);
    // test end

    ImGui::Render();

    int display_w, display_h;
    glfwMakeContextCurrent(Singleton<Window>::GetInstance()->GetWindow());
    glfwGetFramebufferSize(Singleton<Window>::GetInstance()->GetWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwMakeContextCurrent(Singleton<Window>::GetInstance()->GetWindow());
    glfwSwapBuffers(Singleton<Window>::GetInstance()->GetWindow());

    static bool flag;
    if (!flag) {
        //Singleton<Window>::GetInstance()->Dialog("Hello Zhenly", "A error occurs.");
        flag = true;
    }
}

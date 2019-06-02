#include <functional>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>

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
    std::vector<glm::vec3> stones;
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 256; y++) {
            for (int z = 0; z < 16; z++) {
                Block block = chunk->GetBlock(x, y, z);
                switch (block.GetId()) {
                case BlockId::Stone:
                    stones.push_back(glm::vec3(x, y, z));
                }
            }
        }
    }

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

    // 渲染大量圆石
    // Renderer->DrawBlock(BlockId::Stone, stones);

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

    //auto size = window->GetWindowSize();
    //int SCR_WIDTH = size.first;
    //int SCR_HEIGHT = size.second;
    //// Set up G-Buffer
    //// 3 textures:
    //// 1. Positions + depth (RGBA)
    //// 2. Color (RGB)
    //// 3. Normals (RGB)
    // 
    //GLuint gBuffer;
    //glGenFramebuffers(1, &gBuffer);
    //glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    //GLuint gPositionDepth, gNormal, gAlbedo;
    //// - Position + linear depth color buffer
    //glGenTextures(1, &gPositionDepth);
    //glBindTexture(GL_TEXTURE_2D, gPositionDepth);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPositionDepth, 0);
    //// - Normal color buffer
    //glGenTextures(1, &gNormal);
    //glBindTexture(GL_TEXTURE_2D, gNormal);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    //// - Albedo color buffer
    //glGenTextures(1, &gAlbedo);
    //glBindTexture(GL_TEXTURE_2D, gAlbedo);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
    //// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    //GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    //glDrawBuffers(3, attachments);
    //// - Create and attach depth buffer (renderbuffer)
    //GLuint rboDepth;
    //glGenRenderbuffers(1, &rboDepth);
    //glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    //// - Finally check if framebuffer is complete
    //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //    std::cout << "GBuffer Framebuffer not complete!" << std::endl;

    //// Also create framebuffer to hold SSAO processing stage
    //GLuint ssaoFBO, ssaoBlurFBO;
    //glGenFramebuffers(1, &ssaoFBO);  glGenFramebuffers(1, &ssaoBlurFBO);
    //glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    //GLuint ssaoColorBuffer, ssaoColorBufferBlur;
    //// - SSAO color buffer
    //glGenTextures(1, &ssaoColorBuffer);
    //glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
    //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //    std::cout << "SSAO Framebuffer not complete!" << std::endl;
    //// - and blur stage
    //glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    //glGenTextures(1, &ssaoColorBufferBlur);
    //glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
    //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //    std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //// Sample kernel
    //std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    //std::default_random_engine generator;
    //std::vector<glm::vec3> ssaoKernel;
    //for (GLuint i = 0; i < 64; ++i)
    //{
    //    glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
    //    sample = glm::normalize(sample);
    //    sample *= randomFloats(generator);
    //    GLfloat scale = GLfloat(i) / 64.0;

    //    // Scale samples s.t. they're more aligned to center of kernel
    //    scale = 0.1f + scale * scale * (1.0f - 0.1f);
    //    // scale = lerp(0.1f, 1.0f, scale * scale);
    //    sample *= scale;
    //    ssaoKernel.push_back(sample);
    //}
    //Renderer->SsaoShader->Use();
    //for (GLuint i = 0; i < 64; ++i)
    //    Renderer->SsaoShader->SetVector3f(("samples[" + std::to_string(i) + "]").c_str(), ssaoKernel[i]);

    //// Noise texture
    //std::vector<glm::vec3> ssaoNoise;
    //for (GLuint i = 0; i < 16; i++)
    //{
    //    glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
    //    ssaoNoise.push_back(noise);
    //}
    //GLuint noiseTexture; glGenTextures(1, &noiseTexture);
    //glBindTexture(GL_TEXTURE_2D, noiseTexture);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //Renderer->SsaoShader->Use();
    //Renderer->SsaoShader->SetVector2f("noiseScale", glm::vec2(SCR_WIDTH / 4.0f, SCR_HEIGHT / 4.0f));


    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    int currentTime = glfwGetTime();

    // 面剔除
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    // 渲染半透明纹理
    glEnable(GL_BLEND);
    // 使用混合
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

        Renderer->SetView(glm::perspective((float)glm::radians(camera->Zoom), size.first/(float)size.second, 0.1f, 256.0f),
            camera->GetViewMatrix(), camera->Position);

        // 渲染天空盒
        Renderer->RenderSkyBox();

        // 渲染文字
        Renderer->RenderText(to_string(ImGui::GetIO().Framerate).substr(0, 5) + " FPS", glm::vec2(10, 10), 0.4);

        //if (glfwGetTime() - currentTime > 1.0) {
        //    currentTime = glfwGetTime();
        //    Renderer->UpdateLight();
        //}

        // 渲染方块
        Renderer->RenderBlock(false);

        //// 1. Geometry Pass: render scene's geometry/color data into gbuffer
        //glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Renderer->RenderBlock(false, Renderer->GBufferShader);
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);

        ////float* output_image = new float[SCR_WIDTH * SCR_HEIGHT * 4];
        ////glBindTexture(GL_TEXTURE_2D, gNormal);
        ////glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, output_image);
        ////for (int i = 0; i < SCR_WIDTH; i++) {
        ////    for (int j = 0; j < SCR_HEIGHT; j++) {
        ////        int offset = i * SCR_HEIGHT + j;
        ////        offset *= 4;
        ////        printf("%f ", output_image[offset + 0]);
        ////        printf("%f ", output_image[offset + 1]);
        ////        printf("%f ", output_image[offset + 2]);
        ////        printf("%f\n", output_image[offset + 3]);
        ////    }
        ////}

        //// 2. Create SSAO texture
        //glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        //glClear(GL_COLOR_BUFFER_BIT);
        //Renderer->SsaoShader->Use();
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, gPositionDepth);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, gNormal);
        //glActiveTexture(GL_TEXTURE2);
        //glBindTexture(GL_TEXTURE_2D, noiseTexture);
        //Renderer->RenderScreen();
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //// 3. Blur SSAO texture to remove noise
        //glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
        //glClear(GL_COLOR_BUFFER_BIT);
        //Renderer->SsaoBlurShader->Use();
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        //Renderer->RenderScreen();
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //// 4. Lighting Pass: traditional deferred Blinn-Phong lighting now with added screen-space ambient occlusion
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Renderer->SsaoLightShader->Use();
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, gAlbedo);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
        //Renderer->RenderScreen();

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

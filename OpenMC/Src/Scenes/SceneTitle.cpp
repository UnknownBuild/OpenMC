#include "SceneTitle.h"

void SceneTitle::Start() {
    graphics = new Graphics();
    graphics->AddTexture("Resources/Textures/gui/minecraft_title.png", "minecraft_title");
}

void SceneTitle::Update() {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 800, 600);

    // render test

    graphics->Update();
    

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

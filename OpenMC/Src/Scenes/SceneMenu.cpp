#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Helpers/Singleton.h"
#include "Systems/Window.h"
#include "SceneMenu.h"

void SceneMenu::Update() {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 800, 600);

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
        Singleton<Window>::GetInstance()->Dialog("Hello Zhenly", "A error occurs.");
        flag = true;
    }
}

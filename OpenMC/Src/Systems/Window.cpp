#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Windows.h>

#include "Window.h"

void Window::CreateWindow(std::string title, unsigned int width, unsigned int height, bool isFullScreen) {
    if (window != NULL) {
        Dialog("Error", "Cannot create GLFW window twice.");
        return;
    }
    this->isFullScreen = isFullScreen;
    GLFWmonitor* monitor = isFullScreen ? glfwGetPrimaryMonitor() : NULL;
    window = glfwCreateWindow(width, height, title.c_str(), monitor, NULL);
    if (window == NULL) {
        Dialog("Error", "Failed to create GLFW window.");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // V-Sync
}

void Window::DestroyImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Window::DestroyWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
    window = NULL;
}

void Window::Dialog(std::string title, std::string text) {
    MessageBox(NULL, text.c_str(), title.c_str(), MB_OK | MB_ICONEXCLAMATION);
}

void Window::InitGLAD() {
    if (!gladLoadGL()) {
        Dialog("Error", "Failed to initialize GLAD.");
        glfwTerminate();
        exit(-1);
    }
}

void Window::InitGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_SAMPLES, 4);
    // 性能测试
    // 1 - 22
    // 2 - 26 +18%
    // 4 - 32 +45%
    // 8 - 45 +100%
}

void Window::InitImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(NULL);
}

bool Window::IsClose() {
    return glfwWindowShouldClose(window);
}

bool Window::IsFullScreen() {
    return isFullScreen;
}

void Window::SetCursorPosCallback(CursorPosCallback callback) {
    glfwSetCursorPosCallback(window, callback);
}

void Window::SetKeyCallback(KeyCallback callback) {
    glfwSetKeyCallback(window, callback);
}

void Window::SetMouseButtonCallback(MouseButtonCallback callback) {
    glfwSetMouseButtonCallback(window, callback);
}

void Window::SetScrollCallback(ScrollCallback callback) {
    glfwSetScrollCallback(window, callback);
}

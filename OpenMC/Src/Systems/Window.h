#pragma once
#include <string>
#include <GLFW/glfw3.h>

#ifdef _MSC_VER
#undef CreateWindow
#endif

class Window final {
public:
    ~Window() {
        if (window) glfwDestroyWindow(window);
    }

    void CreateWindow(std::string title, unsigned int width, unsigned int height, bool isFullScreen = false);
    void DestroyImGui();
    void DestroyWindow();
    void Dialog(std::string title, std::string text);
    unsigned int GetHeight();
    unsigned int GetWidth();
    void InitGLAD();
    void InitGLFW();
    void InitImGui();
    bool IsClose();
    bool IsFullScreen();

    GLFWwindow* GetWindow() {
        return window;
    }

private:
    GLFWwindow* window = NULL;
    unsigned int width;
    unsigned int height;
    bool isFullScreen;
};

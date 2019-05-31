#pragma once
#include <string>
#include <GLFW/glfw3.h>

#ifdef _MSC_VER
#undef CreateWindow
#endif

class Window final {
public:
    typedef void (*CursorPosCallback) (GLFWwindow*, double, double);
    typedef void (*MouseButtonCallback) (GLFWwindow*, int, int, int);
    typedef void (*ScrollCallback) (GLFWwindow*, double, double);

    void CreateWindow(std::string title, unsigned int width, unsigned int height, bool isFullScreen = false);
    void DestroyImGui();
    void DestroyWindow();
    void Dialog(std::string title, std::string text);
    std::pair<unsigned int, unsigned int> GetWindowSize();
    void InitGLAD();
    void InitGLFW();
    void InitImGui();
    bool IsClose();
    bool IsFullScreen();
    void SetCursorPosCallback(CursorPosCallback callback);
    void SetMouseButtonCallback(MouseButtonCallback callback);
    void SetScrollCallback(ScrollCallback callback);


    GLFWwindow* GetWindow() {
        return window;
    }

private:
    GLFWwindow* window = NULL;
    bool isFullScreen;
};

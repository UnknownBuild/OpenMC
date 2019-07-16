#pragma once
#include <string>
#include <GLFW/glfw3.h>

#ifdef _MSC_VER
#undef CreateWindow
#endif

class Window final {
public:
    typedef void (*CursorPosCallback) (GLFWwindow*, double, double);
    typedef void (*KeyCallback) (GLFWwindow*, int, int, int, int);
    typedef void (*MouseButtonCallback) (GLFWwindow*, int, int, int);
    typedef void (*ScrollCallback) (GLFWwindow*, double, double);

    struct WinSize {
        unsigned int Width;
        unsigned int Height;
        WinSize(int width, int height)
            : Width(static_cast<unsigned int>(width)),
            Height(static_cast<unsigned int>(height)) {}
    };

    void CreateWindow(std::string title, unsigned int width, unsigned int height, bool isFullScreen = false);
    void DestroyImGui();
    void DestroyWindow();
    void Dialog(std::string title, std::string text);

    inline WinSize GetWindowSize() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return WinSize(width, height);
    }

    inline void SetWindowAttribute(int Width, int Height, bool isFullScreen) {
        if (isFullScreen) {
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, Width, Height, GLFW_DONT_CARE);
        }
        else {
            glfwSetWindowMonitor(window, NULL, 100, 100, Width, Height, GLFW_DONT_CARE);
        }
    }

    inline int GetKey(int key) {
        return glfwGetKey(window, key);
    }

    void InitGLAD();
    void InitGLFW();
    void InitImGui();
    bool IsClose();
    bool IsFullScreen();

    inline void SetInputMode(int mode, int value) {
        glfwSetInputMode(window, mode, value);
    }

    void SetCursorPosCallback(CursorPosCallback callback);
    void SetKeyCallback(KeyCallback callback);
    void SetMouseButtonCallback(MouseButtonCallback callback);
    void SetScrollCallback(ScrollCallback callback);


    GLFWwindow* GetWindow() {
        return window;
    }

private:
    GLFWwindow* window = NULL;
    bool isFullScreen;
};

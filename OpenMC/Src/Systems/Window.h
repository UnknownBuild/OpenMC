#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <string>
#include <GLFW/glfw3.h>

class Window final {
public:
    ~Window() {
        if (window) glfwDestroyWindow(window);
    }

    GLFWwindow* Create(std::string title, unsigned int width, unsigned int height, bool isFullScreen = false) {
        this->isFullScreen = isFullScreen;
        GLFWmonitor* monitor = isFullScreen ? glfwGetPrimaryMonitor() : NULL;
        return glfwCreateWindow(width, height, title.c_str(), monitor, NULL);
    }

    GLFWwindow* Get() {
        return window;
    }

    bool IsFullScreen() {
        return isFullScreen;
    }

private:
    GLFWwindow* window = NULL;
    bool isFullScreen;
};

#endif

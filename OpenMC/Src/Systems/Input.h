#pragma once
#include <functional>
#include <vector>

#include "Window.h"

template<class T>
class Callback final {
public:
    Callback& operator +=(T callback) {
        callbacks.push_back(callback);
        return *this;
    }

    std::vector<T>& Get() {
        return callbacks;
    }

private:
    std::vector<T> callbacks;
};

typedef std::function<void(double, double)> CursorPosChangedEvent;
typedef std::function<void(int, int, int)> MouseButtonClickEvent;
typedef std::function<void(double, double)> ScrollChangedEvent;

template<unsigned int ID>
class Input final {
public:
    static void Bind(Window* window) {
        window->SetCursorPosCallback(&Input::cursorPosCallback);
        // TODO
    }

    static Callback<CursorPosChangedEvent> OnCursorPosChanged;

private:
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
        for (auto& c : OnCursorPosChanged.Get()) {
            c(xpos, ypos);
        }
    }
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

template<unsigned int ID>
Callback<CursorPosChangedEvent> Input<ID>::OnCursorPosChanged;

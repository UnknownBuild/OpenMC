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
        window->SetMouseButtonCallback(&Input::mouseButtonCallback);
        window->SetScrollCallback(&Input::scrollCallback);
    }

    static Callback<CursorPosChangedEvent> OnCursorPosChanged;
    static Callback<MouseButtonClickEvent> OnMouseButtonClick;
    static Callback<ScrollChangedEvent> OnScrollChanged;

private:
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
        for (auto& c : OnCursorPosChanged.Get()) {
            c(xpos, ypos);
        }
    }
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        for (auto& c : OnMouseButtonClick.Get()) {
            c(button, action, mods);
        }
    }
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        for (auto& c : OnScrollChanged.Get()) {
            c(xoffset, yoffset);
        }
    }
};

template<unsigned int ID>
Callback<CursorPosChangedEvent> Input<ID>::OnCursorPosChanged;
template<unsigned int ID>
Callback<MouseButtonClickEvent> Input<ID>::OnMouseButtonClick;
template<unsigned int ID>
Callback<ScrollChangedEvent> Input<ID>::OnScrollChanged;

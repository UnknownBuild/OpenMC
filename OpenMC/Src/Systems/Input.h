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
typedef std::function<void(int, int, int, int)> KeyClickEvent;
typedef std::function<void(int, int, int)> MouseButtonClickEvent;
typedef std::function<void(double, double)> ScrollChangedEvent;

template<unsigned int ID>
class Input final {
public:
    static void Bind(Window* window) {
        Input<ID>::window = window;
        window->SetCursorPosCallback(&Input::cursorPosCallback);
        window->SetKeyCallback(&Input::keyCallback);
        window->SetMouseButtonCallback(&Input::mouseButtonCallback);
        window->SetScrollCallback(&Input::scrollCallback);
    }

    static inline bool Press(int key) {
        return window->GetKey(key) == GLFW_PRESS;
    }

    static void Clear() {
        OnCursorPosChanged.Get().clear();
        OnKeyClick.Get().clear();
        OnMouseButtonClick.Get().clear();
        OnScrollChanged.Get().clear();
    }

    static Callback<CursorPosChangedEvent> OnCursorPosChanged;
    static Callback<KeyClickEvent> OnKeyClick;
    static Callback<MouseButtonClickEvent> OnMouseButtonClick;
    static Callback<ScrollChangedEvent> OnScrollChanged;

private:
    static Window* window;

    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
        for (auto& c : OnCursorPosChanged.Get()) {
            c(xpos, ypos);
        }
    }
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        for (auto& c : OnKeyClick.Get()) {
            c(key, scancode, action, mods);
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
Callback<KeyClickEvent> Input<ID>::OnKeyClick;
template<unsigned int ID>
Callback<MouseButtonClickEvent> Input<ID>::OnMouseButtonClick;
template<unsigned int ID>
Callback<ScrollChangedEvent> Input<ID>::OnScrollChanged;

template<unsigned int ID>
Window* Input<ID>::window = nullptr;

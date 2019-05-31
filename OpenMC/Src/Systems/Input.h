#pragma once
#include <vector>
#include "Window.h"

template<class T>
class Callback final {
public:
    T& operator +=(T callback) {
        callbacks.push_back(callback);
        return this;
    }

    std::vector<T>& Get() {
        return callbacks;
    }

private:
    std::vector<T> callbacks;
};

class Input final {
public:
    Window::CursorPosCallback GetCursorPosCallback();
    Window::MouseButtonCallback GetMouseButtonCallback();
    Window::ScrollCallback GetScrollCallback();

    Callback<Window::CursorPosCallback> OnCursorPosChanged;

private:
    double mousePosX, mousePosY;
};

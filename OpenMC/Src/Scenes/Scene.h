#pragma once
#include "../Systems/Window.h"

class Scene {
public:
    void Main(Window* window);

    virtual void Start() {}
    virtual void Update() {}
    virtual void Terminate() {}

    void SetSceneChanging(bool sceneChanging);

protected:
    Window* window;
    bool sceneExiting = false;

private:
    bool sceneChanging = false;
};

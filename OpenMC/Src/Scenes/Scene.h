#pragma once
#include "../Systems/Window.h"

class Scene {
public:
    virtual void Main(Window* window) final {
        Start();
        PostStart();
        while (!window->IsClose() && !sceneChanging) {
            Update();
        }
        PreTerminate();
        Terminate();
    }

    virtual void Start() {}
    virtual void PostStart() {}
    virtual void Update() {}
    virtual void PreTerminate() {}
    virtual void Terminate() {}

    void SetSceneChanging(bool sceneChanging) {
        this->sceneChanging = sceneChanging;
    }

private:
    bool sceneChanging = false;
};

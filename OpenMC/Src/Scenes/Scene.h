#pragma once
#include "../Helpers/Singleton.h"
#include "../Systems/Graphics.h"
#include "../Systems/Window.h"

class Scene {
public:
    void Main(Window* window) {
        Start();
        while (!window->IsClose() && !sceneChanging) {
            Singleton<Graphics>::GetInstance()->Update();
            Update();
        }
        Terminate();
    }

    virtual void Start() {}
    virtual void Update() {}
    virtual void Terminate() {}

    void SetSceneChanging(bool sceneChanging) {
        this->sceneChanging = sceneChanging;
    }

private:
    bool sceneChanging = false;
};

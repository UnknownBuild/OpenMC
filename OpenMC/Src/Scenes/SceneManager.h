#pragma once
#include <vector>

#include "../Systems/Window.h"
#include "Scene.h"

class SceneManager final {
public:
    ~SceneManager();

    void Run(Window* window);
    void Goto(Scene* scene);

private:
    Scene* scene = NULL;
    std::vector<Scene*> destroyScenes;
};

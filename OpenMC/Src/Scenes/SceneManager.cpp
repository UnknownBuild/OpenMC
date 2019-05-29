#include "SceneManager.h"

SceneManager::~SceneManager() {
    if (destroyScenes.size()) {
        for (auto& s : destroyScenes) {
            delete s;
        }
    }
    if (scene) {
        delete scene;
    }
}

void SceneManager::Run(Window* window) {
    while (!window->IsClose() && scene != NULL) {
        if (destroyScenes.size()) {
            for (auto& s : destroyScenes) {
                delete s;
            }
            destroyScenes.clear();
        }
        scene->Main(window);
    }
}

void SceneManager::Goto(Scene* scene) {
    if (this->scene != NULL) {
        destroyScenes.push_back(this->scene);
    }
    if (this->scene) {
        this->scene->SetSceneChanging(true);
    }
    this->scene = scene;
}

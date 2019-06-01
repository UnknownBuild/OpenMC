#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Scene.h"


class SceneTitle final : public Scene {
private:
    Graphics *graphics;

public:
    virtual void Start() override;

    virtual void Update() override;
};

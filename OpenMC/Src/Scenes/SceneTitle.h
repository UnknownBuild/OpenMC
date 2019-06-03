﻿#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Scene.h"
#include "SceneMenu.h"
#include "SceneManager.h"
#include<time.h>


class SceneTitle final : public Scene {
public:
    virtual void Start() override;
    virtual void Update() override;
    void setUpGraphics();
    static void cursorPosCallback(double xpos, double ypos);
    static void mouseButtonCallback(int button, int action, int mods);

    //bool cursorOnButton;

private:
    Graphics *graphics;

    SpriteRenderer* Renderer;
    Camera* camera;
    Window* window;
    int currentTime;
};


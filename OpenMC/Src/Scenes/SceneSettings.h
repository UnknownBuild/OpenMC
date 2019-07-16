#pragma once
#include "Scene.h"
#include "../Camera/Camera.h"
#include "../SpriteRenderer/SpriteRenderer.h"
#include "../Helpers/Config.h"
#include <vector>

class SceneSettings final : public Scene {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void Terminate() override;

    void cursorPosCallback(double xpos, double ypos);
    void mouseButtonCallback(int button, int action, int mods);

private:
    SpriteRenderer* renderer;
    Camera* camera;

    Config config;
    vector<glm::vec2> size_array;

    enum SettingItem { Null=0 ,
        Index_Increase,
        Index_Decrease,
        Sight_Distance_Increase,
        Sight_Distance_Decrease,
        FullScreen_Change,
        Setting_Exit,
        Setting_Save,
    };
    SettingItem settingItem;
    GLint windowSizeIndex;

    GLboolean isFullScreen;
    GLint sightDistance;
    GLint Max_Distance = 10;
    GLint Min_Distance = 5;

    const glm::vec4 YELLOW = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    const glm::vec4 WHITE = glm::vec4(1.0f);

    int loadScene = 0;
};

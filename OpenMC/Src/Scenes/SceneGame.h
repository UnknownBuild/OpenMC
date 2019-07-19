#pragma once
#include "BuildingHelper.h"
#include "../Camera/Camera.h"
#include "../SpriteRenderer/SpriteRenderer.h"
#include "../World/Characters/Player.h"
#include "../World/World.h"
#include "Scene.h"

class SceneGame final : public Scene {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void Terminate() override;

    void cursorPosCallback(double xpos, double ypos);
    void mouseButtonCallback(int button, int action, int mods);
    void keyCallback(int key, int scancode, int action, int mods);
    void ScrollCallback(double xoffset, double yoffset);
    void showBlockPicture();
    glm::vec3 caculateLookingAt();
    glm::vec3 getNewBlockPosition();
    glm::vec3 getIntPosition(glm::vec3 pos);
    void updateNewBlockPosition();

private:
    SpriteRenderer* renderer;
    Camera* camera;
    World* world;
    Player* player;
    BuildingHelper* buildingHelper;

    glm::vec3 lookingAt;
    glm::vec3 position;
    vector<BlockId> blockType;
    int current_index;
    int newBlockDirection;
    glm::vec3 newBlockPosition;
    int status; //0-playing  1-paused   2-help
    bool keys[100];

    enum MenuItem { Null = 0, MainMenu, Help, Back };
    MenuItem menuItem;
    const glm::vec4 YELLOW = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    const glm::vec4 WHITE = glm::vec4(1.0f);

    // test
    glm::vec3 testColor = glm::vec3(2, 2, 2);
};

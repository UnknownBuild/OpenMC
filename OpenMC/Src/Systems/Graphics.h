#pragma once
#include <vector>
#include "../SpriteRenderer/SpriteRenderer.h"
#include "../World/Database/BlockData.h"
#include "../Camera/Camera.h";

class Sprite {
public:
    virtual void Draw() = 0;
};

class Sprite2D : public Sprite {
private:
    std::string path;
    std::string name;
    float x;
    float y;
    float scale;
public:
    void Set(std::string path_, std::string name_, float x_, float y_, float scale_);
    void Draw();
};

class Sprite3D : public Sprite {
private:
    vector<glm::vec3> positions;
    BlockId id;
    int dir;
    bool hasDraw;
    bool isStatic;
public:
    Sprite3D(bool isStatic_);
    void Set(BlockId id_, vector<glm::vec3> positions_, int dir_);
    void Draw();
};

class Graphics final {
public:
    Graphics();
    //void AddTexture(std::string path, std::string name);
    void Add2DSprite(std::string path, std::string name, float x, float y, float scale);
    void AddBlocks(BlockId id, vector<glm::vec3>positions, int dir, bool isStatic);
    void Update();

private:
    std::vector<Sprite*> sprites;
};

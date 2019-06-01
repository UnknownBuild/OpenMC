#pragma once
#include <vector>
#include "../SpriteRenderer/SpriteRenderer.h"

class Sprite {
private:
    int type;
public:
    virtual void Draw() = 0;
};

class Graphics final {
public:
    Graphics();
    void AddTexture(std::string path, std::string name);
    void Update();

private:
    std::vector<Sprite*> sprites;
    SpriteRenderer* Renderer;
};

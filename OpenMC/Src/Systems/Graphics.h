#pragma once
#include <vector>

class Sprite {
public:
    virtual void Draw() = 0;
};

class Graphics final {
public:
    void Update();

private:
    std::vector<Sprite*> sprites;
};

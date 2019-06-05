#pragma once
#include <functional>
#include <string>

#include "SpriteRenderer.h"

class Sprite {
public:
    virtual void Draw(SpriteRenderer* renderer) = 0;
};

class Sprite2D : public Sprite {
public:
    Sprite2D(std::string name, float x, float y, float scale) {
        this->name = name;
        this->x = x;
        this->y = y;
        this->scale = scale;
    }

    Sprite2D(std::string name, float x, float y, float scale, float bound[4]) {
        this->name = name;
        this->x = x;
        this->y = y;
        this->scale = scale;
        for (int i = 0; i < 4; i++) {
            this->bound[i] = bound[i];
        }
    }

    bool IsActiveBound(float x, float y) {
        return x >= bound[0] && x <= bound[1] && y >= bound[2] && y <= bound[3];
    }

    void Draw(SpriteRenderer* renderer) {
        renderer->DrawTexture(ResourceManager::GetTexture(name), glm::vec2(x, y), scale);
    }

private:
    std::string name;
    float x;
    float y;
    float scale;
    float bound[4];
};

class SpriteText : public Sprite {
public:
    SpriteText(std::string text, float x, float y, float scale) {
        this->text = text;
        this->x = x;
        this->y = y;
        this->scale = scale;
    }

    SpriteText(std::string text, float x, float y, float scale, float bound[4]) {
        this->text = text;
        this->x = x;
        this->y = y;
        this->scale = scale;
        for (int i = 0; i < 4; i++) {
            this->bound[i] = bound[i];
        }
    }

    bool IsActiveBound(float x, float y) {
        return x >= bound[0] && x <= bound[1] && y >= bound[2] && y <= bound[3];
    }

    void Draw(SpriteRenderer* renderer) {
        renderer->RenderText(text, glm::vec2(x, y), scale);
    }

private:
    std::string text;
    float x;
    float y;
    float scale;
    float bound[4];
};

#include "Graphics.h"

Graphics::Graphics() {
    
}

void Graphics::Add2DSprite(std::string path, std::string name, float x, float y, float scale) {
    ResourceManager::LoadTexture(path, name);
    Sprite2D *s = new Sprite2D();
    s->Set(path, name, x, y, scale);
    sprites.push_back(s);
}

void Graphics::AddBlocks(BlockId id, vector<glm::vec3> positions, int dir, bool isStatic) {
    Sprite3D *s = new Sprite3D(isStatic);
    s->Set(id, positions, dir);
    sprites.push_back(s);
}

void Graphics::Update() {
    for (auto& s : sprites) {
        s->Draw();
    }
}

void Sprite2D::Set(std::string path_, std::string name_, float x_, float y_, float scale_) {
    path = path_;
    name = name_;
    x = x_;
    y = y_;
    scale = scale_;
}

void Sprite2D::Draw() {
    SpriteRenderer* Renderer = Singleton<SpriteRenderer>::GetInstance();
    Renderer->DrawTexture(ResourceManager::GetTexture(name), glm::vec2(x, y), scale);
}

Sprite3D::Sprite3D(bool isStatic_) {
    isStatic = isStatic_;
    hasDraw = false;
}

void Sprite3D::Set(BlockId id_, vector<glm::vec3> positions_, int dir_) {
    positions = positions_;
    id = id_;
    dir = dir_;
}

void Sprite3D::Draw() {
    if (!hasDraw && isStatic) {
        SpriteRenderer* Renderer = Singleton<SpriteRenderer>::GetInstance();
        Renderer->DrawBlock(id, positions, dir);
        hasDraw = true;
    }
}

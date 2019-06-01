#include "Graphics.h"

Graphics::Graphics() {
    Renderer = new SpriteRenderer();
    Renderer->SetWindowSize(600, 800);
}

void Graphics::AddTexture(std::string path, std::string name) {
    ResourceManager::LoadTexture(path, name);
}

void Graphics::Update() {
    Renderer->DrawTexture(ResourceManager::GetTexture("minecraft_title"), glm::vec2(180, 600), 2);
}

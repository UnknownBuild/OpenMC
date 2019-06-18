#include "../Helpers/Singleton.h"
#include "Map/MapManager.h"
#include "World.h"

bool World::Init(std::string name) {
    return Singleton<MapManager>::GetInstance()->Load(name);
}

bool World::Init(std::string name, uint32_t seed) {
    if (!Singleton<MapManager>::GetInstance()->Init(name, seed)) {
        return false;
    }
    return Singleton<MapManager>::GetInstance()->Load(name);
}


void World::Draw(SpriteRenderer* renderer) {
    std::vector<Chunk*> chunks = Singleton<MapManager>::GetInstance()->GetChunks(0, 0, 4);
}

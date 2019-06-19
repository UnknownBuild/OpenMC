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
    std::vector<Chunk*> chunks = Singleton<MapManager>::GetInstance()->GetActiveChunks(0, 0, 2);
    std::vector<glm::vec3> positions;
    for (auto& chunk : chunks) {
        ChunkXZ xz = chunk->GetXZ();
        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 256; y++) {
                for (int z = 0; z < 16; z++) {
                    Block& block = chunk->GetBlock(x, y, z);
                    if (block.GetId() != BlockId::Air) {
                        positions.push_back(glm::vec3(xz.X + x, y, xz.Z + z));
                    }
                }
            }
        }
    }
    renderer->DrawBlock(BlockId::Stone, positions);
}

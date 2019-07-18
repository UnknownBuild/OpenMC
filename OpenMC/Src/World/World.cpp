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

void World::SetPosition(glm::vec3 position) {
    this->position = position;
}


void World::Draw(SpriteRenderer* renderer) {
    int32_t x = static_cast<int>(position.x);
    int32_t z = static_cast<int>(position.y);
    std::vector<Chunk*> chunks = Singleton<MapManager>::GetInstance()->GetActiveChunks(x, z, 2);
    if (chunks.size() == 0) {
        return;
    }
    std::vector<glm::vec3> stones;
    std::vector<glm::vec3> dirts;
    std::vector<glm::vec3> grassBlocks;
    std::vector<glm::vec3> grasses;
    std::vector<glm::vec3> blueOrchids;
    std::vector<glm::vec3> oakLogs;
    std::vector<glm::vec3> oakLeaves;
    std::cout << chunks.size() << " " << x / 16 << " " << z / 16 << std::endl;
    for (auto& chunk : chunks) {
        ChunkXZ xz = chunk->GetXZ();
        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 256; y++) {
                for (int z = 0; z < 16; z++) {
                    Block& block = chunk->GetBlock(x, y, z);
                    switch (block.GetId()) {
                    case BlockId::Stone:
                        stones.push_back(glm::vec3(xz.X + x, y, xz.Z + z));
                        break;
                    case BlockId::Dirt:
                        dirts.push_back(glm::vec3(xz.X + x, y, xz.Z + z));
                        break;
                    case BlockId::GrassBlock:
                        grassBlocks.push_back(glm::vec3(xz.X + x, y, xz.Z + z));
                        break;
                    case BlockId::Grass:
                        grasses.push_back(glm::vec3(xz.X + x, y, xz.Z + z));
                        break;
                    case BlockId::BlueOrchid:
                        blueOrchids.push_back(glm::vec3(xz.X + x, y, xz.Z + z));
                        break;
                    case BlockId::OakLog:
                        oakLogs.push_back(glm::vec3(xz.X + x, y, xz.Z + z));
                        break;
                    case BlockId::OakLeaves:
                        oakLeaves.push_back(glm::vec3(xz.X + x, y, xz.Z + z));
                        break;
                    }
                }
            }
        }
    }
    renderer->DrawBlock(BlockId::Stone, stones);
    renderer->DrawBlock(BlockId::Dirt, dirts);
    renderer->DrawBlock(BlockId::GrassBlock, grassBlocks);
    renderer->DrawBlock(BlockId::Grass, grasses);
    renderer->DrawBlock(BlockId::BlueOrchid, blueOrchids);
    renderer->DrawBlock(BlockId::OakLog, oakLogs);
    renderer->DrawBlock(BlockId::OakLeaves, oakLeaves);

    // 卸载无用区块

}

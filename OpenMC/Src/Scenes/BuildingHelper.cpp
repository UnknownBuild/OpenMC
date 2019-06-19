#include "BuildingHelper.h"

BuildingHelper::BuildingHelper() {
    renderer = Singleton<SpriteRenderer>::GetInstance();
}

void BuildingHelper::buildTree(glm::vec3 pos, int height) {
    vector<glm::vec3> oakPosition;
    for (int i = 0; i < height; i++) {
        oakPosition.push_back(glm::vec3(pos.x, pos.y + i, pos.z));
    }
    renderer->DrawBlock(BlockId::OakLog, oakPosition);

    vector<glm::vec3> leavePosition;
    int floor = pos.y + height;

    if (height <= 4) {
        for (int i = pos.x - 1; i <= pos.x + 1; i++) {
            for (int j = pos.z -1; j <= pos.z + 1; j++) {
                leavePosition.push_back(glm::vec3(i, floor, j));
            }
        }
        floor++;
    }
    else {
        int width = height / 2;
        width = width < 1 ? 1 : width;
        for (int k = width; k > 1; k--) {
            for (int i = pos.x - k; i <= pos.x + k; i++) {
                for (int j = pos.z - k; j <= pos.z + k; j++) {
                    leavePosition.push_back(glm::vec3(i, floor, j));
                }
            }
            floor++;
        }
    }
    leavePosition.push_back(glm::vec3(pos.x - 1, floor, pos.z));
    leavePosition.push_back(glm::vec3(pos.x, floor, pos.z));
    leavePosition.push_back(glm::vec3(pos.x + 1, floor, pos.z));
    leavePosition.push_back(glm::vec3(pos.x - 1, floor, pos.z + 1));
    leavePosition.push_back(glm::vec3(pos.x, floor, pos.z - 1));
    
    renderer->DrawBlock(BlockId::OakLeaves, leavePosition);
}

void BuildingHelper::buildHouse(glm::vec3 pos) {

}

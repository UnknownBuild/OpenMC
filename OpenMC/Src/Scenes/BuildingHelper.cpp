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
    vector<glm::vec3> oakPostions;
    for (int k = 0; k < 3; k++) {
        for (int i = k - 1; i <= 5 - k; i++) {
            for (int j = k - 1; j <= 5 - k; j++) {
                oakPostions.push_back(glm::vec3(pos.x + i, pos.y + 4 + k, pos.z + j));
            }
        }
    }
    oakPostions.push_back(glm::vec3(pos.x - 1, pos.y, pos.z + 2));
    renderer->DrawBlock(BlockId::OakPlanks, oakPostions);

    vector<glm::vec3> quartzPosition;
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) {
            quartzPosition.push_back(glm::vec3(pos.x + i, pos.y + j, pos.z));
            quartzPosition.push_back(glm::vec3(pos.x + i, pos.y + j, pos.z + 4));
            quartzPosition.push_back(glm::vec3(pos.x + 4, pos.y + j, pos.z + i));
            if (!(i == 2 && j == 2)) {
                quartzPosition.push_back(glm::vec3(pos.x, pos.y + j, pos.z + i));
            }
        }
    }
    renderer->DrawBlock(BlockId::Quartz, quartzPosition);

    vector<glm::vec3> cobble_stonePosition;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cobble_stonePosition.push_back(glm::vec3(pos.x + i, pos.y, pos.z + j));
        }
    }
    renderer->DrawBlock(BlockId::CobbleStone, cobble_stonePosition);

    vector<glm::vec3> oakLogPosition;
    for (int i = 0; i <= 3; i++) {
        oakLogPosition.push_back(glm::vec3(pos.x, pos.y + i, pos.z));
        oakLogPosition.push_back(glm::vec3(pos.x, pos.y + i, pos.z + 4));
        oakLogPosition.push_back(glm::vec3(pos.x + 4, pos.y + i, pos.z + 4));
        oakLogPosition.push_back(glm::vec3(pos.x + 4, pos.y + i, pos.z));
    }
    renderer->DrawBlock(BlockId::OakLog, oakLogPosition);

    vector<glm::vec3> glassPosition;
    glassPosition.push_back(glm::vec3(pos.x + 2, pos.y + 2, pos.z));
    glassPosition.push_back(glm::vec3(pos.x + 2, pos.y + 2, pos.z + 4));
    renderer->DrawBlock(BlockId::Glass, glassPosition);

    vector<glm::vec3> oakDoorPosition;
    oakDoorPosition.push_back(glm::vec3(pos.x, pos.y + 1, pos.z + 2));
    renderer->DrawBlock(BlockId::OakDoor, oakDoorPosition, 3);
}

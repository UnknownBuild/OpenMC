# 个人报告

## 实现功能

### 方块元信息加载

游戏中用到的各种方块在渲染时需要使用不同的渲染方法和纹理贴图，这些信息都储存在`json`文件中：

```json
{
	"id": 1,
	"name": "stone",
	"type": 2,
	"render": 2,
	"textures": ["stone.png"],
	"colors": [[0.8, 0.8, 0.8, 1.0]],
	"animation": 0,
	"light": 0
}
```

在游戏开始时将这些信息从文件读取到游戏内部的数据结构中，并在读取时判断数据是否有错误：

```c++
bool BlockManager::Load() {
    ......
    for (auto& item : d.GetArray()) {
        if (!item.IsObject()) {
            return false;
        }
        auto idIt = item.FindMember("id");
        int id = idIt->value.GetInt();
        for (auto& member : item.GetObject()) {
            if (member.name == "id") {
                blocks[id].Id = static_cast<BlockId>(id);
            } else if (member.name == "name") {
                blocks[id].Name = member.value.GetString();
            } else if (member.name == "type") {
                blocks[id].Type = static_cast<BlockType>(member.value.GetInt());
            }
            else if (member.name == "textures") {
                // 预加载贴图
                for (auto& t : member.value.GetArray()) {
                    std::string texture = t.GetString();
                    if (texture[0] == '*') { // 需要分割的贴图
                        blocks[id].Textures = ResourceManager::LoadSplitTexture(EnvPath::TextureDir + "/blocks/"+ texture.substr(1), texture);
                        break;
                    } else {
                        blocks[id].Textures.push_back(ResourceManager::LoadTexture(EnvPath::TextureDir + "/blocks/" + texture, texture));
                    }
                }
            }
            else if (member.name == "render") {
                blocks[id].Render = static_cast<RenderType>(member.value.GetInt());
            }
            else if (member.name == "colors") {
                for (auto& colorVec : member.value.GetArray()) {
                    glm::vec4 c(colorVec.GetArray()[0].GetFloat(), colorVec.GetArray()[1].GetFloat(), colorVec.GetArray()[2].GetFloat(), colorVec.GetArray()[3].GetFloat());
                    blocks[id].Colors.push_back(c);
                }
            }
            else if (member.name == "light") {
                blocks[id].Light = member.value.GetInt();
            }
            else if (member.name == "animation") {
                blocks[id].Animation = member.value.GetInt();
            }
        }
    }
    return true;
}
```



### 方块的新建和破坏

添加或删除方块首先需要确定当前摄像机所面对的方块。可以利用相机的Position向量及Front向量求出，由于Front向量是表示相机当前朝向的标准化向量，故可以通过在Position向量上累加Front向量来求出相机视线上的点的坐标。在累加求出每个坐标之后，对这些坐标取整，得到对应的游戏坐标，判断该坐标是否存在方块，若存在，则该方块即为摄像机面对的方块，否则继续进行坐标向量累加。

```c++
position = glm::vec3((int)camera->Position.x, (int)camera->Position.y, (int)camera->Position.z);
lookingAt = caculateLookingAt();
updateNewBlockPosition();
if (renderer->GetBlock(lookingAt).Id != BlockId::Air) {
	renderer->SetShowBlock(lookingAt, newBlockDirection);
}
else {
	renderer->HideShowBlock();
}

```

```c++
glm::vec3 SceneGame::caculateLookingAt() {
    glm::vec3 result = camera->Position;
    for (int i = 0; i < 10; i++) {
        result.x += camera->Front.x * 1.0;
        result.y += camera->Front.y * 1.0;
        result.z += camera->Front.z * 1.0;
        BlockData block = renderer->GetBlock(glm::vec3(round(result.x), round(result.y), round(result.z)));

        if (block.Id != BlockId::Air) {
            break;
        }
    }

    result.x = round(result.x);
    result.y = round(result.y);
    result.z = round(result.z);
    return result;
}
```

此外在添加方块时，为了确定放置的位置，还要判断出当前面对的是目标方块的哪个面。同样需要利用相机的Position向量及Front向量。由于游戏坐标位于各个方块的中心点，且方块的边长为1个单位坐标，故方块各个面所在的平面位置坐标均可求出。

对于方块的每组平行面，同一时刻只能看到两个平行面中的一个，可以利用Front向量的符号确定看到的是哪个平行面。然后判断相机视线与哪一组平行面相交。可以先用直线参数方程来求视线与各个面所在平面的交点，再判断该交点的坐标是否处于其他两组平行面之间，以此确定视线与方块交于哪个面：

```c++
void SceneGame::updateNewBlockPosition() {
    glm::vec3 pos = camera->Position;
    glm::vec3 front = camera->Front;
    glm::vec3 temp;
    float border[6] = { lookingAt.y + 0.5, lookingAt.y - 0.5, lookingAt.z + 0.5, lookingAt.x + 0.5, lookingAt.z - 0.5, lookingAt.x - 0.5 };
    int direction = 0;

    float t, x, y, z, b;
    if (front.x != 0) {
        b = front.x < 0 ? border[3] : border[5];
        t = (b - pos.x) / front.x;
        y = pos.y + front.y * t;
        z = pos.z + front.z * t;
        if (y >= border[1] && y <= border[0] && z >= border[4] && z <= border[2]) {
            direction = front.x < 0 ? 3 : 5;
        }
    }
    if (front.y != 0) {
        b = front.y < 0 ? border[0] : border[1];
        t = (b - pos.y) / front.y;
        x = pos.x + front.x * t;
        z = pos.z + front.z * t;
        if (x >= border[5] && x <= border[3] && z >= border[4] && z <= border[2]) {
            direction = front.y < 0 ? 0 : 1;
        }
    }
    if (front.z != 0) {
        b = front.z < 0 ? border[2] : border[4];
        t = (b - pos.z) / front.z;
        x = pos.x + front.x * t;
        y = pos.y + front.y * t;
        if (x >= border[5] && x <= border[3] && y >= border[1] && y <= border[0]) {
            direction = front.z < 0 ? 2 : 4;
        }
    }

    newBlockDirection = direction;
    if (newBlockDirection == 3) {
        newBlockDirection = 5;
    }
    else if (newBlockDirection == 5) {
        newBlockDirection = 3;
    }
    newBlockPosition = lookingAt + normal[direction];
}
```

在添加方块时将方块放置到当前看向的面所朝向的方块位置，如下图的标记所示：

|                    放置前                     |                    放置后                     |
| :-------------------------------------------: | :-------------------------------------------: |
| ![1563111754203](../assets/1563111754203.png) | ![1563113255841](../assets/1563113255841.png) |



## 心得体会

此次项目我主要负责部分游戏逻辑的实现以及部分游戏数据的加载。由于整个项目都是基于OpenGL实现的，因此在开发过程中学习到了很多相关的知识，比如整个三维场景渲染的流程、渲染管线各个部分之间的联系、着色器的使用以及光照模型和阴影映射等。

对于OpenMC这样的游戏型项目，在没有使用游戏引擎的情况下，项目的架构对之后的编程而言很重要。恰当地划分功能模块即有利于提高团队开发的效率，也利于项目各个部分的代码维护。


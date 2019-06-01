#include "SpriteRenderer.h"
#include "../World/Database/BlockData.h"

SpriteRenderer::SpriteRenderer() {
  // 初始化着色器
    this->objectShader = &ResourceManager::LoadShader("GLSL/Object.vs.glsl",
        "GLSL/Object.fs.glsl", "object");
    this->blockShader = &ResourceManager::LoadShader("GLSL/Block.vs.glsl",
        "GLSL/Block.fs.glsl", "block");
    this->flatShader = &ResourceManager::LoadShader("GLSL/2D.vs.glsl",
        "GLSL/2D.fs.glsl", "font");
    this->skyShader = &ResourceManager::LoadShader("GLSL/Sky.vs.glsl",
        "GLSL/Sky.fs.glsl", "sky");
    // 初始化字体
    ResourceManager::InitFont("Resources/Fonts/RAVIE.TTF");
    // 初始化立方体
    this->initRenderData();
    // 初始化天空盒
    vector<std::string> faces {
        "Resources/Textures/sky/cloudtop_ft.jpg",
        "Resources/Textures/sky/cloudtop_bk.jpg",
        "Resources/Textures/sky/cloudtop_up.jpg",
        "Resources/Textures/sky/cloudtop_dn.jpg",
        "Resources/Textures/sky/cloudtop_rt.jpg",
        "Resources/Textures/sky/cloudtop_lf.jpg",

    };
    this->skyBox = &ResourceManager::LoadTexture(faces, "skybox");
    this->lightValue = new LightValue[40 * 40 * 40];
}


int getDis(glm::vec4 a, glm::vec4 b) {
    return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
}


void SpriteRenderer::DrawBlock(BlockId id, vector<glm::vec3> &positions, int dir) {
    BlockData data = Singleton<BlockManager>::GetInstance()->GetBlockData(id);

    BlockInst inst;
    inst.data = data;
    inst.dir = dir;

    vector<glm::vec4> posWithLight;
    for (auto position : positions) {
        posWithLight.push_back(glm::vec4(position, data.Light));
    }

    // 处理光照
    if (data.Light > 0) {
        this->lightBlock.push_back({ data, posWithLight, dir });
    }

    // 简单处理渲染顺序
    for (auto color : data.Colors) {
        if (color.a > 0 && color.a < 1) {
            // 具有透明度的方块
            this->renderData.push_back({ data, posWithLight, dir });
            return;
        }
    }
    this->renderData.push_front({ data, posWithLight, dir });
}



// 更新光照
void SpriteRenderer::UpdateLight() {
    return;

    const int size = 40;


    memset(this->lightValue, 0, size * size * size * sizeof(LightValue));

    // 填充光照范围
    unsigned int blockIndex = 0;
    for (auto& block : this->renderData) {
        unsigned int posIndex = -1;
        for (auto& pos : block.position) {
            posIndex++;
            if (pos.a > 0) {
                continue;
            }
            glm::vec4 relaPos = pos - glm::vec4(viewPos, 1);
            relaPos += glm::vec4(size / 2);
            if (relaPos.x >= 0 && relaPos.x < size &&
                relaPos.y >= 0 && relaPos.y < size &&
                relaPos.z >= 0 && relaPos.z < size) {
                int offset = relaPos.z * size * size + relaPos.y * size + relaPos.x;
                lightValue[offset] = {blockIndex, posIndex, block.data.Type, 0};
            }
        }
        blockIndex++;
    }

    // 注入垂直光
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size ; y++) {
            for (int z = 0; z < size; z++) {
                int offset = z * size * size + y * size + x;
                if (lightValue[offset].type == BlockType::Soild || lightValue[offset].type == BlockType::Face) {
                    break;
                } else {
                    lightValue[offset].value = 10;
                }
            }
        }
    }

    // 注入点光源
    for (auto& light : this->lightBlock) {
        for (auto& pos : light.position) {
            glm::vec4 relaPos = pos - glm::vec4(viewPos, 1);
            relaPos += glm::vec4(size / 2);
            if (relaPos.x >= 0 && relaPos.x < size &&
                relaPos.y >= 0 && relaPos.y < size &&
                relaPos.z >= 0 && relaPos.z < size) {
                int offset = relaPos.z * size * size + relaPos.y * size + relaPos.x;
                lightValue[offset].value = light.data.Light;
            }
        }
    }

    // 扩散 15 次
    for (int t = 0; t < 12; t++) {
        for (int x = 1; x < size - 1; x++) {
            for (int y = 1; y < size - 1; y++) {
                for (int z = 1; z < size - 1; z++) {
                    int offset = z * size * size + y * size + x;
                    uint8_t value = lightValue[offset].value;
                    value = max(value, lightValue[offset - 1].value);
                    value = max(value, lightValue[offset + 1].value);
                    value = max(value, lightValue[offset - size].value);
                    value = max(value, lightValue[offset + size].value);
                    value = max(value, lightValue[offset - size * size].value);
                    value = max(value, lightValue[offset + size * size].value);
                    if (value != lightValue[offset].value) value--;

                    if (lightValue[offset].type == BlockType::Soild || lightValue[offset].type == BlockType::Face) {
                        blockIndex = 0;
                        for (auto& block : this->renderData) {
                            if (blockIndex == lightValue[offset].blockIndex) {
                                block.position[lightValue[offset].posIndex].a = value / 10.0f;
                                break;
                            }
                            blockIndex++;
                        }
                    } else {
                        lightValue[offset].value = value;
                    }
                }
            }
        }
    }
}

void SpriteRenderer::RenderBlock(bool clear) {
    this->renderFrame++;

    for (auto block : this->renderData) {
        int frame = 0;
        if (block.data.Animation != 0) {
            frame = (this->renderFrame / block.data.Animation) % block.data.Textures.size();
        }
        this->DrawBlock(block.data.Textures, block.data.Colors, block.data.Render, block.position, block.dir, frame);
    }

    if (clear) {
        this->renderData.clear();
    }
}


SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &this->quadVAO);
}

// 设置定向光源
void SpriteRenderer::SetLight(glm::vec3 direction) {
    this->blockShader->Use();
    this->blockShader->SetVector3f("dirLight.direction", direction);
    this->blockShader->SetVector3f("dirLight.ambient", glm::vec3(0.3f));
    this->blockShader->SetVector3f("dirLight.diffuse", glm::vec3(0.65f));
    this->blockShader->SetVector3f("dirLight.specular", glm::vec3(0.2f));
}

// 设置视图
void SpriteRenderer::SetView(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPostion) {
    this->blockShader->Use();
    this->blockShader->SetMatrix4("projection", projection);
    this->blockShader->SetMatrix4("view", view);
    this->blockShader->SetVector3f("viewPos", viewPostion);

    this->viewPos = viewPostion;

    this->skyShader->Use();
    this->skyShader->SetMatrix4("projection", projection);
    this->skyShader->SetMatrix4("view", glm::mat4(glm::mat3(view)));
}

void SpriteRenderer::SetWindowSize(int w, int h) {
    this->flatShader->Use().SetMatrix4("projection", glm::ortho(0.0f, (float)w, 0.0f, (float)h));
}


// 已废弃方法
void SpriteRenderer::DrawBlock(const vector<Texture2D>& textures, const vector<glm::vec4>& colors,
    RenderType type, const glm::vec3* position, int count, int dir, int iTexture) {
    return;
}

// 通用渲染方法
void SpriteRenderer::DrawBlock(const vector<Texture2D>& _textures, const vector<glm::vec4>& colors,
    RenderType type, const vector<glm::vec4>& position, int dir, int iTexture) {
    this->blockShader->Use();
    this->blockShader->SetMatrix4("model", glm::mat4(1.0f));
    this->blockShader->SetInteger("hasTexture", true);
    this->blockShader->SetInteger("hasColor", false);
    this->blockShader->SetInteger("material.diffuse", 0); // 漫反射贴图
    this->blockShader->SetFloat("time", glfwGetTime());
    int count = position.size();
    glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * count, &position[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    vector<Texture2D> textures = _textures;
    if (iTexture != 0) {
        if (textures.size() < iTexture) return;
        textures[0] = textures[iTexture];
    }

    for (auto color : colors) {
        if (color.a < 1) {
            glDisable(GL_CULL_FACE);
            break;
        }
    }

    glActiveTexture(GL_TEXTURE0);

    glm::mat4 model = glm::mat4(1.0);
    switch (type) {
    case RenderType::OneTexture: // 单纹理方块
        if (colors.size() > 0) {
            this->blockShader->SetInteger("hasColor", true);
            this->blockShader->SetVector4f("material.color", colors[0]);
        }
        textures[0].Bind();
        glBindVertexArray(this->quadVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, count);
        break;
    case RenderType::ThreeTexture: // 三纹理方块(上、四周、下)
        textures[1].Bind();
        glBindVertexArray(this->quadVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0, count);

        textures[0].Bind();
        glBindVertexArray(this->topVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);

        textures[2].Bind();
        glBindVertexArray(this->bottomVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        break;
    case RenderType::NoiseTexture:// 噪声 + 颜色
        this->blockShader->SetInteger("hasColor", true);
        this->blockShader->SetVector4f("material.color", colors[0]);

        textures[0].Bind();
        glBindVertexArray(this->quadVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0, count);

        glBindVertexArray(this->topVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);

        glBindVertexArray(this->bottomVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        break;
    case RenderType::SideTexture: // 上下噪声，四周纹理
        textures[0].Bind();
        glBindVertexArray(this->quadVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0, count);

        textures[1].Bind();
        this->blockShader->SetInteger("hasColor", true);
        this->blockShader->SetVector4f("material.color", colors[0]);
        glBindVertexArray(this->topVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);

        this->blockShader->SetVector4f("material.color", colors[1]);
        glBindVertexArray(this->bottomVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        break;
    case RenderType::CenterCrossTexture: // 中心交叉纹理
        textures[0].Bind();
        if (colors.size() > 0) {
            this->blockShader->SetInteger("hasColor", true);
            this->blockShader->SetVector4f("material.color", colors[0]);
        }
        glBindVertexArray(this->entityVAO1);
        glDisable(GL_CULL_FACE);
        glDrawElementsInstanced(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0, count);
        glEnable(GL_CULL_FACE);
        break;
    case RenderType::TorchTexture: // 火把
        textures[0].Bind();
        glBindVertexArray(this->entityVAO2);
        glDrawElementsInstanced(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0, count);
        glBindVertexArray(this->topVAO);
        model = glm::translate(model, glm::vec3(0, -0.375, 0));
        model = glm::scale(model, glm::vec3(0.125, 1, 0.125));
        this->blockShader->SetMatrix4("model", model);
        this->blockShader->SetInteger("hasColor", true);
        this->blockShader->SetInteger("hasTexture", false);
        this->blockShader->SetVector4f("material.color", colors[0]);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        break;
    case RenderType::FireTexture: // 火焰
        textures[0].Bind();
        glDisable(GL_CULL_FACE);
        glBindVertexArray(this->entityVAO3);
        glDrawElementsInstanced(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0, count);
        glBindVertexArray(this->quadVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0, count);
        break;
    case RenderType::CustomTexture: // 六面自定义方块 (前后左右上下)
        textures[0].Bind();
        glBindVertexArray(this->quadVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        textures[1].Bind();
        glBindVertexArray(this->backVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        textures[2].Bind();
        glBindVertexArray(this->leftVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        textures[3].Bind();
        glBindVertexArray(this->rightVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        textures[4].Bind();
        glBindVertexArray(this->topVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        textures[5].Bind();
        glBindVertexArray(this->bottomVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        break;
    case RenderType::DirCustomTexture: // 具有方向的方块 （前、侧边、上下）
        model = glm::rotate(model, glm::radians(90.0f * dir), glm::vec3(0, 1, 0));
        this->blockShader->SetMatrix4("model", model);
        textures[0].Bind();
        glBindVertexArray(this->quadVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        textures[1].Bind();
        glBindVertexArray(this->backVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        textures[1].Bind();
        glBindVertexArray(this->leftVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        textures[1].Bind();
        glBindVertexArray(this->rightVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);

        textures[2].Bind();
        glBindVertexArray(this->topVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        textures[2].Bind();
        glBindVertexArray(this->bottomVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        break;
    case RenderType::DoorTexture: // 双层单面（门:上下）
        this->blockShader->SetVector4f("material.color", colors[0]);
        model = glm::rotate(model, glm::radians(90.0f * dir), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(1, 1, 0.125));

        // 前后
        textures[1].Bind();
        this->blockShader->SetMatrix4("model", model);
        this->blockShader->SetInteger("hasColor", false);
        this->blockShader->SetInteger("hasTexture", true);
        glBindVertexArray(this->entityVAO4);
        glDrawElementsInstanced(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0, count);
        // 四周
        this->blockShader->SetInteger("hasColor", true);
        this->blockShader->SetInteger("hasTexture", false);
        glBindVertexArray(this->entityVAO5);
        glDrawElementsInstanced(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0, count);

        model = glm::translate(model, glm::vec3(0, 1, 0));
        this->blockShader->SetMatrix4("model", model);
        this->blockShader->SetInteger("hasColor", false);
        this->blockShader->SetInteger("hasTexture", true);
        textures[0].Bind();
        glBindVertexArray(this->entityVAO4);
        glDrawElementsInstanced(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0, count);

        this->blockShader->SetInteger("hasColor", true);
        this->blockShader->SetInteger("hasTexture", false);
        glBindVertexArray(this->entityVAO5);
        glDrawElementsInstanced(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0, count);
        break;
    case RenderType::GlassTexture: // 渲染染色玻璃 前后、周边贴图、方向
        this->blockShader->SetInteger("hasColor", true);
        this->blockShader->SetVector4f("material.color", colors[0]);
        model = glm::rotate(model, glm::radians(90.0f * dir), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(1, 1, 0.125));
        // 前后
        textures[0].Bind();
        this->blockShader->SetMatrix4("model", model);
        this->blockShader->SetInteger("hasTexture", true);
        glBindVertexArray(this->entityVAO4);
        glDisable(GL_CULL_FACE);
        glDrawElementsInstanced(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0, count);
        // 四周
        this->blockShader->SetVector4f("material.color",
            glm::vec4(colors[0].x, colors[0].y, colors[0].z, 0.2));
        this->blockShader->SetInteger("hasTexture", false);
        glBindVertexArray(this->entityVAO5);
        glDrawElementsInstanced(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0, count);
        glEnable(GL_CULL_FACE);
        break;
    case RenderType::LiquidTexture: // 水面
        this->blockShader->SetInteger("hasColor", true);
        this->blockShader->SetVector4f("material.color", colors[0]);
        model = glm::rotate(model, glm::radians(90.0f * dir), glm::vec3(0, 1, 0));
        model = glm::translate(model, glm::vec3(0, -0.125, 0));

        textures[0].Bind();
        this->blockShader->SetMatrix4("model", model);
        glBindVertexArray(this->topVAO);
        glDisable(GL_CULL_FACE);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        glEnable(GL_CULL_FACE);
        break;
    case RenderType::OffsetTexture: // 偏移纹理(上、四周、下)，平移幅度(上、四周、下)
        textures[0].Bind();
        model = glm::translate(glm::mat4(1.0), glm::vec3(0, colors[0].x, 0));
        this->blockShader->SetMatrix4("model", model);
        glBindVertexArray(this->topVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);

        textures[1].Bind();
        model = glm::translate(glm::mat4(1.0), glm::vec3(colors[0].y, 0, 0));
        this->blockShader->SetMatrix4("model", model);
        glBindVertexArray(this->leftVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);

        model = glm::translate(glm::mat4(1.0), glm::vec3(-colors[0].y, 0, 0));
        this->blockShader->SetMatrix4("model", model);
        glBindVertexArray(this->rightVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);

        model = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -colors[0].y));
        this->blockShader->SetMatrix4("model", model);
        glBindVertexArray(this->quadVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);

        model = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, colors[0].y));
        this->blockShader->SetMatrix4("model", model);
        glBindVertexArray(this->backVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);

        textures[2].Bind();
        model = glm::translate(glm::mat4(1.0), glm::vec3(0, colors[0].z, 0));
        this->blockShader->SetMatrix4("model", model);
        glBindVertexArray(this->bottomVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);

        break;



    default:
        break;
    }
    glEnable(GL_CULL_FACE);
    glBindVertexArray(0);
}

// 渲染带纹理的立方体
void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec3 position, glm::vec3 size, GLfloat rotate, glm::vec3 color) {
    //this->objectShader->Use();
    //glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, position);
    //model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));

    //model = glm::scale(model, size);

    //this->objectShader->SetMatrix4("model", model);
    //this->objectShader->SetInteger("hasTexture", true);
    //this->objectShader->SetInteger("hasColor", true);
    //this->objectShader->SetInteger("material.diffuse", 0); // 漫反射贴图
    //this->objectShader->SetInteger("material.specular", 0); // 镜面反射贴图
    //this->objectShader->SetInteger("material.shininess", 32); // 镜面反射率

    //// Render textured quad
    //this->objectShader->SetVector3f("material.color", color);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glActiveTexture(GL_TEXTURE0);
    //texture.Bind();

    //glBindVertexArray(this->quadVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);

}

// 渲染模型
void SpriteRenderer::DrawSprite(Model& modelObj, glm::vec3 position, glm::vec3 size, GLfloat rotate) {
    //this->objectShader->Use();

    //glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, position);
    //model = glm::rotate(model, rotate, glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::scale(model, size);

    //this->objectShader->SetMatrix4("model", model);
    //modelObj.Draw(this->objectShader);
}

void SpriteRenderer::initRenderData() {
    // 实例化数组
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 10240, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    float verticesQuad[] = {
        // 正面
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 左下角
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 右下角
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // 右上角
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 左上角
        // 后面
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // 右下角
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // 右上角
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // 左上角
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // 左下角
        // 左边
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 右上角
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 左上角
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 左下角
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 右下角
        // 右边
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 右下角
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 右上角
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 左上角
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 左下角
        // 下面
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // 左下角
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // 右下角
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // 右上角
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // 左上角
        // 上面
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // 右下角
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // 右上角
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // 左上角
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f  // 左下角
    };

    unsigned int indicesQuad[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    // 立方体/正面
    this->quadVAO = this->makeVAO(verticesQuad, sizeof(verticesQuad), indicesQuad, sizeof(indicesQuad));
    // 后面
    this->backVAO = this->makeVAO(&verticesQuad[8 * 4 * 1], sizeof(float) * 8 * 4, indicesQuad, 6 * sizeof(unsigned int));
    // 左面
    this->leftVAO = this->makeVAO(&verticesQuad[8 * 4 * 2], sizeof(float) * 8 * 4, indicesQuad, 6 * sizeof(unsigned int));
    // 右面
    this->rightVAO = this->makeVAO(&verticesQuad[8 * 4 * 3], sizeof(float) * 8 * 4, indicesQuad, 6 * sizeof(unsigned int));
    // 底部
    this->bottomVAO = this->makeVAO(&verticesQuad[8 * 4 * 4], sizeof(float) * 8 * 4, indicesQuad, 6 * sizeof(unsigned int));
    // 顶部
    this->topVAO = this->makeVAO(&verticesQuad[8 * 4 * 5], sizeof(float) * 8 * 4, indicesQuad, 6 * sizeof(unsigned int));


    // 中心交叉
    float verticesEntity1[] = {
        // 后面
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 右下角
        0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 右下角
        0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 右上角
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  1.0f, 1.0f, 0.0f, // 右上角
        // 正面
        -0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 左下角
        0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 左下角
        0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  1.0f, 1.0f, 0.0f, // 左上角
        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 左上角
    };

    this->entityVAO1 = this->makeVAO(verticesEntity1, sizeof(verticesEntity1), indicesQuad, 12 * sizeof(unsigned int));

    // 火把
    float verticesEntity2[] = {
        // 后面
        -0.5f, -0.5f, -0.0625f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // 右下角
        -0.5f,  0.5f, -0.0625f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // 右上角
        0.5f,  0.5f,  -0.0625f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // 左上角
        0.5f, -0.5f,  -0.0625f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // 左下角
        // 正面
        0.5f, -0.5f,   0.0625f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 右下角
        0.5f,  0.5f,   0.0625f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // 右上角
        -0.5f,  0.5f,  0.0625f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 左上角
        -0.5f, -0.5f,  0.0625f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 左下角
        // 左边
        -0.0625f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 右下角
        -0.0625f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 右上角
        -0.0625f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 左上角
        -0.0625f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 左下角
        // 右边
        0.0625f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 右下角
        0.0625f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 右上角
        0.0625f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 左上角
        0.0625f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 左下角
    };

    this->entityVAO2 = this->makeVAO(verticesEntity2, sizeof(verticesEntity2), indicesQuad, 24 * sizeof(unsigned int));

    // 顶部交叉
    float verticesEntity3[] = {
        // 后面
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // 右下角
        0.5f, -0.5f,  -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // 左下角
        0.5f,  0.5f,  0.2f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // 左上角
        -0.5f,  0.5f, 0.2f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // 右上角
        // 正面
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 左下角
        0.5f, -0.5f,   0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 右下角
        0.5f,  0.5f,   -0.2f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // 右上角
        -0.5f,  0.5f,  -0.2f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 左上角
        // 左边
        0.2f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 右上角
        0.2f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 左上角
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 左下角
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 右下角
        // 右边
        -0.2f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 左上角
        -0.2f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 右上角
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 右下角
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 左下角
    };

    this->entityVAO3 = this->makeVAO(verticesEntity3, sizeof(verticesEntity3), indicesQuad, 24 * sizeof(unsigned int));

    // 门前后
    float verticesEntity4[] = {
        // 正面
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 右下角
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // 右上角
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 左上角
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 左下角
        // 后面
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // 右下角
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // 右上角
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // 左上角
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // 左下角
    };
    this->entityVAO4 = this->makeVAO(verticesEntity4, sizeof(verticesEntity4), indicesQuad, 12 * sizeof(unsigned int));

    // 门四周
    float verticesEntity5[] = {
        // 左边
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 右下角
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 右上角
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 左上角
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 左下角
        // 右边
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 右下角
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 右上角
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 左上角
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 左下角
        // 下面
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // 右下角
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // 右上角
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // 左上角
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // 左下角
        // 上面
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // 右下角
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // 右上角
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // 左上角
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f  // 左下角
    };

    this->entityVAO5 = this->makeVAO(verticesEntity5, sizeof(verticesEntity5), indicesQuad, 24 * sizeof(unsigned int));

    // 天空盒
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    unsigned int skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    // 2D渲染
    glGenVertexArrays(1, &this->flatVAO);
    glGenBuffers(1, &this->flatVBO);
    glBindVertexArray(this->flatVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->flatVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


unsigned int SpriteRenderer::makeVAO(float* vertices, int verticesLen, unsigned int* indices, int indicesLen) {
    unsigned VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesLen, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLen, indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return VAO;
}

void SpriteRenderer::RenderText(std::string text, glm::vec2 position, GLfloat scale, glm::vec4 color) {
    flatShader->Use();
    flatShader->SetVector4f("textColor", color);
    flatShader->SetInteger("text", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(ResourceManager::fontVAO);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = ResourceManager::Characters[*c];

        GLfloat xpos = position.x + ch.Bearing.x * scale;
        GLfloat ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, ResourceManager::fontVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        position.x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

float DIS_VALUE[] = {
    7	,   1.0,    0.7	,   1.8     ,
    13	,   1.0,    0.35,   0.44    ,
    20	,   1.0,    0.22,   0.20    ,
    32	,   1.0,    0.14,   0.07    ,
    50	,   1.0,    0.09,   0.032   ,
    65	,   1.0,    0.07,   0.017   ,
    100	,   1.0,    0.045,  0.0075  ,
    160	,   1.0,    0.027,  0.0028  ,
    200	,   1.0,    0.022,  0.0019  ,
    325	,   1.0,    0.014,  0.0007  ,
    600	,   1.0,    0.007,  0.0002  ,
    3250,   1.0,    0.0014, 0.000007
};

// 添加点光源
void SpriteRenderer::AddPointLight(glm::vec3 pos, glm::vec3 a, glm::vec3 d, glm::vec3 s, float dis) {
    if (this->pointCount >= 10) return; // 点光源上限

    this->pointLight[this->pointCount].position = pos;
    this->pointLight[this->pointCount].ambient = a;
    this->pointLight[this->pointCount].diffuse = d;
    this->pointLight[this->pointCount].specular = s;

    this->pointLight[this->pointCount].constant = 1.0;
    this->pointLight[this->pointCount].linear = 0.0;
    this->pointLight[this->pointCount].quadratic = 0.0;

    for (int i = 0; i < 12; i++) {
        if (dis < DIS_VALUE[i * 4]) {
            this->pointLight[this->pointCount].constant = DIS_VALUE[i * 4 + 1];
            this->pointLight[this->pointCount].linear = DIS_VALUE[i * 4 + 2];
            this->pointLight[this->pointCount].quadratic = DIS_VALUE[i * 4 + 3];
            break;
        }
    }
    this->blockShader->Use();
    this->blockShader->SetFloat(("pointLights[" + to_string(this->pointCount) + "].constant").c_str(),
        this->pointLight[this->pointCount].constant);
    this->blockShader->SetFloat(("pointLights[" + to_string(this->pointCount) + "].linear").c_str(),
        this->pointLight[this->pointCount].linear);
    this->blockShader->SetFloat(("pointLights[" + to_string(this->pointCount) + "].quadratic").c_str(),
        this->pointLight[this->pointCount].quadratic);
    this->blockShader->SetVector3f(("pointLights[" + to_string(this->pointCount) + "].position").c_str(),
        this->pointLight[this->pointCount].position);
    this->blockShader->SetVector3f(("pointLights[" + to_string(this->pointCount) + "].ambient").c_str(),
        this->pointLight[this->pointCount].ambient);
    this->blockShader->SetVector3f(("pointLights[" + to_string(this->pointCount) + "].diffuse").c_str(),
        this->pointLight[this->pointCount].diffuse);
    this->blockShader->SetVector3f(("pointLights[" + to_string(this->pointCount) + "].specular").c_str(),
        this->pointLight[this->pointCount].specular);

    this->pointCount++;
    this->blockShader->SetInteger("pointCount", this->pointCount);
}
// 清除点光源
void SpriteRenderer::ClearPointLight() {
    this->pointCount = 0;
    this->blockShader->SetInteger("pointCount", this->pointCount);
}
// 渲染天空盒
void SpriteRenderer::RenderSkyBox() {
    glDepthFunc(GL_LEQUAL);
    this->skyShader->Use();
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyBox->ID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}
// 渲染2D纹理
void SpriteRenderer::DrawTexture(Texture2D& texture, glm::vec2 position, float scale, glm::vec4 color) {

    this->flatShader->Use();
    flatShader->SetVector4f("textColor", color);
    flatShader->SetInteger("text", 0);

    glBindVertexArray(this->flatVAO);

    float xpos = position.x;
    float ypos = position.y;

    GLfloat w = texture.Width * scale;
    GLfloat h = texture.Height * scale;

    float vertices[6][4] = {
        { xpos,     ypos + h,   0.0, 0.0 }, // 左上角
        { xpos,     ypos,       0.0, 1.0 }, // 左下角
        { xpos + w, ypos,       1.0, 1.0 }, // 右下角

        { xpos,     ypos + h,   0.0, 0.0 }, // 左上角
        { xpos + w, ypos,       1.0, 1.0 }, // 右下角
        { xpos + w, ypos + h,   1.0, 0.0 }  // 右上角
    };
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    glBindBuffer(GL_ARRAY_BUFFER, this->flatVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}

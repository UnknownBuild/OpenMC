#pragma once
#ifndef SPRITE_RENDERER
#define SPRITE_RENDERER


#include <initializer_list>
#include "../ResourceManager/ResourceManager.h"
#include "../World/Database/BlockManager.h"
#include "../Helpers/EnvPath.h"
#include "../Helpers/Singleton.h"
#include <list>

using std::initializer_list;

struct PointList {
    int id;

    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

//  this->DrawBlock(block.Textures, block.Colors, block.Render, &position[0], position.size(), dir, frame);
struct BlockInst {
    BlockData data;
    vector<glm::vec4> position;
    int dir;
};

struct LightValue {
    unsigned int blockIndex;
    unsigned int posIndex;
    BlockType type;
    uint8_t value;
};

class SpriteRenderer {
 public:
  SpriteRenderer();
  ~SpriteRenderer();

  // 渲染立方体
  void DrawSprite(Texture2D& texture, glm::vec3 position,
                  glm::vec3 size = glm::vec3(10, 10, 10), GLfloat rotate = 0.0f,
                  glm::vec3 color = glm::vec3(1.0f));
  // 渲染模型
  void DrawSprite(Model& model, glm::vec3 position,
                  glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f),
                  GLfloat rotate = 0.0f);
  // 渲染方块
  void DrawBlock(BlockId id, vector<glm::vec3> &position, int dir = 0);
  // 渲染方块
  void DrawBlock(const vector<Texture2D>& textures, const vector<glm::vec4>& colors,
      RenderType type, const glm::vec3* position, int count, int dir = 0, int iTexture = 0);

  void UpdateLight();

  void DrawBlock(const vector<Texture2D>& textures, const vector<glm::vec4>& colors,
      RenderType type, const vector<glm::vec4> &position, int dir = 0, int iTexture = 0, Shader* shader = nullptr);

  void RenderBlock(bool clear = true, Shader* shader = nullptr);
  // 渲染文本
  void RenderText(std::string text, glm::vec2 postion, GLfloat scale = 1.0,
                  glm::vec4 color = glm::vec4(1));
  // 渲染天空盒
  void RenderSkyBox();
  // 渲染2D纹理
  void DrawTexture(Texture2D& texture, glm::vec2 position, float scale = 1.0, glm::vec4 color = glm::vec4(1));
  // 设置参数
  void SetView(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPostion);
  // 设置平行光源
  void SetLight(glm::vec3 direction);
  // 添加点光源
  void AddPointLight(glm::vec3 pos, glm::vec3 a, glm::vec3 d, glm::vec3 s, float dis);
  // 清除点光源
  void ClearPointLight();
  // 更改渲染范围
  void SetWindowSize(int w, int h);

 private:
  void initRenderData();
  unsigned int makeVAO(float *vertices, int verticesLen, unsigned int *indices, int indicesLen);
  unsigned int renderFrame;

  // 着色器
  Shader* objectShader;
  Shader* blockShader;
  Shader* skyShader;
  Shader* flatShader;
  Shader* GBufferShader;

  // 纹理
  Texture2D* skyBox;

  // VAO
  unsigned int quadVAO;
  unsigned int backVAO, leftVAO, rightVAO, topVAO, bottomVAO;
  unsigned int skyboxVAO;
  unsigned int flatVAO;

  unsigned int entityVAO1;
  unsigned int entityVAO2;
  unsigned int entityVAO3;
  unsigned int entityVAO4;
  unsigned int entityVAO5;

  unsigned int instanceVBO;
  unsigned int flatVBO;

  std::list<BlockInst> renderData;
  vector<BlockInst> lightBlock;

  LightValue* lightValue; // 渲染区块

  // 点光源
  PointList pointLight[10];
  int pointCount;

  glm::vec3 viewPos;

};


#endif // !1

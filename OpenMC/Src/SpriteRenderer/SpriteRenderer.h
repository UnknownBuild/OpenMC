#pragma once
#include "../ResourceManager/ResourceManager.h"

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
  void DrawBlock(Texture2D& top, Texture2D& side, Texture2D& bottom, glm::vec3 position);
  void DrawBlock(Texture2D& texture, glm::vec3 position);
  void DrawBlock(Texture2D& texture, glm::vec3 top, glm::vec3 bottom, glm::vec3 position);
  void DrawBlock(glm::vec3 color, glm::vec3 position);

  // 渲染文本
  void RenderText(std::string text, glm::vec2 postion, GLfloat scale = 1.0,
                  glm::vec3 color = glm::vec3(1,1,1));
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

  void RenderSky();

 private:
  void initRenderData();
  glm::mat4 setBlockPostion(glm::vec3 pos);
  Shader* objectShader;
  Shader* skyShader;
  Shader* fontShader;
  Texture2D* noise;
  unsigned int skyTexture;
  unsigned int quadVAO;
  unsigned int topFace;
  unsigned int bottomFace;
  unsigned int skyboxVAO;
  PointList pointLight[10];
  int pointCount;
};

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
  void DrawBlock(Texture2D& top, Texture2D& side, Texture2D& bottom, glm::vec3 position[], int count);
  void DrawBlock(Texture2D& texture, glm::vec3 position[], int count);
  void DrawBlock(Texture2D& texture, glm::vec4 top, glm::vec4 bottom, glm::vec3 position[], int count);
  void DrawBlock(glm::vec4 color, glm::vec3 position[], int count);

  // 渲染文本
  void RenderText(std::string text, glm::vec2 postion, GLfloat scale = 1.0,
                  glm::vec4 color = glm::vec4(1));
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
  // 渲染天空盒
  void RenderSkyBox();
  // 渲染2D纹理
  void DrawTexture(Texture2D& texture, glm::vec2 position, float scale = 1.0, glm::vec4 color = glm::vec4(1));

 private:
  void initRenderData();
  void setBlockShader();

  // 着色器
  Shader* objectShader;
  Shader* blockShader;
  Shader* skyShader;
  Shader* flatShader;

  // 纹理
  Texture2D* noise;
  Texture2D* skyBox;

  // VAO
  unsigned int quadVAO;
  unsigned int skyboxVAO;
  unsigned int topVAO;
  unsigned int bottomVAO;
  unsigned int flatVAO;

  unsigned int instanceVBO;
  unsigned int flatVBO;

  // 点光源
  PointList pointLight[10];
  int pointCount;
};

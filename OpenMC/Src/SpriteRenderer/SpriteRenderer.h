#pragma once
#include "../ResourceManager/ResourceManager.h"

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
                  GLfloat rotate = 0.0f,
                  glm::vec3 value = glm::vec3(0.4, 1, 0.3));
  // 渲染文本
  void RenderText(std::string text, glm::vec2 postion, GLfloat scale = 1.0,
                  glm::vec3 color = glm::vec3(1,1,1));
  // 设置参数
  void SetView(glm::mat4 projection, glm::mat4 view);
  // 设置平行光源
  void SetLight(glm::vec3 color, glm::vec3 direction, glm::vec3 viewPostion);
  // 更改渲染范围
  void SetWindowSize(int w, int h);
 private:
  void initRenderData();
  Shader* objectShader;
  Shader* fontShader;
  unsigned int quadVAO;
};
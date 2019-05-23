#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer() {
  // 初始化着色器
  this->objectShader = &ResourceManager::LoadShader("GLSL/Object.vs.glsl",
    "GLSL/Object.fs.glsl", "object");
  this->fontShader = &ResourceManager::LoadShader("GLSL/Font.vs.glsl",
    "GLSL/Font.fs.glsl", "font");
  // 初始化字体
  ResourceManager::InitFont("Resources/Fonts/RAVIE.TTF");
  // 初始化立方体
  this->initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
  glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::SetLight(glm::vec3 color, glm::vec3 direction, glm::vec3 viewPostion) {
  this->objectShader->Use();
  this->objectShader->SetVector3f("dirLight.color", color);
  this->objectShader->SetVector3f("dirLight.direction", direction);
  this->objectShader->SetVector3f("viewPos", viewPostion);
}

void SpriteRenderer::SetView(glm::mat4 projection, glm::mat4 view) {
  this->objectShader->Use();
  this->objectShader->SetMatrix4("projection", projection);
  this->objectShader->SetMatrix4("view", view);
}

void SpriteRenderer::SetWindowSize(int w, int h) {
  this->fontShader->Use().SetMatrix4("projection", glm::ortho(0.0f, (float)w, 0.0f, (float)h));
}

void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec3 position, glm::vec3 size, GLfloat rotate, glm::vec3 color) {
  this->objectShader->Use();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));

  model = glm::scale(model, size);

  this->objectShader->SetMatrix4("model", model);
  this->objectShader->SetInteger("hasTexture", true);
  this->objectShader->SetInteger("hasColor", true);
  this->objectShader->SetInteger("material.diffuse", 0);
  this->objectShader->SetInteger("material.shininess", 32);
  this->objectShader->SetVector3f("strength", glm::vec3(0.3, 0.8, 0.4));

  // Render textured quad
  this->objectShader->SetVector3f("dirLight.ambient", color);
  this->objectShader->SetVector3f("dirLight.diffuse", color);
  this->objectShader->SetVector3f("dirLight.specular", color);

  glActiveTexture(GL_TEXTURE0);
  texture.Bind();

  glBindVertexArray(this->quadVAO);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Model& modelObj, glm::vec3 position, glm::vec3 size, GLfloat rotate, glm::vec3 value) {
  this->objectShader->Use();
  
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, rotate, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::scale(model, size);

  this->objectShader->SetMatrix4("model", model);
  this->objectShader->SetVector3f("strength", value);
  modelObj.Draw(this->objectShader);
}

void SpriteRenderer::initRenderData()
{
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4,

    8, 9, 10,
    10, 11, 8,

    12, 13, 14,
    14, 15, 12,

    16, 17, 18,
    18, 19, 16,

    20, 21, 22,
    22, 23, 20
  };

  GLuint VBO, EBO;
  glGenVertexArrays(1, &this->quadVAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(this->quadVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void SpriteRenderer::RenderText(std::string text, glm::vec2 postion, GLfloat scale, glm::vec3 color)
{
  fontShader->Use();
  fontShader->SetVector3f("textColor", color);
  fontShader->SetInteger("text", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(ResourceManager::fontVAO);

  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Character ch = ResourceManager::Characters[*c];

    GLfloat xpos = postion.x + ch.Bearing.x * scale;
    GLfloat ypos = postion.y - (ch.Size.y - ch.Bearing.y) * scale;

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
    postion.x += (ch.Advance >> 6) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
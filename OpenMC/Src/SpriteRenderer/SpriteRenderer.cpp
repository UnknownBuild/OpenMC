#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer() {
  // 初始化着色器
  this->objectShader = &ResourceManager::LoadShader("GLSL/Object.vs.glsl",
    "GLSL/Object.fs.glsl", "object");
  this->fontShader = &ResourceManager::LoadShader("GLSL/Font.vs.glsl",
      "GLSL/Font.fs.glsl", "font");
  this->skyShader = &ResourceManager::LoadShader("GLSL/Sky.vs.glsl",
      "GLSL/Sky.fs.glsl", "sky");
  // 初始化字体
  ResourceManager::InitFont("Resources/Fonts/RAVIE.TTF");
  // 初始化立方体
  this->initRenderData();
  // 初始化噪声纹理
  this->noise = &ResourceManager::LoadTexture("Resources/Textures/blocks/grass_block_top.png", "noise");
  // 初始化天空盒
  vector<std::string> faces
  {
      "Resources/Textures/sky/cloudtop_ft.jpg",
      "Resources/Textures/sky/cloudtop_bk.jpg",
      "Resources/Textures/sky/cloudtop_up.jpg",
      "Resources/Textures/sky/cloudtop_dn.jpg",
      "Resources/Textures/sky/cloudtop_rt.jpg",
      "Resources/Textures/sky/cloudtop_lf.jpg",

  };
  this->skyTexture = ResourceManager::LoadCubemap(faces);
}

SpriteRenderer::~SpriteRenderer() {
  glDeleteVertexArrays(1, &this->quadVAO);
}

// 设置定向光源
void SpriteRenderer::SetLight(glm::vec3 direction) {
  this->objectShader->Use();
  this->objectShader->SetVector3f("dirLight.direction", direction);
  this->objectShader->SetVector3f("dirLight.ambient", glm::vec3(0.3f));
  this->objectShader->SetVector3f("dirLight.diffuse", glm::vec3(0.65f));
  this->objectShader->SetVector3f("dirLight.specular", glm::vec3(0.2f));
}

void SpriteRenderer::SetView(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPostion) {
  this->objectShader->Use();
  this->objectShader->SetMatrix4("projection", projection);
  this->objectShader->SetMatrix4("view", view);
  this->objectShader->SetVector3f("viewPos", viewPostion);

  this->skyShader->Use();
  this->skyShader->SetMatrix4("projection", projection);
  this->skyShader->SetMatrix4("view", glm::mat4(glm::mat3(view)));
}

void SpriteRenderer::SetWindowSize(int w, int h) {
  this->fontShader->Use().SetMatrix4("projection", glm::ortho(0.0f, (float)w, 0.0f, (float)h));
}

// 渲染单纹理方块
void SpriteRenderer::DrawBlock(Texture2D& texture, glm::vec3 position) {
    this->objectShader->Use();
    glm::mat4 model = setBlockPostion(position);
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    glBindVertexArray(this->quadVAO);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(this->topFace);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(this->bottomFace);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// 渲染三纹理方块
void SpriteRenderer::DrawBlock(Texture2D& top, Texture2D& side, Texture2D& bottom, glm::vec3 position) {
    this->objectShader->Use();
    glm::mat4 model = setBlockPostion(position);

    glActiveTexture(GL_TEXTURE0);

    side.Bind();
    glBindVertexArray(this->quadVAO);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

    top.Bind();
    glBindVertexArray(this->topFace);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    bottom.Bind();
    glBindVertexArray(this->bottomFace);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

// 渲染噪声纹理方块
void SpriteRenderer::DrawBlock(glm::vec3 color, glm::vec3 position) {
    this->objectShader->Use();
    glm::mat4 model = setBlockPostion(position);
    this->objectShader->SetInteger("hasColor", true);
    this->objectShader->SetVector3f("material.color", color); // glm::vec3(0.5, 0.8, 0.4)

    glActiveTexture(GL_TEXTURE0);

    this->noise->Bind();
    glBindVertexArray(this->quadVAO);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

    glBindVertexArray(this->topFace);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    glBindVertexArray(this->bottomFace);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

}

// 渲染上下噪声纹理方块
void SpriteRenderer::DrawBlock(Texture2D& texture, glm::vec3 top, glm::vec3 bottom, glm::vec3 position) {
    this->objectShader->Use();
    glm::mat4 model = setBlockPostion(position);

    glActiveTexture(GL_TEXTURE0);

    texture.Bind();
    glBindVertexArray(this->quadVAO);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

    this->noise->Bind();
    this->objectShader->SetInteger("hasColor", true);
    this->objectShader->SetVector3f("material.color", top); 
    glBindVertexArray(this->topFace);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    this->objectShader->SetVector3f("material.color", bottom);
    glBindVertexArray(this->bottomFace);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

glm::mat4 SpriteRenderer::setBlockPostion(glm::vec3 pos) {
    this->objectShader->Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    this->objectShader->SetMatrix4("model", model);
    this->objectShader->SetInteger("hasTexture", true);
    this->objectShader->SetInteger("hasColor", false);
    this->objectShader->SetInteger("material.diffuse", 0); // 漫反射贴图
    this->objectShader->SetInteger("material.specular", 0); // 镜面反射贴图
    this->objectShader->SetFloat("material.shininess", 32); // 镜面反射率
    return model;
}

// 渲染带纹理的立方体
void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec3 position, glm::vec3 size, GLfloat rotate, glm::vec3 color) {
  this->objectShader->Use();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));

  model = glm::scale(model, size);

  this->objectShader->SetMatrix4("model", model);
  this->objectShader->SetInteger("hasTexture", true);
  this->objectShader->SetInteger("hasColor", true);
  this->objectShader->SetInteger("material.diffuse", 0); // 漫反射贴图
  this->objectShader->SetInteger("material.specular", 0); // 镜面反射贴图
  this->objectShader->SetInteger("material.shininess", 32); // 镜面反射率

  // Render textured quad
  this->objectShader->SetVector3f("material.color", color);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glActiveTexture(GL_TEXTURE0);
  texture.Bind();

  glBindVertexArray(this->quadVAO);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

}



void SpriteRenderer::DrawSprite(Model& modelObj, glm::vec3 position, glm::vec3 size, GLfloat rotate) {
  this->objectShader->Use();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, rotate, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::scale(model, size);

  this->objectShader->SetMatrix4("model", model);
  modelObj.Draw(this->objectShader);
}

void SpriteRenderer::initRenderData()
{
  float vertices[] = {
    // 后面
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // 右下角
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // 左下角
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // 左上角
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // 右上角
    // 正面
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 左下角
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 右下角
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // 右上角
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 左上角
    // 左边
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 右上角
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 左上角
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 左下角
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 右下角
    // 右边
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 左上角
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 右上角
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 右下角
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 左下角
     // 下面
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // 左下角
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // 右下角
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // 右上角
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // 左上角
     // 上面
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // 左上角
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // 右上角
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // 右下角
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f  // 左下角
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


  // 顶部
  float verticesTop[] = {
      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // 左上角
       0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // 右上角
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // 右下角
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f  // 左下角
  };

  unsigned int indicesTop[] = {
    0, 1, 2,
    2, 3, 0,
  };

  glGenVertexArrays(1, &this->topFace);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(this->topFace);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTop), verticesTop, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesTop), indicesTop, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // 底部
  float verticesBottom[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // 左下角
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // 右下角
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // 右上角
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // 左上角
  };

  unsigned int indicesBottom[] = {
    0, 1, 2,
    2, 3, 0,
  };

  glGenVertexArrays(1, &this->bottomFace);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(this->bottomFace);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBottom), verticesBottom, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesBottom), indicesBottom, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

    this->objectShader->SetFloat(("pointLights[" + to_string(this->pointCount) + "].constant").c_str(),
        this->pointLight[this->pointCount].constant);
    this->objectShader->SetFloat(("pointLights[" + to_string(this->pointCount) + "].linear").c_str(),
        this->pointLight[this->pointCount].linear);
    this->objectShader->SetFloat(("pointLights[" + to_string(this->pointCount) + "].quadratic").c_str(),
        this->pointLight[this->pointCount].quadratic);
    this->objectShader->SetVector3f(("pointLights[" + to_string(this->pointCount) + "].position").c_str(),
        this->pointLight[this->pointCount].position);
    this->objectShader->SetVector3f(("pointLights[" + to_string(this->pointCount) + "].ambient").c_str(),
        this->pointLight[this->pointCount].ambient);
    this->objectShader->SetVector3f(("pointLights[" + to_string(this->pointCount) + "].diffuse").c_str(),
        this->pointLight[this->pointCount].diffuse);
    this->objectShader->SetVector3f(("pointLights[" + to_string(this->pointCount) + "].specular").c_str(),
        this->pointLight[this->pointCount].specular);

    this->pointCount++;
    this->objectShader->SetInteger("pointCount", this->pointCount);
}
// 清除点光源
void SpriteRenderer::ClearPointLight() {
    this->pointCount = 0;
    this->objectShader->SetInteger("pointCount", this->pointCount);
}

void SpriteRenderer::RenderSky() {
    glDepthFunc(GL_LEQUAL);
    this->skyShader->Use();
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

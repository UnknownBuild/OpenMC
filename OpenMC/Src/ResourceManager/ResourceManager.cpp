﻿#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, vector<Texture2D>>    ResourceManager::MultiTextures;
std::map<std::string, Shader>       ResourceManager::Shaders;
std::map<std::string, Model>        ResourceManager::Models;
std::map<GLchar, Character>         ResourceManager::Characters;
unsigned int ResourceManager::fontVAO, ResourceManager::fontVBO;

Shader& ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, std::string name, const GLchar* gShaderFile) {
  Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
  return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name) {
  return Shaders[name];
}

Texture2D& ResourceManager::LoadTexture(std::string file, std::string name, bool alpha) {
  Textures[name] = loadTextureFromFile(file.c_str(), alpha);
  return Textures[name];
}

Texture2D& ResourceManager::LoadTexture(vector<std::string> file, std::string name) {
    Textures[name] = loadCubemap(file);
    return Textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name) {
  return Textures[name];
}

Model& ResourceManager::LoadModel(const GLchar* file, std::string name) {
  Models[name] = loadModelFromFile(file);
  return Models[name];
}
Model& ResourceManager::GetModel(std::string name) {
  return Models[name];
}

vector<Texture2D>& ResourceManager::GetSplitTexture(std::string name) {
    return MultiTextures[name];
}

vector<Texture2D>& ResourceManager::LoadSplitTexture(std::string file, std::string name) {
    MultiTextures[name] = loadSplitTexture(file.c_str());
    return MultiTextures[name];
}

void ResourceManager::Clear(){
  for (auto iter : Shaders)
    glDeleteProgram(iter.second.ID);
  for (auto iter : Textures)
    glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar * vShaderFile, const GLchar * fShaderFile, const GLchar* gShaderFile) {
  std::string vertexCode;
  std::string fragmentCode;
  std::string geometryCode;
  try {
    // Open files
    std::ifstream vertexShaderFile(vShaderFile);
    std::ifstream fragmentShaderFile(fShaderFile);
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vertexShaderFile.rdbuf();
    fShaderStream << fragmentShaderFile.rdbuf();
    vertexShaderFile.close();
    fragmentShaderFile.close();
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
    if (gShaderFile != nullptr)
    {
        std::ifstream geometryShaderFile(gShaderFile);
        std::stringstream gShaderStream;
        gShaderStream << geometryShaderFile.rdbuf();
        geometryShaderFile.close();
        geometryCode = gShaderStream.str();
        if (int(geometryCode[0]) == -17 && int(geometryCode[1]) == -69) {
            geometryCode = geometryCode.substr(3);
        }
    }
    // delete bom header
    if (int(vertexCode[0]) == -17 && int(vertexCode[1]) == -69) {
        vertexCode = vertexCode.substr(3);
    }
    if (int(fragmentCode[0]) == -17 && int(fragmentCode[1]) == -69) {
        fragmentCode = fragmentCode.substr(3);
    }
  } catch (std::exception e) {
    std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
  }
  const GLchar* vShaderCode = vertexCode.c_str();
  const GLchar* fShaderCode = fragmentCode.c_str();
  const GLchar* gShaderCode = geometryCode.c_str();
  Shader shader;
  shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
  return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar * file, bool alpha) {
  Texture2D texture;
  int width, height, nrChannels;
  unsigned char* image = stbi_load(file, &width, &height, &nrChannels, 0);
  if (image) {
    GLenum format = GL_RED;
    if (alpha)
        format = GL_RGBA;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    texture.Image_Format = format;
    texture.Internal_Format = format;
  }
  texture.Generate(width, height, image);
  stbi_image_free(image);
  return texture;
}

vector<Texture2D> ResourceManager::loadSplitTexture(const GLchar* file) {
    vector<Texture2D> textures;
    int width, height, nrChannels;
    unsigned char* image = stbi_load(file, &width, &height, &nrChannels, 0);

    int sWidth = 16;
    int sHeight = 16;
    int count = height / sHeight;

    GLenum format = GL_RED;
    if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    for (int i = 0; i < count; i++) {
        Texture2D texture;
        texture.Image_Format = format;
        texture.Internal_Format = format;
        texture.Generate(sWidth, sHeight, image + (nrChannels * sWidth * sHeight) * i);
        textures.push_back(texture);
    }
    stbi_image_free(image);
    return textures;
}

Texture2D ResourceManager::loadCubemap(vector<std::string> faces) {
    Texture2D texture;
    glGenTextures(1, &texture.ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.ID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return texture;
}



Model ResourceManager::loadModelFromFile(const GLchar* file) {
  Model model;
  model.LoadModel(file);
  return model;
}

// 初始化字体
void ResourceManager::InitFont(const GLchar* path) {
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

  FT_Face face;
  if (FT_New_Face(ft, path, 0, &face))
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

  FT_Set_Pixel_Sizes(face, 0, 48);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  for (GLubyte c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RED,
      face->glyph->bitmap.width,
      face->glyph->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        face->glyph->advance.x
    };
    Characters.insert(std::pair<GLchar, Character>(c, character));
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  glGenVertexArrays(1, &fontVAO);
  glGenBuffers(1, &fontVBO);
  glBindVertexArray(fontVAO);
  glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

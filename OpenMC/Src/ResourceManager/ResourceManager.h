#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "Shader.h"
#include "Model.h"
#include "Texture.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

struct Character {
  GLuint TextureID;
  glm::ivec2 Size; 
  glm::ivec2 Bearing;
  GLuint Advance;
};

class ResourceManager
{
public:
  static std::map<std::string, Shader>    Shaders;
  static std::map<std::string, Texture2D> Textures;
  static std::map<std::string, Model> 		Models;
  static std::map<GLchar, Character> 			Characters;
  static unsigned int 										fontVAO, fontVBO;
  static Shader&   LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, std::string name);
  static Shader&   GetShader(std::string name);
  static Texture2D& LoadTexture(const GLchar* file, std::string name, bool alpha = true);
  static Texture2D* LoadTextureSplit(const GLchar* file, int count);
  static Texture2D& LoadTexture(vector<std::string> file, std::string name);
  static Texture2D& GetTexture(std::string name);
  static Model& LoadModel(const GLchar* file, std::string name);
  static Model& GetModel(std::string name);
  static void InitFont(const GLchar* path);
  static void Clear();

private:
  ResourceManager() {}
  static Shader    loadShaderFromFile(const GLchar* vShaderFile, 
                                      const GLchar* fShaderFile);
  static Texture2D loadCubemap(vector<std::string> faces);
  static Texture2D loadTextureFromFile(const GLchar* file, bool alpha);
  static Model     loadModelFromFile(const GLchar* file);
};

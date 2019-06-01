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
  static std::map<std::string, vector<Texture2D>>    MultiTextures;
  static unsigned int 										fontVAO, fontVBO;
  static Shader&   LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, std::string name, const GLchar* gShaderFile = nullptr);
  static Shader&   GetShader(std::string name);
  static Texture2D& LoadTexture(std::string file, std::string name, bool alpha = true);
  static Texture2D& LoadTexture(vector<std::string> file, std::string name);
  static Texture2D& GetTexture(std::string name);
  static vector<Texture2D>& GetSplitTexture(std::string name);
  static vector<Texture2D>& LoadSplitTexture(std::string file, std::string name);
  static Model& LoadModel(const GLchar* file, std::string name);
  static Model& GetModel(std::string name);
  static void InitFont(const GLchar* path);
  static void Clear();

private:
  ResourceManager() {}
  static Shader    loadShaderFromFile(const GLchar* vShaderFile, 
                                      const GLchar* fShaderFile,
                                      const GLchar* gShaderFile = nullptr);
  static Texture2D loadCubemap(vector<std::string> faces);
  static Texture2D loadTextureFromFile(const GLchar* file, bool alpha);
  static Model     loadModelFromFile(const GLchar* file);
  static vector<Texture2D> loadSplitTexture(const GLchar* file);
};

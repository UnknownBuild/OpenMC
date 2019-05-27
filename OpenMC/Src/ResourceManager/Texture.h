#pragma once

#include <glad/glad.h>

class Texture2D {
public:
  unsigned int ID;
  unsigned int Width, Height;
  unsigned int Internal_Format;
  unsigned int Image_Format;
  unsigned int Wrap_S;
  unsigned int Wrap_T;
  unsigned int Filter_Min;
  unsigned int Filter_Max;
  Texture2D()
    : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_MIRRORED_REPEAT), Wrap_T(GL_MIRRORED_REPEAT), Filter_Min(GL_NEAREST), Filter_Max(GL_NEAREST)
  {
    glGenTextures(1, &this->ID);
  }

  void Generate(unsigned int width, unsigned int height, unsigned char* data) {
    this->Width = width;
    this->Height = height;
    // Create Texture
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
    // Set Texture wrap and filter modes

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
  };
    
  void Bind() const {
    glBindTexture(GL_TEXTURE_2D, this->ID);
  };
};

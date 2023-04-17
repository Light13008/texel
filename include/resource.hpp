
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include "shader.hpp"
#include "texture.hpp"
#include <map>
#include <string>

class ResourceManager {
public:
  static std::map<std::string, Shader> Shaders;
  static std::map<std::string, Texture> Textures;

  static Shader LoadShader(std::string name, const char *vertexPath,
                           const char *fragmentPath,
                           const char *geometryPath = nullptr);
  static Texture LoadTexture(std::string name, const char *texturePath,
                             bool alpha);
  static Texture GetTexture(std::string name);
  static Shader GetShader(std::string name);
  static void Clear();

private:
  ResourceManager() {}
  static Shader loadShaderFromFile(const char *vShaderFile,
                                   const char *fShaderFile,
                                   const char *gShaderFile = nullptr);
  static Texture loadTextureFromFile(const char *file, bool alpha);
};

#endif
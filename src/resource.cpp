#include "resource.hpp"
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stb_image.h>
#include <string>

std::map<std::string, Texture> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

Shader ResourceManager::GetShader(std::string name) { return Shaders[name]; }

Texture ResourceManager::GetTexture(std::string name) { return Textures[name]; }

Shader ResourceManager::LoadShader(std::string name, const char *vertexPath,
                                   const char *fragmentPath,
                                   const char *geometryPath) {
  Shaders[name] = loadShaderFromFile(vertexPath, fragmentPath, geometryPath);
  return Shaders[name];
}

Texture ResourceManager::LoadTexture(std::string name, const char *filePath,
                                     bool alpha) {
  Textures[name] = loadTextureFromFile(filePath, alpha);
  return Textures[name];
}

Texture ResourceManager::loadTextureFromFile(const char *textureFile,
                                             bool alpha) {
  Texture texture;
  if (alpha) {
    texture.Internal_Format = GL_RGBA;
    texture.Image_Format = GL_RGBA;
  }
  int width, height, nrChannels;
  unsigned char *data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
  texture.Generate(width, height, data);
  stbi_image_free(data);
  return texture;
}

Shader ResourceManager::loadShaderFromFile(const char *vShaderFile,
                                           const char *fShaderFile,
                                           const char *gShaderFile) {
  std::string vertexCode;
  std::string fragmentCode;
  std::string geometryCode;
  try {
    std::ifstream vertexShaderFile(vShaderFile);
    std::ifstream fragmentShaderFile(fShaderFile);
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vertexShaderFile.rdbuf();
    fShaderStream << fragmentShaderFile.rdbuf();
    vertexShaderFile.close();
    fragmentShaderFile.close();
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
    if (gShaderFile != nullptr) {
      std::ifstream geometryShaderFile(gShaderFile);
      std::stringstream gShaderStream;
      gShaderStream << geometryShaderFile.rdbuf();
      geometryShaderFile.close();
      geometryCode = gShaderStream.str();
    }
  } catch (std::exception e) {
    std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
  }
  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();
  const char *gShaderCode = geometryCode.c_str();
  // 2. now create shader object from source code
  Shader shader;
  shader.Compile(vShaderCode, fShaderCode,
                 gShaderFile != nullptr ? gShaderCode : nullptr);
  return shader;
}

void ResourceManager::Clear() {
  // (properly) delete all shaders
  for (auto iter : Shaders)
    glDeleteProgram(iter.second.ID);
  // (properly) delete all textures
  for (auto iter : Textures)
    glDeleteTextures(1, &iter.second.ID);
}
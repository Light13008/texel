#ifndef LIGHT_H
#define LIGHT_H
#include <glm/glm.hpp>
#include <shader.hpp>
#include <texture.hpp>

class Spotlight {
public:
  glm::vec2 Position;
  glm::vec2 Direction;
  glm::vec3 Ambient;
  glm::vec3 Diffuse;

  float OuterCutoff;
  float InnerCutoff;

  float Constant;
  float Linear;
  float Quadratic;

  Spotlight();
};

class PostProcessor {

public:
  Shader PostProcessingShader;
  Spotlight Light;
  unsigned int Width;
  unsigned int Height;
  PostProcessor(Shader shader, unsigned int width, unsigned int height,
                Spotlight light);
  ~PostProcessor();

  void BeginRender();
  void EndRender();
  void Render();

private:
  unsigned int VAO;
  unsigned int FBO;
  unsigned int texture;
  void initRenderData();
};
#endif
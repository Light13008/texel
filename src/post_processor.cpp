#include <GL/glew.h>
#include <iostream>
#include <post_processor.hpp>

Spotlight ::Spotlight() = default;

PostProcessor::PostProcessor(Shader shader, unsigned int width,
                             unsigned int height, Spotlight light)
    : PostProcessingShader(shader), Width(width), Height(height), Light(light) {
  glGenFramebuffers(1, &this->FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

  glGenTextures(1, &this->texture);
  glBindTexture(GL_TEXTURE_2D, this->texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         this->texture, 0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
    std::cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  this->initRenderData();
}

PostProcessor::~PostProcessor() { glDeleteVertexArrays(1, &this->VAO); }

void PostProcessor::initRenderData() {
  unsigned int VBO;
  float vertices[] = {
      -1.0f, -1.0f, 1.0f, 1.0f,  -1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, -1.0f, 1.0f,  1.0f,
  };
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(this->VAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void PostProcessor::BeginRender() {
  glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
  glClearColor(this->Light.Ambient.r, this->Light.Ambient.g,
               this->Light.Ambient.b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::Render() {
  this->PostProcessingShader.Use();
  this->PostProcessingShader.SetVector2f("resolution", glm::vec2(800, 600));
  this->PostProcessingShader.SetVector3f("light.ambient", this->Light.Ambient);
  this->PostProcessingShader.SetVector3f("light.diffuse", this->Light.Diffuse);
  this->PostProcessingShader.SetVector2f("light.direction",
                                         this->Light.Direction);
  this->PostProcessingShader.SetVector2f("light.position",
                                         this->Light.Position);
  this->PostProcessingShader.SetFloat("light.constant", this->Light.Constant);
  this->PostProcessingShader.SetFloat("light.linear", this->Light.Linear);
  this->PostProcessingShader.SetFloat("light.quadratic", this->Light.Quadratic);
  this->PostProcessingShader.SetFloat("light.outerCutoff",
                                      this->Light.OuterCutoff);
  this->PostProcessingShader.SetFloat("light.innerCutoff",
                                      this->Light.InnerCutoff);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->texture);
  glBindVertexArray(this->VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void PostProcessor::EndRender() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
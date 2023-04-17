#include <GL/glew.h>
#include <iostream>
#include <shader.hpp>

Shader &Shader::Use() {
  glUseProgram(this->ID);
  return *this;
}

void Shader::SetFloat(const char *name, float value, bool useShader) {
  if (useShader)
    this->Use();
  glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetInteger(const char *name, int value, bool useShader) {
  if (useShader)
    this->Use();
  glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetVector2f(const char *name, float x, float y, bool useShader) {
  if (useShader)
    this->Use();
  glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::SetVector2f(const char *name, const glm::vec2 &value,
                         bool useShader) {
  if (useShader)
    this->Use();
  glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::SetVector3f(const char *name, float x, float y, float z,
                         bool useShader) {
  if (useShader)
    this->Use();
  glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::SetVector3f(const char *name, const glm::vec3 &value,
                         bool useShader) {
  if (useShader)
    this->Use();
  glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char *name, float x, float y, float z, float w,
                         bool useShader) {
  if (useShader)
    this->Use();
  glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::SetVector4f(const char *name, const glm::vec4 &value,
                         bool useShader) {
  if (useShader)
    this->Use();
  glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z,
              value.w);
}
void Shader::SetMatrix4(const char *name, const glm::mat4 &matrix,
                        bool useShader) {
  if (useShader)
    this->Use();
  glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false,
                     glm::value_ptr(matrix));
}

void Shader::Compile(const char *vertexSrc, const char *fragmentSrc,
                     const char *geomSrc) {
  unsigned int vertexShader, fragmentShader, geometryShader;
  GLuint shaderProgram = glCreateProgram();

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSrc, NULL);
  glCompileShader(vertexShader);
  checkCompileErrors(vertexShader, "VERTEX");

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
  glCompileShader(fragmentShader);
  checkCompileErrors(vertexShader, "FRAGMENT");

  if (geomSrc != nullptr) {
    geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &geomSrc, NULL);
    glCompileShader(geometryShader);
    checkCompileErrors(geometryShader, "GEOMETRY");
  }
  this->ID = glCreateProgram();
  glAttachShader(this->ID, vertexShader);
  glAttachShader(this->ID, fragmentShader);

  if (geomSrc != nullptr) {
    glAttachShader(this->ID, geometryShader);
  }
  glLinkProgram(this->ID);
  checkCompileErrors(this->ID, "PROGRAM");
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  if (geomSrc != nullptr) {
    glDeleteShader(geometryShader);
  }
}

void Shader::checkCompileErrors(unsigned int object, std::string type) {
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(object, 1024, NULL, infoLog);
      std::cout
          << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
    }
  } else {
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(object, 1024, NULL, infoLog);
      std::cout
          << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
    }
  }
}
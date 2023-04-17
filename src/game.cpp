#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <const.hpp>
#include <game.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <post_processor.hpp>
#include <resource.hpp>
#include <sprite.hpp>

SpriteRenderer *renderer;

Game::Game(unsigned int width, unsigned int height)
    : state(PLAYING), width(width), height(height) {}

Game::~Game() { delete renderer; }

unsigned int Game::Init() {
  ResourceManager::LoadShader("sprite", "shaders/sprite.vert",
                              "shaders/sprite.frag");
  ResourceManager::LoadShader("post_processor", "shaders/lighting.vert",
                              "shaders/lighting.frag");
  glm::mat4 projection =
      glm::ortho(0.0f, static_cast<float>(this->width),
                 static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
  ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
  ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
  ResourceManager::GetShader("post_processor").Use().SetInteger("texture", 0);

  Shader spriteShader = ResourceManager::GetShader("sprite");
  Shader postProcShader = ResourceManager::GetShader("post_processor");
  // set render-specific controls
  renderer = new SpriteRenderer(spriteShader);
  Spotlight light;
  light.Ambient = glm::vec3(0.2f);
  light.Diffuse = glm::vec3(0.8f);
  light.Direction = glm::vec2(0, -1);
  light.Position = glm::vec2(.5, 1.);
  light.Constant = 1.0f;
  light.Linear = 0.001;
  light.Quadratic = 0.0;
  light.OuterCutoff = 0.2;
  light.InnerCutoff = 0.1;
  postProcessor =
      new PostProcessor(postProcShader, this->width, this->height, light);
  // load textures
  ResourceManager::LoadTexture("face", "assets/face.png", true);
}

void Game::Update(float dt) {}

bool Game::Poll(float dt) {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_QUIT ||
        e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
      return true;
    }
  }
  return false;
}

void Game::Render() {
  Texture face = ResourceManager::GetTexture("face");
  postProcessor->BeginRender();
  renderer->DrawSprite(face, glm::vec2(200.0f, 200.0f),
                       glm::vec2(300.0f, 400.0f), 45.0f,
                       glm::vec3(1.0f, 1.0f, 1.0f));
  postProcessor->EndRender();
  postProcessor->Render();
}
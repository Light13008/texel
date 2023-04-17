#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <const.hpp>
#include <game.hpp>
#include <iostream>
#include <resource.hpp>

using namespace std;

int main(int argc, char *args[]) {
  SDL_Window *window;
  SDL_GLContext context;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Failed to initialize SDL\n";
    return EXIT_FAILURE;
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, TEXEL_OPENGL_MAJOR_VERSION);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, TEXEL_OPENGL_MINOR_VERSION);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, TEXEL_CONTEXT_PROFILE);
  SDL_ShowCursor(SDL_DISABLE);
  window = SDL_CreateWindow("TEXEL", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                            WINDOW_HEIGHT, TEXEL_WINDOW_FLAGS);
  if (!window) {
    std::cout << "Failed to create SDL window: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return EXIT_FAILURE;
  }
  context = SDL_GL_CreateContext(window);
  if (context == NULL) {
    std::cout << "OpenGL context could not be created! SDL Error: "
              << SDL_GetError() << std::endl;
    SDL_Quit();
    return EXIT_FAILURE;
  }
  if (SDL_GL_SetSwapInterval(1) < 0) {
    std::cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return EXIT_FAILURE;
  }
  GLenum glewStatus = glewInit();
  if (glewStatus != GLEW_OK) {
    std::cout << "Failed to initialize GLEW: " << glewStatus << std::endl;
    SDL_Quit();
    return EXIT_FAILURE;
  }
  glEnable(GL_BLEND);
  glEnable(GL_MULTISAMPLE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  Game game = Game(WINDOW_WIDTH, WINDOW_HEIGHT);
  unsigned int result = game.Init();
  bool quit = false;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  while (!quit) {
    float currentFrame = SDL_GetTicks();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    quit = game.Poll(deltaTime);
    game.Update(deltaTime);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    game.Render();
    SDL_GL_SwapWindow(window);
  }
  ResourceManager::Clear();
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
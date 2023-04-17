#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <post_processor.hpp>
#include <sprite.hpp>

enum GameState { PLAYING, PAUSED, MENU };

class Game {
public:
  GameState state;
  unsigned int Init();
  bool Poll(float dt);
  void Update(float dt);
  void Render();
  Game(unsigned int width, unsigned int height);
  ~Game();

private:
  unsigned int width;
  unsigned int height;
  SDL_Window *window;
  SDL_GLContext context;
  SpriteRenderer *renderer;
  PostProcessor *postProcessor;
};

#endif
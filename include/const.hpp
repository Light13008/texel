#ifndef CONST_H
#define CONST_H
#include <SDL2/SDL.h>

static const unsigned int WINDOW_WIDTH = 800;
static const unsigned int WINDOW_HEIGHT = 600;

#if defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) ||                     \
    defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) ||                    \
    defined(__ARM_ARCH_6ZK__)
static const SDL_GLprofile TEXEL_CONTEXT_PROFILE = SDL_GL_CONTEXT_PROFILE_ES;
static const int TEXEL_OPENGL_MAJOR_VERSION = 2;
static const int TEXEL_OPENGL_MINOR_VERSION = 0;
static const Uint32 TEXEL_WINDOW_FLAGS =
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
#else
static const SDL_GLprofile TEXEL_CONTEXT_PROFILE = SDL_GL_CONTEXT_PROFILE_CORE;
static const int TEXEL_OPENGL_MAJOR_VERSION = 3;
static const int TEXEL_OPENGL_MINOR_VERSION = 2;
static const Uint32 TEXEL_WINDOW_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
#endif

#endif

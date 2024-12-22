#ifndef TEXMANAGER_H
#define TEXMANAGER_H
#include "SDL.h"


struct texManager {
static SDL_Texture* loadTex(const char* texture);
static void drawTex(SDL_Texture* tex, SDL_Rect* srcRect, SDL_Rect* destRect);
static void drawTex(SDL_Texture* tex, int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2, int mir);
};

#endif
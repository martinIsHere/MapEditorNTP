#include "texManager.h"
#include "Game.h"
#include "SDL_image.h"


SDL_Texture* texManager::loadTex(const char* texture) {
	//SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return tex;
}
void texManager::drawTex(SDL_Texture* tex, SDL_Rect* srcRect, SDL_Rect* destRect) {
	SDL_RenderCopy(Game::renderer, tex, srcRect, destRect);
}
void texManager::drawTex(SDL_Texture* tex, int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2, int mir) {
	SDL_Rect srcRect;
	srcRect.x = x1;
	srcRect.y = y1;
	srcRect.w = w1;
	srcRect.h = h1;
	SDL_Rect destRect;
	destRect.x = x2;
	destRect.y = y2;
	destRect.w = w2;
	destRect.h = h2;

	SDL_RenderCopyEx(Game::renderer, tex, &srcRect, &destRect, 0, 0, (SDL_RendererFlip)mir);
}
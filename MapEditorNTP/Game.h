#ifndef GAME_H
#define GAME_H
#include <iostream>
#include "SDL.h"
#include "fstream"

class Game {
public:

	// tile sheet
	int TilesOnTileSheet;
	int TileSheetSizex;
	int TileSheetSizey;
	int Tilesx;
	int Tilesy;
	int TileSizex = 16;
	int TileSizey = 16;


	// app
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	int screenWidth;
	int screenHeight;
	bool running;


	// map
	int tileWidth = 64;
	int tileHeight = 64;
	int visibleTilesx;
	int visibleTilesy;
	float offsetx;
	float offsety;
	int mx = 0;
	int my = 0;
	uint16_t width;
	uint16_t height;
	uint16_t* foreGroundIndeces = nullptr;
	uint16_t* indeces = nullptr;
	uint16_t* backIndeces = nullptr;
	uint8_t* solids = nullptr;
	uint8_t* foregroundMirrorState = nullptr;
	uint8_t* frontMirrorState = nullptr;
	uint8_t* backMirrorState = nullptr;
	int currentTileWidth;
	int currentTileHeight;
	int currentSelectedBlock = 5;
	bool mb1 = false;
	bool mb2 = false;
	bool c = false;
	bool xButton = false;
	bool q = false;
	bool cu = false;
	bool ki = false;
	bool changeForegroundMirrorState = false;
	int indexX1D;
	int indexX2D;
	int indexY2D;
	int mToGridx;
	int mToGridy;
	int mToTSGridx;
	int mToTSGridy;
	int x, y;
	SDL_Rect a;
	std::fstream writer;

	// handy rectangles
	SDL_Rect mapArea{ 20,20,768,768 };
	SDL_Rect TileSheetArea{ 840, 20, 540, 760 };
	SDL_Rect hideRectx{ mapArea.w+mapArea.x, 20, 80, mapArea.h+100 };
	SDL_Rect hideRecty{ 20, mapArea.h+mapArea.y, mapArea.w+100, 80 };

	Game();

	int getTile(int x, int y)const;

	int getTile2(int x, int y)const;

	int getTileForeground(int x, int y) const;

	int getForegroundMirrorState(int x, int y) const;

	int getTile3(int x, int y) const;

	int getTile4(int x, int y) const;

	void setTile(int x, int y, int i);

	void setTile2(int x, int y, int i);

	void setTileForeground(int x, int y, int i);

	void init(const char* title, int&& w, int&& h);

	void handleEvents();

	void update();

	void draw();

	void clean();



};

#endif



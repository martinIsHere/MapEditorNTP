#include "Game.h"
#include "SDL.h"
#include "texManager.h"

SDL_Window* Game::window;
SDL_Renderer* Game::renderer;



bool running;
SDL_Event event;

float dirx = 0;
float diry = 0;

Game::Game() {
	running = true;
}

SDL_Texture* MapTex = nullptr;
SDL_Texture* grassBlock = nullptr;


void Game::init(const char* title, int&& w, int&& h) {
	screenWidth = w;
	screenHeight = h;

	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);

	

	std::fstream data("res/map/Town1.bin", std::ios::in | std::ios::binary);


	char tBuffer[2];

	// get first 2 bytes
	data.read(tBuffer, 2);
	width = tBuffer[1] & 0xFFFF;

	// get 2 bytes for height
	data.read(tBuffer, 2);
	height = tBuffer[1] & 0xFFFF;

	foreGroundIndeces = new uint16_t[size_t(width * height)];
	indeces = new uint16_t[size_t(width * height)];
	backIndeces = new uint16_t[size_t(width * height)];
	solids = new uint8_t[size_t(width * height)];
	backMirrorState = new uint8_t[width * height];
	frontMirrorState = new uint8_t[width * height];
	foregroundMirrorState = new uint8_t[width * height];
	
	//for (int i = 0; i < size_t(width * height); i++) {
	//	foreGroundIndeces[i] = 0;
	//}

	//m_frontLayer_array = new uint16_t[m_mapWidth*m_mapHeight];
	//m_backLayer_array = new uint16_t[m_mapWidth*m_mapHeight];
	//m_blockState_array = new bool[m_mapWidth * m_mapHeight];

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			// foreground indeces
			data.read(tBuffer, 2);
			int currentID = (tBuffer[0] & 0xFF) << 8;
			currentID += tBuffer[1] & 0xFF;
			foreGroundIndeces[y * width + x] = currentID;

			// front id
			data.read(tBuffer, 2);
			currentID = (tBuffer[0] & 0xFF)<<8;
			currentID += tBuffer[1] & 0xFF;
			indeces[y * width + x] = currentID;
			//m_frontLayer_array[x*y] = m_current_ID;


			// back id
			data.read(tBuffer, 2);
			currentID = (tBuffer[0] & 0xFF) << 8;
			currentID += tBuffer[1] & 0xFF;
			backIndeces[y * width + x] = currentID;
			//m_backLayer_array[x * y] = m_current_ID;


			
			// state id
			data.read(tBuffer, 1);
			currentID = tBuffer[0] & 0b1;

			solids[y * width + x] = currentID != 0;

			backMirrorState[y * width + x] = ((tBuffer[0] >> 1) & 0b11);
			frontMirrorState[y * width + x] = ((tBuffer[0] >> 3) & 0b11);
			foregroundMirrorState[y * width + x] = ((tBuffer[0] >> 5) & 0b11);

		}
	}

	data.close();


	MapTex = texManager::loadTex("res/img/ssh1.png");
	grassBlock = texManager::loadTex("res/img/grass-tile-3.png");
	SDL_QueryTexture(MapTex, 0, 0, &TileSheetSizex, &TileSheetSizey);
	Tilesx = TileSheetSizex / TileSizex;
	Tilesy = TileSheetSizey / TileSizey;
	TilesOnTileSheet = Tilesx * Tilesy;
	visibleTilesx = mapArea.w / TileSizex;
	visibleTilesy = mapArea.h / TileSizey;
	
	offsetx = mapArea.x;
	offsety = mapArea.y;
	
		currentTileWidth = TileSheetArea.w / Tilesx;
		currentTileHeight = TileSheetArea.h / Tilesy;
	
}

void Game::handleEvents() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				mb1 = true;
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				mb2 = true;
			}
			break;
		case SDL_MOUSEMOTION:
			mx = event.motion.x;
			my = event.motion.y;
			break;

		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				running = false;
				break;
			case SDLK_w:
				diry = -1;
				break;
			case SDLK_s:
				diry = 1;
				break;
			case SDLK_a:
				dirx = -1;
				break;
			case SDLK_d:
				dirx = 1;
				break;
			case SDLK_c:
				c = true;
				break;
			case SDLK_x:
				xButton = true;
				break;
			case SDLK_n:
				cu = true;
				break;
			case SDLK_m:
				ki = true;
				break;
			case SDLK_j:
				changeForegroundMirrorState = true;
				break;
			};
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_w:
				diry = 0;
				break;
			case SDLK_s:
				diry = 0;
				break;
			case SDLK_a:
				dirx = 0;
				break;
			case SDLK_d:
				dirx = 0;
				break;

			case SDLK_h:
				writer.open("res/map/Town1.bin", std::fstream::out | std::fstream::binary);
				unsigned char tBuffer;
				tBuffer = (width >> 8) & 0xFF;
				writer << tBuffer;
				tBuffer = width & 0xFF;
				writer << tBuffer;

				tBuffer = (height >> 8) & 0xFF;
				writer << tBuffer;
				tBuffer = height & 0xFF;
				writer << tBuffer;

				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {

						tBuffer = (foreGroundIndeces[y * width + x] >> 8) & 0xff;
						writer << tBuffer;
						tBuffer = (foreGroundIndeces[y * width + x]) & 0xff;
						writer << tBuffer;

						tBuffer = (indeces[y * width + x] >> 8) & 0xff;
						writer << tBuffer;
						tBuffer = (indeces[y * width + x]) & 0xff;
						writer << tBuffer;
					
						tBuffer = (backIndeces[y * width + x] >> 8) & 0xff;
						writer << tBuffer;
						tBuffer = (backIndeces[y * width + x]) & 0xff;
						writer << tBuffer;

						tBuffer = solids[y * width + x] & 0b1;
						//tBuffer += (frontMirrorState[y*width+x] & 0b11)<<1;
						tBuffer += (backMirrorState[y * width + x] & 0b11) << 1;
						tBuffer += (frontMirrorState[y * width + x] & 0b11) << 3;
						tBuffer += (foregroundMirrorState[y * width + x] & 0b11) << 5;
						writer << tBuffer;

						// counseling the office
					
					}
				}
				writer.close();
				std::cout << "saved map!\n";
				break;
			case SDLK_q:
				q = true;
				break;
			};
			break;

		};
	};
}

void Game::update() {

}

int Game::getTile(int x, int y) const {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		return indeces[y * width + x];
	}
	else { return 0; }
}


int Game::getTile2(int x, int y) const {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		return backIndeces[y * width + x];
	}
	else { return 0; }
}

int Game::getTileForeground(int x, int y) const {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		return foreGroundIndeces[y * width + x];
	}
	else { return 0; }
}

int Game::getForegroundMirrorState(int x, int y) const {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		return foregroundMirrorState[y * width + x];
	}
	else { return 0; }
}

int Game::getTile3(int x, int y) const {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		return frontMirrorState[y * width + x];
	}
	else { return 0; }
}

int Game::getTile4(int x, int y) const {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		return backMirrorState[y * width + x];
	}
	else { return 0; }
}

void Game::setTile(int x, int y, int i) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		indeces[y * width + x] = i;
	}
}

void Game::setTile2(int x, int y, int i) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		backIndeces[y * width + x] = i;
	}
}

void Game::setTileForeground(int x, int y, int i) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		foreGroundIndeces[y * width + x] = i;
	}
}

void Game::draw() {
	
	offsetx += dirx * tileWidth;
	offsety += diry * tileHeight;

	if (offsetx < mapArea.x) { offsetx = mapArea.x; }
	if (offsety < mapArea.y) { offsety = mapArea.y; }

	if (offsetx > (width * tileWidth) - mapArea.w) offsetx = (width * tileWidth) - mapArea.w;
	if (offsety > (height * tileHeight) - mapArea.h) offsety = (height * tileHeight) - mapArea.h;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	
	

	
	//SDL_SetRenderDrawColor(renderer, 0, 150, 0, 0);
	//SDL_RenderFillRect(renderer, &mapArea);

	
	SDL_RenderDrawRect(renderer, &mapArea);

	if (MapTex == nullptr) std::cout << "error loading texture/n"; 
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
	for (int y = 0; y < visibleTilesx; y++) {
		for (int x = 0; x < visibleTilesy; x++) {

			indexX1D = getTile2(x + int(offsetx / tileWidth), y + int(offsety / tileHeight));
			indexX2D = indexX1D % Tilesx;
			indexY2D = indexX1D / Tilesx;


			if (x * tileWidth < mapArea.w && y * tileWidth < mapArea.h) {
				texManager::drawTex(MapTex, indexX2D * TileSizex, indexY2D * TileSizey, TileSizex, TileSizey, x * tileWidth + 20, y * tileHeight + 20, tileWidth, tileHeight, (SDL_RendererFlip)getTile4(x + int(offsetx / tileWidth), y + int(offsety / tileHeight)));

			}

			indexX1D = getTile(x + int(offsetx/tileWidth), y + int(offsety/tileHeight));
			indexX2D = indexX1D % Tilesx;
			indexY2D = indexX1D / Tilesx;

			if (x * tileWidth < mapArea.w && y * tileWidth < mapArea.h) {
				texManager::drawTex(MapTex, indexX2D * TileSizex, indexY2D * TileSizey, TileSizex, TileSizey, x * tileWidth + 20, y * tileHeight + 20, tileWidth, tileHeight, (SDL_RendererFlip)getTile3(x + int(offsetx / tileWidth), y + int(offsety / tileHeight)));
			
			}

			indexX1D = getTileForeground(x + int(offsetx / tileWidth), y + int(offsety / tileHeight));
			indexX2D = indexX1D % Tilesx;
			indexY2D = indexX1D / Tilesx;

			if (x * tileWidth < mapArea.w && y * tileWidth < mapArea.h) {
				texManager::drawTex(MapTex, indexX2D * TileSizex, indexY2D * TileSizey, TileSizex, TileSizey, x * tileWidth + 20, y * tileHeight + 20, tileWidth, tileHeight, (SDL_RendererFlip)getForegroundMirrorState(x + int(offsetx / tileWidth), y + int(offsety / tileHeight)));

			}

			if (solids[y * width + x]) {
				SDL_Rect destRect;
				destRect.x = (x*tileWidth+mapArea.x*2) - int(offsetx);
				destRect.y = (y* tileWidth+ mapArea.y*2) - int(offsety);
				destRect.w = tileWidth;
				destRect.h = tileWidth;
				SDL_RenderDrawRect(renderer, &destRect);
			}

		}
	}

	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
	//SDL_RenderFillRect(renderer, new SDL_Rect{100,100,64,64});
	/*
	for (int y = 0; y < visibleTilesx; y++) {
		for (int x = 0; x < visibleTilesy; x++) {
			indexX1D = getTile(x + int(offsetx / tileWidth), y + int(offsety / tileHeight));
			indexX2D = indexX1D % Tilesx;
			indexY2D = indexX1D / Tilesx;


				texManager::drawTex(MapTex, 
									indexX2D * TileSizex, indexY2D * TileSizey, 
									TileSizex,
									TileSizey, x * tileWidth + 20, y * tileHeight + 20,
									tileWidth,
									tileHeight
								);



				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
				a = { x * tileWidth + 20 + 5, y * tileHeight + 20, 5, 5 };

			a = { x * tileWidth + 20, y * tileHeight + 20, 5, 5 };
			if (solids[(y + int(offsety / tileHeight)) * width + (x + int(offsetx / tileWidth))]) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
				SDL_RenderFillRect(renderer, &a);
			}
		}
	}
	*/
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(renderer, &hideRectx);
	SDL_RenderFillRect(renderer, &hideRecty);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
	SDL_RenderDrawRect(renderer, &TileSheetArea);

	texManager::drawTex(MapTex, 
						0, 
						0, 
						TileSheetSizex,
						TileSheetSizey, 
						TileSheetArea.x+1, 
						TileSheetArea.y+1,
						TileSheetArea.w-2, 
						TileSheetArea.h-2, 
						0);

	
	for (int y = 0; y < Tilesy; y++) {
		for (int x = 0; x < Tilesx; x++) {
			a = { x * currentTileWidth + TileSheetArea.x,     y * currentTileHeight + TileSheetArea.y,    currentTileWidth,    currentTileHeight + 2 };
			SDL_SetRenderDrawColor(renderer,255,0,0,0);
			SDL_RenderDrawRect(renderer, &a);
		}
	}
	

	
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);


	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0);
	SDL_RenderDrawRect(renderer, new SDL_Rect{ (currentSelectedBlock % Tilesx * currentTileWidth) + TileSheetArea.x, (int(currentSelectedBlock / Tilesx) * currentTileHeight) + TileSheetArea.y, currentTileWidth, currentTileHeight + 2});


	mToGridx = int(((mx-mapArea.x) / tileWidth) * tileWidth)+mapArea.x;
	mToGridy = int(((my-mapArea.y) / tileHeight) * tileHeight)+mapArea.y;

	if (mToGridx < mapArea.x + mapArea.w && mToGridy < mapArea.y + mapArea.h) {

		if (mb1) {
			setTile((mToGridx+offsetx)/tileWidth, (mToGridy + offsety) /tileHeight, currentSelectedBlock);
		}
		if (mb2) {
			y = (mToGridy + offsety) / tileHeight;
			x = (mToGridx + offsetx) / tileWidth;

			if (!solids[y * width + x]) {
				solids[y * width + x] = true;
			}
			else { solids[y * width + x] = false; }
		}
		if (c) {
			setTile2((mToGridx + offsetx) / tileWidth, (mToGridy + offsety) / tileHeight, currentSelectedBlock);
		}

		if (xButton) {
			setTileForeground((mToGridx + offsetx) / tileWidth, (mToGridy + offsety) / tileHeight, currentSelectedBlock);
		}

		if (cu) {
			y = (mToGridy + offsety) / tileHeight;
			x = (mToGridx + offsetx) / tileWidth;

			
			backMirrorState[y * width + x] += 1;
			backMirrorState[y * width + x] %= 4;

			std::cout << (int)backMirrorState[y * width + x] << "\n";

		}
		if (ki) {
			y = (mToGridy + offsety) / tileHeight;
			x = (mToGridx + offsetx) / tileWidth;


			frontMirrorState[y * width + x] += 1;
			frontMirrorState[y * width + x] %= 4;
			std::cout << (int)frontMirrorState[y * width + x] << "\n";
		}
		if (changeForegroundMirrorState) {
			y = (mToGridy + offsety) / tileHeight;
			x = (mToGridx + offsetx) / tileWidth;


			foregroundMirrorState[y * width + x] += 1;
			foregroundMirrorState[y * width + x] %= 4;
			std::cout << (int)foregroundMirrorState[y * width + x] << "\n";
		}
		cu = false;
		ki = false;
		changeForegroundMirrorState = false;
		c = false;
		xButton = false;
		if (q) {
			y = (mToGridy + offsety) / tileHeight;
			x = (mToGridx + offsetx) / tileWidth;
		}
		a = { mToGridx, mToGridy, tileWidth, tileHeight };
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderDrawRect(renderer, &a);
	}

	mToTSGridx = int(((mx - TileSheetArea.x) / currentTileWidth) * currentTileWidth) + TileSheetArea.x;
	mToTSGridy = int(((my - TileSheetArea.y) / currentTileHeight) * currentTileHeight) + TileSheetArea.y;
	
	if (mToTSGridx < TileSheetArea.x + TileSheetArea.w && mToTSGridy < TileSheetArea.y + TileSheetArea.h && mToTSGridx >= TileSheetArea.x && mToTSGridy >= TileSheetArea.y) {
		if (mb1) {
			currentSelectedBlock = ((mToTSGridy - TileSheetArea.y) / currentTileHeight) * Tilesx + ((mToTSGridx - TileSheetArea.x) / currentTileWidth);
		}
		a = { mToTSGridx, mToTSGridy, currentTileWidth, currentTileHeight };
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderDrawRect(renderer, &a);
	}
	


	mb1 = false;
	mb2 = false;
	q = false;
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}



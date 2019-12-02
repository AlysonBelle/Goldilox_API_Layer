# include "rt.h"
# include "SDL.hpp"

void SDL::init( int width, int height ) {
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		throw "Failed to init SDL!";
	}
	//SDL_WINDOWPOS_UNDEFINED
	if (!(this->window = SDL_CreateWindow("RT", 50, 50,
	width, height, SDL_WINDOW_SHOWN))) {
		throw "Failed to create window!";
	}
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
	this->width = width;
	this->height = height;
}

SDL::SDL( int width, int height ) {
	this->init(width, height);
}

SDL::SDL( SDL &object ) {
	*this = object;
}

SDL::~SDL( void ) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL	&SDL::operator=( SDL const &object ) {
	this->window = object.window;
	return *this;
}

void	SDL::display( void ) {
		handleInput();
		SDL_RenderPresent(renderer);
}

void	SDL::putpixel(unsigned int x, unsigned int y, Color c) {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
	SDL_RenderDrawPoint(renderer, x, y);
}

void	SDL::putpixel(unsigned int x, unsigned int y, unsigned int colour) {
	SDL_SetRenderDrawColor(renderer, colour >> 16, (colour >> 8) % 256, colour % 256, 255);
	SDL_RenderDrawPoint(renderer, x, y);
}

void	SDL::drawLine(int x1, int y1, int x2, int y2, Color c) {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void	SDL::clearpixels( void ) {
	SDL_RenderClear(renderer);
}

eKey  SDL::handleInput( void ) {
	SDL_Event	event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT) {
			exit(0);
		}
		int keyCode = event.key.keysym.sym;

		if (keyCode == SDLK_ESCAPE) {
			exit(0);
			return eKExit;
		}
		if (keyCode == SDLK_UP) {
			return eKUp;
		}
		if (keyCode == SDLK_DOWN) {
			return eKDown;
		}
		if (keyCode == SDLK_RIGHT) {
			return eKRight;
		}
		if (keyCode == SDLK_LEFT) {
			return eKLeft;
		}
		if (keyCode == SDLK_w) {
			return eKForward;
		}
		if (keyCode == SDLK_a) {
			return eKBackward;
		}
		if (keyCode == SDLK_d) {
			return eKRight;
		}
		if (keyCode == SDLK_a) {
			return eKLeft;
		}
	}
	return eKNone;
}

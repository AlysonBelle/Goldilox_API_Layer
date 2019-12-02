#ifndef SDL_H
# define SDL_H

# include <SDL2/SDL.h>

# include "rt.h"
# include "color.h"

class SDL {

public:
	SDL( int width, int height );
	SDL( SDL &object );
	~SDL( void );
	SDL &operator=( SDL const &object );
	void init( int width, int height );
	eKey handleInput() ;
	void display() ;
    void putpixel(unsigned int x, unsigned int y, unsigned int colour) ;
	void putpixel(unsigned int x, unsigned int y, Color c);

	void saveImage( char *name );
	void drawLine(int x1, int y1, int x2, int y2, Color c);

    void clearpixels() ;

	unsigned int	width;
	unsigned int	height;

	SDL_Window		*window;
	SDL_Renderer	*renderer;

private:

};

#endif

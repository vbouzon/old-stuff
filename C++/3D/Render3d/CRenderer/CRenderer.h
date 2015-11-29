#ifndef CRENDERER_H
#define CRENDERER_H

#include <windows.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>

class CRenderer
{
	int Xres,Yres;
	const SDL_VideoInfo* VideoInfo;
	SDL_Surface *Screen;

public:
	bool Init(int Width,int Height);
	void Config();
};
             
#endif

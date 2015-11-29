#include "CRenderer.h"


bool CRenderer::Init(int width, int height)
{
	Xres = width;
	Yres = height;
	if (SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		std::cout << "Erreur d'initialisation d'un periferique SDL: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}
	atexit(SDL_Quit);
	VideoInfo = SDL_GetVideoInfo();

	if (!VideoInfo)
	{
		std::cout << "Erreur video: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	Screen = SDL_SetVideoMode(Xres, Yres, VideoInfo->vfmt->BitsPerPixel , SDL_OPENGL);

	if (!Screen)
	{
		std::cout << "Impossible d initialiser le mode " << Xres << "x" << Yres << " " << VideoInfo->vfmt->BitsPerPixel << std::endl;
		SDL_Quit();
		return false;
	}

	return true;

}
void CRenderer::Config()
{
    float ratio = (float) Xres / (float) Yres;

    /* Our shading model--Gouraud (smooth). */
    glShadeModel( GL_SMOOTH );

    /* Culling. */
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

    /* Set the clear color. */
    glClearColor( 0, 0, 0, 0 );

    /* Setup our viewport. */
    glViewport( 0, 0, Xres, Yres );

    /*
     * Change to the projection matrix and set
     * our viewing volume.
     */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    /*
     * EXERCISE:
     * Replace this with a call to glFrustum.
     */
    gluPerspective( 60.0, ratio, 1.0, 1024.0 );

}

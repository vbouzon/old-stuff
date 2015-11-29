//---------------------------------------------------------------//
// OpenGL.cpp
//
// Implémentation des fonction liées à OpenGL.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include "OpenGL.h"



//---------------------------------------------------------------//
// Fonctions
//---------------------------------------------------------------//

// OpenGLInitialiser
// -> Fonction d'initialisation de la librairie OpenGL.
//---------------------------------------------------------------//
BOOL OpenGLInitialiser(HDC &hdc, HGLRC &hrc)
{
	// Description du format de pixel
	PIXELFORMATDESCRIPTOR pfd ;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER |
				  PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	// Choix du format de pixel
	int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (nPixelFormat == 0)
		return FALSE;

	// Mise en place du format de pixel
	if (!SetPixelFormat (hdc, nPixelFormat, &pfd))
		return FALSE;

	// Création du Rendering Context
	if (!(hrc = wglCreateContext(hdc)))
		return FALSE;

	// Activation des contextes OpenGL
	if (!wglMakeCurrent(hdc, hrc))
		return FALSE;

	return TRUE;
}



// OpenGLInitialiser
// -> Fonction de desactivation de la librairie OpenGL.
//---------------------------------------------------------------//
void OpenGLDetruire(HWND hWnd, HDC &hdc, HGLRC &hrc)
{
	// Désactivation des contextes OpenGL
	wglMakeCurrent(NULL, NULL);

	// Destruction du Rendering Context
	if (hrc)
		wglDeleteContext(hrc);

	// Destruction du Device Context
	if (hdc)
		ReleaseDC(hWnd, hdc);
}
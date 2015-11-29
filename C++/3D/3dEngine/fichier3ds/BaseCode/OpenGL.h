//---------------------------------------------------------------//
// OpenGL.h
//
// Définit les fonctions liées à OpenGL.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//

#ifndef OpenGL_h
#define OpenGL_h



//---------------------------------------------------------------//
// Liens
//---------------------------------------------------------------//

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include <windows.h>

#include <gl\gl.h>
#include <gl\glu.h>



//---------------------------------------------------------------//
// Fonctions
//---------------------------------------------------------------//

BOOL OpenGLInitialiser(HDC &hdc, HGLRC &hrc);
void OpenGLDetruire(HWND hWnd, HDC &hdc, HGLRC &hrc);


#endif
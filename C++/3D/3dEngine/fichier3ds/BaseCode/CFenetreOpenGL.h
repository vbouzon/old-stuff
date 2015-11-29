//---------------------------------------------------------------//
// CFenetreOpenGL.h
//
// Définition de la classe CFenetreOpenGL et de ses fonctions
// liées.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//

#ifndef CFenetreOpenGL_h
#define CFenetreOpenGL_h



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include <windows.h>

#include "OpenGL.h"



//---------------------------------------------------------------//
// CFenetreOpenGL
//
// -> Représente une fenêtre Windows dont le format de pixel et
// -> les contextes d'affichage permettent la prise en charge
// -> OpenGL.
//---------------------------------------------------------------//
class CFenetreOpenGL
{
	// Attributs ------------------------------------------------//

	public:

		HDC m_hDC;
		HGLRC m_hRC;
		HWND m_hWnd;

		unsigned int m_uiPosX, m_uiPosY;
		unsigned int m_uiDimX, m_uiDimY;

		char m_sTitre[255];

		bool m_bPleinEcran;

		void (* m_fnDessin)(void);
		void (* m_fnOnClose)(void);
		void (* m_fnOnEraseBkgnd)(void);
		void (* m_fnOnSize)(CFenetreOpenGL *, unsigned int, unsigned int, unsigned int);

	// Construction / Destruction -------------------------------//

	public:

		CFenetreOpenGL(void);
		~CFenetreOpenGL(void);

	// Méthodes -------------------------------------------------//

	public:

		void Detruire(void);

		void Creer(char * titre = "", unsigned int pos_x = 0,
				   unsigned int pos_y = 0, unsigned int dim_x = 640,
				   unsigned int dim_y = 480, bool m_pleinecran = false);
};



//---------------------------------------------------------------//
// Fonctions
//---------------------------------------------------------------//

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void fnDessinDefaut(void);
void fnOnCloseDefaut(void);
void fnOnEraseBkgndDefaut(void);
void fnOnSizeDefaut(CFenetreOpenGL * fenetre, unsigned int flag, unsigned int dim_x, unsigned int dim_y);



#endif
//---------------------------------------------------------------//
// CFenetreOpenGL.cpp
//
// Impl�mentation des m�thodes et des fonction li�es � la classe
// CFenetreOpenGL.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include "CFenetreOpenGL.h"



//---------------------------------------------------------------//
// Fonctions
//---------------------------------------------------------------//

// WndProc
// -> Fonction de traitement des messages Windows li�s � cette
// -> fen�tre OpenGL
//---------------------------------------------------------------//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
						 LPARAM lParam)
{
	//-> R�cup�ration du pointeur de la fen�tre pass� en param�tre
	CFenetreOpenGL * fenetre = (CFenetreOpenGL *)GetWindowLong(hWnd, GWL_USERDATA);

	switch (uMsg)
	{
		case WM_CLOSE:
			fenetre->m_fnOnClose();
			break;
		
		case WM_ERASEBKGND:
			fenetre->m_fnOnEraseBkgnd();
			break;
		
		case WM_SIZE:
			if (fenetre)
				fenetre->m_fnOnSize(fenetre, wParam, LOWORD(lParam), HIWORD(lParam));
			break;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}



// -> Fonctions par d�faut
//---------------------------------------------------------------//
void fnOnEraseBkgndDefaut(void) {return;}



// fnOnSizeDefaut
// -> Fonction de redimensionnement par d�faut
//---------------------------------------------------------------//
void fnOnSizeDefaut(CFenetreOpenGL * fenetre, unsigned int flag,
					unsigned int dim_x, unsigned int dim_y)
{
	// R�cup�ration des param�tres
	fenetre->m_uiDimX = dim_x; fenetre->m_uiDimY = dim_y;

	// D�finition du cadre
	glViewport(0, 0, dim_x, dim_y);

	// Initialisation de la matrice de projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// D�finition de la perspective
	gluPerspective(60.0, (float)dim_x / (float)dim_y, 1.01, 1000.0);
}



// fnDessinDefaut
// -> Fonction de dessin par d�faut
//---------------------------------------------------------------//
void fnDessinDefaut(void)
{
	// Initialisation de la matrice de visualisation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// D�finition de la couleur de fond
	glClearColor(0.0, 0.0, 0.0, 0.0);
}



// fnOnCloseDefaut
// -> Fonction de fermeture par d�faut
//---------------------------------------------------------------//
void fnOnCloseDefaut(void)
	{PostQuitMessage(0);}



//---------------------------------------------------------------//
// Construction / Destruction
//---------------------------------------------------------------//

// CFenetreOpenGL
// -> Constructeur par d�faut
//---------------------------------------------------------------//
CFenetreOpenGL::CFenetreOpenGL(void)
{
	// Initialisation des attributs
	m_hDC = NULL;
	m_hRC = NULL;
	m_hWnd = NULL;

	m_uiPosX = 0; m_uiPosY = 0;
	m_uiDimX = 0; m_uiDimY = 0;

	strcpy(m_sTitre, "");

	m_bPleinEcran = false;

	m_fnDessin = fnDessinDefaut;
	m_fnOnClose = fnOnCloseDefaut;
	m_fnOnEraseBkgnd = fnOnEraseBkgndDefaut;
	m_fnOnSize = fnOnSizeDefaut;
}



// ~CFenetreOpenGL
// -> Destructeur par d�faut
//---------------------------------------------------------------//
CFenetreOpenGL::~CFenetreOpenGL(void)
{
	Detruire();
}



//---------------------------------------------------------------//
// M�thodes
//---------------------------------------------------------------//

// Detruire
// -> M�thode de destruction des attributs
//---------------------------------------------------------------//
void CFenetreOpenGL::Detruire(void)
{
	// Si le mode d'affichage est plein �cran ...
	if (m_bPleinEcran)
		ChangeDisplaySettings(NULL, 0);
	
	// Destruction d'OpenGL
	OpenGLDetruire(m_hWnd, m_hDC, m_hRC);

	// Destruction de la fen�tre
	if (m_hWnd)
		DestroyWindow(m_hWnd);
	UnregisterClass("OpenGL", GetModuleHandle(NULL));
}



// Creer
// -> M�thode de cr�ation de la fen�tre
//---------------------------------------------------------------//
void CFenetreOpenGL::Creer(char * titre, unsigned int pos_x,
						   unsigned int pos_y, unsigned int dim_x,
						   unsigned int dim_y, bool m_pleinecran)
{
	// Variables locales
	DWORD dwStyle;
	DWORD dwExStyle;

	// R�cup�ration des param�tres
	m_uiPosX = pos_x; m_uiPosY = pos_y;
	m_uiDimX = dim_x; m_uiDimY = dim_y;

	strcpy(m_sTitre, titre);

	m_bPleinEcran = m_pleinecran;

	// D�finition de la classe parent
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC) WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "OpenGL";

	// Enregistrement de la classe parent
	if (!RegisterClass(&wc))
	{
		PostQuitMessage(0);
		return;
	}

	// Si le mode d'affichage est plein �cran ...
	if (m_bPleinEcran)
	{
		// D�finition du mode d'affichage
		DEVMODE dmScreen;
		memset(&dmScreen, 0, sizeof(DEVMODE));
		dmScreen.dmSize = sizeof(DEVMODE);
		dmScreen.dmPelsWidth = m_uiDimX;
		dmScreen.dmPelsHeight = m_uiDimY;
		dmScreen.dmBitsPerPel = 32;
		dmScreen.dmFields = DM_BITSPERPEL | DM_PELSWIDTH
							| DM_PELSHEIGHT;

		// Mise en place du mode d'affichage
		if (ChangeDisplaySettings(&dmScreen, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle = WS_OVERLAPPEDWINDOW;
		}
		else
		{
			dwStyle = WS_POPUP;
			dwExStyle = WS_EX_APPWINDOW;
		}
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	// Cr�ation de la fen�tre
	m_hWnd = CreateWindowEx(dwExStyle, "OpenGL", m_sTitre, dwStyle,
						    m_uiPosX, m_uiPosY, m_uiDimX, m_uiDimY,
						    NULL, NULL, GetModuleHandle(NULL), NULL);
	if (!m_hWnd)
	{
		Detruire();
		PostQuitMessage(0);
		return;
	}

	// Passage en param�tre du pointeur de la fen�tre
	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);

	// R�cup�ration du Device Context
	m_hDC = GetDC(m_hWnd);

	// Initialisation d'OpenGL
	if (!OpenGLInitialiser(m_hDC, m_hRC))
	{
		Detruire();
		PostQuitMessage(0);
		return;
	}

	// Affichage de la fen�tre
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	SetFocus(m_hWnd);
}
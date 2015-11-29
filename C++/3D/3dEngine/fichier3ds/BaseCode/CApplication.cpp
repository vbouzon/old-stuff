//---------------------------------------------------------------//
// CApplication.cpp
//
// Implémentation des méthodes et des fonction liées à la classe
// CApplication.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include "CApplication.h"



//---------------------------------------------------------------//
// Construction / Destruction
//---------------------------------------------------------------//

// CApplication
// -> Constructeur par défaut
//---------------------------------------------------------------//
CApplication::CApplication(void)
{
	// Initialisation des attributs
	m_pFenetre = NULL;
}



// ~CApplication
// -> Destructeur par défaut
//---------------------------------------------------------------//
CApplication::~CApplication(void)
{
	Detruire();
}



//---------------------------------------------------------------//
// Méthodes
//---------------------------------------------------------------//

// Detruire
// -> Méthode de destruction des attributs
//---------------------------------------------------------------//
void CApplication::Detruire(void)
{
	if (m_pFenetre)
		delete m_pFenetre;
}



// Boucler
// -> Méthode de traitement en boucle des évènements Windows
//---------------------------------------------------------------//
void CApplication::Boucler(void)
{
	MSG msg;
	bool quitter = false;

	while (!quitter)
	{
		// Si un message en attente ...
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				quitter = TRUE;
			else
			{
				// Traitement du message
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else if (m_pFenetre)
		{
			// Dessin de la scène
			m_pFenetre->m_fnDessin();

			// Echange des buffers
			SwapBuffers(m_pFenetre->m_hDC);
		}
	}
}



// CreerFenetreOpenGL
// -> Méthode de création de la fenêtre OpenGL associée à
// -> l'application
//---------------------------------------------------------------//
void CApplication::CreerFenetreOpenGL(char * titre, unsigned int pos_x,
									  unsigned int pos_y, unsigned int dim_x,
									  unsigned int dim_y, bool m_pleinecran)
{
	m_pFenetre = new CFenetreOpenGL();
	m_pFenetre->Creer(titre, pos_x, pos_y, dim_x, dim_y, m_pleinecran);
}
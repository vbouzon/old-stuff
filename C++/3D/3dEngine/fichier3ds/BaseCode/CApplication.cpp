//---------------------------------------------------------------//
// CApplication.cpp
//
// Impl�mentation des m�thodes et des fonction li�es � la classe
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
// -> Constructeur par d�faut
//---------------------------------------------------------------//
CApplication::CApplication(void)
{
	// Initialisation des attributs
	m_pFenetre = NULL;
}



// ~CApplication
// -> Destructeur par d�faut
//---------------------------------------------------------------//
CApplication::~CApplication(void)
{
	Detruire();
}



//---------------------------------------------------------------//
// M�thodes
//---------------------------------------------------------------//

// Detruire
// -> M�thode de destruction des attributs
//---------------------------------------------------------------//
void CApplication::Detruire(void)
{
	if (m_pFenetre)
		delete m_pFenetre;
}



// Boucler
// -> M�thode de traitement en boucle des �v�nements Windows
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
			// Dessin de la sc�ne
			m_pFenetre->m_fnDessin();

			// Echange des buffers
			SwapBuffers(m_pFenetre->m_hDC);
		}
	}
}



// CreerFenetreOpenGL
// -> M�thode de cr�ation de la fen�tre OpenGL associ�e �
// -> l'application
//---------------------------------------------------------------//
void CApplication::CreerFenetreOpenGL(char * titre, unsigned int pos_x,
									  unsigned int pos_y, unsigned int dim_x,
									  unsigned int dim_y, bool m_pleinecran)
{
	m_pFenetre = new CFenetreOpenGL();
	m_pFenetre->Creer(titre, pos_x, pos_y, dim_x, dim_y, m_pleinecran);
}
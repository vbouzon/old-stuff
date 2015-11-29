//---------------------------------------------------------------//
// CApplication.h
//
// Définition de la classe CApplication et de ses fonctions
// liées.

//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//
#ifndef CApplication_h
#define CApplication_h



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include <windows.h>

#include "CFenetreOpenGL.h"



//---------------------------------------------------------------//
// CApplication
//
// -> Représente une application qui affiche une fenêtre OpenGL
// -> afin de pouvoir dessiner une scène 3D.
//---------------------------------------------------------------//
class CApplication
{
	// Attributs ------------------------------------------------//

	public:

		CFenetreOpenGL * m_pFenetre;

	// Construction / Destruction -------------------------------//

	public:

		CApplication(void);
		~CApplication(void);

	// Méthodes -------------------------------------------------//

	public:

		void Detruire(void);
		void Boucler(void);

		void CreerFenetreOpenGL(char * titre = "", unsigned int pos_x = 0,
								unsigned int pos_y = 0, unsigned int dim_x = 640,
								unsigned int dim_y = 480, bool m_pleinecran = false);
};



#endif
//---------------------------------------------------------------//
// CFichier.h
//
// D�finition de la classe CFichier et de ses fonctions li�es.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//

#ifndef CFichier_h
#define CFichier_h



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include ".\Math\CVecteur.h"



//---------------------------------------------------------------//
// CFichier
//
// -> Repr�sente un fichier qui contient des donn�es exploitables
// -> pour le dessin d'une sc�ne 3D.
//---------------------------------------------------------------//
class CFichier
{
	// Attributs ------------------------------------------------//

	public:

		unsigned char * m_ucpBuffer;
		unsigned int m_uiLongeur;

		CVecteur3 m_v3Min;
		CVecteur3 m_v3Max;

	// Construction / Destruction -------------------------------//

	public:

		CFichier(void);
		~CFichier(void);

	// M�thodes -------------------------------------------------//

	public:

		virtual void Detruire(void);
		virtual void Charger(char * repertoire, char * fichier);
		virtual void Dessiner(void);
};



#endif
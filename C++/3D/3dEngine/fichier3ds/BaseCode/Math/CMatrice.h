//---------------------------------------------------------------//
// CMatrice.h
//
// D�finition des classes CMatrice et de leurs fonctions li�es.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//

#ifndef CMatrice_h
#define CMatrice_h



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include <math.h>

#include "Math.h"
#include "CVecteur.h"



//---------------------------------------------------------------//
// CMatrice4
//
// -> Repr�sente une matrice 4x4
//---------------------------------------------------------------//
class CMatrice4
{
	// Attributs ------------------------------------------------//

	public:

		double m_composante[4][4];

	// Construction / Destruction -------------------------------//

	public:

		CMatrice4(void);
		~CMatrice4(void);

	// M�thodes -------------------------------------------------//

	public:

		void Identite(void);
		void DefinirRotation(double angle, double x, double y, double z);
};



//---------------------------------------------------------------//
// Fonctions
//---------------------------------------------------------------//

const CVecteur3 Mult(const CMatrice4& m, const CVecteur3& v);



#endif
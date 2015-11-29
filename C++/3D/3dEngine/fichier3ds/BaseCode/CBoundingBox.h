//---------------------------------------------------------------//
// CBoundingBox.h
//
// Définit de la classe CBoundingBox et des fonctions liées.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//

#ifndef CBoundingBox_h
#define CBoundingBox_h



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include "./Math/CVecteur.h"



//---------------------------------------------------------------//
// CBoundingBox
//
// -> Représente une boite (cube) qui englobe un ensemble de
// -> points.
//---------------------------------------------------------------//
class CBoundingBox
{
	// Attributs ------------------------------------------------//

	public:

		CVecteur3 m_v3Min;
		CVecteur3 m_v3Max;

	// Construction / Destruction -------------------------------//

	public:

		CBoundingBox(void);
		~CBoundingBox(void);

	// Méthodes -------------------------------------------------//

	public:

		void Detruire(void);

		CVecteur3 CalculerCentre(void);

		double CalculerLargeur(void);
		double CalculerHauteur(void);
		double CalculerProfondeur(void);
};



#endif
//---------------------------------------------------------------//
// CBoundingBox.cpp
//
// Impl�mentation des m�thodes et des fonctions li�es � la classe
// CBoundingBox.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include "CBoundingBox.h"



//---------------------------------------------------------------//
// Construction / Destruction
//---------------------------------------------------------------//

// CBoundingBox
// -> Constructeur par d�faut
//---------------------------------------------------------------//
CBoundingBox::CBoundingBox(void)
{
	// Initialisation des attributs
	m_v3Min.Definir(0.0, 0.0, 0.0);
	m_v3Max.Definir(0.0, 0.0, 0.0);
}



// ~CBoundingBox
// -> Destructeur par d�faut
//---------------------------------------------------------------//
CBoundingBox::~CBoundingBox(void)
	{Detruire();}



//---------------------------------------------------------------//
// M�thodes
//---------------------------------------------------------------//

// Detruire
// -> M�thode de destruction des attributs
//---------------------------------------------------------------//
void CBoundingBox::Detruire(void)
	{}



// CalculerCentre
// -> M�thode de calcul du centre de la boite
//---------------------------------------------------------------//
CVecteur3 CBoundingBox::CalculerCentre(void)
{
	CVecteur3 res;
	for (int i = 0; i < 3; i++)
		res[i] = (m_v3Max[i] + m_v3Min[i]) / 2.0;
	return res;
}



// CalculerLargeur
// -> M�thode de calcul de la largeur de la boite
//---------------------------------------------------------------//
double CBoundingBox::CalculerLargeur(void)
{
	double largeur = 0.0;
	largeur = m_v3Max[0] - m_v3Min[0];
	return largeur;
}



// CalculerHauteur
// -> M�thode de calcul de la hauteur de la boite
//---------------------------------------------------------------//
double CBoundingBox::CalculerHauteur(void)
{
	double largeur = 0.0;
	largeur = m_v3Max[1] - m_v3Min[1];
	return largeur;
}



// CalculerProfondeur
// -> M�thode de calcul de la profondeur de la boite
//---------------------------------------------------------------//
double CBoundingBox::CalculerProfondeur(void)
{
	double largeur = 0.0;
	largeur = m_v3Max[2] - m_v3Min[2];
	return largeur;
}
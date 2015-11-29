//---------------------------------------------------------------//
// CMatrice.cpp
//
// Implémentation des méthodes et des fonctions liées de la classe
// CMatrice.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include "CMatrice.h"



//---------------------------------------------------------------//
// Construction / Destruction
//---------------------------------------------------------------//

// CMatrice4
// -> Constructeur par défaut
//---------------------------------------------------------------//
CMatrice4::CMatrice4(void)
{
	// Initialisation des attributs
	Identite();
}



// ~CMatrice4
// -> Destructeur par défaut
//---------------------------------------------------------------//
CMatrice4::~CMatrice4(void)
{
}



//---------------------------------------------------------------//
// Méthodes
//---------------------------------------------------------------//

// CMatrice4
// -> Méthode d'initialisation de la matrice à l'identité
//---------------------------------------------------------------//
void CMatrice4::Identite(void)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (i == j)
				m_composante[i][j] = 1.0;
			else
				m_composante[i][j] = 0.0;
}



// CMatrice4
// -> Méthode de définition d'une matrice de rotation
//---------------------------------------------------------------//
void CMatrice4::DefinirRotation(double angle, double x, double y, double z)
{
	double c = cos(RADIAN(angle));
	double s = sin(RADIAN(angle));

	m_composante[0][0] = x * x + c * (1.0 - (x * x));
	m_composante[0][1] = x * y * (1.0 - c) - z * s;
	m_composante[0][2] = x * z * (1.0 - c) + y * s;
	m_composante[0][3] = 0.0;
	
	m_composante[1][0] = x * y * (1.0 - c) + z * s;
	m_composante[1][1] = y * y + c * (1.0 - (y * y));
	m_composante[1][2] = y * z * (1.0 - c) - x * s;
	m_composante[1][3] = 0.0;
	
	m_composante[2][0] = x * z * (1.0 - c) - y * s;
	m_composante[2][1] = y * z * (1.0 - c) + x * s;
	m_composante[2][2] = z * z + c * (1.0 - (z * z));
	m_composante[2][3] = 0.0;
	
	m_composante[3][0] = 0.0;
	m_composante[3][1] = 0.0;
	m_composante[3][2] = 0.0;
	m_composante[3][3] = 1.0;
}



//---------------------------------------------------------------//
// Fonctions
//---------------------------------------------------------------//

// Mult
// -> Fonction de multiplication d'un vecteur par une matrice
//---------------------------------------------------------------//
const CVecteur3 Mult(const CMatrice4& m, const CVecteur3& v)
{
	CVecteur3 res;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			res[i] += m.m_composante[i][j] * v.m_composante[j];
	return res;
}

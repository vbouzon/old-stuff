//---------------------------------------------------------------//
// CVecteur.cpp
//
// Implémentation des méthodes et des fonction liées des classes
// CVecteur.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include "CVecteur.h"



//---------------------------------------------------------------//
// Construction / Destruction
//---------------------------------------------------------------//

// CVecteur3
// -> Constructeur par défaut
//---------------------------------------------------------------//
CVecteur3::CVecteur3(void)
{
	// Initialisation des attributs
	Definir(0.0, 0.0, 0.0);
}



// CVecteur3
// -> Constructeur avec trois composantes en paramètre
//---------------------------------------------------------------//
CVecteur3::CVecteur3(double x, double y, double z)
{
	Definir(x, y, z);
}



// ~CVecteur3
// -> Destructeur par défaut
//---------------------------------------------------------------//
CVecteur3::~CVecteur3(void)
{
}



//---------------------------------------------------------------//
// Méthodes
//---------------------------------------------------------------//

// Definir
// -> Méthode de définition des composantes
//---------------------------------------------------------------//
void CVecteur3::Definir(double x, double y, double z)
{
	m_composante[0] = x;
	m_composante[1] = y;
	m_composante[2] = z;
}



// CalculerNorme
// -> Méthode de calcul de la norme
//---------------------------------------------------------------//
double CVecteur3::CalculerNorme(void)
{
	return sqrt(m_composante[0] * m_composante[0] +
				m_composante[1] * m_composante[1] +
				m_composante[2] * m_composante[2]);
}



// Normaliser
// -> Méthode de normalisation
//---------------------------------------------------------------//
void CVecteur3::Normaliser(void)
{
	double n = CalculerNorme();
	m_composante[0] /= n;
	m_composante[1] /= n;
	m_composante[2] /= n;
}



//---------------------------------------------------------------//
// Opérateurs
//---------------------------------------------------------------//

// operator[]
// -> Opérateur d'accessibilité
//---------------------------------------------------------------//
double& CVecteur3::operator[](int i)
{
	return m_composante[i];
}



// operator=
// -> Opérateur d'égalité
//---------------------------------------------------------------//
CVecteur3& CVecteur3::operator=(const CVecteur3& v)
{
	m_composante[0] = v.m_composante[0];
	m_composante[1] = v.m_composante[1];
	m_composante[2] = v.m_composante[2];
	return *this;
}



// operator+=
// -> Opérateur d'addtion et d'égalité
//---------------------------------------------------------------//
CVecteur3& CVecteur3::operator+=(const CVecteur3& v)
{
	m_composante[0] += v.m_composante[0];
	m_composante[1] += v.m_composante[1];
	m_composante[2] += v.m_composante[2];
	return *this;
}



// operator+
// -> Opérateur d'addtion
//---------------------------------------------------------------//
const CVecteur3 CVecteur3::operator+(const CVecteur3& v) const
{
	CVecteur3 res;
	res.m_composante[0] = m_composante[0] + v.m_composante[0];
	res.m_composante[1] = m_composante[1] + v.m_composante[1];
	res.m_composante[2] = m_composante[2] + v.m_composante[2];
	return res;
}



// operator-
// -> Opérateur de soustraction
//---------------------------------------------------------------//
const CVecteur3 CVecteur3::operator-(const CVecteur3& v) const
{
	CVecteur3 res;
	res.m_composante[0] = m_composante[0] - v.m_composante[0];
	res.m_composante[1] = m_composante[1] - v.m_composante[1];
	res.m_composante[2] = m_composante[2] - v.m_composante[2];
	return res;
}



// operator*
// -> Opérateur de multiplication par un scalaire
//---------------------------------------------------------------//
const CVecteur3 CVecteur3::operator*(double d) const
{
	CVecteur3 res;
	res.m_composante[0] = m_composante[0] * d;
	res.m_composante[1] = m_composante[1] * d;
	res.m_composante[2] = m_composante[2] * d;
	return res;
}



//---------------------------------------------------------------//
// Fonctions
//---------------------------------------------------------------//

// ProduitVectoriel
// -> Fonction de calcul du produit vectoriel de deux vecteurs
//---------------------------------------------------------------//
CVecteur3 ProduitVectoriel(CVecteur3& v0, CVecteur3& v1)
{
	return (CVecteur3((v0[1] * v1[2] - v0[2] * v1[1]),
					  (v0[2] * v1[0] - v0[0] * v1[2]),
					  (v0[0] * v1[1] - v0[1] * v1[0])));
}



// CalculerNormale
// -> Fonction de calcul du vecteur normal à partir de trois
// -> points
//---------------------------------------------------------------//
CVecteur3 CalculerNormale(CVecteur3& p0, CVecteur3& p1, CVecteur3& p2)
{
	CVecteur3 v0 = p1 - p0;
	CVecteur3 v1 = p2 - p0;

	CVecteur3 v = ProduitVectoriel(v1, v0);
	v.Normaliser();
	
	return v;
}



//---------------------------------------------------------------//
// Construction / Destruction
//---------------------------------------------------------------//

// CVecteur3
// -> Constructeur par défaut
//---------------------------------------------------------------//
CVecteur2::CVecteur2(void)
{
	// Initialisation des attributs
	Definir(0.0, 0.0);
}



// CVecteur3
// -> Constructeur avec deux composantes en paramètre
//---------------------------------------------------------------//
CVecteur2::CVecteur2(double x, double y)
{
	Definir(x, y);
}



// ~CVecteur3
// -> Destructeur par défaut
//---------------------------------------------------------------//
CVecteur2::~CVecteur2(void)
{
}



//---------------------------------------------------------------//
// Méthodes
//---------------------------------------------------------------//

// Definir
// -> Méthode de définition des composantes
//---------------------------------------------------------------//
void CVecteur2::Definir(double x, double y)
{
	m_composante[0] = x;
	m_composante[1] = y;
}



//---------------------------------------------------------------//
// Opérateurs
//---------------------------------------------------------------//

// operator[]
// -> Opérateur d'accessibilité
//---------------------------------------------------------------//
double& CVecteur2::operator[](int i)
{
	return m_composante[i];
}



// operator=
// -> Opérateur d'égalité
//---------------------------------------------------------------//
CVecteur2& CVecteur2::operator=(const CVecteur2& v)
{
	m_composante[0] = v.m_composante[0];
	m_composante[1] = v.m_composante[1];
	return *this;
}



// operator+=
// -> Opérateur d'addtion et d'égalité
//---------------------------------------------------------------//
CVecteur2& CVecteur2::operator+=(const CVecteur2& v)
{
	m_composante[0] += v.m_composante[0];
	m_composante[1] += v.m_composante[1];
	return *this;
}



// operator+
// -> Opérateur d'addtion
//---------------------------------------------------------------//
const CVecteur2 CVecteur2::operator+(const CVecteur2& v) const
{
	CVecteur2 res;
	res.m_composante[0] = m_composante[0] + v.m_composante[0];
	res.m_composante[1] = m_composante[1] + v.m_composante[1];
	return res;
}



// operator-
// -> Opérateur de soustraction
//---------------------------------------------------------------//
const CVecteur2 CVecteur2::operator-(const CVecteur2& v) const
{
	CVecteur2 res;
	res.m_composante[0] = m_composante[0] - v.m_composante[0];
	res.m_composante[1] = m_composante[1] - v.m_composante[1];
	return res;
}



// operator*
// -> Opérateur de multiplication par un scalaire
//---------------------------------------------------------------//
const CVecteur2 CVecteur2::operator*(double d) const
{
	CVecteur2 res;
	res.m_composante[0] = m_composante[0] * d;
	res.m_composante[1] = m_composante[1] * d;
	return res;
}

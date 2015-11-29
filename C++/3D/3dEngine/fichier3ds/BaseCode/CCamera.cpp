//---------------------------------------------------------------//
// CCamera.cpp
//
// Implémentation des méthodes et des fonction liées à la classe
// CCamera.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include "CCamera.h"



//---------------------------------------------------------------//
// Construction / Destruction
//---------------------------------------------------------------//

// CCamera
// -> Constructeur par défaut
//---------------------------------------------------------------//
CCamera::CCamera(void)
{
	//-> Initialisation des attributs
	m_v3Position.Definir(0.0f, 0.0f, -1.0f);
	m_v3Visee.Definir(0.0f, 0.0f, 0.0f);
	m_v3Verticale.Definir(0.0f, 1.0f, 0.0f);

	m_v3AxeX.Definir(1.0, 0.0, 0.0);
	m_v3AxeY.Definir(0.0, 1.0, 0.0);
	m_v3AxeZ.Definir(0.0, 0.0, 1.0);
}



// ~CCamera
// -> Destructeur par défaut
//---------------------------------------------------------------//
CCamera::~CCamera(void)
	{Detruire();}



//---------------------------------------------------------------//
// Méthodes
//---------------------------------------------------------------//

// Detruire
// -> Méthode de destruction des attributs
//---------------------------------------------------------------//
void CCamera::Detruire(void)
	{}



// Positionner
// -> Méthode de positionnement de la caméra
//---------------------------------------------------------------//
void CCamera::Positionner(void)
{
	// Mise en place de l'observateur
	gluLookAt(m_v3Position[0], m_v3Position[1], m_v3Position[2],
		      m_v3Visee[0], m_v3Visee[1], m_v3Visee[2],
			  m_v3Verticale[0], m_v3Verticale[1], m_v3Verticale[2]);

	// Récupération de la matrice de visualisation
	double matrice[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, matrice);

	// Definition des axes du repère local
	m_v3AxeX.Definir( matrice[0],  matrice[4],  matrice[8]);
	m_v3AxeY.Definir( matrice[1],  matrice[5],  matrice[9]);
	m_v3AxeZ.Definir(-matrice[2], -matrice[6], -matrice[10]);
}



// CalculerR
// -> Méthode de calcul de la coordonnée sphérique r
//---------------------------------------------------------------//
double CCamera::CalculerR(void)
{
	CVecteur3 focal = m_v3Position - m_v3Visee;
	double r = focal.CalculerNorme();
	return r;
}



// CalculerTheta
// -> Méthode de calcul de la coordonnée sphérique theta
//---------------------------------------------------------------//
double CCamera::CalculerTheta(void)
{
	CVecteur3 focal = m_v3Position - m_v3Visee;
	focal.Normaliser();
	double theta = DEGREE(acosf((float)focal[2]));
	return theta;
}



// CalculerPhi
// -> Méthode de calcul de la coordonnée sphérique phi
//---------------------------------------------------------------//
double CCamera::CalculerPhi(void)
{
	CVecteur3 focal = m_v3Position - m_v3Visee;
	focal.Normaliser();
	double phi = DEGREE(acosf((float)focal[1]));
	return phi;
}



// DefinirR
// -> Méthode de mise de la position selon la coordonnée 
// -> sphérique r
//---------------------------------------------------------------//
void CCamera::DefinirR(double r)
{
	m_v3Position = m_v3Visee - m_v3AxeZ * r;
}



// DefinirTheta
// -> Méthode de mise de la position selon la coordonnée 
// -> sphérique theta
//---------------------------------------------------------------//
void CCamera::DefinirTheta(double theta)
{
	// Matrice de rotation
	CMatrice4 m;
	m.DefinirRotation(theta, m_v3AxeX[0], m_v3AxeX[1], m_v3AxeX[2]);
	
	// Calcul de la nouvelle focale
	CVecteur3 focal = m_v3Position - m_v3Visee;
	CVecteur3 temp = Mult(m, focal);

	// Mise à jour de la position
	m_v3Position = m_v3Visee + temp;
}



// DefinirPhi
// -> Méthode de mise de la position selon la coordonnée 
// -> sphérique phi
//---------------------------------------------------------------//
void CCamera::DefinirPhi(double phi)
{
	// Matrice de rotation
	CMatrice4 m;
	m.DefinirRotation(phi, m_v3AxeY[0], m_v3AxeY[1], m_v3AxeY[2]);
	
	// Calcul de la nouvelle focale
	CVecteur3 focal = m_v3Position - m_v3Visee;
	CVecteur3 temp = Mult(m, focal);

	// Mise à jour de la position
	m_v3Position = m_v3Visee + temp;
}



//---------------------------------------------------------------//
// Fonctions
//---------------------------------------------------------------//

// MiseAJourCameraSpheriqueClavier
// -> Fonction de mise à jour des coordonnées de la caméra selon
// -> un mouvement sphérique
//---------------------------------------------------------------//
void MiseAJourCameraSpheriqueClavier(CCamera * camera, double vitesse)
{
	// Touche fleche haut
	if (GetKeyState(VK_UP) & 0x80)
		camera->DefinirTheta(-vitesse);

	// Touche fleche bas
	if (GetKeyState(VK_DOWN) & 0x80)
		camera->DefinirTheta(vitesse);

	// Touche fleche droit
	if (GetKeyState(VK_RIGHT) & 0x80)
		camera->DefinirPhi(vitesse);

	// Touche fleche gauche
	if (GetKeyState(VK_LEFT) & 0x80)
		camera->DefinirPhi(-vitesse);

	// Touche page precedente
	if (GetKeyState(VK_PRIOR) & 0x80)
	{
		double r = camera->CalculerR();
		if (r > 1.0)
			camera->DefinirR(r - vitesse / 10.0);
	}

	// Touche page suivante
	if (GetKeyState(VK_NEXT) & 0x80)
	{
		double r = camera->CalculerR();
		camera->DefinirR(r + vitesse / 10.0);
	}
}
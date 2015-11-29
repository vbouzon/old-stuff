//---------------------------------------------------------------//
// CCamera.h
//
// Définition de la classe CCamera et de ses fonctions liées.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//

#ifndef CCamera_h
#define CCamera_h



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include "OpenGL.h"

#include "./Math/CVecteur.h"
#include "./Math/CMatrice.h"



//---------------------------------------------------------------//
// CCamera
//
// -> Représente un observateur de scène.
//---------------------------------------------------------------//
class CCamera
{
	// Attributs ------------------------------------------------//

	public:

		CVecteur3 m_v3Position;
		CVecteur3 m_v3Visee;
		CVecteur3 m_v3Verticale;

		CVecteur3 m_v3AxeX;
		CVecteur3 m_v3AxeY;
		CVecteur3 m_v3AxeZ;

	// Construction / Destruction -------------------------------//

	public:

		CCamera(void);
		~CCamera(void);

	// Méthodes -------------------------------------------------//

	public:

		virtual void Detruire(void);
		
		void Positionner(void);

		double CalculerR(void);
		double CalculerTheta(void);
		double CalculerPhi(void);

		void DefinirR(double r);
		void DefinirTheta(double theta);
		void DefinirPhi(double phi);
};



//---------------------------------------------------------------//
// Fonctions
//---------------------------------------------------------------//

void MiseAJourCameraSpheriqueClavier(CCamera * camera, double vitesse);



#endif
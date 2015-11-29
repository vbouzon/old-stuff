//---------------------------------------------------------------//
// Main.cpp
//
// Point d'entrée du programme. Initialise et affiche une
// fenêtre OpenGL. Dans cette fenêtre nous allons dessiner les
// données conservées dans un fichier .3ds.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include "Main.h"



//---------------------------------------------------------------//
// Variables globales
//---------------------------------------------------------------//

CCamera * pCamera = 0;
CFichier * pFichier3DS = 0;

CVecteur3 translation;



//---------------------------------------------------------------//
// Fonctions
//---------------------------------------------------------------//

// fnDessin
// -> Fonction de dessin
//---------------------------------------------------------------//
void fnDessin(void)
{
	// Variable statique
	static double angle = 0.0;

	// Initialisation de la matrice de visualisation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Définition de la couleur de fond
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// Variables d'état
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Positionnement de la camera
	MiseAJourCameraSpheriqueClavier(pCamera, 1.0);
	pCamera->Positionner();

	// Dessin du fichier
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
		glTranslatef(-translation[0], -translation[1], -translation[2]);
		pFichier3DS->Dessiner();
	glPopMatrix();

	// Mise à jour de l'angle de rotation
	if (angle < 359.99) angle += 1.0;
	else angle = 0.0;
}



// WinMain
// -> Fonction principale : création et lancement de
// -> l'application
//---------------------------------------------------------------//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	// Initialisation de l'application et de la fenêtre
	CApplication * pApplication = new CApplication();
	pApplication->CreerFenetreOpenGL("Tutoriel - Fichier 3DS (http:://freddec.free.fr)",
									 0, 0, 640, 480, false);

	// Définition des fonctions de messages de la fenêtre
	pApplication->m_pFenetre->m_fnDessin = fnDessin;

	// Chargement du fichier .3ds
	pFichier3DS = new CFichier3DS();
	pFichier3DS->Charger("Ressources\\Fichiers3DS\\jeep1\\", "jeep1.3ds");

	// Construction de la bounding box
	CBoundingBox * boite = new CBoundingBox();
	boite->m_v3Max = pFichier3DS->m_v3Max;
	boite->m_v3Min = pFichier3DS->m_v3Min;
	translation = boite->CalculerCentre();

	// Initialisation de la caméra
	pCamera = new CCamera();
	pCamera->m_v3Position[2] += boite->CalculerProfondeur();

	// Boucle de traitement des messages
	pApplication->Boucler();

	// Destruction de l'application
	delete pFichier3DS;
	delete pApplication;
	return 0;
}
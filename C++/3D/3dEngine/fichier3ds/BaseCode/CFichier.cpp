//---------------------------------------------------------------//
// CFichier.cpp
//
// Implémentation des méthodes et des fonction liées à la classe
// CFichier.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "CFichier.h"



//---------------------------------------------------------------//
// Construction / Destruction
//---------------------------------------------------------------//

// CFichier
// -> Constructeur par défaut
//---------------------------------------------------------------//
CFichier::CFichier(void)
{
	// Initialisation des attributs
	m_ucpBuffer = 0;
	m_uiLongeur = 0;

	m_v3Min.Definir(0.0, 0.0, 0.0);
	m_v3Max.Definir(0.0, 0.0, 0.0);
}



// ~CFichier
// -> Destructeur par défaut
//---------------------------------------------------------------//
CFichier::~CFichier(void)
{
	Detruire();
}



//---------------------------------------------------------------//
// Méthodes
//---------------------------------------------------------------//

// Detruire
// -> Méthode de destruction des attributs
//---------------------------------------------------------------//
void CFichier::Detruire(void)
{
}



// Charger
// -> Méthode de chargement des données
//---------------------------------------------------------------//
void CFichier::Charger(char * repertoire, char * fichier)
{
	// Concatenation du répertoire et du nom de fichier
	char * sFichier = new char[strlen(repertoire) + strlen(fichier)];
	strcpy(sFichier, repertoire);
	strcat(sFichier, fichier);

	// Ouverture du fichier
	FILE * f = fopen(sFichier, "rb");
	if (!f)
		return;

	// Calcul de la longueur du fichier
	fseek(f, 0, SEEK_END);
	int iFin = ftell(f);
	fseek(f, 0, SEEK_SET);
	int iDebut = ftell(f);
	m_uiLongeur = iFin - iDebut;

	// Lecture des données
	m_ucpBuffer = new unsigned char[m_uiLongeur];
	fread(m_ucpBuffer, 1, m_uiLongeur, f);

	// Fermeture du fichier
	if (f)
		fclose(f);
}



// Dessiner
// -> Méthode de dessin
//---------------------------------------------------------------//
void CFichier::Dessiner(void)
{
}
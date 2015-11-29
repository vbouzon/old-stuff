//---------------------------------------------------------------//
// CTexture.h
//
// Définition de la classe CTexture et de ses fonctions liées.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//

#ifndef CTexture_h
#define CTexture_h



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include <stdio.h>



//---------------------------------------------------------------//
// Structures
//---------------------------------------------------------------//

// sBMPEnteteFichier
// -> structure représentant l'entête d'un fichier BMP
//---------------------------------------------------------------//
struct sBMPEnteteFichier
{
	unsigned char m_ucType[2];		// Type du fichier
	unsigned int m_uiTaille;		// Taille en octet du fichier
	unsigned int m_uiReserve;		// Octets réservés
	unsigned int m_uiOffset;		// Offset du début des données
};



// sBMPEnteteInformation
// -> structure représentant les informations d'un fichier BMP
//---------------------------------------------------------------//
struct sBMPEnteteInformation
{
	unsigned int m_uiTaille;			// Taille en octet
	unsigned int m_uiDimX;				// Largeur en pixel de l'image
	unsigned int m_uiDimY;				// Hauteur en pixel de l'image
	unsigned short m_uiPlans;			// Nombre de plans
	unsigned short m_uiBpp;				// Nombre d'octets par pixel
	unsigned int m_uiCompression;		// Type de compression
	unsigned int m_uiXPPM;				// Nombre de X pixels par métre
	unsigned int m_uiYPPM;				// Nombre de Y pixels par métre
	unsigned int m_uiClrUsed;			// Octets réservés
	unsigned int m_uiClrImp;			// Octets réservés
};



//---------------------------------------------------------------//
// CTexture
//
// -> Représente un tableau de pixel d'une image qui sera chargé
// -> dans une unité de texture.
//---------------------------------------------------------------//
class CTexture
{
	// Attributs ------------------------------------------------//

	public:

		unsigned int m_uiID;
		unsigned int m_uiDimX;
		unsigned int m_uiDimY;
		unsigned int m_uiComposante;

		unsigned char * m_ucpBuffer;
		unsigned char * m_ucpPixel;

	// Construction / Destruction -------------------------------//

	public:

		CTexture(void);
		~CTexture(void);

	// Méthodes -------------------------------------------------//

	public:

		virtual void Detruire(void);

		void Creer(void);
		void Charger(char * fichier);
		void Activer(void);

		void ChargerBMP(void);
		void ChargerJPG(FILE * f);
};



#endif
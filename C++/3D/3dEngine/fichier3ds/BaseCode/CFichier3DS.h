//---------------------------------------------------------------//
// CFichier3DS.h
//
// Définition de la classe CFichier3DS et de ses fonctions liées.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//

#ifndef CFichier3DS_h
#define CFichier3DS_h



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include <vector>
using namespace std;

#include ".\Math\CVecteur.h"

#include "CFichier.h"
#include "CTexture.h"



//---------------------------------------------------------------//
// Constantes
//---------------------------------------------------------------//

#define NOEUD3DS_MAIN			0x4D4D
#define NOEUD3DS_VERSION		0x0002
#define NOEUD3DS_EDIT3DS		0x3D3D
#define NOEUD3DS_MATERIAL		0xAFFF
#define NOEUD3DS_EDITCFG		0x3D3E
#define NOEUD3DS_MESH			0x4000
#define NOEUD3DS_MATNAME		0xA000	
#define NOEUD3DS_AMBIENT		0xA010
#define NOEUD3DS_DIFFUSE		0xA020
#define NOEUD3DS_SPECULAR		0xA030
#define NOEUD3DS_SHININESS		0xA040
#define NOEUD3DS_TRANSPARENT	0xA050
#define NOEUD3DS_TEXTURE		0xA200
#define NOEUD3DS_TEXFILE		0xA300
#define NOEUD3DS_TRIOBJ			0x4100
#define NOEUD3DS_VERTLIST		0x4110
#define NOEUD3DS_TRILIST		0x4120
#define NOEUD3DS_OBJMAT			0x4130
#define NOEUD3DS_UVMAP			0x4140



//---------------------------------------------------------------//
// Structures
//---------------------------------------------------------------//

// s3DSNoeud
// -> structure représentant un noeud
//---------------------------------------------------------------//
struct s3DSNoeud
{
	unsigned short m_usID;			// Identifiant du noeud
	unsigned int m_uiLongueur;		// Longueur en octet du noeud
};



// s3DSSommet
// -> structure représentant un sommet
//---------------------------------------------------------------//
struct s3DSSommet
{
	double m_fCoord[3];		// Les coordonnées du sommet
};



// s3DSTexCoord
// -> structure représentant une coordonnée de texture
//---------------------------------------------------------------//
struct s3DSTexCoord
{
	double m_fCoord[2];		// Les coordonnées de texture
};



// s3DSFace
// -> structure représentant un face d'un maillage
//---------------------------------------------------------------//
struct s3DSFace
{
	unsigned short m_usIndice[3];	// Indice des sommets de la face
	CVecteur3 m_v3Normale;			// Vecteur normale de la face
};



// s3DSMateriel
// -> structure représentant un matériel
//---------------------------------------------------------------//
struct s3DSMateriel
{
	char m_sNom[64];			// Nom du matériel
	float m_fAmbiant[4];		// Couleur ambiante
	float m_fDiffuse[4];		// Couleur diffuse
	float m_fSpeculaire[4];		// Couleur spéculaire
	float m_fEclat;				// Eclat de la couleur spéculaire
	CTexture m_Texture;			// Texture du matériel
};



// s3DSObjMat
// -> structure représentant la liste des faces d'un même maillage
// -> posédant le même matériel
//---------------------------------------------------------------//
struct s3DSObjMat
{
	unsigned short m_usMaterielId;		// Identifiant du matériel
	vector<unsigned short> m_vFace;		// Liste des faces
};



// s3DSMaillage
// -> structure représentant un maillage
//---------------------------------------------------------------//
struct s3DSMaillage
{
	vector<s3DSSommet *> m_vSommet;			// Liste des sommets
	vector<s3DSFace *> m_vFace;				// Liste des faces
	vector<s3DSTexCoord *> m_vTexCoord;		// Liste des coordonnées de texture
	vector<s3DSObjMat *> m_vMateriel;		// Liste des matériels associés aux faces
};



//---------------------------------------------------------------//
// CFichier3DS
//
// -> Représente un fichier .3ds qui contient des données
// -> exploitables pour le dessin d'une scène 3D.
//---------------------------------------------------------------//
class CFichier3DS :
	public CFichier
{
	// Attributs ------------------------------------------------//

	public:

		vector<s3DSMateriel *> m_vMateriel;		// Liste des matériels du fichier
		vector<s3DSMaillage *> m_vMaillage;		// Liste des maillages du fichier

	// Construction / Destruction -------------------------------//

	public:

		CFichier3DS(void);
		~CFichier3DS(void);

	// Méthodes -------------------------------------------------//

	public:

		virtual void Detruire(void);
		virtual void Charger(char * repertoire, char * fichier);
		virtual void Dessiner(void);
};



#endif
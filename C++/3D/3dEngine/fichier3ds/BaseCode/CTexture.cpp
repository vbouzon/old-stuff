//---------------------------------------------------------------//
// CTexture.cpp
//
// Implémentation des méthodes et des fonction liées à la classe
// CTexture.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include "jpeglib.h"
#pragma comment(lib, "jpeg.lib")

#include "OpenGL.h"
#include "CTexture.h"



//---------------------------------------------------------------//
// Constantes
//---------------------------------------------------------------//

const char BMP_ENTETE[] = "BM";



//---------------------------------------------------------------//
// Construction / Destruction
//---------------------------------------------------------------//



// CTexture
// -> Constructeur par défaut
//---------------------------------------------------------------//
CTexture::CTexture(void)
{
	// Initialisation des attributs
	m_uiID = 0;
	m_uiDimX = 0;
	m_uiDimY = 0;

	m_ucpBuffer = 0;
	m_ucpPixel = 0;
}



// ~CTexture
// -> Destructeur par défaut
//---------------------------------------------------------------//
CTexture::~CTexture(void)
{
	Detruire();
}



//---------------------------------------------------------------//
//---------------------------------------------------------------//



//---------------------------------------------------------------//
// Méthodes
//---------------------------------------------------------------//



// Detruire
// -> Méthode de destruction des attributs
//---------------------------------------------------------------//
void CTexture::Detruire(void)
{
	glDeleteTextures(1, &m_uiID);
}



// Creer
// -> Méthode de création de la texture dans une unité de texture
//---------------------------------------------------------------//
void CTexture::Creer(void)
{
	// Si le tableau de pixel est vide ...
	if (!m_ucpPixel)
		return;

	// Création d'un nouvel identifiant
	glGenTextures(1, &m_uiID);
	glBindTexture(GL_TEXTURE_2D, m_uiID);

	// Paramétrage du filtre
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);						

	// Construction des mipmaps
	unsigned int temp = GL_RGB;
	if (m_uiComposante == 4)
		temp = GL_RGBA;

	gluBuild2DMipmaps(GL_TEXTURE_2D, m_uiComposante, m_uiDimX, m_uiDimY,
					  temp, GL_UNSIGNED_BYTE, m_ucpPixel); 
}



// Charger
// -> Méthode de chargement
//---------------------------------------------------------------//
void CTexture::Charger(char * fichier)
{
	// Récupération de l'extension du fichier pour déterminer la
	// méthode de chargement.
	char * extension = strrchr(fichier, '.');

	// Ouverture du fichier
	FILE * f;
	if (!(f = fopen(fichier, "rb")))
		return;

	// Calcul de la longueur du fichier
	fseek(f, 0, SEEK_END);
	int iFin = ftell(f);
	fseek(f, 0, SEEK_SET);
	int iDebut = ftell(f);
	unsigned int longeur = iFin - iDebut;

	// Lecture des données
	m_ucpBuffer = new unsigned char[longeur];
	fread(m_ucpBuffer, 1, longeur, f);
	fseek(f, 0, SEEK_SET);

	// Si le format du fichier est ..
	if (memcmp(m_ucpBuffer, BMP_ENTETE, 2) == 0)
		ChargerBMP();
	else
	{
		if (strcmp(extension, ".jpg") == 0)
			ChargerJPG(f);
	}

	// Fermeture du fichier
	if (f)
		fclose(f);

	// Création
	Creer();
}



// Activer
// -> Méthode d'activation
//---------------------------------------------------------------//
void CTexture::Activer(void)
	{glBindTexture(GL_TEXTURE_2D, m_uiID);}



// ChargerBMP
// -> Méthode de chargement d'un fichier BMP
//---------------------------------------------------------------//
void CTexture::ChargerBMP(void)
{
	// Lecture de l'entête du fichier
	sBMPEnteteFichier * pEnteteFichier;
	pEnteteFichier = (sBMPEnteteFichier *)m_ucpBuffer;
	m_ucpBuffer += 14;

	// Lecture de l'entête du fichier
	sBMPEnteteInformation * pInfoFichier;
	pInfoFichier = (sBMPEnteteInformation *)m_ucpBuffer;
	m_ucpBuffer += 40;

	if (pInfoFichier->m_uiBpp != 24
		|| pInfoFichier->m_uiDimX == 0
		|| pInfoFichier->m_uiDimY == 0)
	return;

	// Enregistrement des données
	m_uiDimX = pInfoFichier->m_uiDimX;
	m_uiDimY = pInfoFichier->m_uiDimY;
	m_uiComposante = 3;

	m_ucpPixel = new unsigned char[m_uiDimX * m_uiDimY * 3];

	// Conversion des composantes BGR en RGB
	for (unsigned int i = 0; i < m_uiDimX * m_uiDimY * 3; i += 3)
	{
		m_ucpPixel[i + 0] = m_ucpBuffer[i + 2];
		m_ucpPixel[i + 1] = m_ucpBuffer[i + 1];
		m_ucpPixel[i + 2] = m_ucpBuffer[i + 0];
	}

	// Retournement vertical de l'image
	unsigned char * ucpCopy = new unsigned char[m_uiDimX * m_uiDimY * 3];
	for (i = 0; i < m_uiDimY; i++)
		memcpy(ucpCopy + (m_uiDimX * 3 * i), m_ucpPixel + (m_uiDimX * 3 * (m_uiDimY - 1 - i)), m_uiDimX * 3);
	memcpy(m_ucpPixel, ucpCopy, m_uiDimX * m_uiDimY * 3);
	delete [] ucpCopy;
}



// ChargerBMP
// -> Méthode de chargement d'un fichier JPG
//---------------------------------------------------------------//
void CTexture::ChargerJPG(FILE * f)
{
	// Structure d'erreur pour le décodage jpeg.
	jpeg_error_mgr jerr;

	// Mise en place de la structure d'erreur.
	struct jpeg_decompress_struct cinfo;
	cinfo.err = jpeg_std_error(&jerr);

	// Initialisation de la structure de décompression.
	jpeg_create_decompress(&cinfo);

	// Spécification de la source de données.	
	jpeg_stdio_src(&cinfo, f);

	// Lecture de l'entête.
	jpeg_read_header(&cinfo, TRUE);

	// Début de la décompression.
	jpeg_start_decompress(&cinfo);

	// Récupération des données.
	m_uiComposante = cinfo.num_components;
	m_uiDimX = cinfo.image_width;
	m_uiDimY = cinfo.image_height;

	// Allocation mémoire du tableau de pixels.
	m_ucpPixel = new unsigned char[m_uiDimX * m_uiDimY * m_uiComposante];

	// Création d'un tableau de ligne de pixels.
	unsigned char ** ligne = new unsigned char * [m_uiDimY];
	for (int i = 0; i < m_uiDimY; i++)
		ligne[i] = &m_ucpPixel[i * m_uiDimX * m_uiComposante];

	// Lecture ligne par ligne des pixels de l'image.
	int lu = 0;
	while (cinfo.output_scanline < cinfo.output_height) 
		lu += jpeg_read_scanlines(&cinfo, &ligne[lu], cinfo.output_height - lu);
	
	// Libération de la mémoire.
	delete [] ligne;

	// Fin de la décompression.
	jpeg_finish_decompress(&cinfo);

	// Destruction de la structure de décompression.
	jpeg_destroy_decompress(&cinfo);
}
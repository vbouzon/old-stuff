//---------------------------------------------------------------//
// CTexture.cpp
//
// Impl�mentation des m�thodes et des fonction li�es � la classe
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
// -> Constructeur par d�faut
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
// -> Destructeur par d�faut
//---------------------------------------------------------------//
CTexture::~CTexture(void)
{
	Detruire();
}



//---------------------------------------------------------------//
//---------------------------------------------------------------//



//---------------------------------------------------------------//
// M�thodes
//---------------------------------------------------------------//



// Detruire
// -> M�thode de destruction des attributs
//---------------------------------------------------------------//
void CTexture::Detruire(void)
{
	glDeleteTextures(1, &m_uiID);
}



// Creer
// -> M�thode de cr�ation de la texture dans une unit� de texture
//---------------------------------------------------------------//
void CTexture::Creer(void)
{
	// Si le tableau de pixel est vide ...
	if (!m_ucpPixel)
		return;

	// Cr�ation d'un nouvel identifiant
	glGenTextures(1, &m_uiID);
	glBindTexture(GL_TEXTURE_2D, m_uiID);

	// Param�trage du filtre
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
// -> M�thode de chargement
//---------------------------------------------------------------//
void CTexture::Charger(char * fichier)
{
	// R�cup�ration de l'extension du fichier pour d�terminer la
	// m�thode de chargement.
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

	// Lecture des donn�es
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

	// Cr�ation
	Creer();
}



// Activer
// -> M�thode d'activation
//---------------------------------------------------------------//
void CTexture::Activer(void)
	{glBindTexture(GL_TEXTURE_2D, m_uiID);}



// ChargerBMP
// -> M�thode de chargement d'un fichier BMP
//---------------------------------------------------------------//
void CTexture::ChargerBMP(void)
{
	// Lecture de l'ent�te du fichier
	sBMPEnteteFichier * pEnteteFichier;
	pEnteteFichier = (sBMPEnteteFichier *)m_ucpBuffer;
	m_ucpBuffer += 14;

	// Lecture de l'ent�te du fichier
	sBMPEnteteInformation * pInfoFichier;
	pInfoFichier = (sBMPEnteteInformation *)m_ucpBuffer;
	m_ucpBuffer += 40;

	if (pInfoFichier->m_uiBpp != 24
		|| pInfoFichier->m_uiDimX == 0
		|| pInfoFichier->m_uiDimY == 0)
	return;

	// Enregistrement des donn�es
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
// -> M�thode de chargement d'un fichier JPG
//---------------------------------------------------------------//
void CTexture::ChargerJPG(FILE * f)
{
	// Structure d'erreur pour le d�codage jpeg.
	jpeg_error_mgr jerr;

	// Mise en place de la structure d'erreur.
	struct jpeg_decompress_struct cinfo;
	cinfo.err = jpeg_std_error(&jerr);

	// Initialisation de la structure de d�compression.
	jpeg_create_decompress(&cinfo);

	// Sp�cification de la source de donn�es.	
	jpeg_stdio_src(&cinfo, f);

	// Lecture de l'ent�te.
	jpeg_read_header(&cinfo, TRUE);

	// D�but de la d�compression.
	jpeg_start_decompress(&cinfo);

	// R�cup�ration des donn�es.
	m_uiComposante = cinfo.num_components;
	m_uiDimX = cinfo.image_width;
	m_uiDimY = cinfo.image_height;

	// Allocation m�moire du tableau de pixels.
	m_ucpPixel = new unsigned char[m_uiDimX * m_uiDimY * m_uiComposante];

	// Cr�ation d'un tableau de ligne de pixels.
	unsigned char ** ligne = new unsigned char * [m_uiDimY];
	for (int i = 0; i < m_uiDimY; i++)
		ligne[i] = &m_ucpPixel[i * m_uiDimX * m_uiComposante];

	// Lecture ligne par ligne des pixels de l'image.
	int lu = 0;
	while (cinfo.output_scanline < cinfo.output_height) 
		lu += jpeg_read_scanlines(&cinfo, &ligne[lu], cinfo.output_height - lu);
	
	// Lib�ration de la m�moire.
	delete [] ligne;

	// Fin de la d�compression.
	jpeg_finish_decompress(&cinfo);

	// Destruction de la structure de d�compression.
	jpeg_destroy_decompress(&cinfo);
}
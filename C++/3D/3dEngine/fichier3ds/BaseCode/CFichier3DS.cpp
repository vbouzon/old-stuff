//---------------------------------------------------------------//
// CFichier3DS.cpp
//
// Implémentation des méthodes et des fonction liées à la classe
// CFichier3DS.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include "OpenGL.h"
#include "CFichier3DS.h"



//---------------------------------------------------------------//
// Construction / Destruction
//---------------------------------------------------------------//

// CFichier
// -> Constructeur par défaut
//---------------------------------------------------------------//
CFichier3DS::CFichier3DS(void)
{
}



// ~CFichier
// -> Destructeur par défaut
//---------------------------------------------------------------//
CFichier3DS::~CFichier3DS(void)
	{Detruire();}



//---------------------------------------------------------------//
// Méthodes
//---------------------------------------------------------------//

// Detruire
// -> Méthode de destruction des attributs
//---------------------------------------------------------------//
void CFichier3DS::Detruire(void)
{
	// Méthode de base
	CFichier::Detruire();

	// Destruction de la liste des matériels
	int nMateriel = (int)m_vMateriel.size();
	for (int i = 0; i < nMateriel; i++)
	{
		m_vMateriel[i]->m_Texture.Detruire();
		delete m_vMateriel[i];
	}
	m_vMateriel.clear();

	// Destruction de la liste des maillages
	int n = 0, j = 0;
	int nMaillage = (int)m_vMaillage.size();
	for (i = 0; i < nMaillage; i++)
	{
		n = (int)m_vMaillage[i]->m_vFace.size();
		for (j = 0; j < n; j++)
			delete m_vMaillage[i]->m_vFace[j];
		m_vMaillage[i]->m_vFace.clear();
		
		n = (int)m_vMaillage[i]->m_vMateriel.size();
		for (j = 0; j < n; j++)
			delete m_vMaillage[i]->m_vMateriel[j];
		m_vMaillage[i]->m_vMateriel.clear();
		
		n = (int)m_vMaillage[i]->m_vSommet.size();
		for (j = 0; j < n; j++)
			delete m_vMaillage[i]->m_vSommet[j];
		m_vMaillage[i]->m_vSommet.clear();
		
		n = (int)m_vMaillage[i]->m_vTexCoord.size();
		for (j = 0; j < n; j++)
			delete m_vMaillage[i]->m_vTexCoord[j];
		m_vMaillage[i]->m_vTexCoord.clear();
		
		delete m_vMaillage[i];
	}
	m_vMaillage.clear();
}



// Charger
// -> Méthode de chargement des données
//---------------------------------------------------------------//
void CFichier3DS::Charger(char * repertoire, char * fichier)
{
	// Méthode de base
	CFichier::Charger(repertoire, fichier);
	if (m_ucpBuffer == NULL)
		return;

	unsigned int uiBytesLus = 0;

	// Variables locales
	s3DSNoeud * noeud = new s3DSNoeud;
	uiBytesLus = 6;

	// Lecture de l'entête du fichier
	memcpy(&noeud->m_usID, m_ucpBuffer, 2);
	m_ucpBuffer += 2;
	memcpy(&noeud->m_uiLongueur, m_ucpBuffer, 4);
	m_ucpBuffer += 4;
	if (noeud->m_usID != NOEUD3DS_MAIN)
	{
		delete [] m_ucpBuffer;
		return;
	}

	// Lecture des noeuds du fichier
	while (uiBytesLus < m_uiLongeur)
	{
		// Lecture du noeud courant
		memcpy(&noeud->m_usID, m_ucpBuffer, 2);
		m_ucpBuffer += 2;
		memcpy(&noeud->m_uiLongueur, m_ucpBuffer, 4);
		m_ucpBuffer += 4;

		// Identification du noeud
		switch (noeud->m_usID)
		{
			// Noeud de la version du fichier
			case NOEUD3DS_VERSION :
			{
				// Vérification du numéro de version
				if (*(int *)&m_ucpBuffer[6] < 3)
					MessageBox(NULL, "Attention", "Le numéro de version est inférieur à 3 !", MB_OK);

				// On passe on noeud suivant
				uiBytesLus += noeud->m_uiLongueur;
				m_ucpBuffer += noeud->m_uiLongueur - 6;
				break;
			}

			// Noeud d'un nouveau materiel
			case NOEUD3DS_EDIT3DS :
			{
				// On passe on noeud suivant
				uiBytesLus += 6;
				break;
			}

			// Noeud d'un nouveau materiel
			case NOEUD3DS_MATERIAL :
			{
				// Nouveau materiel
				s3DSMateriel * materiel = new s3DSMateriel;
				m_vMateriel.push_back(materiel);

				// On passe on noeud suivant
				uiBytesLus += 6;
				break;
			}
			
			// Noeud d'un nom de materiel
			case NOEUD3DS_MATNAME :
			{
				// Récupération du dernier matériel ajouté
				s3DSMateriel * materiel = m_vMateriel[(int)m_vMateriel.size() - 1];

				// Lecture du nom
				strcpy(materiel->m_sNom, (char *)m_ucpBuffer);

				// On passe on noeud suivant
				uiBytesLus += noeud->m_uiLongueur;
				m_ucpBuffer += noeud->m_uiLongueur - 6;
				break;
			}

			// Noeud de couleur ambiant d'un materiel
			case NOEUD3DS_AMBIENT :
			{
				// Récupération du dernier matériel ajouté
				s3DSMateriel * materiel = m_vMateriel[(int)m_vMateriel.size() - 1];

				// Lecture de la couleur
				for (int x = 0; x < 3; x++)
					materiel->m_fAmbiant[x] = (255.0f - m_ucpBuffer[x]) / 255.0f;
				materiel->m_fAmbiant[3] = 1.0;

				// On passe on noeud suivant
				uiBytesLus += noeud->m_uiLongueur;
				m_ucpBuffer += noeud->m_uiLongueur - 6;
				break;
			}

			// Noeud de couleur diffuse d'un materiel
			case NOEUD3DS_DIFFUSE :
			{
				// Récupération du dernier matériel ajouté
				s3DSMateriel * materiel = m_vMateriel[(int)m_vMateriel.size() - 1];

				// Lecture de la couleur
				for (int x = 0; x < 3; x++)
					materiel->m_fDiffuse[x] = (255.0f - m_ucpBuffer[x]) / 255.0f;
				materiel->m_fDiffuse[3] = 1.0;

				// On passe on noeud suivant
				uiBytesLus += noeud->m_uiLongueur;
				m_ucpBuffer += noeud->m_uiLongueur - 6;
				break;
			}

			// Noeud de couleur spéculaire d'un materiel
			case NOEUD3DS_SPECULAR :
			{
				// Récupération du dernier matériel ajouté
				s3DSMateriel * materiel = m_vMateriel[(int)m_vMateriel.size() - 1];

				// Lecture de la couleur
				for (int x = 0; x < 3; x++)
					materiel->m_fSpeculaire[x] = (255.0f - m_ucpBuffer[x]) / 255.0f;
				materiel->m_fSpeculaire[3] = 1.0;

				// On passe on noeud suivant
				uiBytesLus += noeud->m_uiLongueur;
				m_ucpBuffer += noeud->m_uiLongueur - 6;
				break;
			}

			// Noeud de l'éclat d'un materiel
			case NOEUD3DS_SHININESS :
			{
				// Récupération du dernier matériel ajouté
				s3DSMateriel * materiel = m_vMateriel[(int)m_vMateriel.size() - 1];
				materiel->m_fEclat = 0.0;

				// Lecture de l'éclat
				m_ucpBuffer += 6;
				materiel->m_fEclat = *(unsigned short *)m_ucpBuffer;
				materiel->m_fEclat /= 100.0;

				// On passe on noeud suivant
				uiBytesLus += noeud->m_uiLongueur;
				m_ucpBuffer += noeud->m_uiLongueur - 12;
				break;
			}
			
			// Noeud de la trabsparence d'un materiel
			case NOEUD3DS_TRANSPARENT :
			{
				// Récupération du dernier matériel ajouté
				s3DSMateriel * materiel = m_vMateriel[(int)m_vMateriel.size() - 1];

				// Lecture de l'éclat
				m_ucpBuffer += 6;
				unsigned short temp = *(unsigned short *)m_ucpBuffer;
				materiel->m_fDiffuse[3] = (100.0f - temp) / 100.0f;

				// On passe on noeud suivant
				uiBytesLus += noeud->m_uiLongueur;
				m_ucpBuffer += noeud->m_uiLongueur - 12;
				break;
			}
			
			// Noeud de la texture d'un materiel
			case NOEUD3DS_TEXTURE :
			{
				// On passe on noeud suivant
				uiBytesLus += 6;
				break;
			}
			
			// Noeud du fichier de texture d'un materiel
			case NOEUD3DS_TEXFILE :
			{
				// Récupération du dernier matériel ajouté
				s3DSMateriel * materiel = m_vMateriel[(int)m_vMateriel.size() - 1];

				// Lecture du nom du fichier de la texture
				char temp[64];
				strcpy(temp, (char *)m_ucpBuffer);

				// Chargement de la texture
				char * sFichier = new char[strlen(repertoire) + strlen(temp)];
				strcpy(sFichier, repertoire);
				strcat(sFichier, temp);
				materiel->m_Texture.Charger(sFichier);

				// On passe on noeud suivant
				uiBytesLus += noeud->m_uiLongueur;
				m_ucpBuffer += noeud->m_uiLongueur - 6;
				break;
			}
			
			// Noeud d'un objet géométrique
			case NOEUD3DS_MESH :
			{
				// Lecture du nom de l'objet
				char nom[128];
				strcpy(nom, (char *)m_ucpBuffer);

				// On passe on noeud suivant
				uiBytesLus += 6 + strlen(nom) + 1;
				m_ucpBuffer += strlen(nom) + 1;
				break;
			}
			
			// Noeud d'un maillage
			case NOEUD3DS_TRIOBJ :
			{
				// Nouveau materiel
				s3DSMaillage * maillage = new s3DSMaillage;
				m_vMaillage.push_back(maillage);

				// On passe on noeud suivant
				uiBytesLus += 6;
				break;
			}
			
			// Noeud de la liste des sommets d'un maillage
			case NOEUD3DS_VERTLIST :
			{
				// Récupération du dernier maillage ajouté
				s3DSMaillage * maillage = m_vMaillage[(int)m_vMaillage.size() - 1];

				// Récupération du nombre de sommet
				unsigned short n = *(unsigned short *)m_ucpBuffer;
				m_ucpBuffer += 2;

				// Lecture des sommets
				for (int x = 0; x < n; x++)
				{
					// Nouveau sommet
					s3DSSommet * sommet = new s3DSSommet;
					maillage->m_vSommet.push_back(sommet);

					// Lecture des coordonnées
					sommet->m_fCoord[0] = *(float *)&m_ucpBuffer[0];
					sommet->m_fCoord[2] = *(float *)&m_ucpBuffer[4];
					sommet->m_fCoord[1] = *(float *)&m_ucpBuffer[8];
					m_ucpBuffer += 12;

					// Si les coordonnées sont les plus petites ou les plus grandes ...
					for (int i = 0; i < 3; i++)
					{
						if (sommet->m_fCoord[i] > m_v3Max[i]) m_v3Max[i] = sommet->m_fCoord[i];
						if (sommet->m_fCoord[i] < m_v3Min[i]) m_v3Min[i] = sommet->m_fCoord[i];
					}
				}
							
				// On passe on noeud suivant
				uiBytesLus += noeud->m_uiLongueur;
				break;
			}
			
			// Noeud de la liste des faces d'un maillage
			case NOEUD3DS_TRILIST :
			{
				// Récupération du dernier maillage ajouté
				s3DSMaillage * maillage = m_vMaillage[(int)m_vMaillage.size() - 1];

				// Récupération du nombre de face
				unsigned short n = *(unsigned short *)m_ucpBuffer;
				m_ucpBuffer += 2;
				uiBytesLus += 2;

				// Lecture des sommets
				for (int x = 0; x < n; x++)
				{
					// Nouvelle face
					s3DSFace * face = new s3DSFace;
					maillage->m_vFace.push_back(face);

					// Lecture des indices de sommet
					face->m_usIndice[0] = *(unsigned short *)&m_ucpBuffer[0];
					face->m_usIndice[1] = *(unsigned short *)&m_ucpBuffer[2];
					face->m_usIndice[2] = *(unsigned short *)&m_ucpBuffer[4];
					m_ucpBuffer += 8;
					uiBytesLus += 8;

					// Calcul de la normale de face
					face->m_v3Normale = CalculerNormale(CVecteur3(maillage->m_vSommet[face->m_usIndice[0]]->m_fCoord[0],
																  maillage->m_vSommet[face->m_usIndice[0]]->m_fCoord[1],
																  maillage->m_vSommet[face->m_usIndice[0]]->m_fCoord[2]),
														CVecteur3(maillage->m_vSommet[face->m_usIndice[1]]->m_fCoord[0],
																  maillage->m_vSommet[face->m_usIndice[1]]->m_fCoord[1],
																  maillage->m_vSommet[face->m_usIndice[1]]->m_fCoord[2]),
														CVecteur3(maillage->m_vSommet[face->m_usIndice[2]]->m_fCoord[0],
																  maillage->m_vSommet[face->m_usIndice[2]]->m_fCoord[1],
																  maillage->m_vSommet[face->m_usIndice[2]]->m_fCoord[2]));
				}
				uiBytesLus += 6;
				break;
			}
			
			// Noeud de la liste de faces avec un materiel
			case NOEUD3DS_OBJMAT :
			{
				// Récupération du dernier maillage ajouté
				s3DSMaillage * maillage = m_vMaillage[(int)m_vMaillage.size() - 1];

				// Récupération du nom de matériel
				char matName[64];
				strcpy(matName, (char *)m_ucpBuffer);
				m_ucpBuffer += strlen(matName) + 3;

				// Recupération de l'index du matériel
				int matIndex = 0;
				int n = (int)m_vMateriel.size();
				for (int i = 0; i < n; i++)
					if (strcmp(matName, m_vMateriel[i]->m_sNom) == 0)
						break;
				matIndex = i;

				// Création du matériel objet
				s3DSObjMat * materiel = new s3DSObjMat;
				maillage->m_vMateriel.push_back(materiel);
				materiel->m_usMaterielId = matIndex;

				// Lecture des index de faces
				unsigned short faceIndex;
				n = (noeud->m_uiLongueur - 9 - strlen(matName)) / 2;
				for (i = 0; i < n; i++)
				{
					faceIndex = *(unsigned short *)&m_ucpBuffer[0];
					m_ucpBuffer += 2;
					materiel->m_vFace.push_back(faceIndex);
				}

				// On passe on noeud suivant
				uiBytesLus += noeud->m_uiLongueur;
				break;
			}
			
			// Noeud de la liste des coordonnees de texture
			case NOEUD3DS_UVMAP :
			{
				// Récupération du dernier maillage ajouté
				s3DSMaillage * maillage = m_vMaillage[(int)m_vMaillage.size() - 1];

				// Récupération du nombre de coordonnées de texture
				unsigned short n = *(unsigned short *)m_ucpBuffer;
				m_ucpBuffer += 2;

				// Lecture des coordonnées de texture
				for (int x = 0; x < n; x++)
				{
					// Nouvelle coordonnée de texture
					s3DSTexCoord * texCoord = new s3DSTexCoord;
					maillage->m_vTexCoord.push_back(texCoord);

					// Lecture des indices de sommet
					texCoord->m_fCoord[0] = *(float *)&m_ucpBuffer[0];
					texCoord->m_fCoord[1] = *(float *)&m_ucpBuffer[4];
					m_ucpBuffer += 8;
				}

				// On passe on noeud suivant
				uiBytesLus += noeud->m_uiLongueur;
				break;
			}
						
			// Traitement par défaut
			default :
			{
				// On passe on noeud suivant
				uiBytesLus += noeud->m_uiLongueur;
				m_ucpBuffer += noeud->m_uiLongueur - 6;
			}
		}
	}
}



// Dessiner
// -> Méthode de dessin
//---------------------------------------------------------------//
void CFichier3DS::Dessiner(void)
{
	// Méthode de base
	CFichier::Dessiner();

	// Dessin de tous les maillages
	int nMaillage = (int)m_vMaillage.size();
	for (int i = 0; i < nMaillage; i++)
	{
		// Si le maillage possède un matériel ...
		if ((int)m_vMaillage[i]->m_vMateriel.size() > 0)
		{
			// Récupération des données
			s3DSFace ** pFace = &m_vMaillage[i]->m_vFace[0];
			s3DSSommet ** pSommet = &m_vMaillage[i]->m_vSommet[0];
			s3DSTexCoord ** pTexCoord = &m_vMaillage[i]->m_vTexCoord[0];
			s3DSObjMat ** pObjMat = &m_vMaillage[i]->m_vMateriel[0];

			int nMateriel = (int)m_vMaillage[i]->m_vMateriel.size();
			for (int k = 0; k < nMateriel; k++)
			{
				int faceIndex;
				int nFace = (int)pObjMat[k]->m_vFace.size();
		
				// Activation du matériel
				glBindTexture(GL_TEXTURE_2D, m_vMateriel[pObjMat[k]->m_usMaterielId]->m_Texture.m_uiID);

				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_vMateriel[pObjMat[k]->m_usMaterielId]->m_fAmbiant);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_vMateriel[pObjMat[k]->m_usMaterielId]->m_fDiffuse);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_vMateriel[pObjMat[k]->m_usMaterielId]->m_fSpeculaire);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_vMateriel[pObjMat[k]->m_usMaterielId]->m_fEclat);

				// Dessin de toutes les faces
				glBegin(GL_TRIANGLES);
				for (int j = 0; j < nFace; j++)
				{
					faceIndex = pObjMat[k]->m_vFace[j];

					glNormal3d(pFace[faceIndex]->m_v3Normale[0],
							   pFace[faceIndex]->m_v3Normale[1],
							   pFace[faceIndex]->m_v3Normale[2]);

					for (int k = 0; k < 3; k++)
					{
						glTexCoord2dv(pTexCoord[pFace[faceIndex]->m_usIndice[k]]->m_fCoord);
						glVertex3dv(pSommet[pFace[faceIndex]->m_usIndice[k]]->m_fCoord);
					}
				}
				glEnd();
			}
		}
		else
		{
			// Récupération des données
			s3DSFace ** pFace = &m_vMaillage[i]->m_vFace[0];
			s3DSSommet ** pSommet = &m_vMaillage[i]->m_vSommet[0];

			int nFace = (int)m_vMaillage[i]->m_vFace.size();
	
			// Dessin de toutes les faces
			glBegin(GL_TRIANGLES);
			for (int j = 0; j < nFace; j++)
			{
				glNormal3d(pFace[j]->m_v3Normale[0],
						   pFace[j]->m_v3Normale[1],
						   pFace[j]->m_v3Normale[2]);

				for (int k = 0; k < 3; k++)
					glVertex3dv(pSommet[pFace[j]->m_usIndice[k]]->m_fCoord);
			}
			glEnd();
		}
	}
}
//
//	md2.cpp - source file
//
//	David Henry - tfc_duke@club-internet.fr
//


#ifdef WIN32
#define	WIN32_LEAN_AND_MEAN
#include	<windows.h>
#endif

#include	<GL/gl.h>
#include	<fstream>

#include	"md2.h"
#include	"texture.h"



/////////////////////////////////////////////////
//					CModel					   //
/////////////////////////////////////////////////


// table de 162 vecteurs normaux précalculés
vec3_t CMD2Model::m_kAnorms[ NUMVERTEXNORMALS ] = {
#include	"anorms.h"
};



// ----------------------------------------------
// constructeur - initialisation des variables à 0.
// ----------------------------------------------

CMD2Model::CMD2Model( void )
{
	m_pSkins		= 0;
	m_pTexCoords	= 0;
	m_pTriangles	= 0;
	m_pFrames		= 0;
	m_pGLcmds		= 0;

	m_fScale		= 1.0;

	m_uiTexID		= 0;
}



// ----------------------------------------------
// LoadModel() - charge un modèle à partir d'un
// fichier <.md2>.
// ----------------------------------------------

bool CMD2Model::LoadModel( std::string szFilename )
{
	std::fstream	file;	// fichier


	// tentative d'ouverture du fichier
	file.open( szFilename.c_str(), std::ios::in | std::ios::binary );

	if( file.fail() )
		return false;

	// lecture du header
	file.read( (char *)&m_kHeader, sizeof( md2_header_t ) );

	// vérification de l'authenticité du modèle
	if( (m_kHeader.version != MD2_VERSION) || m_kHeader.ident != MD2_IDENT )
		return false;


	// allocation de mémoire pour les données du modèle
	m_pSkins = new md2_skin_t[ m_kHeader.num_skins ];
	m_pTexCoords = new md2_texCoord_t[ m_kHeader.num_st ];
	m_pTriangles = new md2_triangle_t[ m_kHeader.num_tris ];
	m_pFrames = new md2_frame_t[ m_kHeader.num_frames ];
	m_pGLcmds = new int[ m_kHeader.num_glcmds ];

	// lecture des noms de skins
	file.seekg( m_kHeader.offset_skins, std::ios::beg );
	file.read( (char *)m_pSkins, sizeof( char ) * 68 * m_kHeader.num_skins );

	// lecture des coordonnées de texture
	file.seekg( m_kHeader.offset_st, std::ios::beg );
	file.read( (char *)m_pTexCoords, sizeof( md2_texCoord_t ) * m_kHeader.num_st );

	// lecture des triangles
	file.seekg( m_kHeader.offset_tris, std::ios::beg );
	file.read( (char *)m_pTriangles, sizeof( md2_triangle_t ) * m_kHeader.num_tris );

	// lecture des frames
	file.seekg( m_kHeader.offset_frames, std::ios::beg );

	for( int i = 0; i < m_kHeader.num_frames; i++ )
	{
		// allocation de mémoire pour les vertices
		m_pFrames[i].verts = new md2_vertex_t[ m_kHeader.num_vertices ];

		// lecture des données de la frame
		file.read( (char *)&m_pFrames[i].scale, sizeof( vec3_t ) );
		file.read( (char *)&m_pFrames[i].translate, sizeof( vec3_t ) );
		file.read( (char *)&m_pFrames[i].name, sizeof( char ) * 16 );
		file.read( (char *)m_pFrames[i].verts, sizeof( md2_vertex_t ) * m_kHeader.num_vertices );
	}

	// lecture des commandes opengl
	file.seekg( m_kHeader.offset_glcmds, std::ios::beg );
	file.read( (char *)m_pGLcmds, sizeof( int ) * m_kHeader.num_glcmds );


	// fermeture du fichier
	file.close();

	// succès
	return true;
}



// ----------------------------------------------
// LoadTextures() - charge la texture du modèle
// à partir du chemin d'accès spécifié.
// ----------------------------------------------

void CMD2Model::LoadTexture( std::string szFilename )
{
	// pointeur sur le texture manager
	CTextureManager *pTexMgr = CTextureManager::GetInstance();

	// chargement de la texture
	m_uiTexID = pTexMgr->LoadTexture( szFilename, true );
}



// ----------------------------------------------
// FreeModel() - libère la mémoire allouée pour
// le modèle.
// ----------------------------------------------

void CMD2Model::FreeModel( void )
{
	if( m_pSkins )
		delete [] m_pSkins;

	if( m_pTexCoords )
		delete [] m_pTexCoords;

	if( m_pTriangles )
		delete [] m_pTriangles;

	if( m_pFrames )
	{
		for( int i = 0; i < m_kHeader.num_frames; i++ )
		{
			if( m_pFrames[i].verts )
				delete [] m_pFrames[i].verts;
		}

		delete [] m_pFrames;
	}
}



// ----------------------------------------------
// RenderFrame() - dessine le modèle à la frame
// spécifiée.
// ----------------------------------------------

void CMD2Model::RenderFrame( int iFrame )
{
	// calcul de l'index maximum d'une frame du modèle
	int iMaxFrame = m_kHeader.num_frames - 1;

	// vérification de la validité de iFrame
	if( (iFrame < 0) || (iFrame > iMaxFrame) )
		return;


	// activation de la texture du modèle
	glBindTexture( GL_TEXTURE_2D, m_uiTexID );

	// dessin du modèle
	glBegin( GL_TRIANGLES );
		// dessine chaque triangle
		for( int i = 0; i < m_kHeader.num_tris; i++ )
		{
			// dessigne chaque vertex du triangle
			for( int k = 0; k < 3; k++ )
			{
				md2_frame_t *pFrame = &m_pFrames[ iFrame ];
				md2_vertex_t *pVert = &pFrame->verts[ m_pTriangles[i].vertex[k] ];


				// [coordonnées de texture]
				GLfloat s = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].s / m_kHeader.skinwidth;
				GLfloat t = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].t / m_kHeader.skinheight;

				// application des coordonnées de texture
				glTexCoord2f( s, t );

				// [normale]
				glNormal3fv( m_kAnorms[ pVert->normalIndex ] );

				// [vertex]
				vec3_t v;

				// calcul de la position absolue du vertex et redimensionnement
				v[0] = (pFrame->scale[0] * pVert->v[0] + pFrame->translate[0]) * m_fScale;
				v[1] = (pFrame->scale[1] * pVert->v[1] + pFrame->translate[1]) * m_fScale;
				v[2] = (pFrame->scale[2] * pVert->v[2] + pFrame->translate[2]) * m_fScale;

				glVertex3fv( v );
			}
		}
	glEnd();
}



// ----------------------------------------------
// RenderFrameWithGLcmds() - dessine le modèle à
// la frame spécifiée en utilisant les commandes
// OpenGL.
// ----------------------------------------------

void CMD2Model::RenderFrameWithGLcmds( int iFrame )
{
	// calcul de l'index maximum d'une frame du modèle
	int iMaxFrame = m_kHeader.num_frames - 1;

	// vérification de la validité des indices de frame
	if( (iFrame < 0) || (iFrame > iMaxFrame) )
		return;


	// activation de la texture du modèle
	glBindTexture( GL_TEXTURE_2D, m_uiTexID );

	// pointeur sur les commandes opengl
	int	*pGLCmds = &m_pGLcmds[0];

	// on dessine chaque triangle!
	while( int i = *(pGLCmds++) )
	{
		if( i < 0 )
		{
			glBegin( GL_TRIANGLE_FAN );
			i = -i;
		}
		else
		{
			glBegin( GL_TRIANGLE_STRIP );
		}


		for( /* rien */; i > 0; i--, pGLCmds += 3 )
		{
			// pGLCmds[0] : coordonnée de texture s
			// pGLCmds[1] : coordonnée de texture t
			// pGLCmds[2] : index vertex à dessiner

			md2_glcmd_t *pGLcmd = (md2_glcmd_t *)pGLCmds;
			md2_frame_t *pFrame = &m_pFrames[ iFrame ];
			md2_vertex_t *pVert = &pFrame->verts[ pGLcmd->index ];


			// [coordonnées de texture]
			glTexCoord2f( pGLcmd->s, pGLcmd->t );

			// [normale]
			glNormal3fv( m_kAnorms[ pVert->normalIndex ] );

			// [vertex]
			vec3_t v;

			// calcul de la position absolue du vertex et redimensionnement
			v[0] = (pFrame->scale[0] * pVert->v[0] + pFrame->translate[0]) * m_fScale;
			v[1] = (pFrame->scale[1] * pVert->v[1] + pFrame->translate[1]) * m_fScale;
			v[2] = (pFrame->scale[2] * pVert->v[2] + pFrame->translate[2]) * m_fScale;

			glVertex3fv( v );
		}

		glEnd();
	}
}



// ----------------------------------------------
// DrawModelItp() - dessine le modèle avec une
// interpolation de deux frames données.
// ----------------------------------------------

void CMD2Model::DrawModelItp( int iFrameA, int iFrameB, float fInterp )
{
	// calcul de l'index maximum d'une frame du modèle
	int iMaxFrame = m_kHeader.num_frames - 1;

	// vérification de la validité des indices de frame
	if( (iFrameA < 0) || (iFrameB < 0) )
		return;

	if( (iFrameA > iMaxFrame) || (iFrameB > iMaxFrame) )
		return;


	// activation de la texture du modèle
	glBindTexture( GL_TEXTURE_2D, m_uiTexID );

	// dessin du modèle
	glBegin( GL_TRIANGLES );
		// dessine chaque triangle
		for( int i = 0; i < m_kHeader.num_tris; i++ )
		{
			// dessigne chaque vertex du triangle
			for( int k = 0; k < 3; k++ )
			{
				md2_frame_t *pFrameA = &m_pFrames[ iFrameA ];
				md2_frame_t *pFrameB = &m_pFrames[ iFrameB ];

				md2_vertex_t *pVertA = &pFrameA->verts[ m_pTriangles[i].vertex[k] ];
				md2_vertex_t *pVertB = &pFrameB->verts[ m_pTriangles[i].vertex[k] ];


				// [coordonnées de texture]
				GLfloat s = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].s / m_kHeader.skinwidth;
				GLfloat t = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].t / m_kHeader.skinheight;

				// application des coordonnées de texture
				glTexCoord2f( s, t );

				// [normale]
				vec3_t normA, normB, n;

				memcpy( normA, m_kAnorms[ pVertA->normalIndex ], 3 * sizeof( float ) );
				memcpy( normB, m_kAnorms[ pVertB->normalIndex ], 3 * sizeof( float ) );

				// interpolation linéaire
				n[0] = normA[0] + fInterp * (normB[0] - normA[0]);
				n[1] = normA[1] + fInterp * (normB[1] - normA[1]);
				n[2] = normA[2] + fInterp * (normB[2] - normA[2]);

				// spécification de la normale
				glNormal3fv( n );

				// [vertex]
				vec3_t vecA, vecB, v;

				// calcul de la position absolue des vertices
				vecA[0] = pFrameA->scale[0] * pVertA->v[0] + pFrameA->translate[0];
				vecA[1] = pFrameA->scale[1] * pVertA->v[1] + pFrameA->translate[1];
				vecA[2] = pFrameA->scale[2] * pVertA->v[2] + pFrameA->translate[2];

				vecB[0] = pFrameB->scale[0] * pVertB->v[0] + pFrameB->translate[0];
				vecB[1] = pFrameB->scale[1] * pVertB->v[1] + pFrameB->translate[1];
				vecB[2] = pFrameB->scale[2] * pVertB->v[2] + pFrameB->translate[2];

				// interpolation linéaire et redimensionnement
				v[0] = (vecA[0] + fInterp * (vecB[0] - vecA[0])) * m_fScale;
				v[1] = (vecA[1] + fInterp * (vecB[1] - vecA[1])) * m_fScale;
				v[2] = (vecA[2] + fInterp * (vecB[2] - vecA[2])) * m_fScale;

				// dessin du vertex
				glVertex3fv( v );
			}
		}
	glEnd();
}



// ----------------------------------------------
// DrawModelItpWithGLcmds() - dessine le modèle
// avec une interpolation de deux frames données
// en utilisant les commandes OpenGL.
// ----------------------------------------------

void CMD2Model::DrawModelItpWithGLcmds( int iFrameA, int iFrameB, float fInterp )
{
	// calcul de l'index maximum d'une frame du modèle
	int iMaxFrame = m_kHeader.num_frames - 1;

	// vérification de la validité des indices de frame
	if( (iFrameA < 0) || (iFrameB < 0) )
		return;

	if( (iFrameA > iMaxFrame) || (iFrameB > iMaxFrame) )
		return;


	// activation de la texture du modèle
	glBindTexture( GL_TEXTURE_2D, m_uiTexID );

	// pointeur sur les commandes opengl
	int	*pGLCmds = &m_pGLcmds[0];

	// on dessine chaque triangle!
	while( int i = *(pGLCmds++) )
	{
		if( i < 0 )
		{
			glBegin( GL_TRIANGLE_FAN );
			i = -i;
		}
		else
		{
			glBegin( GL_TRIANGLE_STRIP );
		}


		for( /* rien */; i > 0; i--, pGLCmds += 3 )
		{
			md2_glcmd_t *pGLcmd = (md2_glcmd_t *)pGLCmds;

			md2_frame_t *pFrameA = &m_pFrames[ iFrameA ];
			md2_frame_t *pFrameB = &m_pFrames[ iFrameB ];

			md2_vertex_t *pVertA = &pFrameA->verts[ pGLcmd->index ];
			md2_vertex_t *pVertB = &pFrameB->verts[ pGLcmd->index ];


			// [coordonnées de texture]
			glTexCoord2f( pGLcmd->s, pGLcmd->t );

			// [normale]
			vec3_t normA, normB, n;

			memcpy( normA, m_kAnorms[ pVertA->normalIndex ], 3 * sizeof( float ) );
			memcpy( normB, m_kAnorms[ pVertB->normalIndex ], 3 * sizeof( float ) );

			// interpolation linéaire
			n[0] = normA[0] + fInterp * (normB[0] - normA[0]);
			n[1] = normA[1] + fInterp * (normB[1] - normA[1]);
			n[2] = normA[2] + fInterp * (normB[2] - normA[2]);

			// spécification de la normale
			glNormal3fv( n );

			// [vertex]
			vec3_t vecA, vecB, v;

			// calcul de la position absolue des vertices
			vecA[0] = pFrameA->scale[0] * pVertA->v[0] + pFrameA->translate[0];
			vecA[1] = pFrameA->scale[1] * pVertA->v[1] + pFrameA->translate[1];
			vecA[2] = pFrameA->scale[2] * pVertA->v[2] + pFrameA->translate[2];

			vecB[0] = pFrameB->scale[0] * pVertB->v[0] + pFrameB->translate[0];
			vecB[1] = pFrameB->scale[1] * pVertB->v[1] + pFrameB->translate[1];
			vecB[2] = pFrameB->scale[2] * pVertB->v[2] + pFrameB->translate[2];

			// interpolation linéaire et redimensionnement
			v[0] = (vecA[0] + fInterp * (vecB[0] - vecA[0])) * m_fScale;
			v[1] = (vecA[1] + fInterp * (vecB[1] - vecA[1])) * m_fScale;
			v[2] = (vecA[2] + fInterp * (vecB[2] - vecA[2])) * m_fScale;

			// dessin du vertex
			glVertex3fv( v );
		}

		glEnd();
	}
}



/////////////////////////////////////////////////
//					CEntity					   //
/////////////////////////////////////////////////


// ----------------------------------------------
// constructeur - initialisation des variables à 0.
// ----------------------------------------------

CEntity::CEntity( void )
{
	m_pModel		= 0;

	m_iCurrFrame	= 0;
	m_iNextFrame	= 0;
	m_fInterp		= 0.0;

	m_fPercent		= 0.0;
	m_fScale		= 1.0;
	
	tx = 0;
	ty = 0;
	tz = 0;
	
	ry = 0;
	rz = 0;
 }

void CEntity::Translate( float a, float b, float c)
{
     tx += a;
     ty += b;
     tz += c;
     
     if (m_pChild)m_pChild->Translate(a,b,c);
}

void CEntity::RotateY(float a)
{
     ry += a;
     ry = ry - 360 * (int)(ry/360);
     if (m_pChild)m_pChild->RotateY(a);
}

void CEntity::RotateZ(float a)
{
     rz += a;
     rz = rz - 360 * (int)(rz/360);
     if (m_pChild)m_pChild->RotateZ(a);
}

float CEntity::GetAngleZ()
{
    return rz;
}
// ----------------------------------------------
// DrawEntity() - dessine le modèle de l'entitée.
// si le modèle n'est pas animé, on dessine la
// frame iFrame. Si cette dernière est négative,
// on dessine la frame courante.
// ----------------------------------------------

void CEntity::DrawEntity( int iFrame, bool bAnimated, bool bUseGLCmds )
{
	glPushMatrix();
	
	glTranslated(tx,ty,tz);
	
		// rotation des axes
		glRotatef( -90.0, 1.0, 0.0, 0.0 );
		glRotatef( -ry, 0.0, 1.0, 0.0 );
		glRotatef( -rz, 0.0, 0.0, 1.0 );

		// redimensionnement du modèle
		m_pModel->SetScale( m_fScale );


		// rendu du modèle
		if( bAnimated )
		{
			if( bUseGLCmds )
			{
				// dessine le modèle en utilisant les commandes OpenGL
				m_pModel->DrawModelItpWithGLcmds( m_iCurrFrame, m_iNextFrame, m_fInterp );
			}
			else
			{
				// dessine chaque triangle du modèle
				m_pModel->DrawModelItp( m_iCurrFrame, m_iNextFrame, m_fInterp );
			}

			// incrémentation du pourcentage d'interpolation entre les deux frames
			m_fInterp += m_fPercent;
		}
		else
		{
			if( bUseGLCmds )
			{
				// dessine le modèle en utilisant les commandes OpenGL
				if( iFrame < 0 )
					m_pModel->RenderFrameWithGLcmds( m_iCurrFrame );
				else
					m_pModel->RenderFrameWithGLcmds( iFrame );
			}
			else
			{
				// dessine chaque triangle du modèle
				if( iFrame == -1 )
					m_pModel->RenderFrame( m_iCurrFrame );
				else
					m_pModel->RenderFrame( iFrame );
			}
		}

	glPopMatrix();
}



// ----------------------------------------------
// Animate() - calcul les frames courante et
// suivante à partir des frames de début et de
// fin d'animation et du pourcentage d'interpolation.
// ----------------------------------------------

void CEntity::Animate( int iStartFrame, int iEndFrame, float fPercent )
{
	// m_iCurrFrame doit être compris entre iStartFrame et iEndFrame
	if( m_iCurrFrame < iStartFrame )
		m_iCurrFrame = iStartFrame;

	if( m_iCurrFrame > iEndFrame )
		m_iCurrFrame = iStartFrame;

	m_fPercent = fPercent;

	// animation : calcul des frames courante et suivante
	if( m_fInterp >= 1.0 )
	{
		m_fInterp = 0.0f;
		m_iCurrFrame++;

		if( m_iCurrFrame >= iEndFrame )
			m_iCurrFrame = iStartFrame;

		m_iNextFrame = m_iCurrFrame + 1;

		if( m_iNextFrame >= iEndFrame )
			m_iNextFrame = iStartFrame;
	}
}

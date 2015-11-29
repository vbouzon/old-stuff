//
//	md2.h - header file
//
//	David Henry - tfc_duke@club-internet.fr
//


#ifndef		__MD2_H
#define		__MD2_H


#include	<string>
#include	<map>



// nombre de vecteurs normaux précalculés
#define NUMVERTEXNORMALS		162

// identifiant "IDP2" ou 844121161
#define MD2_IDENT				(('2'<<24) + ('P'<<16) + ('D'<<8) + 'I')

// numéro de version
#define	MD2_VERSION				8



// vecteur
typedef float vec3_t[3];



// header md2
typedef struct
{
	int	ident;			// numéro magique : "IDP2"
	int	version;		// version du format : 8

	int	skinwidth;		// largeur texture
	int	skinheight;		// hauteur texture

	int	framesize;		// taille d'une frame en octets

	int	num_skins;		// nombre de skins
	int	num_vertices;	// nombre de vertices par frame
	int	num_st;			// nombre de coordonnées de texture
	int	num_tris;		// nombre de triangles
	int	num_glcmds;		// nombre de commandes opengl
	int	num_frames;		// nombre de frames

	int	offset_skins;	// offset données skins
	int	offset_st;		// offset données coordonnées de texture
	int	offset_tris;	// offset données triangles
	int	offset_frames;	// offset données frames
	int	offset_glcmds;	// offset données commandes OpenGL
	int	offset_end;		// offset fin de fichier

} md2_header_t;



// texture
typedef struct
{
	char	name[68];	// nom du fichier texture

} md2_skin_t;



// coordonnées de texture
typedef struct
{
	short s;
	short t;

} md2_texCoord_t;



// données triangle
typedef struct
{
   unsigned short vertex[3];	// indices vertices du triangle
   unsigned short st[3];		// indices coordonnées de texture

} md2_triangle_t;



// données vertex
typedef struct
{
	unsigned char v[3];			// position dans l'espace (relative au modèle)
	unsigned char normalIndex;	// index normale du vertex

} md2_vertex_t;



// données frame
typedef struct
{
	vec3_t			scale;		// redimensionnement
	vec3_t			translate;	// vecteur translation
	char			name[16];	// nom de la frame
	md2_vertex_t	*verts;		// liste de vertices

} md2_frame_t;



// commande OpenGL
typedef struct
{
	float	s;			// coordonnée de texture s
	float	t;			// coordonnée de texture t
	int		index;		// index vertex

} md2_glcmd_t;




// ==============================================
// CMD2Model - classe objet modèle <.md2>
// ==============================================

class CMD2Model
{
public:
	// constructeur/destructeur
	CMD2Model( void );
	~CMD2Model( void ) { FreeModel(); }


public:
	// fonctions publiques
	bool	LoadModel( std::string szFilename );
	void	LoadTexture( std::string szFilename );

	void	FreeModel( void );

	void	RenderFrame( int iFrame );
	void	RenderFrameWithGLcmds( int iFrame );
	void	DrawModelItp( int iFrameA, int iFrameB, float fInterp );
	void	DrawModelItpWithGLcmds( int iFrameA, int iFrameB, float fInterp );

	void	SetScale( GLfloat fScale ) { m_fScale = fScale; }
	GLfloat	GetScaleValue( void ) { return m_fScale; }


public:
	// variables membres
	static vec3_t	m_kAnorms[ NUMVERTEXNORMALS ];

	md2_header_t	m_kHeader;		// header md2
	md2_skin_t		*m_pSkins;		// données skins
	md2_texCoord_t	*m_pTexCoords;	// coordonnées de texture
	md2_triangle_t	*m_pTriangles;	// données triangles
	md2_frame_t		*m_pFrames;		// données frames
	int				*m_pGLcmds;		// liste de commandes OpenGL

	GLfloat			m_fScale;		// redimensionnement du modèle

	GLuint			m_uiTexID;		// ID texture du modèle

};



// ==============================================
// CEntity - classe objet entitée.
// ==============================================

class CEntity
{
public:
	// constructeur/destructeur
	CEntity( void );
	~CEntity( void ) { }


public:
	// fonctions publiques
	void	SetModel( CMD2Model *pModel ) { m_pModel = pModel; }

	void	DrawEntity( int iFrame, bool bAnimated, bool bUseGLCmds );
	
	void    Translate( float a , float b , float c);
	
    void    RotateY(float a);
	void    RotateZ(float a);
	
	float   GetAngleZ();
   
	void	Animate( int iStartFrame, int iEndFrame, float fPercent );

	void	SetScale( float fScale ) { m_fScale = fScale; }
	float	GetScaleValue( void ) { return m_fScale; }
	
	void    SetChild( CEntity *Child ){ m_pChild = Child; }
	
    


private:
	// variables membres
	CMD2Model	*m_pModel;		// pointeur modèle <.md2>

	float        tx;
    float        ty;
    float        tz;
    
    float        rz;
    float        ry;

    
    int			m_iCurrFrame;	// index frame courante
	int			m_iNextFrame;	// index frame suivante
	float		m_fInterp;		// pourcentage interpolation

	float		m_fPercent;		// incrémentation de m_fInterp
	float		m_fScale;		// redimensionnement du modèle
	
	CEntity    *m_pChild;

};


#endif	// __MD2_H

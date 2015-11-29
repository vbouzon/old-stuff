// tga.h
#ifndef TGA_H
#define TGA_H



// --------------------------------------------
// TGAHEADER - targa header.
// --------------------------------------------

#pragma pack(1)

typedef struct tagTGAHEADER
{
	unsigned char	id_lenght;			// taille du tagTGAHEADER
	unsigned char	color_map_type;		// toujours 0
	unsigned char	image_type;			// type de compression de l'image

	short int		cm_first_entry;		// colormap_origin (toujours 0)
	short int		cm_length;			// colormap_length (toujours 0)
	unsigned char	cm_size;			// colormap_size (toujours 0)

	short int		is_xorigin;			// coordonnées X du coin bas gauche de l'image (toujours 0)
	short int		is_yorigin;			// coordonnées Y du coin bas gauche de l'image (toujours 0)

	short int		is_width;			// largeur de l'image (en pixels)
	short int		is_height;			// hauteur de l'image (en pixels)

	unsigned char	is_pixel_depth;		// nombre de bits par pixel: 16, 24, 32
	unsigned char	is_image_descriptor;// 24 bits = 0x00; 32 bits = 0x80

} TGAHEADER, *PTGAHEADER;

#pragma pack(4)



#ifdef WIN32
#include	<windows.h>
#else

// --------------------------------------------
// RGBTRIPLE - pixel 24 bits.
// --------------------------------------------

typedef struct tagRGBTRIPLE			// rgbt
{
	unsigned char	rgbtBlue;		// bleu
	unsigned char	rgbtGreen;		// vert
	unsigned char	rgbtRed;		// rouge

} RGBTRIPLE, *PRGBTRIPLE;

#endif	// WIN32



// --------------------------------------------
// BGRAQUAD - pixel 32 bits.
// --------------------------------------------

typedef struct tagBGRAQUAD			// rgbt
{
	unsigned char	bgraBlue;		// bleu
	unsigned char	bgraGreen;		// vert
	unsigned char	bgraRed;		// rouge
	unsigned char	bgraAlpha;		// alpha

} BGRAQUAD, *PBGRAQUAD;



// prototype
int LoadFileTGA( const char *filename, unsigned char **pixels, int *width, int *height, bool flipvert );


#endif // TGA_H

#include <stdio.h>
#include <math.h>
#include <ctype.h>
//#include <curses.h>

/*
// Fonctions contenues dans ce fichier

// 01 : f_2D_Read_image_data_gcc() : lecture d'une image SRF 
// 02 : f_2D_Sauv_SRF_gcc() : sauvegarde d'une image SRF dans un fichier

//****************************************
// Prototypes
//****************************************
*/
extern float **f_2D_Read_image_data_gcc(char *bname, int *wi, int *he);

extern int f_2D_Sauv_SRF_gcc(char *image_file_name, float **image, int larg, int haut);

/*
// ***************************************************************
// Definition de variables annexes necessaires au traitement
// des images SRF
// ***************************************************************
*/

#undef PARM
#define PARM(a) a
#define F_BWDITHER  2
#define F_FULLCOLOR 0
#define F_GREYSCALE 1
#define CONV24_8BIT  0
#define CONV24_24BIT 1
#define PIC8  CONV24_8BIT
#define PIC24 CONV24_24BIT
#define F_SUNRAS    ( 7 + F_JPGINC + F_TIFINC)
#define F_JPGINC  0
#define F_TIFINC  0
#define   RAS_MAGIC 0x59a66a95
#define RT_OLD          0       /* Raw pixrect image in 68000 byte order */
#define RT_STANDARD     1       /* Raw pixrect image in 68000 byte order */
#define RT_BYTE_ENCODED 2       /* Run-length compression of bytes */
#define RT_FORMAT_RGB   3       /* XRGB or RGB instead of XBGR or BGR */
#define RMT_RAW         2
#define RMT_NONE        0
#define RMT_EQUAL_RGB   1
#define RAS_RLE 0x80



/* MONO returns total intensity of r,g,b triple (i = .33R + .5G + .17B) */
#define MONO(rd,gn,bl) ( ((int)(rd)*11 + (int)(gn)*16 + (int)(bl)*5) >> 5)

typedef unsigned char byte;

/* structure d'une image au format xv */
typedef struct { byte *pic;                  /* image data */
		 int   w, h;                 /* pic size */
		 int   type;                 /* PIC8 or PIC24 */

		 byte  r[256],g[256],b[256];
		                             /* colormap, if PIC8 */

		 int   normw, normh;         /* 'normal size' of image file
					        (normally eq. w,h, except when
						doing 'quick' load for icons */

		 int   frmType;              /* def. Format type to save in */
		 int   colType;              /* def. Color type to save in */
		 char  fullInfo[128];        /* Format: field in info box */
		 char  shrtInfo[128];        /* short format info */
		 char *comment;              /* comment text */

		 int   numpages;             /* # of page files, if >1 */
		 char  pagebname[64];        /* basename of page files */
	       } PICINFO;

/* structure de l'enete d'une image rasterfile */
struct rasterfile {
     int  ras_magic;
     int  ras_width;
     int  ras_height;
     int  ras_depth;
     int  ras_length;
     int  ras_type;
     int  ras_maptype;
     int  ras_maplength;
};

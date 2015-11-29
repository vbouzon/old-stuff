/* *********************************************************** */
/* Nom du fichier : util_srf_gcc.c                             */  
/* Descriptif :ensemble de fonctions permettant la lecture et  */
/*       l'ecriture d'image srf. Ces fonctions sont extraites  */
/*       du code de XV et adaptees aux besoins du LSI          */
/* Auteur : G. Trossevin                                       */
/* Date de la derniere modif.: 10/12/97                        */
/* Nom du coordinateur informatique : F. Goudail               */ 
/* *********************************************************** */

#include "util_srf_gcc.h"
#include <stdlib.h>
/*
//********************************************************************
// Fonctions contenues dans ce fichier

// 01 : f_2D_Read_image_data_gcc() : lecture d'une image SRF
// 02 : f_2D_Sauv_SRF_gcc() : sauvegarde d'une image SRF dans un fichier

//********************************************************************

*/

static int  rle_read       PARM((byte *, int, int, FILE *, int));
static void sunRas1to8     PARM((byte *, byte *, int));
static void sunRas8to1     PARM((byte *, byte *, int, int));
static int  read_sun_long  PARM((int *, FILE *));
static int  write_sun_long PARM((int, FILE *));
static void fixBGR         PARM((unsigned char *, int, int));


/* *********************************************************** */
void xvbcopy(char *src,char *dst,size_t len)
{     
if (src==dst || len<=0) return;
 
if (src<dst && src+len>dst)
{
    src = src + len - 1;
    dst = dst + len - 1;
    for ( ; len>0; len--, src--, dst--) *dst = *src;
}
 
else
{
    for ( ; len>0; len--, src++, dst++) *dst = *src;
}
}
/*
// *********************************************************** 
//
// 01 : LECTURE D'UNE IMAGE SRF
//
// **********************************************************
*/
float **f_2D_Read_image_data_gcc(char *bname, int *wi, int *he)
{
  FILE     *fp;
  int	   linesize,lsize,csize,isize,i,w,h,d,ligne,colonne;
  byte     *image, *line, *pic8;
  struct   rasterfile sunheader;
  PICINFO  *pinfo;
  float    **f_alloue_2d, **image_ptr;



/* ouverture de l'image srf */

if(bname==NULL) 
  fp = stdin;
else
{
  fp = fopen(bname,"r");
  if (!fp) return(NULL);
}

/* lecture de l'entete srf */
pinfo = (PICINFO *) malloc (sizeof(PICINFO)); 
read_sun_long (&sunheader.ras_magic	 , fp);
read_sun_long (&sunheader.ras_width	 , fp);
read_sun_long (&sunheader.ras_height	 , fp);
read_sun_long (&sunheader.ras_depth	 , fp);
read_sun_long (&sunheader.ras_length	 , fp);
read_sun_long (&sunheader.ras_type	 , fp);
read_sun_long (&sunheader.ras_maptype  , fp);
read_sun_long (&sunheader.ras_maplength, fp);

/* ensemble de tests pour voir si on sait traiter l'image en entree */
if (sunheader.ras_magic != RAS_MAGIC)
{
    fclose(fp);
    return(NULL);
}

if (sunheader.ras_depth != 1
    && sunheader.ras_depth != 8
    && sunheader.ras_depth != 24
    && sunheader.ras_depth != 32)
{
    fprintf (stderr, "On ne peut pas traiter ce type d'image\n");
    fclose(fp);
    return(NULL);
}

if (sunheader.ras_type != RT_OLD
    && sunheader.ras_type != RT_STANDARD
    && sunheader.ras_type != RT_BYTE_ENCODED
    && sunheader.ras_type != RT_FORMAT_RGB)
{
    fprintf (stderr, "On ne peut pas traiter ce type d'image\n");
    fclose(fp);
    return(NULL);
}


if (sunheader.ras_maptype != RMT_RAW
    && sunheader.ras_maptype != RMT_NONE
    && sunheader.ras_maptype != RMT_EQUAL_RGB)
{
    fprintf (stderr, "On ne peut pas traiter ce type d'image\n");
    fclose(fp);
    return(NULL);
}

w = *wi = sunheader.ras_width;
h = *he = sunheader.ras_height;
d = sunheader.ras_depth;
isize = sunheader.ras_length ? sunheader.ras_length : (w * h * d) / 8;
csize = (sunheader.ras_maptype == RMT_NONE) ? 0 : sunheader.ras_maplength;


/* calcul de l'image au format xv */
lsize = w * h;     
if (d == 24 || d == 32) lsize = lsize * 3;
linesize = w * d;
if (linesize % 16) linesize += (16 - (linesize % 16));
linesize /= 8;

/* ********** test sur la colormap *************** */ 
/* lecture de la colormap si il y en a une */
if (sunheader.ras_maptype == RMT_EQUAL_RGB && csize)
{
    fread (pinfo->r, (size_t) 1, (size_t) sunheader.ras_maplength/3, fp);
    fread (pinfo->g, (size_t) 1, (size_t) sunheader.ras_maplength/3, fp);
    fread (pinfo->b, (size_t) 1, (size_t) sunheader.ras_maplength/3, fp);
}
else if (sunheader.ras_maptype == RMT_RAW && csize)
        fseek (fp, (long) csize, 1);

/* il n'y a pas de colormap il faut en recreer une */
/* soit elle correspond a une image binaire soit on */
/* fait une colormap lineaire */
else
{
    if (sunheader.ras_depth == 1)
    {
        pinfo->r[0] = pinfo->g[0] = pinfo->b[0] = 0;
        pinfo->r[1] = pinfo->g[1] = pinfo->b[1] = 255;
    }
    else
        if (sunheader.ras_depth == 8)
        {
            for (i = 0; i < 256; i++)
	    pinfo->r[i] = pinfo->g[i] = pinfo->b[i] = i;
        }
}

/* ************ fin test sur la colormap ******** */

image = (byte *) malloc ((size_t) lsize);
line  = (byte *) malloc ((size_t) linesize);

for (i = 0; i < h; i++)
{
    if (sunheader.ras_type == RT_BYTE_ENCODED)
    {
        if (rle_read (line, 1, linesize, fp, (i==0)) != linesize) break;
    }
    else
    {
         if (fread (line, (size_t) 1, (size_t) linesize, fp) != linesize)
	 {
	     free(image);  free(line);  fclose(fp);
	     return (NULL);
         }
    }
   switch (d)
   {
      case 1:  sunRas1to8 (image + w * i, line, w);	                
                 break;
      case 8:  xvbcopy((char *) line, (char *) image + w * i, (size_t) w);
	         break;
      case 24: xvbcopy((char *) line, (char *) image + w * i * 3, (size_t) w*3);
             break;
      case 32:
      {
	int k;
	byte *ip, *op;
	ip = line;
	op = (byte *) (image + w * i * 3);
	for (k = 0; k<w; k++)
        {
	  *ip++;           /* skip 'alpha' */
	  *op++ = *ip++;   /* rouge   */
	  *op++ = *ip++;   /* vert */
	  *op++ = *ip++;   /* bleu */
	}
      }
   }
}
free(line);

if (d == 24 || d == 32)
{
    if (sunheader.ras_type != RT_FORMAT_RGB) fixBGR(image,w,h);
    pinfo->type = PIC24;
}
else pinfo->type = PIC8;
pinfo->pic = image;
pinfo->w = w;  
pinfo->h = h;
pinfo->normw = pinfo->w;   pinfo->normh = pinfo->h;
pinfo->frmType = F_SUNRAS;
pinfo->colType = (d==1) ? F_BWDITHER : F_FULLCOLOR;
sprintf(pinfo->fullInfo, "Sun %s rasterfile.  (%d plane%s)  (%ld bytes)",sunheader.ras_type == RT_BYTE_ENCODED ? "rle" : "standard",
		d, d == 1 ? "" : "s",
	  	(long) (sizeof(struct rasterfile) + csize + isize));

sprintf(pinfo->shrtInfo, "%dx%d Sun Rasterfile.",w,h);
pinfo->comment = (char *) NULL;


/* **creation de la structure au format LSI ** */
image_ptr = f_alloue_2d;
ligne = colonne = 0;
for (i=0;i < pinfo->w*pinfo->h ; i++)
{
    if (i>(pinfo->w-2))
    {
        if ((i % pinfo->w)==0)
	{
            ligne++;
	    colonne=0;
        }
    }
/* si la colormap n'est pas lineaire cela signifie que */
/* la valeur du pixel doit etre obtenu a travers la */
/* composante en rouge de la colormap */ 
/*//if (sunheader.ras_maplength!=768)
*/
	if (sunheader.ras_type == 2 || sunheader.ras_maplength != 768)
    image_ptr[ligne][colonne] = (float) pinfo->r[pinfo->pic[i]];
else
    image_ptr[ligne][colonne] = (float) pinfo->pic[i];
colonne++;
}


fclose(fp);
free(pinfo);
return(image_ptr);

}/* // f_2D_Read_image_data_gcc()
*/
/* *********************************************************** */
static int rle_read (byte *ptr,int  size,int  nitems,FILE * fp,int  init)
{
  static int count, ch;
  int readbytes, c, read;

  if (init) { count = ch = 0; }

  readbytes = size * nitems;
  for (read = 0; read < readbytes; read++) {
    if (count) {
      *ptr++ = (byte) ch;
      count--;
    }

    else {
      c = getc(fp);
      if (c == EOF) break;

      if (c == RAS_RLE) {   /* 0x80 */
	count = getc(fp);
	if (count == EOF) break;

	if (count < 0) count &= 0xff;
	if (count == 0) *ptr++ = c;
        else {
          if ((ch = getc(fp)) == EOF) break;
          *ptr++ = ch;
        }
      }
      else *ptr++ = c;
    }
  }

  return (read/size);
}

/* *********************************************************** */
static void sunRas1to8 (byte *dest,byte * src,int len)
{
  int i, b;
  int c = 0;

  for (i = 0, b = -1; i < len; i++) {
    if (b < 0) {
      b = 7;
      c = ~*src++;
    }
    *dest++ = ((c >> (b--)) & 1);
  }
}

/* *********************************************************** */
static void sunRas8to1 (byte *dest,byte * src,int len,int flip)
{
  int i, b;
  int c;

  for (c = b = i = 0; i < len; i++) {
    c <<= 1;
    c |= (*src++ ? 1 : 0);
    if (b++ == 7) {
      if (flip) c = ~c;
      *dest++ = (byte) (c & 0xff);
      b = c = 0;
    }
  }
  if (b) {
    if (flip) c = ~c;
    *dest = (byte) ((c<<(8-b)) & 0xff);
  }
}

/*
// ***********************************************************
//
// 02 : SAUVEGARDE D'UNE IMAGE SRF
//
// **********************************************************
*/
int f_2D_Sauv_SRF_gcc(char *image_file_name, float **image, int larg, int haut)

{
  byte *pic;
  int   ptype,w,h;
  byte *rmap, *gmap, *bmap;
  int   ligne, colonne, taille, numcols, colorstyle, userle;
  FILE *fp;  
  struct rasterfile sunheader;
  int   linesize, i, color, d, y, flipbw;
  byte *line, *graypic, graymap[256], *sp, *dp;


if(image_file_name==NULL) 
  fp = stdout;
else 
  fp = fopen(image_file_name,"w");

/* *** initialisation pour creation de l'entete srf *** */
graypic = NULL;
flipbw  = 0;
ptype = PIC8;

taille = larg * haut;
pic = (byte *) malloc ((size_t)taille);  
ligne = colonne = 0;
numcols = 256;
colorstyle = 1;
userle = 0;


for (i=0; i<256; i++)
	graymap[i] = i;
rmap = gmap = bmap = graymap;

for (i=0; i < taille; i++)
{
    if (i>(larg-2))
    {
  	if ((i % larg) == 0)
	{
	    ligne++;
	    colonne = 0;
	}
    }    
    //pic[i] = image[ligne][colonne];
colonne++;
}

w = larg;
h = haut;

/* partie de code xv conserve pour peut etre utilisation */
/* mais ne nous servant pas actuellement : si l'image est*/
/* de type PIC24 et en niveau de gris on la transforme en*/
/* image 8 bits                                          */
if (ptype == PIC24  && colorstyle == F_GREYSCALE)
{
    graypic = (byte *) malloc((size_t) w*h);
    for (i=0,sp=pic,dp=graypic; i<w*h; i++,sp+=3,dp++)
    {
      *dp = MONO(sp[0],sp[1],sp[2]);
    }
    for (i=0; i<256; i++) graymap[i] = i;
    rmap = gmap = bmap = graymap;
    numcols = 256;
    ptype = PIC8;
    pic = graypic;
}

if      (ptype==PIC24)    { d = 24;  linesize = w * 3; }
else if (colorstyle != F_BWDITHER) { d = 8;   linesize = w;     }
     else { 
          d = 1;
          linesize = w;
          if (linesize % 8) linesize += (8 - linesize % 8);
          linesize /= 8;
          }

if (linesize % 2) linesize++;
line = (byte *) malloc((size_t) linesize);
if (!line)
{
    if (graypic) free(graypic);
    return (2);
}

if (d==1)
{
    /* set flipbw if color#0 is black */
    flipbw = (MONO(rmap[0],gmap[0],bmap[0]) < MONO(rmap[1],gmap[1],bmap[1]));
}

/* met en place et ecrit l'entete srf */
sunheader.ras_magic	  = RAS_MAGIC;
sunheader.ras_width	  = w;
sunheader.ras_height	  = h;
sunheader.ras_depth	  = d;
sunheader.ras_length	  = linesize * h;
sunheader.ras_type	  = RT_STANDARD;
sunheader.ras_maptype   = (d==1 || d==24) ? RMT_NONE : RMT_EQUAL_RGB;
sunheader.ras_maplength = (d==1 || d==24) ? 0 : 3 * numcols;

write_sun_long (sunheader.ras_magic	 , fp);
write_sun_long (sunheader.ras_width	 , fp);
write_sun_long (sunheader.ras_height	 , fp);
write_sun_long (sunheader.ras_depth	 , fp);
write_sun_long (sunheader.ras_length	 , fp);
write_sun_long (sunheader.ras_type	 , fp);
write_sun_long (sunheader.ras_maptype  , fp);
write_sun_long (sunheader.ras_maplength, fp);


/* ecriture de la colormap */
if (d == 8)
    if (colorstyle == 1)
      for (color=0; color<3; color++)
	for (i=0; i<numcols; i++)
	  putc (MONO(rmap[i],gmap[i],bmap[i]), fp);
    else
    {
      fwrite (rmap, sizeof(byte), (size_t) numcols, fp);
      fwrite (gmap, sizeof(byte), (size_t) numcols, fp);
      fwrite (bmap, sizeof(byte), (size_t) numcols, fp);
    }





/* ecriture de l'image */
line[linesize-1] = 0;
for (y = 0; y < h; y++)
{
    if (d == 24)
    {
      byte *lptr, *pix;
      for (i=0,lptr=line,pix=pic+y*w*3; i<w; i++,pix+=3)
      {
	*lptr++ = pix[2];
	*lptr++ = pix[1];
	*lptr++ = pix[0];
      }
     }
     else
         if (d == 8)
            xvbcopy((char *) pic + y * w, (char *) line, (size_t) w);
         else /* d == 1 */
            sunRas8to1 (line, pic + y * w, w, flipbw);

     if (fwrite (line, (size_t) 1, (size_t) linesize, fp) != linesize)
     {
         if (graypic) free(graypic);
         free(line);
         return (2);
     }
}

free (line);
if (graypic) free(graypic);
fclose(fp);
return (1);

}
/*// fin f_2D_Sauv_SRF_gcc()
*/

/* *********************************************************** */
/* remet dans l'ordre les bits */
static int read_sun_long(int *l,FILE *fp)
{
  int c0, c1, c2, c3;

  c0 = fgetc(fp);
  c1 = fgetc(fp);
  c2 = fgetc(fp);
  c3 = fgetc(fp);

  *l = (((unsigned long) c0 & 0xff) << 24) |
       (((unsigned long) c1 & 0xff) << 16) |
       (((unsigned long) c2 & 0xff) <<  8) |
       (((unsigned long) c3 & 0xff));

  if (ferror(fp) || feof(fp)) return EOF;

  return 0;
}


/* *********************************************************** */
/* met dans l'ordre SUN les bits avant de les ecrire */
static int write_sun_long (int l,FILE *fp)
{
    char c;

    c = ((l >> 24) & 0xff);
    if (putc (c, fp) == EOF) return (EOF);
    c = ((l >> 16) & 0xff);
    if (putc (c, fp) == EOF) return (EOF);
    c = ((l >> 8) & 0xff);
    if (putc (c, fp) == EOF) return (EOF);
    c = (l & 0xff);
    if (putc (c, fp) == EOF) return (EOF);
    return (0);
}


/* *********************************************************** */
/* kr3 - fix up BGR order SUN 24-bit rasters to be RGB order */
static void fixBGR(unsigned char *img,int w,int h)
{
  int i,npixels;
  unsigned char tmp;

  npixels = w*h;
  for (i=0; i<npixels; i++) {
    tmp = img[0];                   /* swap red and blue channels */
    img[0] = img[2];
    img[2] = tmp;
    img += 3;                       /* bump to next pixel */
  }
}

/* **************FIN DU FICHIER util_srf_gcc.c************** */

float** f_alloue_2d (int dim1,int dim2)
{
        float **array;
        int i, j;

        array = (float **) calloc ((unsigned) dim1, sizeof (float *));
        for (i = 0; i < dim1; i++){
                array [i] = (float *) calloc ((unsigned) dim2, sizeof (float));
                if (array[i] == (float*)0){
               fprintf (stderr, "pas assez de place en memoire avec ch_alloue_2d \n");
                exit(1);
                }}
        for (i = 0; i < dim1; i++){
                for (j = 0; j < dim2; j++){
                        array[i][j] = (float) 0.0;
                }/* next j */
        }/* next i */
        if (array == (float **)0){
                fprintf (stderr, "pas assez de place en memoire vec f_alloue_2d\n");
                exit(1);
        }
        return (array);
}


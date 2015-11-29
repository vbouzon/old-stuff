/*----------------------------------------
   DIBPAL.C --  Fonctions de création  de palette
               (c) Charles Petzold, 1998
  ----------------------------------------*/

#include <windows.h>
#include "dibfile.h"
#include "dibhelp.h"
#include "dibpal.h"

/*------------------------------------------------------------
   DibPalDibTable: Crée une palette avec la table des couleurs de la DIB
  ------------------------------------------------------------*/

HPALETTE DibPalDibTable (HDIB hdib)
{
     HPALETTE     hPalette ;
     int          i, iNum ;
     LOGPALETTE * plp ;
     RGBQUAD      rgb ;

     if (0 == (iNum = DibNumColors (hdib)))
          return NULL ;

     plp = malloc (sizeof (LOGPALETTE) + (iNum - 1) * sizeof (PALETTEENTRY)) ;

     plp->palVersion    = 0x0300 ;
     plp->palNumEntries = iNum ;

     for (i = 0 ; i < iNum ; i++)
     {
          DibGetColor (hdib, i, &rgb) ;

          plp->palPalEntry[i].peRed   = rgb.rgbRed ;
          plp->palPalEntry[i].peGreen = rgb.rgbGreen ;
          plp->palPalEntry[i].peBlue  = rgb.rgbBlue ;
          plp->palPalEntry[i].peFlags = 0 ;
     }
     hPalette = CreatePalette (plp) ;
     free (plp) ;
     return hPalette ;
}

/*------------------------------------------------------------------------
   DibPalAllPurpose: Crée une palette convenant à une large variété
          d'images ; la palette contient 247 entrées, mais 15 d'entre elles 
          sont des duplicatas ou correspondent aux 20 couleurs standard.
  ------------------------------------------------------------------------*/

HPALETTE DibPalAllPurpose (void)
{
     HPALETTE     hPalette ;
     int          i, incr, R, G, B ;
     LOGPALETTE * plp ;

     plp = malloc (sizeof (LOGPALETTE) + 246 * sizeof (PALETTEENTRY)) ;

     plp->palVersion    = 0x0300 ;
     plp->palNumEntries = 247 ;

          // La boucle suivante calcule 31 nuances de gris, dont 3 
          //        correspondent aux 20 couleurs standard

     for (i = 0, G = 0, incr = 8 ; G <= 0xFF ; i++, G += incr)
     {
          plp->palPalEntry[i].peRed   = (BYTE) G ;
          plp->palPalEntry[i].peGreen = (BYTE) G ;
          plp->palPalEntry[i].peBlue  = (BYTE) G ;
          plp->palPalEntry[i].peFlags = 0 ;

          incr = (incr == 9 ? 8 : 9) ;
     }

          // La boucle suivante se charge de 216 entrées, dont 8
          //        correspondent aux 20 couleurs standard et
          //        4 correspondent aux nuances de gris ci-dessus.

     for (R = 0 ; R <= 0xFF ; R += 0x33)
     for (G = 0 ; G <= 0xFF ; G += 0x33)
     for (B = 0 ; B <= 0xFF ; B += 0x33)
     {
          plp->palPalEntry[i].peRed   = (BYTE) R ;
          plp->palPalEntry[i].peGreen = (BYTE) G ;
          plp->palPalEntry[i].peBlue  = (BYTE) B ;
          plp->palPalEntry[i].peFlags = 0 ;

          i++ ;
     }
     hPalette = CreatePalette (plp) ;

     free (plp) ;
     return hPalette ;
}

/*------------------------------------------------------------------------
   DibPalUniformGrays:  Crée une palette de iNum gris uniformément espacés
  ------------------------------------------------------------------------*/

HPALETTE DibPalUniformGrays (int iNum)
{
     HPALETTE     hPalette ;
     int          i ;
     LOGPALETTE * plp ;

     plp = malloc (sizeof (LOGPALETTE) + (iNum - 1) * sizeof (PALETTEENTRY)) ;

     plp->palVersion    = 0x0300 ;
     plp->palNumEntries = iNum ;

     for (i = 0 ; i < iNum ; i++)
     {
          plp->palPalEntry[i].peRed   =
          plp->palPalEntry[i].peGreen = 
          plp->palPalEntry[i].peBlue  = (BYTE) (i * 255 / (iNum - 1)) ;
          plp->palPalEntry[i].peFlags = 0 ;
     }
     hPalette = CreatePalette (plp) ;
     free (plp) ;
     return hPalette ;
}

/*------------------------------------------------------------------------
   DibPalUniformColors: Crée une palette de iNumR x iNumG x iNumB couleurs
  ------------------------------------------------------------------------*/

HPALETTE DibPalUniformColors (int iNumR, int iNumG, int iNumB)
{
     HPALETTE     hPalette ;
     int          i, iNum, R, G, B ;
     LOGPALETTE * plp ;

     iNum = iNumR * iNumG * iNumB ;

     plp = malloc (sizeof (LOGPALETTE) + (iNum - 1) * sizeof (PALETTEENTRY)) ;
     plp->palVersion    = 0x0300 ;
     plp->palNumEntries = iNumR * iNumG * iNumB ;

     i = 0 ;
     for (R = 0 ; R < iNumR ; R++)
     for (G = 0 ; G < iNumG ; G++)
     for (B = 0 ; B < iNumB ; B++)
     {
          plp->palPalEntry[i].peRed   = (BYTE) (R * 255 / (iNumR - 1)) ;
          plp->palPalEntry[i].peGreen = (BYTE) (G * 255 / (iNumG - 1)) ;
          plp->palPalEntry[i].peBlue  = (BYTE) (B * 255 / (iNumB - 1)) ;
          plp->palPalEntry[i].peFlags = 0 ;

          i++ ;
     }
     hPalette = CreatePalette (plp) ;
     free (plp) ;
     return hPalette ;
}

/*---------------------------------------------------------------
   DibPalVga:  Crée une palette basée sur les 16 couleurs VGA standard
  ---------------------------------------------------------------*/

HPALETTE DibPalVga (void)
{
     static RGBQUAD rgb [16] = { 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x80, 0x00,
                                 0x00, 0x80, 0x00, 0x00,
                                 0x00, 0x80, 0x80, 0x00,
                                 0x80, 0x00, 0x00, 0x00,
                                 0x80, 0x00, 0x80, 0x00,
                                 0x80, 0x80, 0x00, 0x00,
                                 0x80, 0x80, 0x80, 0x00,
                                 0xC0, 0xC0, 0xC0, 0x00,
                                 0x00, 0x00, 0xFF, 0x00,
                                 0x00, 0xFF, 0x00, 0x00,
                                 0x00, 0xFF, 0xFF, 0x00,
                                 0xFF, 0x00, 0x00, 0x00,
                                 0xFF, 0x00, 0xFF, 0x00,
                                 0xFF, 0xFF, 0x00, 0x00,
                                 0xFF, 0xFF, 0xFF, 0x00 } ;
     HPALETTE       hPalette ;
     int            i ;
     LOGPALETTE   * plp ;

     plp = malloc (sizeof (LOGPALETTE) + 15 * sizeof (PALETTEENTRY)) ;
     plp->palVersion    = 0x0300 ;
     plp->palNumEntries = 16 ;

     for (i = 0 ; i < 16 ; i++)
     {
          plp->palPalEntry[i].peRed   = rgb[i].rgbRed ;
          plp->palPalEntry[i].peGreen = rgb[i].rgbGreen ;
          plp->palPalEntry[i].peBlue  = rgb[i].rgbBlue ;
          plp->palPalEntry[i].peFlags = 0 ;
     }
     hPalette = CreatePalette (plp) ;
     free (plp) ;
     return hPalette ;
}

/*---------------------------------------------
   Macro utilisée dans les procédures d'optimisation de la palette
  ---------------------------------------------*/

#define PACK_RGB(R,G,B,iRes) ((int) (R) | ((int) (G) <<  (iRes)) |       \
                                          ((int) (B) << ((iRes) + (iRes))))

/*--------------------------------------------------------------------
   AccumColorCounts: remplit piCount (indexé par une couleur RVB compressée)
     avec le nombre des points de cette couleur.
  --------------------------------------------------------------------*/

static void AccumColorCounts (HDIB hdib, int * piCount, int iRes)
{
     int     x, y, cx, cy ;
     RGBQUAD rgb ;

     cx = DibWidth (hdib) ;
     cy = DibHeight (hdib) ;

     for (y = 0 ; y < cy ; y++)
     for (x = 0 ; x < cx ; x++)
     {
          DibGetPixelColor (hdib, x, y, &rgb) ;

          rgb.rgbRed   >>= (8 - iRes) ;
          rgb.rgbGreen >>= (8 - iRes) ;
          rgb.rgbBlue  >>= (8 - iRes) ;

          ++piCount [PACK_RGB (rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue, iRes)] ;
     }
}

/*--------------------------------------------------------------
   DibPalPopularity:  Algorithme de popularité pour couleurs optimisées
  --------------------------------------------------------------*/

HPALETTE DibPalPopularity (HDIB hdib, int iRes)
{
     HPALETTE     hPalette ;
     int          i, iArraySize, iEntry, iCount, iIndex, iMask, R, G, B ;
     int        * piCount ;
     LOGPALETTE * plp ;

          // Contrôles de validité
    
     if (DibBitCount (hdib) < 16)
          return NULL ;

     if (iRes < 3 || iRes > 8)
          return NULL ;

          // Alloue un tableau pour le comptage des couleurs des points

     iArraySize = 1 << (3 * iRes) ;
     iMask = (1 << iRes) - 1 ;

     if (NULL == (piCount = calloc (iArraySize, sizeof (int))))
          return NULL ;

          // Lit les nombres de couleurs

     AccumColorCounts (hdib, piCount, iRes) ;

          // Crée une palette

     plp = malloc (sizeof (LOGPALETTE) + 235 * sizeof (PALETTEENTRY)) ;

     plp->palVersion = 0x0300 ;

     for (iEntry = 0 ; iEntry < 236 ; iEntry++)
     {
          for (i = 0, iCount = 0 ; i < iArraySize ; i++)
               if (piCount[i] > iCount)

               {
                    iCount = piCount[i] ;
                    iIndex = i ;
               }

          if (iCount == 0)
               break ;

          R = (iMask &  iIndex                  ) << (8 - iRes) ;
          G = (iMask & (iIndex >>         iRes )) << (8 - iRes) ;
          B = (iMask & (iIndex >> (iRes + iRes))) << (8 - iRes) ;

          plp->palPalEntry[iEntry].peRed   = (BYTE) R ; 
          plp->palPalEntry[iEntry].peGreen = (BYTE) G ; 
          plp->palPalEntry[iEntry].peBlue  = (BYTE) B ; 
          plp->palPalEntry[iEntry].peFlags = 0 ;

          piCount [iIndex] = 0 ;
     }
          // À la sortie de la boucle,
          // iEntry sera égal au nombre d'entrées stockées

     plp->palNumEntries = iEntry ;

          // Cré la palette, fait le ménage et renvoie 
           //   l'identificateur de la palette 

     hPalette = CreatePalette (plp) ;

     free (piCount) ;
     free (plp) ;

     return hPalette ;
}

/*-------------------------------------------------------
   Structures utilisées pour l'application de l'algorithme de coupe médiane
  -------------------------------------------------------*/

typedef struct           // définit les dimensions d'une boîte
{
     int Rmin, Rmax, Gmin, Gmax, Bmin, Bmax ;
}
MINMAX ;

typedef struct           // pour routine de comparaison de qsort
{
     int     iBoxCount ;
     RGBQUAD rgbBoxAv ;
}

BOXES ;

/*----------------------------
   FindAverageColor: dans une boîte
  ----------------------------*/

static int FindAverageColor (int * piCount, MINMAX mm, 
                             int iRes, RGBQUAD * prgb)
{
     int R, G, B, iR, iG, iB, iTotal, iCount ;

          // Initialisation de variables

     iTotal = iR = iG = iB = 0 ;
          
          // Boucle parcourant toutes les couleurs de la boîte
          
     for (R = mm.Rmin ; R <= mm.Rmax ; R++)
     for (G = mm.Gmin ; G <= mm.Gmax ; G++)
     for (B = mm.Bmin ; B <= mm.Bmax ; B++)
     {
               // Lit le nombre de points de cette couleur

          iCount = piCount [PACK_RGB (R, G, B, iRes)] ;

               // Pondère le nombre de points avec la valeur de la couleur

          iR += iCount * R ;
          iG += iCount * G ;
          iB += iCount * B ;

          iTotal += iCount ;
     }
          // Recherche la couleur moyenne

     prgb->rgbRed   = (BYTE) ((iR / iTotal) << (8 - iRes)) ;
     prgb->rgbGreen = (BYTE) ((iG / iTotal) << (8 - iRes)) ;
     prgb->rgbBlue  = (BYTE) ((iB / iTotal) << (8 - iRes)) ;

          // Renvoie le nombre total de points dans la boîte
     
     return iTotal ;
}

/*------------------------------
   CutBox:  Divise une boîte en deux
  ------------------------------*/

static void CutBox (int * piCount, int iBoxCount, MINMAX mm,
                    int iRes, int iLevel, BOXES * pboxes, int * piEntry)
{
     int    iCount, R, G, B ;
     MINMAX mmNew ;
     
          // Si la boîte est vide, retour

     if (iBoxCount == 0)
          return ;

          // Si le niveau d'imbrication est 8, ou si la boîte égale un point,
          //   nous sommes prêts à trouver la couleur dans la boîte et à
          //   l'enregistrer avec le nombre de points de cette couleur

     if (iLevel == 8 || (mm.Rmin == mm.Rmax && 
                         mm.Gmin == mm.Gmax && 
                         mm.Bmin == mm.Bmax))
     {
          pboxes[*piEntry].iBoxCount = 
               FindAverageColor (piCount, mm, iRes, &pboxes[*piEntry].rgbBoxAv) ;

          (*piEntry) ++ ;
     }
          // Autrement, si bleu est le plus grand côté, le diviser

     else if ((mm.Bmax - mm.Bmin > mm.Rmax - mm.Rmin) && 
              (mm.Bmax - mm.Bmin > mm.Gmax - mm.Gmin))
     {
               // Initialise un compteur et parcourt le côté bleu

          iCount = 0 ;

          for (B = mm.Bmin ; B < mm.Bmax ; B++)
          {
                    // Accumule tous les points pour chaque valeur de bleu

               for (R = mm.Rmin ; R <= mm.Rmax ; R++)
               for (G = mm.Gmin ; G <= mm.Gmax ; G++)
                    iCount += piCount [PACK_RGB (R, G, B, iRes)] ;

                    // Si supérieur à la moitié de la boîte, nous avons terminé

               if (iCount >= iBoxCount / 2)
                    break ;

                    // Si la valeur bleue suivante est le maximum,
                    //   nous avons terminé

               if (B == mm.Bmax - 1)
                    break ;
          }
               // Coupe les deux boîtes à diviser.
               //   Le second argument de CutBox est le nouveau nbre de boîtes.
               //   Le troisième argument est les nouvelles valeurs min et max.

          mmNew = mm ;
          mmNew.Bmin = mm.Bmin ;
          mmNew.Bmax = B ;

          CutBox (piCount, iCount, mmNew, iRes, iLevel + 1, 
                  pboxes, piEntry) ;

          mmNew.Bmin = B + 1 ;
          mmNew.Bmax = mm.Bmax ;

          CutBox (piCount, iBoxCount - iCount, mmNew, iRes, iLevel + 1,
                  pboxes, piEntry) ;
     }
          // Si le rouge est le côté le plus grand,
          //   nous le coupons (comme le bleu)

     else if (mm.Rmax - mm.Rmin > mm.Gmax - mm.Gmin)
     {
          iCount = 0 ;

          for (R = mm.Rmin ; R < mm.Rmax ; R++)
          {
               for (B = mm.Bmin ; B <= mm.Bmax ; B++)
               for (G = mm.Gmin ; G <= mm.Gmax ; G++)
                    iCount += piCount [PACK_RGB (R, G, B, iRes)] ;

               if (iCount >= iBoxCount / 2)
                    break ;

               if (R == mm.Rmax - 1)
                    break ;
          }
          mmNew = mm ;
          mmNew.Rmin = mm.Rmin ;
          mmNew.Rmax = R ;

          CutBox (piCount, iCount, mmNew, iRes, iLevel + 1, 
                  pboxes, piEntry) ;

          mmNew.Rmin = R + 1 ;
          mmNew.Rmax = mm.Rmax ;

          CutBox (piCount, iBoxCount - iCount, mmNew, iRes, iLevel + 1,
                  pboxes, piEntry) ;
     }
          // Autrement, couper dans le vert
     else 
     {
          iCount = 0 ;

          for (G = mm.Gmin ; G < mm.Gmax ; G++)
          {
               for (B = mm.Bmin ; B <= mm.Bmax ; B++)
               for (R = mm.Rmin ; R <= mm.Rmax ; R++)
                    iCount += piCount [PACK_RGB (R, G, B, iRes)] ;

               if (iCount >= iBoxCount / 2)
                    break ;

               if (G == mm.Gmax - 1)
                    break ;
          }
          mmNew = mm ;
          mmNew.Gmin = mm.Gmin ;
          mmNew.Gmax = G ;

          CutBox (piCount, iCount, mmNew, iRes, iLevel + 1, 
                  pboxes, piEntry) ;

          mmNew.Gmin = G + 1 ;
          mmNew.Gmax = mm.Gmax ;

          CutBox (piCount, iBoxCount - iCount, mmNew, iRes, iLevel + 1,
                  pboxes, piEntry) ;
     }
}

/*---------------------------
   Routine de comparaison de qsort
  ---------------------------*/

static int Compare (const BOXES * pbox1, const BOXES * pbox2)
{
     return pbox1->iBoxCount - pbox2->iBoxCount ;
}

/*-----------------------------------------------------------------
   DibPalMedianCut:  Crée une palette basée sur algorithme de coupe médiane
  -----------------------------------------------------------------*/

HPALETTE DibPalMedianCut (HDIB hdib, int iRes)
{
     BOXES        boxes [256] ;
     HPALETTE     hPalette ;
     int          i, iArraySize, iCount, R, G, B, iTotCount, iDim, iEntry = 0 ;
     int        * piCount ;
     LOGPALETTE * plp ;
     MINMAX       mm ;

          // Contrôles de validité
    
     if (DibBitCount (hdib) < 16)
          return NULL ;

     if (iRes < 3 || iRes > 8)
          return NULL ;

          // Accumule les nombres de couleurs des points

     iArraySize = 1 << (3 * iRes) ;

     if (NULL == (piCount = calloc (iArraySize, sizeof (int))))
          return NULL ;

     AccumColorCounts (hdib, piCount, iRes) ;

          // Recherche les dimensions de la boîte totale

     iDim = 1 << iRes ;

     mm.Rmin = mm.Gmin = mm.Bmin = iDim - 1 ;
     mm.Rmax = mm.Gmax = mm.Bmax = 0 ;

     iTotCount = 0 ;

     for (R = 0 ; R < iDim ; R++)
     for (G = 0 ; G < iDim ; G++)
     for (B = 0 ; B < iDim ; B++)
          if ((iCount = piCount [PACK_RGB (R, G, B, iRes)]) > 0)
          {
               iTotCount += iCount ;

               if (R < mm.Rmin) mm.Rmin = R ;
               if (G < mm.Gmin) mm.Gmin = G ;
               if (B < mm.Bmin) mm.Bmin = B ;
               if (R > mm.Rmax) mm.Rmax = R ;
               if (G > mm.Gmax) mm.Gmax = G ;
               if (B > mm.Bmax) mm.Bmax = B ;
          }

          // Coupe la première boîte (fonction itérative).
          //   Au retour de la fonction, la structure des boîtes
          //   pourra contenir 256 valeurs RVB,
          //        une par boîte, et le nombre de points dans
          //        chaque boîte.
          //   La valeur iEntry indiquera le nombre de boîtes non vides.

     CutBox (piCount, iTotCount, mm, iRes, 0, boxes, &iEntry) ;
     free (piCount) ;

          // Trie la table RGB par le nombre de points de chaque couleur

     qsort (boxes, iEntry, sizeof (BOXES), Compare) ;

     plp = malloc (sizeof (LOGPALETTE) + (iEntry - 1) * sizeof (PALETTEENTRY)) ;

     if (plp == NULL)
          return NULL ;

     plp->palVersion    = 0x0300 ;
     plp->palNumEntries = iEntry ;

     for (i = 0 ; i < iEntry ; i++)
     {
          plp->palPalEntry[i].peRed   = boxes[i].rgbBoxAv.rgbRed ;
          plp->palPalEntry[i].peGreen = boxes[i].rgbBoxAv.rgbGreen ;
          plp->palPalEntry[i].peBlue  = boxes[i].rgbBoxAv.rgbBlue ;
          plp->palPalEntry[i].peFlags = 0 ;
     }

     hPalette = CreatePalette (plp) ;

     free (plp) ;
     return hPalette ;
}


/*------------------------------------------
   DIBHELP.C - Procédures d'assistance pour section DIB
                (c) Charles Petzold, 1998
  ------------------------------------------*/

#define WINVER 0x0500
#include <windows.h>
#include "dibhelp.h"

#define HDIB_SIGNATURE (* (int *) "Dib ")

typedef struct
{
     PBYTE    * ppRow ;            // impérativement premier membre pour macros !
     int        iSignature ;
     HBITMAP    hBitmap ;
     BYTE     * pBits ;
     DIBSECTION ds ;
     int        iRShift[3] ;
     int        iLShift[3] ;
}
DIBSTRUCT, * PDIBSTRUCT ;

/*---------------------------------------------------------------
   DibIsValid:  Renvoie TRUE si hdib pointe sur une DIBSTRUCT valide
  ---------------------------------------------------------------*/

BOOL DibIsValid (HDIB hdib)
{
     PDIBSTRUCT pdib = hdib ;

     if (pdib == NULL)
          return FALSE ;

     if (IsBadReadPtr (pdib, sizeof (DIBSTRUCT)))
          return FALSE ;

     if (pdib->iSignature != HDIB_SIGNATURE)
          return FALSE ;
     return TRUE ;
}

/*-----------------------------------------------------------------------
   DibBitmapHandle:  Renvoie l'identificateur de l'objet bitmap de la section DIB
 -----------------------------------------------------------------------*/

HBITMAP DibBitmapHandle (HDIB hdib)
{
     if (!DibIsValid (hdib))
          return NULL ;
     
     return ((PDIBSTRUCT) hdib)->hBitmap ;
}

/*-------------------------------------------
   DibWidth:  Renvoie largeur du point de l'image bitmap
  -------------------------------------------*/

int DibWidth (HDIB hdib)
{
     if (!DibIsValid (hdib))
          return 0 ;
     
     return ((PDIBSTRUCT) hdib)->ds.dsBm.bmWidth ;
}

/*---------------------------------------------
   DibHeight:  Renvoie hauteur du point de l'image bitmap
  ---------------------------------------------*/

int DibHeight (HDIB hdib)
{
     if (!DibIsValid (hdib))
          return 0 ; 
     
     return ((PDIBSTRUCT) hdib)->ds.dsBm.bmHeight ;
}

/*----------------------------------------------------
   DibBitCount:  Renvoie le nombre de bits par point
  ----------------------------------------------------*/

int DibBitCount (HDIB hdib)
{
     if (!DibIsValid (hdib))
          return 0 ;
     
     return ((PDIBSTRUCT) hdib)->ds.dsBm.bmBitsPixel ;
}

/*--------------------------------------------------------------
   DibRowLength:  Renvoie le nombre d'octets par ligne de points
  --------------------------------------------------------------*/

int DibRowLength (HDIB hdib)
{
     if (!DibIsValid (hdib))
          return 0 ;
     
     return 4 * ((DibWidth (hdib) * DibBitCount (hdib) + 31) / 32) ;
}

/*----------------------------------------------------------------
   DibNumColors:  Renvoie le nombre des couleurs de la table des couleurs
  ----------------------------------------------------------------*/

int DibNumColors (HDIB hdib)
{
     PDIBSTRUCT pdib = hdib ;

     if (!DibIsValid (hdib))
          return 0 ;

     if (pdib->ds.dsBmih.biClrUsed != 0)
     {
          return pdib->ds.dsBmih.biClrUsed ;
     }
     else if (DibBitCount (hdib) <= 8)
     {
          return 1 << DibBitCount (hdib) ;
     }
     return 0 ;
}

/*------------------------------------------
   DibMask:  Renvoie l'un des masques de couleurs
  ------------------------------------------*/

DWORD DibMask (HDIB hdib, int i)
{
     PDIBSTRUCT pdib = hdib ;

     if (!DibIsValid (hdib) || i < 0 || i > 2)
          return 0 ;
     
     return pdib->ds.dsBitfields[i] ;
}

/*---------------------------------------------------
   DibRShift:  Renvoie l'une des valeurs de décalage à droite
  ---------------------------------------------------*/

int DibRShift (HDIB hdib, int i)
{
     PDIBSTRUCT pdib = hdib ;

     if (!DibIsValid (hdib) || i < 0 || i > 2)
          return 0 ;
     
     return pdib->iRShift[i] ;
}

/*--------------------------------------------------
   DibLShift:  Renvoie l'une des valeurs de décalage à gauche
  --------------------------------------------------*/

int DibLShift (HDIB hdib, int i)
{
     PDIBSTRUCT pdib = hdib ;

     if (!DibIsValid (hdib) || i < 0 || i > 2)
          return 0 ;
     
     return pdib->iLShift[i] ;
}

/*---------------------------------------------------------------
   DibCompression:  Renvoie la valeur du membre biCompression
  ---------------------------------------------------------------*/

int DibCompression (HDIB hdib)
{
     if (!DibIsValid (hdib))
          return 0 ;

     return ((PDIBSTRUCT) hdib)->ds.dsBmih.biCompression ;
}

/*--------------------------------------------------------------
   DibIsAddressable:  Renvoie TRUE si la DIB n'est pas compressée
  --------------------------------------------------------------*/

BOOL DibIsAddressable (HDIB hdib)
{
     int iCompression ;

     if (!DibIsValid (hdib))
          return FALSE ;

     iCompression = DibCompression (hdib) ;

     if (iCompression == BI_RGB || iCompression == BI_BITFIELDS)
         return TRUE ;

     return FALSE ;
}

/*---------------------------------------------------------------------------
   Ces fonctions renvoient les tailles de divers composants de la section DIB
     TELS QU'ILS SE PRÉSENTERAIENT dans une DIB compressée.  
     Ces fonctions facilitent la conversion de la section DIB en DIB compressée
     et l'enregistrement des fichiers DIB.
  ---------------------------------------------------------------------------*/

DWORD DibInfoHeaderSize (HDIB hdib)
{
     if (!DibIsValid (hdib))
          return 0 ;

     return ((PDIBSTRUCT) hdib)->ds.dsBmih.biSize ;
}

DWORD DibMaskSize (HDIB hdib)
{
     PDIBSTRUCT pdib = hdib ;

     if (!DibIsValid (hdib))
          return 0 ;

     if (pdib->ds.dsBmih.biCompression == BI_BITFIELDS)
          return 3 * sizeof (DWORD) ;

     return 0 ;
}

DWORD DibColorSize (HDIB hdib)
{
     return DibNumColors (hdib) * sizeof (RGBQUAD) ;
} 

DWORD DibInfoSize (HDIB hdib)
{
     return DibInfoHeaderSize(hdib) + DibMaskSize(hdib) + DibColorSize(hdib) ;
}

DWORD DibBitsSize (HDIB hdib)
{
     PDIBSTRUCT pdib = hdib ;

     if (!DibIsValid (hdib))
          return 0 ;

     if (pdib->ds.dsBmih.biSizeImage != 0)
     {
          return pdib->ds.dsBmih.biSizeImage ;
     }
     return DibHeight (hdib) * DibRowLength (hdib) ;
}

DWORD DibTotalSize (HDIB hdib)
{
     return DibInfoSize (hdib) + DibBitsSize (hdib) ;
}

/*----------------------------------------------------------------------
   Ces fonctions renvoient des pointeurs sur les divers composants
     de la section DIB.     
  ----------------------------------------------------------------------*/

BITMAPINFOHEADER * DibInfoHeaderPtr (HDIB hdib)
{
     if (!DibIsValid (hdib))
          return NULL ;
     
     return & (((PDIBSTRUCT) hdib)->ds.dsBmih) ;
}

DWORD * DibMaskPtr (HDIB hdib)
{
     PDIBSTRUCT pdib = hdib ;

     if (!DibIsValid (hdib))
          return 0 ;

     return pdib->ds.dsBitfields ;
}

void * DibBitsPtr (HDIB hdib)
{
     if (!DibIsValid (hdib))
          return NULL ;
     
     return ((PDIBSTRUCT) hdib)->pBits ;
}

/*------------------------------------------------------
   DibGetColor:  Lit l'entrée dans la table des couleurs de la DIB
  ------------------------------------------------------*/

BOOL DibGetColor (HDIB hdib, int index, RGBQUAD * prgb)
{
     PDIBSTRUCT pdib = hdib ;
     HDC        hdcMem ;
     int        iReturn ;

     if (!DibIsValid (hdib))
          return 0 ;

     hdcMem = CreateCompatibleDC (NULL) ;
     SelectObject (hdcMem, pdib->hBitmap) ;
     iReturn = GetDIBColorTable (hdcMem, index, 1, prgb) ;
     DeleteDC (hdcMem) ;
     return iReturn ? TRUE : FALSE ;
}

/*----------------------------------------------------
   DibSetColor:  Écrit une entrée dans la table des couleurs de la DIB
  ----------------------------------------------------*/
 
BOOL DibSetColor (HDIB hdib, int index, RGBQUAD * prgb)
{
     PDIBSTRUCT pdib = hdib ;
     HDC        hdcMem ;
     int        iReturn ;

     if (!DibIsValid (hdib))
          return 0 ;

     hdcMem = CreateCompatibleDC (NULL) ;
     SelectObject (hdcMem, pdib->hBitmap) ;
     iReturn = SetDIBColorTable (hdcMem, index, 1, prgb) ;
     DeleteDC (hdcMem) ;

     return iReturn ? TRUE : FALSE ;
}
/*-----------------------------------------------------------------
   DibPixelPtr:  Renvoie un pointeur sur le point à la position (x, y)
  -----------------------------------------------------------------*/

BYTE * DibPixelPtr (HDIB hdib, int x, int y)
{
     if (!DibIsAddressable (hdib))
          return NULL ;

     if (x < 0 || x >= DibWidth (hdib) || y < 0 || y >= DibHeight (hdib))
          return NULL ;

     return (((PDIBSTRUCT) hdib)->ppRow)[y] + (x * DibBitCount (hdib) >> 3) ;
}

/*-----------------------------------------------
   DibGetPixel:  Obtient une valeur de point à (x, y)
  -----------------------------------------------*/

DWORD DibGetPixel (HDIB hdib, int x, int y)
{
     PBYTE pPixel ;

     if (!(pPixel = DibPixelPtr (hdib, x, y)))
          return 0 ;

     switch (DibBitCount (hdib))
     {
     case  1:  return 0x01 & (* pPixel >> (7 - (x & 7))) ;
     case  4:  return 0x0F & (* pPixel >> (x & 1 ? 0 : 4)) ;
     case  8:  return * pPixel ;
     case 16:  return * (WORD *) pPixel ;
     case 24:  return 0x00FFFFFF & * (DWORD *) pPixel ; 
     case 32:  return * (DWORD *) pPixel ;
     }
     return 0 ;
}

/*--------------------------------------------
   DibSetPixel:  Écrit une valeur de point à (x, y)
  --------------------------------------------*/

BOOL DibSetPixel (HDIB hdib, int x, int y, DWORD dwPixel)
{
     PBYTE pPixel ;

     if (!(pPixel = DibPixelPtr (hdib, x, y)))
          return FALSE ;

     switch (DibBitCount (hdib))
     {
     case  1:  * pPixel &= ~(1     << (7 - (x & 7))) ;
               * pPixel |= dwPixel << (7 - (x & 7)) ;
               break ;

     case  4:  * pPixel &= 0x0F    << (x & 1 ? 4 : 0) ;
               * pPixel |= dwPixel << (x & 1 ? 0 : 4) ;
               break ;

     case  8:  * pPixel = (BYTE) dwPixel ;
               break ;

     case 16:  * (WORD *) pPixel = (WORD) dwPixel ;
               break ;

     case 24:  * (RGBTRIPLE *) pPixel = * (RGBTRIPLE *) &dwPixel ; 
               break ;

     case 32:  * (DWORD *) pPixel = dwPixel ;
               break ;
     default:
          return FALSE ;
     }
     return TRUE ;
}

/*------------------------------------------------------
   DibGetPixelColor:  Obtient la couleur du point à (x, y)
  ------------------------------------------------------*/

BOOL DibGetPixelColor (HDIB hdib, int x, int y, RGBQUAD * prgb)
{
     DWORD      dwPixel ;
     int        iBitCount ;
     PDIBSTRUCT pdib = hdib ;

          // Lit le nombre de bits et l'utilise comme test de validité

     if (0 == (iBitCount = DibBitCount (hdib)))
          return FALSE ;

          // Lit la valeur du point

     dwPixel = DibGetPixel (hdib, x, y) ;

          // Si le nombre de bits est égal ou inférieur à 8,
           // indexer la table des couleurs

     if (iBitCount <= 8)
          return DibGetColor (hdib, (int) dwPixel, prgb) ;

          // Si le nombre de bits est égal à 24, utiliser le point

     else if (iBitCount == 24)
     {
          * (RGBTRIPLE *) prgb = * (RGBTRIPLE *) & dwPixel ;
          prgb->rgbReserved = 0 ;
     }

          // Si le nombre de bits est égal à 32 et si le membre
          // biCompression contient BI_RGB, utilise le point

     else if (iBitCount == 32 && 
               pdib->ds.dsBmih.biCompression == BI_RGB)
     {
          * prgb = * (RGBQUAD *) & dwPixel ;
     }

          // Dans les autres cas, utiliser le masque et les valeurs de décalage 
          //   (nous n'utilisons pas les fonctions DibMask et DibShift pour 
           //    améliorer les performances)
     else
     {
          prgb->rgbRed   = (BYTE) (((pdib->ds.dsBitfields[0] & dwPixel) 
                                   >> pdib->iRShift[0]) << pdib->iLShift[0]) ;

          prgb->rgbGreen = (BYTE) (((pdib->ds.dsBitfields[1] & dwPixel) 
                                   >> pdib->iRShift[1]) << pdib->iLShift[1]) ;

          prgb->rgbBlue  = (BYTE) (((pdib->ds.dsBitfields[2] & dwPixel) 
                                   >> pdib->iRShift[2]) << pdib->iLShift[2]) ;
     }
     return TRUE ;
}

/*---------------------------------------------------
   DibSetPixelColor:  Écrit la couleur du point à (x, y)
  ---------------------------------------------------*/

BOOL DibSetPixelColor (HDIB hdib, int x, int y, RGBQUAD * prgb)
{
     DWORD      dwPixel ;
     int        iBitCount ;
     PDIBSTRUCT pdib = hdib ;

          // Ne pas utiliser cette fonction pour DIB avec tables de couleurs

     iBitCount = DibBitCount (hdib) ;

     if (iBitCount <= 8)
          return FALSE ;

          // Le reste est le contraire de DibGetPixelColor

     else if (iBitCount == 24)
     {
          * (RGBTRIPLE *) & dwPixel = * (RGBTRIPLE *) prgb ;
          dwPixel &= 0x00FFFFFF ;
     }

     else if (iBitCount == 32 &&
               pdib->ds.dsBmih.biCompression == BI_RGB)
     {
          * (RGBQUAD *) & dwPixel = * prgb ;
     }

     else
     {
          dwPixel  = (((DWORD) prgb->rgbRed >> pdib->iLShift[0]) 
                         << pdib->iRShift[0]) ; 

          dwPixel |= (((DWORD) prgb->rgbGreen >> pdib->iLShift[1])
                         << pdib->iRShift[1]) ; 

          dwPixel |= (((DWORD) prgb->rgbBlue >> pdib->iLShift[2])
                         << pdib->iRShift[2]) ; 
     }

     DibSetPixel (hdib, x, y, dwPixel) ;
     return TRUE ;
}
/*--------------------------------------------------------------
   Le calcul des valeurs de décalage à partir des masques de couleurs
     est nécessaire pour la fonction DibCreateFromInfo.
  --------------------------------------------------------------*/

static int MaskToRShift (DWORD dwMask)
{
     int iShift ;

     if (dwMask == 0)
          return 0 ;

     for (iShift = 0 ; !(dwMask & 1) ; iShift++)
          dwMask >>= 1 ;

     return iShift ;
}

static int MaskToLShift (DWORD dwMask)
{
     int iShift ;

     if (dwMask == 0)
          return 0 ;

     while (!(dwMask & 1))
          dwMask >>= 1 ;

     for (iShift = 0 ; dwMask & 1 ; iShift++)
          dwMask >>= 1 ;

     return 8 - iShift ;
}

/*-------------------------------------------------------------------------
   DibCreateFromInfo: Elle est appelée par toutes les fonctions de création
     de DIB.
     Cette fonction est chargée d'appeler CreateDIBSection, d'allouer la
     mémoire pour DIBSTRUCT de créer le pointeur de lignes.
  -------------------------------------------------------------------------*/

HDIB DibCreateFromInfo (BITMAPINFO * pbmi)
{
     BYTE      * pBits ;
     DIBSTRUCT * pdib ;
     HBITMAP     hBitmap ;
     int         i, iRowLength, cy, y ;
     
     hBitmap = CreateDIBSection (NULL, pbmi, DIB_RGB_COLORS, &pBits, NULL, 0) ;

     if (hBitmap == NULL)
          return NULL ;

     if (NULL == (pdib = malloc (sizeof (DIBSTRUCT))))
     {
          DeleteObject (hBitmap) ;
          return NULL ;
     }

     pdib->iSignature = HDIB_SIGNATURE ;
     pdib->hBitmap    = hBitmap ;
     pdib->pBits      = pBits ;

     GetObject (hBitmap, sizeof (DIBSECTION), &pdib->ds) ;

          // Notez que nous pouvons dorénavant utiliser les fonctions 
          //   d'information sur la DIB définies ci-dessus.

          // Si la compression est BI_BITFIELDS, calcule les décalages
          // à partir des masques

     if (DibCompression (pdib) == BI_BITFIELDS)
     {
          for (i = 0 ; i < 3 ; i++)
          {
               pdib->iLShift[i] = MaskToLShift (pdib->ds.dsBitfields[i]) ;
               pdib->iRShift[i] = MaskToRShift (pdib->ds.dsBitfields[i]) ;
          }
     }

          // Si la compression est BI_RGB mais le nombre de bits est 16 ou 32,
          //   affecter les valeurs des membres de bits et des masques

     else if (DibCompression (pdib) == BI_RGB)
     {
          if (DibBitCount (pdib) == 16)
          {
               pdib->ds.dsBitfields[0] = 0x00007C00 ;
               pdib->ds.dsBitfields[1] = 0x000003E0 ;
               pdib->ds.dsBitfields[2] = 0x0000001F ;

               pdib->iRShift[0] = 10 ;
               pdib->iRShift[1] =  5 ;
               pdib->iRShift[2] =  0 ; 

               pdib->iLShift[0] =  3 ;
               pdib->iLShift[1] =  3 ;
               pdib->iLShift[2] =  3 ;
          }
          else if (DibBitCount (pdib) == 24 || DibBitCount (pdib) == 32)
          {
               pdib->ds.dsBitfields[0] = 0x00FF0000 ;
               pdib->ds.dsBitfields[1] = 0x0000FF00 ;
               pdib->ds.dsBitfields[2] = 0x000000FF ;

               pdib->iRShift[0] = 16 ;
               pdib->iRShift[1] =  8 ;
               pdib->iRShift[2] =  0 ; 

               pdib->iLShift[0] =  0 ;
               pdib->iLShift[1] =  0 ;
               pdib->iLShift[2] =  0 ;
          }
     }
          // Alloue un tableau de pointeurs à chaque ligne de la DIB

     cy = DibHeight (pdib) ;

     if (NULL == (pdib->ppRow = malloc (cy * sizeof (BYTE *))))
     {
          free (pdib) ;
          DeleteObject (hBitmap) ;
          return NULL ;
     }

          // Initialisation des pointeurs.

     iRowLength = DibRowLength (pdib) ; 

     if (pbmi->bmiHeader.biHeight > 0)       // DIB inversée (tête en bas)
     {
          for (y = 0 ; y < cy ; y++)
               pdib->ppRow[y] = pBits + (cy - y - 1) * iRowLength ;
     }
     else                                    // DIB droite (tête en haut)
     {
          for (y = 0 ; y < cy ; y++)
               pdib->ppRow[y] = pBits + y * iRowLength ;
     }
     return pdib ;
}

/*--------------------------------------------------
   DibDelete:  Libère toute la mémoire pour la section DIB
  --------------------------------------------------*/

BOOL DibDelete (HDIB hdib)
{
     DIBSTRUCT * pdib = hdib ;

     if (!DibIsValid (hdib))
          return FALSE ;

     free (pdib->ppRow) ;
     DeleteObject (pdib->hBitmap) ;
     free (pdib) ;
     return TRUE ;
}

/*----------------------------------------------------
   DibCreate: Crée un HDIB avec les arguments explicites
  ----------------------------------------------------*/ 

HDIB DibCreate (int cx, int cy, int cBits, int cColors)
{
     BITMAPINFO * pbmi ;
     DWORD        dwInfoSize ;
     HDIB         hDib ;
     int          cEntries ;

     if (cx <= 0 || cy <= 0 || 
         ((cBits !=  1) && (cBits !=  4) && (cBits !=  8) && 
          (cBits != 16) && (cBits != 24) && (cBits != 32)))
     {
          return NULL ;
     }

     if (cColors != 0)
          cEntries = cColors ;
     else if (cBits <= 8)
          cEntries = 1 << cBits ;

     dwInfoSize = sizeof (BITMAPINFOHEADER) + (cEntries - 1) * sizeof (RGBQUAD);

     if (NULL == (pbmi = malloc (dwInfoSize)))
     {
          return NULL ;
     }

     ZeroMemory (pbmi, dwInfoSize) ;

     pbmi->bmiHeader.biSize          = sizeof (BITMAPINFOHEADER) ;
     pbmi->bmiHeader.biWidth         = cx ;
     pbmi->bmiHeader.biHeight        = cy ;
     pbmi->bmiHeader.biPlanes        = 1 ;
     pbmi->bmiHeader.biBitCount      = cBits ;
     pbmi->bmiHeader.biCompression   = BI_RGB ;
     pbmi->bmiHeader.biSizeImage     = 0 ;
     pbmi->bmiHeader.biXPelsPerMeter = 0 ;
     pbmi->bmiHeader.biYPelsPerMeter = 0 ;
     pbmi->bmiHeader.biClrUsed       = cColors ;
     pbmi->bmiHeader.biClrImportant  = 0 ;

     hDib = DibCreateFromInfo (pbmi) ;
     free (pbmi) ;

     return hDib ;
}

/*--------------------------------------------------
   DibCopyToInfo:  Construit la structure BITMAPINFO .
                   Utilisée par DibCopy et par DibCopyToDdb
  --------------------------------------------------*/

static BITMAPINFO * DibCopyToInfo (HDIB hdib)
{
     BITMAPINFO * pbmi ;
     int          i, iNumColors ;
     RGBQUAD    * prgb ;

     if (!DibIsValid (hdib))
          return NULL ;

          // Allocation de la mémoire

     if (NULL == (pbmi = malloc (DibInfoSize (hdib))))
          return NULL ;

          // Copie de l'en-tête d'informations

     CopyMemory (pbmi, DibInfoHeaderPtr (hdib), 
                                        sizeof (BITMAPINFOHEADER));
          
          // Copie des éventuels masques de couleurs

     prgb = (RGBQUAD *) ((BYTE *) pbmi + sizeof (BITMAPINFOHEADER)) ;

     if (DibMaskSize (hdib))
     {
          CopyMemory (prgb, DibMaskPtr (hdib), 3 * sizeof (DWORD)) ;

          prgb = (RGBQUAD *) ((BYTE *) prgb + 3 * sizeof (DWORD)) ;
     }
          // Copie de la table des couleurs

     iNumColors = DibNumColors (hdib) ;

     for (i = 0 ; i < iNumColors ; i++)
          DibGetColor (hdib, i, prgb + i) ;

     return pbmi ;
}

/*-------------------------------------------------------------------
   DibCopy:  Crée une nouvelle section DIB avec une section DIB existante,
     en permutant éventuellement la largeur et la hauteur de la DIB.
  -------------------------------------------------------------------*/

HDIB DibCopy (HDIB hdibSrc, BOOL fRotate)
{
     BITMAPINFO * pbmi ;
     BYTE       * pBitsSrc, * pBitsDst ;
     HDIB         hdibDst ;

     if (!DibIsValid (hdibSrc))
          return NULL ;

     if (NULL == (pbmi = DibCopyToInfo (hdibSrc)))
          return NULL ;

     if (fRotate)
     {
          pbmi->bmiHeader.biWidth = DibHeight (hdibSrc) ;
          pbmi->bmiHeader.biHeight = DibWidth (hdibSrc) ;
     }

     hdibDst = DibCreateFromInfo (pbmi) ;
     free (pbmi) ;

     if (hdibDst == NULL)
          return NULL ;

          // Copie des bits

     if (!fRotate)
     {
          pBitsSrc = DibBitsPtr (hdibSrc) ;
          pBitsDst = DibBitsPtr (hdibDst) ;

          CopyMemory (pBitsDst, pBitsSrc, DibBitsSize (hdibSrc)) ;
     }
     return hdibDst ;
}

/*----------------------------------------------------------------------
   DibCopyToPackedDib est généralement utilisée pour enregistrer les DIB et
     pour les transférer vers le Presse-papiers. Dans cette seconde situation,
     le second argument doit être mis à TRUE pour que la mémoire soit allouée
     avec le drapeau GMEM_SHARE.
  ----------------------------------------------------------------------*/

BITMAPINFO * DibCopyToPackedDib (HDIB hdib, BOOL fUseGlobal)
{
     BITMAPINFO * pPackedDib ;
     BYTE       * pBits ;
     DWORD        dwDibSize ;
     HDC          hdcMem ;
     HGLOBAL      hGlobal ;
     int          iNumColors ;
     PDIBSTRUCT   pdib = hdib ;
     RGBQUAD    * prgb ;

     if (!DibIsValid (hdib))
          return NULL ;

          // Allocation de mémoire pour une DIB compressée

     dwDibSize = DibTotalSize (hdib) ;

     if (fUseGlobal)
     {
          hGlobal = GlobalAlloc (GHND | GMEM_SHARE, dwDibSize) ;
          pPackedDib = GlobalLock (hGlobal) ;
     }
     else
     {
          pPackedDib = malloc (dwDibSize) ;
     }

     if (pPackedDib == NULL) 
          return NULL ;

          // Copie de l'en-tête d'informations

     CopyMemory (pPackedDib, &pdib->ds.dsBmih, sizeof (BITMAPINFOHEADER)) ;

     prgb = (RGBQUAD *) ((BYTE *) pPackedDib + sizeof (BITMAPINFOHEADER)) ;

          // Copie des éventuels masques de couleurs

     if (pdib->ds.dsBmih.biCompression == BI_BITFIELDS)
     {
          CopyMemory (prgb, pdib->ds.dsBitfields, 3 * sizeof (DWORD)) ;

          prgb = (RGBQUAD *) ((BYTE *) prgb + 3 * sizeof (DWORD)) ;
     }
          // Copie de la table des couleurs

     if (iNumColors = DibNumColors (hdib))
     {
          hdcMem = CreateCompatibleDC (NULL) ;
          SelectObject (hdcMem, pdib->hBitmap) ;
          GetDIBColorTable (hdcMem, 0, iNumColors, prgb) ;
          DeleteDC (hdcMem) ;
     }

     pBits = (BYTE *) (prgb + iNumColors) ;

          // Copie des bits

     CopyMemory (pBits, pdib->pBits, DibBitsSize (pdib)) ;

          // Si le dernier argument est TRUE, déverrouille le bloc de
          //   mémoire globale et le transtype en pointeur
          //   en préparation de son renvoi par la fonction

     if (fUseGlobal)
     {
          GlobalUnlock (hGlobal) ;
          pPackedDib = (BITMAPINFO *) hGlobal ;     
     }
     return pPackedDib ;
}

/*------------------------------------------------------------------
   DibCopyFromPackedDib est généralement utilisé pour coller des DIB
     du Presse-papiers.
  ------------------------------------------------------------------*/

HDIB DibCopyFromPackedDib (BITMAPINFO * pPackedDib)
{
     BYTE     * pBits ;     
     DWORD      dwInfoSize, dwMaskSize, dwColorSize ;
     int        iBitCount ;
     PDIBSTRUCT pdib ;

          // Lit la taille de l'en-tête d'informations et effectue
          //   le contrôle de validité.
     
     dwInfoSize = pPackedDib->bmiHeader.biSize ;

     if (dwInfoSize != sizeof (BITMAPCOREHEADER) &&
         dwInfoSize != sizeof (BITMAPINFOHEADER) &&
         dwInfoSize != sizeof (BITMAPV4HEADER) &&
         dwInfoSize != sizeof (BITMAPV5HEADER))
     {
          return NULL ;
     }
          // Lit la taille possible des masques des couleurs

     if (dwInfoSize == sizeof (BITMAPINFOHEADER) &&
          pPackedDib->bmiHeader.biCompression == BI_BITFIELDS)
     {
          dwMaskSize = 3 * sizeof (DWORD) ;
     }
     else
     {
          dwMaskSize = 0 ;
     }
          // Lit la taille de la table des couleurs

     if (dwInfoSize == sizeof (BITMAPCOREHEADER))
     {
          iBitCount = ((BITMAPCOREHEADER *) pPackedDib)->bcBitCount ;

          if (iBitCount <= 8)
          {
               dwColorSize = (1 << iBitCount) * sizeof (RGBTRIPLE) ;
          }
          else
               dwColorSize = 0 ;
     }
     else           // Toutes les DIB non compatibles OS/2
     {
          if (pPackedDib->bmiHeader.biClrUsed > 0)
          {
               dwColorSize = pPackedDib->bmiHeader.biClrUsed * sizeof (RGBQUAD);
          }
          else if (pPackedDib->bmiHeader.biBitCount <= 8)
          {
               dwColorSize = (1 << pPackedDib->bmiHeader.biBitCount) * 
                                                       sizeof (RGBQUAD) ;
          }
          else
          {
               dwColorSize = 0 ;
          }
     }
          // Enfin, lit le pointeur sur les bits de la DIB compressée

     pBits = (BYTE *) pPackedDib + dwInfoSize + dwMaskSize + dwColorSize ;

          // Crée HDIB avec le pointeur sur la DIB compressée

     pdib = DibCreateFromInfo (pPackedDib) ;

          // Copie les bits des points

     CopyMemory (pdib->pBits, pBits, DibBitsSize (pdib)) ;

     return pdib ;
}

/*-----------------------------------------------------
   DibFileLoad:  Crée une section DIB avec un fichier DIB
  -----------------------------------------------------*/

HDIB DibFileLoad (const TCHAR * szFileName)
{
     BITMAPFILEHEADER bmfh ;
     BITMAPINFO     * pbmi ;
     BOOL             bSuccess ;
     DWORD            dwInfoSize, dwBitsSize, dwBytesRead ;
     HANDLE           hFile ;
     HDIB             hDib ;

          // Ouvre le fichier: accès en lecture, interdit l'accès en écriture

     hFile = CreateFile (szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, 
                         OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL) ;

     if (hFile == INVALID_HANDLE_VALUE)
          return NULL ;

          // Lecture de BITMAPFILEHEADER

     bSuccess = ReadFile (hFile, &bmfh, sizeof (BITMAPFILEHEADER), 
                          &dwBytesRead, NULL) ;

     if (!bSuccess || (dwBytesRead != sizeof (BITMAPFILEHEADER))         
                   || (bmfh.bfType != * (WORD *) "BM"))
     {
          CloseHandle (hFile) ;
          return NULL ;
     }
          // Alloue mémoire pour la structure d'informations & lit la structure

     dwInfoSize = bmfh.bfOffBits - sizeof (BITMAPFILEHEADER) ;

     if (NULL == (pbmi = malloc (dwInfoSize)))
     {
          CloseHandle (hFile) ;
          return NULL ;
     }

     bSuccess = ReadFile (hFile, pbmi, dwInfoSize, &dwBytesRead, NULL) ;

     if (!bSuccess || (dwBytesRead != dwInfoSize))
     {
          CloseHandle (hFile) ;
          free (pbmi) ;
          return NULL ;
     }

          // Crée la DIB

     hDib = DibCreateFromInfo (pbmi) ;
     free (pbmi) ;

     if (hDib == NULL)
     {
          CloseHandle (hFile) ;
          return NULL ;
     }
          // Lit les bits

     dwBitsSize = bmfh.bfSize - bmfh.bfOffBits ;

     bSuccess = ReadFile (hFile, ((PDIBSTRUCT) hDib)->pBits, 
                          dwBitsSize, &dwBytesRead, NULL) ;
     CloseHandle (hFile) ;

     if (!bSuccess || (dwBytesRead != dwBitsSize))
     {
          DibDelete (hDib) ;
          return NULL ;
     }
     return hDib ;
}

/*---------------------------------------------
   DibFileSave:  Enregistre une section DIB dans un fichier
  ---------------------------------------------*/

BOOL DibFileSave (HDIB hdib, const TCHAR * szFileName)
{
     BITMAPFILEHEADER bmfh ;
     BITMAPINFO     * pbmi ;
     BOOL             bSuccess ;
     DWORD            dwTotalSize, dwBytesWritten ;
     HANDLE           hFile ;

     hFile = CreateFile (szFileName, GENERIC_WRITE, 0, NULL,
                         CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) ;

     if (hFile == INVALID_HANDLE_VALUE)
          return FALSE ;

     dwTotalSize  = DibTotalSize (hdib) ;
     bmfh.bfType      = * (WORD *) "BM" ;
     bmfh.bfSize      = sizeof (BITMAPFILEHEADER) + dwTotalSize ;
     bmfh.bfReserved1 = 0 ;
     bmfh.bfReserved2 = 0 ;
     bmfh.bfOffBits   = bmfh.bfSize - DibBitsSize (hdib) ;

          // Écrit le BITMAPFILEHEADER

     bSuccess = WriteFile (hFile, &bmfh, sizeof (BITMAPFILEHEADER), 
                           &dwBytesWritten, NULL) ;

     if (!bSuccess || (dwBytesWritten != sizeof (BITMAPFILEHEADER)))
     {
          CloseHandle (hFile) ;
          DeleteFile (szFileName) ;
          return FALSE ;
     }
          // Convertit toute la DIB en format DIB compressée

     if (NULL == (pbmi = DibCopyToPackedDib (hdib, FALSE)))
     {
          CloseHandle (hFile) ;
          DeleteFile (szFileName) ;
          return FALSE ;
     }
          // Écrit la DIB compressée

     bSuccess = WriteFile (hFile, pbmi, dwTotalSize, &dwBytesWritten, NULL) ;
     CloseHandle (hFile) ;
     free (pbmi) ;

     if (!bSuccess || (dwBytesWritten != dwTotalSize))
     {
          DeleteFile (szFileName) ;
          return FALSE ;
     }
     return TRUE ;
}

/*---------------------------------------------------
   DibCopyToDdb:  Pour un affichage plus performant
  ---------------------------------------------------*/

HBITMAP DibCopyToDdb (HDIB hdib, HWND hwnd, HPALETTE hPalette)
{
     BITMAPINFO * pbmi ;
     HBITMAP      hBitmap ;
     HDC          hdc ;

     if (!DibIsValid (hdib))
          return NULL ;

     if (NULL == (pbmi = DibCopyToInfo (hdib)))
          return NULL ;

     hdc = GetDC (hwnd) ;

     if (hPalette)
     {
          SelectPalette (hdc, hPalette, FALSE) ;
          RealizePalette (hdc) ;
     }
     
     hBitmap = CreateDIBitmap (hdc, DibInfoHeaderPtr (hdib), CBM_INIT,
                               DibBitsPtr (hdib), pbmi, DIB_RGB_COLORS) ;

     ReleaseDC (hwnd, hdc) ;
     free (pbmi) ;

     return hBitmap ;
}
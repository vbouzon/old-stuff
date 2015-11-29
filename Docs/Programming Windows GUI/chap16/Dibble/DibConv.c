/*-------------------------------------------------------
   DIBCONV.C -- Convertit des DIB d'un format vers un autre
                (c) Charles Petzold, 1998
  -------------------------------------------------------*/

#include <windows.h>
#include "dibhelp.h"
#include "dibpal.h"
#include "dibconv.h"

HDIB DibConvert (HDIB hdibSrc, int iBitCountDst)
{
     HDIB         hdibDst ;
     HPALETTE     hPalette ;
     int          i, x, y, cx, cy, iBitCountSrc, cColors ;
     PALETTEENTRY pe ;
     RGBQUAD      rgb ;
     WORD         wNumEntries ;

     cx = DibWidth (hdibSrc) ;
     cy = DibHeight (hdibSrc) ;
     iBitCountSrc = DibBitCount (hdibSrc) ;

     if (iBitCountSrc == iBitCountDst)
          return NULL ;

          // DIB avec table des couleurs en DIB 
          //   avec table des couleurs plus grande :

     if ((iBitCountSrc < iBitCountDst) && (iBitCountDst <= 8))
     {
          cColors = DibNumColors (hdibSrc) ;
          hdibDst = DibCreate (cx, cy, iBitCountDst, cColors) ;

          for (i = 0 ; i < cColors ; i++)
          {
               DibGetColor (hdibSrc, i, &rgb) ;
               DibSetColor (hdibDst, i, &rgb) ;
          }

          for (x = 0 ; x < cx ; x++)
          for (y = 0 ; y < cy ; y++)
          {
               DibSetPixel (hdibDst, x, y, DibGetPixel (hdibSrc, x, y)) ;
          }
     }
          // Toute DIB en DIB sans table des couleurs

     else if (iBitCountDst >= 16)
     {
          hdibDst = DibCreate (cx, cy, iBitCountDst, 0) ;

          for (x = 0 ; x < cx ; x++)
          for (y = 0 ; y < cy ; y++)
          {
               DibGetPixelColor (hdibSrc, x, y, &rgb) ;
               DibSetPixelColor (hdibDst, x, y, &rgb) ;
          }
     }
          // DIB sans table des couleurs en DIB 8 bits

     else if (iBitCountSrc >= 16 && iBitCountDst == 8)
     {
          hPalette = DibPalMedianCut (hdibSrc, 6) ;

          GetObject (hPalette, sizeof (WORD), &wNumEntries) ;

          hdibDst = DibCreate (cx, cy, 8, wNumEntries) ;
          
          for (i = 0 ; i < (int) wNumEntries ; i++)
          {
               GetPaletteEntries (hPalette, i, 1, &pe) ;

               rgb.rgbRed   = pe.peRed ;
               rgb.rgbGreen = pe.peGreen ;
               rgb.rgbBlue  = pe.peBlue ;
               rgb.rgbReserved = 0 ;

               DibSetColor (hdibDst, i, &rgb) ;
          }

          for (x = 0 ; x < cx ; x++)
          for (y = 0 ; y < cy ; y++)
          {
               DibGetPixelColor (hdibSrc, x, y, &rgb) ;

               DibSetPixel (hdibDst, x, y,
                    GetNearestPaletteIndex (hPalette, 
                         RGB (rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue))) ;
          }
          DeleteObject (hPalette) ;
     }
          // Toute DIB en DIB monochrome

     else if (iBitCountDst == 1)
     {
          hdibDst = DibCreate (cx, cy, 1, 0) ;
          hPalette = DibPalUniformGrays (2) ;

          for (i = 0 ; i < 2 ; i++)
          {
               GetPaletteEntries (hPalette, i, 1, &pe) ;

               rgb.rgbRed   = pe.peRed ;
               rgb.rgbGreen = pe.peGreen ;
               rgb.rgbBlue  = pe.peBlue ;
               rgb.rgbReserved = 0 ;

               DibSetColor (hdibDst, i, &rgb) ;
          }

          for (x = 0 ; x < cx ; x++)
          for (y = 0 ; y < cy ; y++)
          {
               DibGetPixelColor (hdibSrc, x, y, &rgb) ;

               DibSetPixel (hdibDst, x, y,
                    GetNearestPaletteIndex (hPalette, 
                         RGB (rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue))) ;
          }
          DeleteObject (hPalette) ;
     }
          // Toute DIB non monochrome en DIB 4 bits

     else if (iBitCountSrc >= 8 && iBitCountDst == 4)
     {
          hdibDst = DibCreate (cx, cy, 4, 0) ;
          hPalette = DibPalVga () ;

          for (i = 0 ; i < 16 ; i++)
          {
               GetPaletteEntries (hPalette, i, 1, &pe) ;

               rgb.rgbRed   = pe.peRed ;
               rgb.rgbGreen = pe.peGreen ;
               rgb.rgbBlue  = pe.peBlue ;
               rgb.rgbReserved = 0 ;

               DibSetColor (hdibDst, i, &rgb) ;
          }

          for (x = 0 ; x < cx ; x++)
          for (y = 0 ; y < cy ; y++)
          {
               DibGetPixelColor (hdibSrc, x, y, &rgb) ;

               DibSetPixel (hdibDst, x, y,
                    GetNearestPaletteIndex (hPalette, 
                         RGB (rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue))) ;
          }
          DeleteObject (hPalette) ;
     }
          // Ne devrait pas être nécessaire

     else
          hdibDst = NULL ;

     return hdibDst ;
}

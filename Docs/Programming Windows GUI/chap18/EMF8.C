/*-------------------------------------
   EMF8.C -- D�mo EMF N� 8
             (c) Charles Petzold, 1998
  -------------------------------------*/

#include <windows.h>

TCHAR szClass [] = TEXT ("EMF8") ;
TCHAR szTitle [] = TEXT ("D�mo EMF N� 8") ;

void DrawRuler (HDC hdc, int cx, int cy)
{
     int     iAdj, i, iHeight ;
     LOGFONT lf ;
     TCHAR   ch ;

     iAdj = GetVersion () & 0x80000000 ? 0 : 1 ;

          // Stylo noir de 1 point de largeur

     SelectObject (hdc, CreatePen (PS_SOLID, cx / 72 / 6, 0)) ;

          // Rectangle entourant le stylo entier (avec ajustement)

     Rectangle (hdc, iAdj, iAdj, cx + iAdj + 1, cy + iAdj + 1) ;

          // Marques
          
     for (i = 1 ; i < 96 ; i++)
     {
               if (i % 16 == 0) iHeight = cy /  2 ;    // pouces
          else if (i %  8 == 0) iHeight = cy /  3 ;    // demi pouces
          else if (i %  4 == 0) iHeight = cy /  5 ;    // quart de pouce
          else if (i %  2 == 0) iHeight = cy /  8 ;    // huiti�me de pouce
          else                  iHeight = cy / 12 ;    // seizi�me de pouce

          MoveToEx (hdc, i * cx / 96, cy, NULL) ;
          LineTo   (hdc, i * cx / 96, cy - iHeight) ;
     }
          // Cr�er la police logique

     FillMemory (&lf, sizeof (lf), 0) ;
     lf.lfHeight = cy / 2 ;
     lstrcpy (lf.lfFaceName, TEXT ("Times New Roman")) ;

     SelectObject (hdc, CreateFontIndirect (&lf)) ;
     SetTextAlign (hdc, TA_BOTTOM | TA_CENTER) ;
     SetBkMode    (hdc, TRANSPARENT) ;

          // afficher les nombres

     for (i = 1 ; i <= 5 ; i++)
     {
          ch = (TCHAR) (i + '0') ;
          TextOut (hdc, i * cx / 6, cy / 2, &ch, 1) ;
     }
          // Effacer

     DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
     DeleteObject (SelectObject (hdc, GetStockObject (BLACK_PEN))) ;
}

void CreateRoutine (HWND hwnd)
{
     HDC          hdcEMF ;
     HENHMETAFILE hemf ;
     int          cxMms, cyMms, cxPix, cyPix, xDpi, yDpi ;

     hdcEMF = CreateEnhMetaFile (NULL, TEXT ("emf8.emf"), NULL,
                                 TEXT ("EMF8\0EMF Demo #8\0")) ;
     if (hdcEMF == NULL)
          return ;

     cxMms = GetDeviceCaps (hdcEMF, HORZSIZE) ;
     cyMms = GetDeviceCaps (hdcEMF, VERTSIZE) ;
     cxPix = GetDeviceCaps (hdcEMF, HORZRES) ;
     cyPix = GetDeviceCaps (hdcEMF, VERTRES) ;

     xDpi = cxPix * 254 / cxMms / 10 ;
     yDpi = cyPix * 254 / cyMms / 10 ;

     DrawRuler (hdcEMF, 6 * xDpi, yDpi) ;

     hemf = CloseEnhMetaFile (hdcEMF) ;

     DeleteEnhMetaFile (hemf) ;
}

void PaintRoutine (HWND hwnd, HDC hdc, int cxArea, int cyArea)
{
     ENHMETAHEADER emh ;
     HENHMETAFILE  hemf ;
     int           cxImage, cyImage ;
     RECT          rect ;

     hemf = GetEnhMetaFile (TEXT ("emf8.emf")) ;

     GetEnhMetaFileHeader (hemf, sizeof (emh), &emh) ;

     cxImage = emh.rclBounds.right - emh.rclBounds.left ;
     cyImage = emh.rclBounds.bottom - emh.rclBounds.top ;

     rect.left   = (cxArea - cxImage) / 2 ;
     rect.right  = (cxArea + cxImage) / 2 ;
     rect.top    = (cyArea - cyImage) / 2 ;
     rect.bottom = (cyArea + cyImage) / 2 ;

     PlayEnhMetaFile (hdc, hemf, &rect) ;

     DeleteEnhMetaFile (hemf) ;
}

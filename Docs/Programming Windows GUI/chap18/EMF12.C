/*---------------------------------------
   EMF12.C -- Démo EMF N° 12
              (c) Charles Petzold, 1998
  ---------------------------------------*/

#include <windows.h>

TCHAR szClass [] = TEXT ("EMF12") ;
TCHAR szTitle [] = TEXT ("Démo EMF N° 12") ;

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
          else if (i %  8 == 0) iHeight = cy /  3 ;    // demi pouce
          else if (i %  4 == 0) iHeight = cy /  5 ;    // quart de pouce
          else if (i %  2 == 0) iHeight = cy /  8 ;    // huitième de pouce
          else                  iHeight = cy / 12 ;    // seizième de pouce

          MoveToEx (hdc, i * cx / 96, cy, NULL) ;
          LineTo   (hdc, i * cx / 96, cy - iHeight) ;
     }
          // Crée la police logique 

     FillMemory (&lf, sizeof (lf), 0) ;
     lf.lfHeight = cy / 2 ;
     lstrcpy (lf.lfFaceName, TEXT ("Times New Roman")) ;

     SelectObject (hdc, CreateFontIndirect (&lf)) ;
     SetTextAlign (hdc, TA_BOTTOM | TA_CENTER) ;
     SetBkMode    (hdc, TRANSPARENT) ;

          // Affiche les nombres

     for (i = 1 ; i <= 5 ; i++)
     {
          ch = (TCHAR) (i + '0') ;
          TextOut (hdc, i * cx / 6, cy / 2, &ch, 1) ;
     }
          // Efface

     DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
     DeleteObject (SelectObject (hdc, GetStockObject (BLACK_PEN))) ;
}

void CreateRoutine (HWND hwnd)
{
     HDC          hdcEMF ;
     HENHMETAFILE hemf ;
     int          cxMms, cyMms, cxPix, cyPix, xDpi, yDpi ;
     
     hdcEMF = CreateEnhMetaFile (NULL, TEXT ("emf12.emf"), NULL,
                                 TEXT ("EMF13\0EMF Demo #12\0")) ;
     
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
     POINT         pt ;
     int           cxImage, cyImage ;
     RECT          rect ;
     
     SetMapMode (hdc, MM_HIMETRIC) ;
     
     SetViewportOrgEx (hdc, 0, cyArea, NULL) ;
     
     pt.x = cxArea ;
     pt.y = 0 ;
     
     DPtoLP (hdc, &pt, 1) ;
     
     hemf = GetEnhMetaFile (TEXT ("emf12.emf")) ;
     
     GetEnhMetaFileHeader (hemf, sizeof (emh), &emh) ;
     
     cxImage = emh.rclFrame.right - emh.rclFrame.left ;
     cyImage = emh.rclFrame.bottom - emh.rclFrame.top ;
     
     rect.left   = (pt.x - cxImage) / 2 ;
     rect.top    = (pt.y + cyImage) / 2 ;
     rect.right  = (pt.x + cxImage) / 2 ;
     rect.bottom = (pt.y - cyImage) / 2 ;
     
     PlayEnhMetaFile (hdc, hemf, &rect) ;
     
     DeleteEnhMetaFile (hemf) ;
}

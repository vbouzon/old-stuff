/*---------------------------------------
   EMF11.C -- Démo EMF N° 11
              (c) Charles Petzold, 1998
  ---------------------------------------*/

#include <windows.h>

TCHAR szClass [] = TEXT ("EMF11") ;
TCHAR szTitle [] = TEXT ("Démo EMF N° 11") ;

void DrawRuler (HDC hdc, int cx, int cy)
{
     int     i, iHeight ;
     LOGFONT lf ;
     TCHAR   ch ;

          // Stylo noir de 1 point de largeur

     SelectObject (hdc, CreatePen (PS_SOLID, cx / 72 / 6, 0)) ;

          // Rectangle entourant le stylo entier (avec ajustement)

     if (GetVersion () & 0x80000000)              // Windows 98
          Rectangle (hdc, 0, -2, cx + 2, cy) ;
     else                                         // Windows NT
          Rectangle (hdc, 0, -1, cx + 1, cy) ;

          // Tick marks
          
     for (i = 1 ; i < 96 ; i++)
     {
               if (i % 16 == 0) iHeight = cy /  2 ;    // pouces
          else if (i %  8 == 0) iHeight = cy /  3 ;    // demi pouce
          else if (i %  4 == 0) iHeight = cy /  5 ;    // quart de pouce
          else if (i %  2 == 0) iHeight = cy /  8 ;    // huitièmes de pouce
          else                  iHeight = cy / 12 ;    // seuzième de pouce

          MoveToEx (hdc, i * cx / 96, 0, NULL) ;
          LineTo   (hdc, i * cx / 96, iHeight) ;
     }
          // Créer la police logique 

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
     
     hdcEMF = CreateEnhMetaFile (NULL, TEXT ("emf11.emf"), NULL,
                                 TEXT ("EMF11\0EMF Demo #11\0")) ;
     
     SetMapMode (hdcEMF, MM_LOENGLISH) ;
     
     DrawRuler (hdcEMF, 600, 100) ;
     
     hemf = CloseEnhMetaFile (hdcEMF) ;
     
     DeleteEnhMetaFile (hemf) ;
}

void PaintRoutine (HWND hwnd, HDC hdc, int cxArea, int cyArea)
{
     ENHMETAHEADER emh ;
     HENHMETAFILE  hemf ;
     int           cxMms, cyMms, cxPix, cyPix, cxImage, cyImage ;
     RECT          rect ;
     
     cxMms = GetDeviceCaps (hdc, HORZSIZE) ;
     cyMms = GetDeviceCaps (hdc, VERTSIZE) ;
     cxPix = GetDeviceCaps (hdc, HORZRES) ;
     cyPix = GetDeviceCaps (hdc, VERTRES) ;
     
     hemf = GetEnhMetaFile (TEXT ("emf11.emf")) ;
     
     GetEnhMetaFileHeader (hemf, sizeof (emh), &emh) ;
     
     cxImage = emh.rclFrame.right - emh.rclFrame.left ;
     cyImage = emh.rclFrame.bottom - emh.rclFrame.top ;
     
     cxImage = cxImage * cxPix / cxMms / 100 ;
     cyImage = cyImage * cyPix / cyMms / 100 ;
     
     rect.left   = (cxArea - cxImage) / 2 ;
     rect.top    = (cyArea - cyImage) / 2 ;
     rect.right  = (cxArea + cxImage) / 2 ;
     rect.bottom = (cyArea + cyImage) / 2 ;
     
     PlayEnhMetaFile (hdc, hemf, &rect) ;
   
     DeleteEnhMetaFile (hemf) ;
}

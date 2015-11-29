/*---------------------------------------
   EZTEST.C -- Test de EZFONT
               (c) Charles Petzold, 1998
  ---------------------------------------*/

#include <windows.h>
#include "ezfont.h"

TCHAR szAppName [] = TEXT ("EZTest") ;
TCHAR szTitle   [] = TEXT ("Test de EZFONT") ;

void PaintRoutine (HWND hwnd, HDC hdc, int cxArea, int cyArea)
{
     HFONT      hFont ;
     int        y, iPointSize ;
     LOGFONT    lf ;
     TCHAR      szBuffer [100] ;
     TEXTMETRIC tm ;

          // Définit le mode topographique Twips logiques

     SetMapMode (hdc, MM_ANISOTROPIC) ;
     SetWindowExtEx (hdc, 1440, 1440, NULL) ;
     SetViewportExtEx (hdc, GetDeviceCaps (hdc, LOGPIXELSX),
                            GetDeviceCaps (hdc, LOGPIXELSY), NULL) ;

          // Essayer quelques polices

     y = 0 ;

     for (iPointSize = 80 ; iPointSize <= 120 ; iPointSize++)
     {
          hFont = EzCreateFont (hdc, TEXT ("Times New Roman"), 
                                iPointSize, 0, 0, TRUE) ;

          GetObject (hFont, sizeof (LOGFONT), &lf) ;

          SelectObject (hdc, hFont) ;
          GetTextMetrics (hdc, &tm) ;

          TextOut (hdc, 0, y, szBuffer, 
               wsprintf (szBuffer, 
                         TEXT (" Police Times New Roman de %i.%i points, ")
                         TEXT ("lf.lfHeight = %i, tm.tmHeight = %i"),
                         iPointSize / 10, iPointSize % 10,
                         lf.lfHeight, tm.tmHeight)) ;

          DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
          y += tm.tmHeight ;
     }
}
/*--------------------------------------
   PIPES.C - Démo d'animation de la palette
              (c) Charles Petzold, 1998
  --------------------------------------*/

#include <windows.h>

#define ID_TIMER 1

TCHAR szAppName [] = TEXT ("Pipes") ;
TCHAR szTitle   [] = TEXT ("Pipes : Démo d'animation de la palette") ;

static LOGPALETTE * plp ;

HPALETTE CreateRoutine (HWND hwnd)
{
     HPALETTE hPalette ;
     int      i ;

     plp = malloc (sizeof (LOGPALETTE) + 32 * sizeof (PALETTEENTRY)) ;
     
          // Initialisation des membres de la structure LOGPALETTE
     
     plp->palVersion    = 0x300 ;
     plp->palNumEntries = 16 ;

     for (i = 0 ; i <= 8 ; i++)
     {
          plp->palPalEntry[i].peRed   = (BYTE) min (255, 0x20 * i) ;
          plp->palPalEntry[i].peGreen = 0 ;
          plp->palPalEntry[i].peBlue  = (BYTE) min (255, 0x20 * i) ;
          plp->palPalEntry[i].peFlags = PC_RESERVED ;

          plp->palPalEntry[16 - i] = plp->palPalEntry[i] ;
          plp->palPalEntry[16 + i] = plp->palPalEntry[i] ;
          plp->palPalEntry[32 - i] = plp->palPalEntry[i] ;
     }

     hPalette = CreatePalette (plp) ;
     
     SetTimer (hwnd, ID_TIMER, 100, NULL) ;
     return hPalette ;
}

void PaintRoutine (HDC hdc, int cxClient, int cyClient)
{
     HBRUSH hBrush ;
     int    i ;
     RECT   rect ;

          // Tracer l'arrière-plan de la fenêtre

     SetRect (&rect, 0, 0, cxClient, cyClient) ;
     hBrush = SelectObject (hdc, GetStockObject (WHITE_BRUSH)) ;
     FillRect (hdc, &rect, hBrush) ;

          // Tracer les intérieurs des conduites

     for (i = 0 ; i < 128 ; i++)
     {
          hBrush = CreateSolidBrush (PALETTEINDEX (i % 16)) ;
          SelectObject (hdc, hBrush) ;

          rect.left   = (127 - i) * cxClient / 128 ;
          rect.right  = (128 - i) * cxClient / 128 ;
          rect.top    = 4 * cyClient / 14 ;
          rect.bottom = 5 * cyClient / 14 ;

          FillRect (hdc, &rect, hBrush) ;

          rect.left   =  i      * cxClient / 128 ;
          rect.right  = (i + 1) * cxClient / 128 ;
          rect.top    =  9 * cyClient / 14 ;
          rect.bottom = 10 * cyClient / 14 ;

          FillRect (hdc, &rect, hBrush) ;

          DeleteObject (SelectObject (hdc, GetStockObject (WHITE_BRUSH))) ;
     }

          // Tracer les bords des conduites

     MoveToEx (hdc, 0,         4 * cyClient / 14, NULL) ;
     LineTo   (hdc, cxClient,  4 * cyClient / 14) ;

     MoveToEx (hdc, 0,         5 * cyClient / 14, NULL) ;
     LineTo   (hdc, cxClient,  5 * cyClient / 14) ;

     MoveToEx (hdc, 0,         9 * cyClient / 14, NULL) ;
     LineTo   (hdc, cxClient,  9 * cyClient / 14) ;

     MoveToEx (hdc, 0,        10 * cyClient / 14, NULL) ;
     LineTo   (hdc, cxClient, 10 * cyClient / 14) ;
     return ;
}

void TimerRoutine (HDC hdc, HPALETTE hPalette)
{
     static int iIndex ;

     AnimatePalette (hPalette, 0, 16, plp->palPalEntry + iIndex) ;
     iIndex = (iIndex + 1) % 16 ;

     return ;
}

void DestroyRoutine (HWND hwnd, HPALETTE hPalette)
{
     KillTimer (hwnd, ID_TIMER) ;
     DeleteObject (hPalette) ;
     free (plp) ;
     return ;
}

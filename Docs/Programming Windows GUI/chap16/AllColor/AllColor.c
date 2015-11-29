/*-----------------------------------------
   ALLCOLOR.C -- Démo d'animation de la palette
                 (c) Charles Petzold, 1998
  -----------------------------------------*/

#include <windows.h>

#define ID_TIMER    1

TCHAR szAppName [] = TEXT ("AllColor") ;
TCHAR szTitle   [] = TEXT ("AllColor : Démo d'animation de la palette") ;

static int          iIncr ;
static PALETTEENTRY pe ;

HPALETTE CreateRoutine (HWND hwnd)
{
     HDC        hdc ;
     HPALETTE   hPalette ;
     LOGPALETTE lp ;

          // Détermine la résolution des couleurs et initialise iIncr

     hdc = GetDC (hwnd) ;
     iIncr = 1 << (8 - GetDeviceCaps (hdc, COLORRES) / 3) ;
     ReleaseDC (hwnd, hdc) ;

          // Crée la palette logique
     
     lp.palVersion             = 0x0300 ;
     lp.palNumEntries          = 1 ;
     lp.palPalEntry[0].peRed   = 0 ;
     lp.palPalEntry[0].peGreen = 0 ;
     lp.palPalEntry[0].peBlue  = 0 ;
     lp.palPalEntry[0].peFlags = PC_RESERVED ;
   
     hPalette = CreatePalette (&lp) ;

          // Enregistrement global pour réduire le code à saisir

     pe = lp.palPalEntry[0] ;
     
     SetTimer (hwnd, ID_TIMER, 10, NULL) ;
     return hPalette ;
}

void DisplayRGB (HDC hdc, PALETTEENTRY * ppe)
{
     TCHAR szBuffer [16] ;

     wsprintf (szBuffer, TEXT (" %02X-%02X-%02X "),
               ppe->peRed, ppe->peGreen, ppe->peBlue) ;

     TextOut (hdc, 0, 0, szBuffer, lstrlen (szBuffer)) ;
}

void PaintRoutine (HDC hdc, int cxClient, int cyClient)
{
     HBRUSH   hBrush ;
     RECT     rect ;

          // Trace index 0 de la palette sur toute la fenêtre

     hBrush = CreateSolidBrush (PALETTEINDEX (0)) ;
     SetRect (&rect, 0, 0, cxClient, cyClient) ;
     FillRect (hdc, &rect, hBrush) ;
     DeleteObject (SelectObject (hdc, GetStockObject (WHITE_BRUSH))) ;

          // Affiche la valeur RVB

     DisplayRGB (hdc, &pe) ;
     return ;
}

void TimerRoutine (HDC hdc, HPALETTE hPalette)
{
     static BOOL  bRedUp = TRUE, bGreenUp = TRUE, bBlueUp = TRUE ;

          // Définit la nouvelle valeur de couleur

     pe.peBlue += (bBlueUp ? iIncr : -iIncr) ;

     if (pe.peBlue == (BYTE) (bBlueUp ? 0 : 256 - iIncr))
     {
          pe.peBlue = (bBlueUp ? 256 - iIncr : 0) ;
          bBlueUp ^= TRUE ;
          pe.peGreen += (bGreenUp ? iIncr : -iIncr) ;

          if (pe.peGreen == (BYTE) (bGreenUp ? 0 : 256 - iIncr))
          {
               pe.peGreen = (bGreenUp ? 256 - iIncr : 0) ;
               bGreenUp ^= TRUE ;
               pe.peRed += (bRedUp ? iIncr : -iIncr) ;

               if (pe.peRed == (BYTE) (bRedUp ? 0 : 256 - iIncr))
               {
                    pe.peRed = (bRedUp ? 256 - iIncr : 0) ;
                    bRedUp ^= TRUE ;
               }
          }
     }

          // Anime la palette
     
     AnimatePalette (hPalette, 0, 1, &pe) ;
     DisplayRGB (hdc, &pe) ;
     return ;
}

void DestroyRoutine (HWND hwnd, HPALETTE hPalette)
{
     KillTimer (hwnd, ID_TIMER) ;
     DeleteObject (hPalette) ;
     return ;
}

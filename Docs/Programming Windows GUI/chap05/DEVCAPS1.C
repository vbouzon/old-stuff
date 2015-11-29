/*---------------------------------------------------------
   DEVCAPS1.C -- Caractéristiques des périphériques d'affichage V.1
                 (c) Charles Petzold, 1998
  ---------------------------------------------------------*/

#include <windows.h>

#define NUMLINES ((int) (sizeof devcaps / sizeof devcaps [0]))

struct
{
     int     iIndex ;
     TCHAR * szLabel ;
     TCHAR * szDesc ;
}
devcaps [] =
{
     HORZSIZE,      TEXT ("HORZSIZE"),     TEXT ("Largeur en millimètres :"),
     VERTSIZE,      TEXT ("VERTSIZE"),     TEXT ("Hauteur en millimètres :"),
     HORZRES,       TEXT ("HORZRES"),      TEXT ("Hauteur en pixels :"),
     VERTRES,       TEXT ("VERTRES"),      TEXT ("Hauteur en lignes d'affichage :"),
     BITSPIXEL,     TEXT ("BITSPIXEL"),    TEXT ("Bits de couleur par pixel :"),
     PLANES,        TEXT ("PLANES"),       TEXT ("Nombre de plans couleur :"),
     NUMBRUSHES,    TEXT ("NUMBRUSHES"),   TEXT ("Nombre de pinceaux :"),
     NUMPENS,       TEXT ("NUMPENS"),      TEXT ("Nombre de crayons :"),
     NUMMARKERS,    TEXT ("NUMMARKERS"),   TEXT ("Nombre de marqueurs :"),
     NUMFONTS,      TEXT ("NUMFONTS"),     TEXT ("Nombre de polices :"),
     NUMCOLORS,     TEXT ("NUMCOLORS"),    TEXT ("Nombre de couleurs :"),
     PDEVICESIZE,   TEXT ("PDEVICESIZE"),  TEXT ("Taille de la structure :"),
     ASPECTX,       TEXT ("ASPECTX"),      TEXT ("Largeur relative d'un pixel :"),
     ASPECTY,       TEXT ("ASPECTY"),      TEXT ("Hauteur relative d'un pixel :"),
     ASPECTXY,      TEXT ("ASPECTXY"),     TEXT ("Diagonale relative d'un pixel :"),
     LOGPIXELSX,    TEXT ("LOGPIXELSX"),   TEXT ("Points horizontaux par inch :"),
     LOGPIXELSY,    TEXT ("LOGPIXELSY"),   TEXT ("Points verticaux par  inch :"),
     SIZEPALETTE,   TEXT ("SIZEPALETTE"),  TEXT ("Taille de la palette :"),
     NUMRESERVED,   TEXT ("NUMRESERVED"),  TEXT ("Entrées réservées :"),
     COLORRES,      TEXT ("COLORRES"),     TEXT ("Résolution couleurs :")
} ;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("DevCaps1") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;
     
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
     
     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Le programme nécessite Windows NT !"),
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName, TEXT ("Caractéristiques Ecran"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;
     
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;
     
     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static int  cxChar, cxCaps, cyChar ;
     TCHAR       szBuffer[10] ;
     HDC         hdc ;
     int         i ;
     PAINTSTRUCT ps ;
     TEXTMETRIC  tm ;
     
     switch (message)
     {
     case WM_CREATE:
          hdc = GetDC (hwnd) ;
          
          GetTextMetrics (hdc, &tm) ;
          cxChar = tm.tmAveCharWidth ;
          cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;
          cyChar = tm.tmHeight + tm.tmExternalLeading ;
          
          ReleaseDC (hwnd, hdc) ;
          return 0 ;
          
     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;
          
          for (i = 0 ; i < NUMLINES ; i++)
          {
               TextOut (hdc, 0, cyChar * i,
                        devcaps[i].szLabel,
                        lstrlen (devcaps[i].szLabel)) ;
               
               TextOut (hdc, 14 * cxCaps, cyChar * i,
                        devcaps[i].szDesc,
                        lstrlen (devcaps[i].szDesc)) ;
               
               SetTextAlign (hdc, TA_RIGHT | TA_TOP) ;
               TextOut (hdc, 14 * cxCaps + 35 * cxChar, cyChar * i, szBuffer,
                        wsprintf (szBuffer, TEXT ("%5d"),
                             GetDeviceCaps (hdc, devcaps[i].iIndex))) ;
               
               SetTextAlign (hdc, TA_LEFT | TA_TOP) ;
          }
          
          EndPaint (hwnd, &ps) ;
          return 0 ;
          
     case WM_DESTROY:
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

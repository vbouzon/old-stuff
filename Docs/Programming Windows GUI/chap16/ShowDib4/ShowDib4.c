/*-------------------------------------------------------
   SHOWDIB4.C - Affiche les DIB avec une palette "universelle"
                 (c) Charles Petzold, 1998
  -------------------------------------------------------*/

#include <windows.h>
#include "..\\ShowDib3\\PackeDib.h"
#include "resource.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

TCHAR szAppName[] = TEXT ("ShowDib4") ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     HWND     hwnd ;
     MSG      msg ;
     WNDCLASS wndclass ;

     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = szAppName ;
     wndclass.lpszClassName = szAppName ;

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Ce programme fonctionne exclusivement sous Windows NT !"), 
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }

     hwnd = CreateWindow (szAppName, TEXT ("Show DIB n°4 : palette universelle"),
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

/*------------------------------------------------------------------------
   CreateAllPurposePalette: Crée une palette convenant à des images
          très variées ; la palette possède 247 entrées, mais 15 d'entre elles
          sont des duplicatas ou correspondent aux 20 couleurs standard.
  ------------------------------------------------------------------------*/

HPALETTE CreateAllPurposePalette (void)
{
     HPALETTE hPalette ;
     int          i, incr, R, G, B ;
     LOGPALETTE * plp ;

     plp = malloc (sizeof (LOGPALETTE) + 246 * sizeof (PALETTEENTRY)) ;

     plp->palVersion    = 0x0300 ;
     plp->palNumEntries = 247 ;

          // La boucle suivante calcule 31 niveaux de gris, dont 3  
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
          //        correspondent aux 20 couleurs standard, et
          //        4 correspondent aux niveaux de gris précédents.

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

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static BITMAPINFO * pPackedDib ;
     static HPALETTE     hPalette ;
     static int          cxClient, cyClient ;
     static OPENFILENAME ofn ;
     static TCHAR        szFileName [MAX_PATH], szTitleName [MAX_PATH] ;
     static TCHAR        szFilter[] = TEXT ("Fichiers bitmap (*.BMP)\0*.bmp\0")
                                      TEXT ("Tous les fichiers (*.*)\0*.*\0\0") ;
     HDC                 hdc ;
     PAINTSTRUCT         ps ;

     switch (message)
     {
     case WM_CREATE:
          ofn.lStructSize       = sizeof (OPENFILENAME) ;
          ofn.hwndOwner         = hwnd ;
          ofn.hInstance         = NULL ;
          ofn.lpstrFilter       = szFilter ;
          ofn.lpstrCustomFilter = NULL ;
          ofn.nMaxCustFilter    = 0 ;
          ofn.nFilterIndex      = 0 ;
          ofn.lpstrFile         = szFileName ;
          ofn.nMaxFile          = MAX_PATH ;
          ofn.lpstrFileTitle    = szTitleName ;
          ofn.nMaxFileTitle     = MAX_PATH ;
          ofn.lpstrInitialDir   = NULL ;
          ofn.lpstrTitle        = NULL ;
          ofn.Flags             = 0 ;
          ofn.nFileOffset       = 0 ;
          ofn.nFileExtension    = 0 ;
          ofn.lpstrDefExt       = TEXT ("bmp") ;
          ofn.lCustData         = 0 ;
          ofn.lpfnHook          = NULL ;
          ofn.lpTemplateName    = NULL ;

               // Crée la palette universelle

          hPalette = CreateAllPurposePalette () ;
          return 0 ;

     case WM_SIZE:
          cxClient = LOWORD (lParam) ;
          cyClient = HIWORD (lParam) ;
          return 0 ;

     case WM_COMMAND:
          switch (LOWORD (wParam))
          {
          case IDM_FILE_OPEN:

                    // Affiche la boîte de dialogue Ouverture

               if (!GetOpenFileName (&ofn))
                    return 0 ;
               
                    // Libérer la mémoire s'il existe une DIB compressée

               if (pPackedDib)
               {
                    free (pPackedDib) ;
                    pPackedDib = NULL ;
               }
               
                    // Charge la DIB compressée dans la mémoire

               SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
               ShowCursor (TRUE) ;

               pPackedDib = PackedDibLoad (szFileName) ;

               ShowCursor (FALSE) ;
               SetCursor (LoadCursor (NULL, IDC_ARROW)) ;

               if (!pPackedDib)
               {
                    MessageBox (hwnd, TEXT ("Impossible de charger le fichier DIB"), 
                                szAppName, 0) ;
               }
               InvalidateRect (hwnd, NULL, TRUE) ;
               return 0 ;
          }
          break ;

     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;

          if (pPackedDib)
          {
               SelectPalette (hdc, hPalette, FALSE) ;
               RealizePalette (hdc) ;
          
               SetDIBitsToDevice (hdc, 
                                  0,   
                                  0,   
                                  PackedDibGetWidth (pPackedDib), 
                                  PackedDibGetHeight (pPackedDib),
                                  0,                            
                                  0,                            
                                  0,                            
                                  PackedDibGetHeight (pPackedDib),  
                                  PackedDibGetBitsPtr (pPackedDib), 
                                  pPackedDib, 
                                  DIB_RGB_COLORS) ;
          }
          EndPaint (hwnd, &ps) ;
          return 0 ;

     case WM_QUERYNEWPALETTE:
          hdc = GetDC (hwnd) ;
          SelectPalette (hdc, hPalette, FALSE) ;
          RealizePalette (hdc) ;
          InvalidateRect (hwnd, NULL, TRUE) ;

          ReleaseDC (hwnd, hdc) ;
          return TRUE ;

     case WM_PALETTECHANGED:
          if ((HWND) wParam != hwnd)

          hdc = GetDC (hwnd) ;
          SelectPalette (hdc, hPalette, FALSE) ;
          RealizePalette (hdc) ;
          UpdateColors (hdc) ;

          ReleaseDC (hwnd, hdc) ;
          break ;
          
     case WM_DESTROY:
          if (pPackedDib)
               free (pPackedDib) ;

          DeleteObject (hPalette) ;

          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

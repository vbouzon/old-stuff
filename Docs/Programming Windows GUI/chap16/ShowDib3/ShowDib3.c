/*------------------------------------------------
   SHOWDIB3.C - Affiche une DIB avec sa palette native
                 (c) Charles Petzold, 1998
  ------------------------------------------------*/

#include <windows.h>
#include "PackeDib.h"
#include "resource.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

TCHAR szAppName[] = TEXT ("ShowDib3") ;

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

     hwnd = CreateWindow (szAppName, TEXT ("Affiche DIB n°3 : palette native"),
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
               
                    // S'il existe une palette logique, la supprimer

               if (hPalette)
               {
                    DeleteObject (hPalette) ;
                    hPalette = NULL ;
               }

                    // Charger la DIB compressée dans la mémoire

               SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
               ShowCursor (TRUE) ;

               pPackedDib = PackedDibLoad (szFileName) ;

               ShowCursor (FALSE) ;
               SetCursor (LoadCursor (NULL, IDC_ARROW)) ;

               if (pPackedDib)
               {
                         // Créer la palette avec la table 
                         // des couleurs de la DIB

                    hPalette = PackedDibCreatePalette (pPackedDib) ;
               }
               else
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

          if (hPalette)
          {
               SelectPalette (hdc, hPalette, FALSE) ;
               RealizePalette (hdc) ;
          }
          
          if (pPackedDib)
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
          EndPaint (hwnd, &ps) ;
          return 0 ;

     case WM_QUERYNEWPALETTE:
          if (!hPalette)
               return FALSE ;

          hdc = GetDC (hwnd) ;
          SelectPalette (hdc, hPalette, FALSE) ;
          RealizePalette (hdc) ;
          InvalidateRect (hwnd, NULL, TRUE) ;

          ReleaseDC (hwnd, hdc) ;
          return TRUE ;

     case WM_PALETTECHANGED:
          if (!hPalette || (HWND) wParam == hwnd)
               break ;

          hdc = GetDC (hwnd) ;
          SelectPalette (hdc, hPalette, FALSE) ;
          RealizePalette (hdc) ;
          UpdateColors (hdc) ;

          ReleaseDC (hwnd, hdc) ;
          break ;

          
     case WM_DESTROY:
          if (pPackedDib)
               free (pPackedDib) ;

          if (hPalette)
               DeleteObject (hPalette) ;

          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

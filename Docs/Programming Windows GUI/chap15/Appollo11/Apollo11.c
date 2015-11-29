/*----------------------------------------------
   APOLLO11.C -- Programme de capture d'écran
                 (c) Charles Petzold, 1998
  ----------------------------------------------*/

#include <windows.h>
#include "dibfile.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

TCHAR szAppName[] = TEXT ("Apollo11") ;

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
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Ce programme fonctionne exclusivement sous Windows NT !"), 
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
   hwnd = CreateWindow (szAppName, TEXT ("Apollo 11"),
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
     static BITMAPFILEHEADER * pbmfh [2] ;
     static BITMAPINFO       * pbmi  [2] ;
     static BYTE             * pBits [2] ;
     static int                nxClient, nyClient, nxDib [2], nyDib [2] ;
     HDC                       hdc ;
     PAINTSTRUCT               ps ;

     switch (message)
     {
     case WM_CREATE:
          pbmfh[0] = DibLoadImage (TEXT ("Apollo11.bmp")) ;
          pbmfh[1] = DibLoadImage (TEXT ("ApolloTD.bmp")) ;

          if (pbmfh[0] == NULL || pbmfh[1] == NULL)
          {
               MessageBox (hwnd, TEXT ("Impossible de charger le fichier de la DIB"), 
                                szAppName, 0) ;
               return 0 ;
          }
               // Crée pointeurs sur les structures d'info & de bits

          pbmi  [0] = (BITMAPINFO *) (pbmfh[0] + 1) ;
          pbmi  [1] = (BITMAPINFO *) (pbmfh[1] + 1) ;
          pBits [0] = (BYTE *) pbmfh[0] + pbmfh[0]->bfOffBits ;
          pBits [1] = (BYTE *) pbmfh[1] + pbmfh[1]->bfOffBits ;

               // Lit largeur et hauteur de la DIB (BITMAPINFOHEADER)
               // Notez que nyDib est la valeur absolue de la valeur d'en-tête !!!

          nxDib [0] =      pbmi[0]->bmiHeader.biWidth ;
          nxDib [1] =      pbmi[1]->bmiHeader.biWidth ;

          nyDib [0] = abs (pbmi[0]->bmiHeader.biHeight) ;
          nyDib [1] = abs (pbmi[1]->bmiHeader.biHeight) ;
          return 0 ;

     case WM_SIZE:
          nxClient = LOWORD (lParam) ;
          nyClient = HIWORD (lParam) ;
          return 0 ;
         
     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;

               // DIB inversée taille réelle

          SetDIBitsToDevice (hdc, 
                             0,                   // xDst
                             nyClient / 4,        // yDst
                             nxDib[0],            // nxSrc
                             nyDib[0],            // nySrc
                             0,                   // xSrc
                             0,                   // ySrc
                             0,                   // première ligne de balayage
                             nyDib[0],            // nombre de lignes de balayage
                             pBits[0], 
                             pbmi[0], 
                             DIB_RGB_COLORS) ;

               // DIB inversée partielle

          SetDIBitsToDevice (hdc, 
                             240,                 // xDst
                             nyClient / 4,        // yDst
                             80,                  // nxSrc
                             166,                 // nySrc
                             80,                  // xSrc
                             60,                  // ySrc
                             0,                   // première ligne de balayage
                             nyDib[0],            // nombre de lignes de balayage
                             pBits[0], 
                             pbmi[0], 
                             DIB_RGB_COLORS) ;

               // DIB normale taille réelle

          SetDIBitsToDevice (hdc, 
                             340,                 // xDst
                             nyClient / 4,        // yDst
                             nxDib[0],            // nxSrc
                             nyDib[0],            // nySrc
                             0,                   // xSrc
                             0,                   // ySrc
                             0,                   // première ligne de balayage
                             nyDib[0],            // nombre de lignes de balayage
                             pBits[0], 
                             pbmi[0], 
                             DIB_RGB_COLORS) ;

               // DIB normale partielle

          SetDIBitsToDevice (hdc, 
                             580,                 // xDst
                             nyClient / 4,        // yDst
                             80,                  // nxSrc
                             166,                 // nySrc
                             80,                  // xSrc
                             60,                  // ySrc
                             0,                   // première ligne de balayage
                             nyDib[1],            // nombre de lignes de balayage
                             pBits[1], 
                             pbmi[1], 
                             DIB_RGB_COLORS) ;

          EndPaint (hwnd, &ps) ;
          return 0 ;
          
     case WM_DESTROY:
          if (pbmfh[0])
               free (pbmfh[0]) ;
          if (pbmfh[1])
               free (pbmfh[1]) ;

          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

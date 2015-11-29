/*----------------------------------------------
   SHOWDIB1.C - Affiche une DIB dans la zone client
                 (c) Charles Petzold, 1998
  ----------------------------------------------*/

#include <windows.h>
#include "dibfile.h"
#include "resource.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

TCHAR szAppName[] = TEXT ("ShowDib1") ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     HACCEL   hAccel ;
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

     hwnd = CreateWindow (szAppName, TEXT ("Show DIB #1"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT, 
                          NULL, NULL, hInstance, NULL) ;

     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;

     hAccel = LoadAccelerators (hInstance, szAppName) ;

     while (GetMessage (&msg, NULL, 0, 0))
     {
          if (!TranslateAccelerator (hwnd, hAccel, &msg))
          {
               TranslateMessage (&msg) ;
               DispatchMessage (&msg) ;
          }
     }
     return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static BITMAPFILEHEADER * pbmfh ;
     static BITMAPINFO       * pbmi ;
     static BYTE             * pBits ;
     static int                nxClient, nyClient, nxDib, nyDib ;
     static TCHAR              szFileName [MAX_PATH], szTitleName [MAX_PATH] ;
     BOOL                      bSuccess ;
     HDC                       hdc ;
     PAINTSTRUCT               ps ;

     switch (message)
     {
     case WM_CREATE:
          DibFileInitialize (hwnd) ;
          return 0 ;

     case WM_SIZE:
          nxClient = LOWORD (lParam) ;
          nyClient = HIWORD (lParam) ;
          return 0 ;

     case WM_INITMENUPOPUP:
          EnableMenuItem ((HMENU) wParam, IDM_FILE_SAVE,   
                          pbmfh ? MF_ENABLED : MF_GRAYED) ;
          return 0 ;

     case WM_COMMAND:
          switch (LOWORD (wParam))
          {
          case IDM_FILE_OPEN:
                    // Affiche la boîte de dialogue Ouvrir

               if (!DibFileOpenDlg (hwnd, szFileName, szTitleName))
                    return 0 ;
               
                    // S'il existe une DIB, libérer la mémoire

               if (pbmfh)
               {
                    free (pbmfh) ;
                    pbmfh = NULL ;
               }
                    // Charger toute la DIB dans la mémoire

               SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
               ShowCursor (TRUE) ;

               pbmfh = DibLoadImage (szFileName) ;
               ShowCursor (FALSE) ;
               SetCursor (LoadCursor (NULL, IDC_ARROW)) ;

                    // Invalider la zone client pour mise à jour ultérieure

               InvalidateRect (hwnd, NULL, TRUE) ;

               if (pbmfh == NULL)
               {
                    MessageBox (hwnd, TEXT ("Impossible de charger le fichier de la DIB"), 
                                szAppName, 0) ;
                    return 0 ;
               }
                    // Obtention des pointeurs sur structure info et bits

               pbmi  = (BITMAPINFO *) (pbmfh + 1) ;
               pBits = (BYTE *) pbmfh + pbmfh->bfOffBits ;

                    // Obtention de la largeur et de la hauteur de la DIB

               if (pbmi->bmiHeader.biSize == sizeof (BITMAPCOREHEADER))
               {
                    nxDib = ((BITMAPCOREHEADER *) pbmi)->bcWidth ;
                    nyDib = ((BITMAPCOREHEADER *) pbmi)->bcHeight ;
               }
               else
               {
                    nxDib =      pbmi->bmiHeader.biWidth ;
                    nyDib = abs (pbmi->bmiHeader.biHeight) ;
               }
               return 0 ;

          case IDM_FILE_SAVE:
                    // Affiche la boîte de dialogue Enregistrer

               if (!DibFileSaveDlg (hwnd, szFileName, szTitleName))
                    return 0 ;
               
                    // Stocke la DIB dans la mémoire

               SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
               ShowCursor (TRUE) ;
               bSuccess = DibSaveImage (szFileName, pbmfh) ;
               ShowCursor (FALSE) ;
               SetCursor (LoadCursor (NULL, IDC_ARROW)) ;

               if (!bSuccess)
                    MessageBox (hwnd, TEXT ("Enregistrement impossible du fichier de la DIB"), 
                                szAppName, 0) ;
               return 0 ;
          }
          break ;

     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;

          if (pbmfh)
               SetDIBitsToDevice (hdc, 
                                  0,         // xDst
                                  0,         // yDst
                                  nxDib,     // nxSrc
                                  nyDib,     // nySrc
                                  0,         // xSrc
                                  0,         // ySrc
                                  0,         // première ligne de balayage
                                  nyDib,     // nombre de lignes de balayage
                                  pBits, 
                                  pbmi, 
                                  DIB_RGB_COLORS) ;

          EndPaint (hwnd, &ps) ;
          return 0 ;
          
     case WM_DESTROY:
          if (pbmfh)
               free (pbmfh) ;

          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

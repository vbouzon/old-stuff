/*------------------------------------------------
   FONTDEMO.C -- Démonstration de police
                 (c) Charles Petzold, 1998
  ------------------------------------------------*/

#include <windows.h>
#include "..\\EZTest\\EzFont.h"
#include "..\\EZTest\\resource.h"

extern  void     PaintRoutine (HWND, HDC, int, int) ;
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

HINSTANCE hInst ;

extern TCHAR szAppName [] ;
extern TCHAR szTitle [] ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     TCHAR    szResource [] = TEXT ("FontDemo") ;
     HWND     hwnd ;
     MSG      msg ;
     WNDCLASS wndclass ;
     
     hInst = hInstance ;
     
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = szResource ;
     wndclass.lpszClassName = szAppName ;
     
     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Le programme nécessite Windows NT!"),
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName, szTitle,
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
     static DOCINFO  di = { sizeof (DOCINFO), TEXT ("Démo police : Impression") } ;
     static int      cxClient, cyClient ;
     static PRINTDLG pd = { sizeof (PRINTDLG) } ;
     BOOL            fSuccess ;
     HDC             hdc, hdcPrn ;
     int             cxPage, cyPage ;
     PAINTSTRUCT     ps ;
     
     switch (message)
     {
     case WM_COMMAND:
          switch (wParam)
          {
          case IDM_PRINT:

                    // Obtenir l'imprimante

               pd.hwndOwner = hwnd ;
          	pd.Flags     = PD_RETURNDC | PD_NOPAGENUMS | PD_NOSELECTION ;

	          if (!PrintDlg (&pd))
                    return 0 ;

               if (NULL == (hdcPrn = pd.hDC))
               {
                    MessageBox (hwnd, TEXT ("Impossible de trouver l'imprimante"),
                                szAppName, MB_ICONEXCLAMATION | MB_OK) ;
                    return 0 ;
               }
                    // Obtenir la zone imprimable de la page

               cxPage = GetDeviceCaps (hdcPrn, HORZRES) ;
               cyPage = GetDeviceCaps (hdcPrn, VERTRES) ;

               fSuccess = FALSE ;

                    // Créer la page d'impression

               SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
               ShowCursor (TRUE) ;

               if ((StartDoc (hdcPrn, &di) > 0) && (StartPage (hdcPrn) > 0))
               {
                    PaintRoutine (hwnd, hdcPrn, cxPage, cyPage) ;
                    
                    if (EndPage (hdcPrn) > 0)
                    {
                         fSuccess = TRUE ;
                         EndDoc (hdcPrn) ;
                    }
               }
               DeleteDC (hdcPrn) ;

               ShowCursor (FALSE) ;
               SetCursor (LoadCursor (NULL, IDC_ARROW)) ;

               if (!fSuccess)
                    MessageBox (hwnd, 
                                TEXT ("Erreur en cours d'impression"),
                                szAppName, MB_ICONEXCLAMATION | MB_OK) ;
               return 0 ;

          case IDM_ABOUT:
               MessageBox (hwnd, TEXT ("Démonstration de police\n")
                                 TEXT ("(c) Charles Petzold, 1998"),
                           szAppName, MB_ICONINFORMATION | MB_OK);
               return 0 ;
          }
          break ;
          
     case WM_SIZE:
          cxClient = LOWORD (lParam) ;
          cyClient = HIWORD (lParam) ;
          return 0 ;
          
     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;
          
          PaintRoutine (hwnd, hdc, cxClient, cyClient) ;
          
          EndPaint (hwnd, &ps) ;
          return 0 ;
          
     case WM_DESTROY :
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

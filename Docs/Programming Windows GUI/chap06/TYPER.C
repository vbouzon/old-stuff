 /*--------------------------------------
   TYPER.C -- Frappe au clavier
              (c) Charles Petzold, 1998
  --------------------------------------*/

#include <windows.h>

#define BUFFER(x,y) *(pBuffer + y * cxBuffer + x)

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("Typer") ;
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
          MessageBox (NULL, TEXT ("Le programme n�cessite Windows NT !"), 
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName, TEXT ("Frappe au clavier"),
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
     static DWORD   dwCharSet = DEFAULT_CHARSET ;
     static int     cxChar, cyChar, cxClient, cyClient, cxBuffer, cyBuffer,
                    xCaret, yCaret ;
     static TCHAR * pBuffer = NULL ;
     HDC            hdc ;
     int            x, y, i ;
     PAINTSTRUCT    ps ;
     TEXTMETRIC     tm ;
     
     switch (message)
     {
     case WM_INPUTLANGCHANGE:
          dwCharSet = wParam ;
case WM_CREATE:
          hdc = GetDC (hwnd) ;
          SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
                                   dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;
          
          GetTextMetrics (hdc, &tm) ;
          cxChar = tm.tmAveCharWidth ;
          cyChar = tm.tmHeight ;
          
          DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
          ReleaseDC (hwnd, hdc) ;
     case WM_SIZE:
               // La taille de la fen�tre en pixels

          if (message == WM_SIZE)
          {
               cxClient = LOWORD (lParam) ;
               cyClient = HIWORD (lParam) ;
          }
               // Calcule la taille de la fen�tre en caract�res
          
          cxBuffer = max (1, cxClient / cxChar) ;
          cyBuffer = max (1, cyClient / cyChar) ;
          
               // Alloue de la m�moire au tampon et la vide
          
          if (pBuffer != NULL)
               free (pBuffer) ;

          pBuffer = (TCHAR *) malloc (cxBuffer * cyBuffer * sizeof (TCHAR)) ;
          
          for (y = 0 ; y < cyBuffer ; y++)
               for (x = 0 ; x < cxBuffer ; x++)
                    BUFFER(x,y) = ' ' ;
                    
               // Place le caret au coin sup�rieur gauche

          xCaret = 0 ;
          yCaret = 0 ;
                    
          if (hwnd == GetFocus ())
               SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;

          InvalidateRect (hwnd, NULL, TRUE) ;
          return 0 ;
                    
     case WM_SETFOCUS:
               // Cr�e et affiche le caret
          CreateCaret (hwnd, NULL, cxChar, cyChar) ;
          SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
          ShowCaret (hwnd) ;
          return 0 ;
          
     case WM_KILLFOCUS:
               // Cache et d�truit le caret

          HideCaret (hwnd) ;
          DestroyCaret () ;
          return 0 ;
          
     case WM_KEYDOWN:
          switch (wParam)
          {
          case VK_HOME:
               xCaret = 0 ;
               break ;
               
          case VK_END:
               xCaret = cxBuffer - 1 ;
               break ;
               
          case VK_PRIOR:
               yCaret = 0 ;
               break ;
               
          case VK_NEXT:
               yCaret = cyBuffer - 1 ;
               break ;
               
          case VK_LEFT:
               xCaret = max (xCaret - 1, 0) ;
               break ;
               
          case VK_RIGHT:
               xCaret = min (xCaret + 1, cxBuffer - 1) ;
               break ;
               
          case VK_UP:
               yCaret = max (yCaret - 1, 0) ;
               break ;
               
          case VK_DOWN:
               yCaret = min (yCaret + 1, cyBuffer - 1) ;
               break ;
               
          case VK_DELETE:
               for (x = xCaret ; x < cxBuffer - 1 ; x++)
                    BUFFER (x, yCaret) = BUFFER (x + 1, yCaret) ;
               
               BUFFER (cxBuffer - 1, yCaret) = ' ' ;
               
               HideCaret (hwnd) ;
               hdc = GetDC (hwnd) ;
          
               SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
                                   dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;
                    
               TextOut (hdc, xCaret * cxChar, yCaret * cyChar,
                        & BUFFER (xCaret, yCaret),
                        cxBuffer - xCaret) ;

               DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
               ReleaseDC (hwnd, hdc) ;
               ShowCaret (hwnd) ;
               break ;
          }
          SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
          return 0 ;
          
     case WM_CHAR:
          for (i = 0 ; i < (int) LOWORD (lParam) ; i++)
          {
               switch (wParam)
               {
               case '\b':                    // Retour arri�re
                    if (xCaret > 0)
                    {
                         xCaret-- ;
                         SendMessage (hwnd, WM_KEYDOWN, VK_DELETE, 1) ;
                    }
                    break ;
                    
               case '\t':                    // Tabulation
                    do
                    {
                         SendMessage (hwnd, WM_CHAR, ' ', 1) ;
                    }
                    while (xCaret % 8 != 0) ;
                    break ;
               case '\n':                    // Saut de ligne
                    if (++yCaret == cyBuffer)
                         yCaret = 0 ;
                    break ;
                    
               case '\r':                    // Retour chariot
                    xCaret = 0 ;
                    
                    if (++yCaret == cyBuffer)
                         yCaret = 0 ;
                    break ;
                    
               case '\x1B':                  // �chappement
                    for (y = 0 ; y < cyBuffer ; y++)
                         for (x = 0 ; x < cxBuffer ; x++)
                              BUFFER (x, y) = ' ' ;
                         
                    xCaret = 0 ;
                    yCaret = 0 ;
                         
                    InvalidateRect (hwnd, NULL, FALSE) ;
                    break ;
                         
               default:                      // Codes caract�res
                    BUFFER (xCaret, yCaret) = (TCHAR) wParam ;
                    
                    HideCaret (hwnd) ;
                    hdc = GetDC (hwnd) ;
          
                    SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
                                   dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;

                    TextOut (hdc, xCaret * cxChar, yCaret * cyChar,
                             & BUFFER (xCaret, yCaret), 1) ;

                    DeleteObject (
                         SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
                    ReleaseDC (hwnd, hdc) ;
                    ShowCaret (hwnd) ;

                    if (++xCaret == cxBuffer)
                    {
                         xCaret = 0 ;
                         
                         if (++yCaret == cyBuffer)
                              yCaret = 0 ;
                    }
                    break ;
               }
          }
          
          SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
          return 0 ;
          
     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;
          
          SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
                                   dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;
                              
          for (y = 0 ; y < cyBuffer ; y++)
               TextOut (hdc, 0, y * cyChar, & BUFFER(0,y), cxBuffer) ;

          DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
          EndPaint (hwnd, &ps) ;
          return 0 ;
          
     case WM_DESTROY:
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}


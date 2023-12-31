/*-----------------------------------------
   JUSTIFY1.C -- Alignement N� 1
                 (c) Charles Petzold, 1998
  -----------------------------------------*/

#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

TCHAR szAppName[] = TEXT ("Justify1") ;

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
          MessageBox (NULL, TEXT ("Le programme n�cessite Windows NT!"),
               szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName, TEXT ("Alignement N� 1"),
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

void DrawRuler (HDC hdc, RECT * prc)
{
     static int iRuleSize [16] = { 360, 72, 144, 72, 216, 72, 144, 72,
                                   288, 72, 144, 72, 216, 72, 144, 72 } ;
     int        i, j ;
     POINT      ptClient ;
     
     SaveDC (hdc) ;
     
          // D�finit les Twips logiques du mode topographique
     
     SetMapMode (hdc, MM_ANISOTROPIC) ;
     SetWindowExtEx (hdc, 1440, 1440, NULL) ;
     SetViewportExtEx (hdc, GetDeviceCaps (hdc, LOGPIXELSX),
                            GetDeviceCaps (hdc, LOGPIXELSY), NULL) ;
     
          // D�place l'origine d'un demi pouce en haut � gauche
     
     SetWindowOrgEx (hdc, -720, -720, NULL) ;
     
          // Trouve la marge de droite (un quart de pouce de la droite)
     
     ptClient.x = prc->right ;
     ptClient.y = prc->bottom ;
     DPtoLP (hdc, &ptClient, 1) ;
     ptClient.x -= 360 ;
     
          // Dessine les r�gles
     
     MoveToEx (hdc, 0,          -360, NULL) ;
     LineTo   (hdc, ptClient.x, -360) ;
     MoveToEx (hdc, -360,          0, NULL) ;
     LineTo   (hdc, -360, ptClient.y) ;
     
     for (i = 0, j = 0 ; i <= ptClient.x ; i += 1440 / 16, j++)
     {
          MoveToEx (hdc, i, -360, NULL) ;
          LineTo   (hdc, i, -360 - iRuleSize [j % 16]) ;
     }
     
     for (i = 0, j = 0 ; i <= ptClient.y ; i += 1440 / 16, j++)
     {
          MoveToEx (hdc, -360, i, NULL) ;
          LineTo   (hdc, -360 - iRuleSize [j % 16], i) ;
     }
     
     RestoreDC (hdc, -1) ;
}

void Justify (HDC hdc, PTSTR pText, RECT * prc, int iAlign)
{
     int   xStart, yStart, cSpaceChars ;
     PTSTR pBegin, pEnd ;
     SIZE  size ;
     
     yStart = prc->top ;
     do                            // Initialise le nombre d'espaces
     {
          cSpaceChars = 0 ;        // dans la ligne

          while (*pText == ' ')    // saute les espaces
               pText++ ;

          pBegin = pText ;         // place le curseur sur le premier caract�re de la ligne
          
          do                       
          {
               pEnd = pText ;      // place le curseur sur le dernier caract�re de la ligne

                    // saute � l'espace suivant 
               
               while (*pText != '\0' && *pText++ != ' ') ;

               if (*pText == '\0')
                    break ;

                    // calcule les espacements

               cSpaceChars++ ;
               GetTextExtentPoint32(hdc, pBegin, pText - pBegin - 1, &size) ;
          }
          while (size.cx < (prc->right - prc->left)) ;
          
          cSpaceChars-- ;               // dernier espace de la ligne
          
          while (*(pEnd - 1) == ' ')    // �limine les espaces 
          {
               pEnd-- ;
               cSpaceChars-- ;
          }

               // s'il s'agit de la fin du texte et pas d'espace, d�finir pEnd 
          
          if (*pText == '\0' || cSpaceChars <= 0)
               pEnd = pText ;
          
          GetTextExtentPoint32 (hdc, pBegin, pEnd - pBegin, &size) ;
          
          switch (iAlign)               // utilise l'alignement pour xStart
          {
          case IDM_ALIGN_LEFT:
               xStart = prc->left ;
               break ;
               
          case IDM_ALIGN_RIGHT:
               xStart = prc->right - size.cx ;
               break ;
               
          case IDM_ALIGN_CENTER:
               xStart = (prc->right + prc->left - size.cx) / 2 ;
               break ;
               
          case IDM_ALIGN_JUSTIFIED:
               if (*pText != '\0' && cSpaceChars > 0)
                    SetTextJustification (hdc,
                                          prc->right - prc->left - size.cx,
                                          cSpaceChars) ;
               xStart = prc->left ;
               break ;
          }
               // affiche le texte
          
          TextOut (hdc, xStart, yStart, pBegin, pEnd - pBegin) ;

               // pr�paration de la ligne suivante

          SetTextJustification (hdc, 0, 0) ;
          yStart += size.cy ;
          pText = pEnd ;
     }
     while (*pText && yStart < prc->bottom - size.cy) ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static CHOOSEFONT cf ;
     static DOCINFO    di = { sizeof (DOCINFO), TEXT ("Justify1: Printing") } ;
     static int        iAlign = IDM_ALIGN_LEFT ;
     static LOGFONT    lf ;
     static PRINTDLG   pd ;
     static TCHAR      szText[] = { 
                              TEXT ("You don't know about me, without you ")
                              TEXT ("have read a book by the name of \"The ")
                              TEXT ("Adventures of Tom Sawyer,\" but that ")
                              TEXT ("ain't no matter. That book was made by ")
                              TEXT ("Mr. Mark Twain, and he told the truth, ")
                              TEXT ("mainly. There was things which he ")
                              TEXT ("stretched, but mainly he told the truth. ")
                              TEXT ("That is nothing. I never seen anybody ")
                              TEXT ("but lied, one time or another, without ")
                              TEXT ("it was Aunt Polly, or the widow, or ")
                              TEXT ("maybe Mary. Aunt Polly -- Tom's Aunt ")
                              TEXT ("Polly, she is -- and Mary, and the Widow ")
                              TEXT ("Douglas, is all told about in that book ")
                              TEXT ("-- which is mostly a true book; with ")
                              TEXT ("some stretchers, as I said before.") } ;
     BOOL              fSuccess ;
     HDC               hdc, hdcPrn ;
     HMENU             hMenu ;
     int               iSavePointSize ;
     PAINTSTRUCT       ps ;
     RECT              rect ;
     
     switch (message)
     {
     case WM_CREATE:
               // initialise la structure CHOOSEFONT

          GetObject (GetStockObject (SYSTEM_FONT), sizeof (lf), &lf) ;

          cf.lStructSize    = sizeof (CHOOSEFONT) ;
          cf.hwndOwner      = hwnd ;
          cf.hDC            = NULL ;
          cf.lpLogFont      = &lf ;
          cf.iPointSize     = 0 ;
          cf.Flags          = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | 
                              CF_EFFECTS ;
          cf.rgbColors      = 0 ;
          cf.lCustData      = 0 ;
          cf.lpfnHook       = NULL ;
          cf.lpTemplateName = NULL ;
          cf.hInstance      = NULL ;
          cf.lpszStyle      = NULL ;
          cf.nFontType      = 0 ;      
          cf.nSizeMin       = 0 ;
          cf.nSizeMax       = 0 ;
  
          return 0 ;

     case WM_COMMAND:
          hMenu = GetMenu (hwnd) ;
          
          switch (LOWORD (wParam))
          {
          case IDM_FILE_PRINT:
                                   // Recherche l'imprimante

               pd.lStructSize = sizeof (PRINTDLG) ;
               pd.hwndOwner   = hwnd ;
          	pd.Flags       = PD_RETURNDC | PD_NOPAGENUMS | PD_NOSELECTION ;

	          if (!PrintDlg (&pd))
                    return 0 ;

               if (NULL == (hdcPrn = pd.hDC))
               {
                    MessageBox (hwnd, TEXT ("Impossible de trouver l'imprimante"),
                                szAppName, MB_ICONEXCLAMATION | MB_OK) ;
                    return 0 ;
               }
                    // d�finit les marges � 1 pouce

               rect.left   = GetDeviceCaps (hdcPrn, LOGPIXELSX) -
                             GetDeviceCaps (hdcPrn, PHYSICALOFFSETX) ;

               rect.top    = GetDeviceCaps (hdcPrn, LOGPIXELSY) -
                             GetDeviceCaps (hdcPrn, PHYSICALOFFSETY) ;

               rect.right  = GetDeviceCaps (hdcPrn, PHYSICALWIDTH) -
                             GetDeviceCaps (hdcPrn, LOGPIXELSX) -
                             GetDeviceCaps (hdcPrn, PHYSICALOFFSETX) ;

               rect.bottom = GetDeviceCaps (hdcPrn, PHYSICALHEIGHT) - 
                             GetDeviceCaps (hdcPrn, LOGPIXELSY) -
                             GetDeviceCaps (hdcPrn, PHYSICALOFFSETY) ;

                    // Imprime le texte

               SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
               ShowCursor (TRUE) ;

               fSuccess = FALSE ;

               if ((StartDoc (hdcPrn, &di) > 0) && (StartPage (hdcPrn) > 0))
               {
                         // choix de la police avec lfHeight ajust�

                    iSavePointSize = lf.lfHeight ;
                    lf.lfHeight = -(GetDeviceCaps (hdcPrn, LOGPIXELSY) *
                                         cf.iPointSize) / 720 ;

                    SelectObject (hdcPrn, CreateFontIndirect (&lf)) ;
                    lf.lfHeight = iSavePointSize ;

                         // D�finit la couleur du texte 

                    SetTextColor (hdcPrn, cf.rgbColors) ;
               
                         // affiche le texte

                    Justify (hdcPrn, szText, &rect, iAlign) ;

                    if (EndPage (hdcPrn) > 0)
                    {
                         fSuccess = TRUE ;
                         EndDoc (hdcPrn) ;
                    }
               }
               ShowCursor (FALSE) ;
               SetCursor (LoadCursor (NULL, IDC_ARROW)) ;

               DeleteDC (hdcPrn) ;

               if (!fSuccess)
                    MessageBox (hwnd, TEXT ("Impossible d'imprimer le texte"),
                                szAppName, MB_ICONEXCLAMATION | MB_OK) ;
               return 0 ;

          case IDM_FONT:
               if (ChooseFont (&cf))
                    InvalidateRect (hwnd, NULL, TRUE) ;
               return 0 ;
               
          case IDM_ALIGN_LEFT:
          case IDM_ALIGN_RIGHT:
          case IDM_ALIGN_CENTER:
          case IDM_ALIGN_JUSTIFIED:
               CheckMenuItem (hMenu, iAlign, MF_UNCHECKED) ;
               iAlign = LOWORD (wParam) ;
               CheckMenuItem (hMenu, iAlign, MF_CHECKED) ;
               InvalidateRect (hwnd, NULL, TRUE) ;
               return 0 ;
          }
          return 0 ;

     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;
          
          GetClientRect (hwnd, &rect) ;
          DrawRuler (hdc, &rect) ;
          
          rect.left  += GetDeviceCaps (hdc, LOGPIXELSX) / 2 ;
          rect.top   += GetDeviceCaps (hdc, LOGPIXELSY) / 2 ;
          rect.right -= GetDeviceCaps (hdc, LOGPIXELSX) / 4 ;

          SelectObject (hdc, CreateFontIndirect (&lf)) ;
          SetTextColor (hdc, cf.rgbColors) ;
          
          Justify (hdc, szText, &rect, iAlign) ;
          
          DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT)));
          EndPaint (hwnd, &ps) ;
          return 0 ;
          
     case WM_DESTROY:
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}
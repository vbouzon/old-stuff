/*-----------------------------------------
   JUSTIFY2.C -- Alignement N° 2
                 (c) Charles Petzold, 1998
  -----------------------------------------*/

#include <windows.h>
#include "resource.h"

#define OUTWIDTH 6       // largeur en pouces
#define LASTCHAR 127     // code du dernier caractère du texte

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

TCHAR szAppName[] = TEXT ("Justify2") ;

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
          MessageBox (NULL, TEXT ("Le programme nécessite Windows NT!"),
               szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName, TEXT ("Alignement N° 2"),
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
     
          // définit les twips logiques du mode topographique
     
     SetMapMode (hdc, MM_ANISOTROPIC) ;
     SetWindowExtEx (hdc, 1440, 1440, NULL) ;
     SetViewportExtEx (hdc, GetDeviceCaps (hdc, LOGPIXELSX),
                            GetDeviceCaps (hdc, LOGPIXELSY), NULL) ;
     
          // déplacer l'origine d'un demi pouce en haut à gauche
     
     SetWindowOrgEx (hdc, -720, -720, NULL) ;
     
          // trouve la marge de droite (un quart de pouce à droite)
     
     ptClient.x = prc->right ;
     ptClient.y = prc->bottom ;
     DPtoLP (hdc, &ptClient, 1) ;
     ptClient.x -= 360 ;
     
          // dessine les règles
     
     MoveToEx (hdc, 0,               -360, NULL) ;
     LineTo   (hdc, OUTWIDTH * 1440, -360) ;
     MoveToEx (hdc, -360,               0, NULL) ;
     LineTo   (hdc, -360,      ptClient.y) ;
     
     for (i = 0, j = 0 ; i <= ptClient.x && i <= OUTWIDTH * 1440 ;
                         i += 1440 / 16, j++)
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

/*----------------------------------------------------------------------
   GetCharDesignWidths:  Obtient la largeur des caractères des polices 
                         dans leur taille originale
  ----------------------------------------------------------------------*/

UINT GetCharDesignWidths (HDC hdc, UINT uFirst, UINT uLast, int * piWidths)
{
     HFONT             hFont, hFontDesign ;
     LOGFONT           lf ;
     OUTLINETEXTMETRIC otm ;

     hFont = GetCurrentObject (hdc, OBJ_FONT) ;
     GetObject (hFont, sizeof (LOGFONT), &lf) ;

          // GetOutlineTextmetrics (nous n'utilisons qu'un champ
          //   du contexte de périphérique de la police sélectionnée)

     otm.otmSize = sizeof (OUTLINETEXTMETRIC) ;
     GetOutlineTextMetrics (hdc, sizeof (OUTLINETEXTMETRIC), &otm) ;

          // crée une nouvelle police fondée sur la taille

     lf.lfHeight = - (int) otm.otmEMSquare ;
     lf.lfWidth  = 0 ;
     hFontDesign = CreateFontIndirect (&lf) ;

          // sélectionne la police dans le  contexte de périphérique et obtient les largeurs

     SaveDC (hdc) ;
     SetMapMode (hdc, MM_TEXT) ;
     SelectObject (hdc, hFontDesign) ;

     GetCharWidth (hdc, uFirst, uLast, piWidths) ;
     SelectObject (hdc, hFont) ;
     RestoreDC (hdc, -1) ;

          // Efface

     DeleteObject (hFontDesign) ;

     return otm.otmEMSquare ;
}

/*---------------------------------------------------------------------
   GetScaledWidths:  obtient les largeurs des caractères pour la 
                     taille de police sélectionnée
  ---------------------------------------------------------------------*/

void GetScaledWidths (HDC hdc, double * pdWidths)
{
     double  dScale ;
     HFONT   hFont ;
     int     aiDesignWidths [LASTCHAR + 1] ;
     int     i ;
     LOGFONT lf ;
     UINT    uEMSquare ;

          // appel de la fonction

     uEMSquare = GetCharDesignWidths (hdc, 0, LASTCHAR, aiDesignWidths) ;

          // obtient LOGFONT pour la police courante du contexte de périphérique

     hFont = GetCurrentObject (hdc, OBJ_FONT) ;
     GetObject (hFont, sizeof (LOGFONT), &lf) ;

          // échelonne les largeurs et les stocke comme valeurs à virgule flottante

     dScale = (double) -lf.lfHeight / (double) uEMSquare ;

     for (i = 0 ; i <= LASTCHAR ; i++)
          pdWidths[i] = dScale * aiDesignWidths[i] ;
}

/*--------------------------------------------------------------
   GetTextExtentFloat:  Calcule la largeur du texte en virgule flottante
  --------------------------------------------------------------*/

double GetTextExtentFloat (double * pdWidths, PTSTR psText, int iCount)
{
     double dWidth = 0 ;
     int    i ;

     for (i = 0 ; i < iCount ; i++)
          dWidth += pdWidths [psText[i]] ;

     return dWidth ;
}

/*------------------------------------------------------------------
   Justify:  pour assurer la compatibilité écran/imprimante
  ------------------------------------------------------------------*/

void Justify (HDC hdc, PTSTR pText, RECT * prc, int iAlign)
{
     double dWidth, adWidths[LASTCHAR + 1] ;
     int    xStart, yStart, cSpaceChars ;
     PTSTR  pBegin, pEnd ;
     SIZE   size ;

          // remplit le tableau adWidths avec les caractères en virgule flottante
     GetScaledWidths (hdc, adWidths) ;

     yStart = prc->top ;
     do                            // Initialise le nombre d'espaces
     {
          cSpaceChars = 0 ;        // pour chaque ligne du texte

          while (*pText == ' ')    // saute les espaces
               pText++ ;

          pBegin = pText ;         // place le curseur sur le premier caractère de la ligne
          
          do                       
          {
               pEnd = pText ;      // place le curseur sur le dernier caractère de la ligne
                    // saute à l'espace suivant
               
               while (*pText != '\0' && *pText++ != ' ') ;

               if (*pText == '\0')
                    break ;

                    // calcule les espacements
               cSpaceChars++ ;
               dWidth = GetTextExtentFloat (adWidths, pBegin, 
                                                      pText - pBegin - 1) ;
          }
          while (dWidth < (double) (prc->right - prc->left)) ;
          
          cSpaceChars-- ;               // dernier espace de la ligne
          
          while (*(pEnd - 1) == ' ')    // élimine les espaces
          {
               pEnd-- ;
               cSpaceChars-- ;
          }

               // si fin du texte et pas d'espace, définit pEnd
          
          if (*pText == '\0' || cSpaceChars <= 0)
               pEnd = pText ;

               // obtient les extensions en entier

          GetTextExtentPoint32(hdc, pBegin, pEnd - pBegin, &size) ;
          
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

               // préparation de la prochaine ligne

          SetTextJustification (hdc, 0, 0) ;
          yStart += size.cy ;
          pText = pEnd ;
     }
     while (*pText && yStart < prc->bottom - size.cy) ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static CHOOSEFONT cf ;
     static DOCINFO    di = { sizeof (DOCINFO), TEXT ("Justify2: Printing") } ;
     static int        iAlign = IDM_ALIGN_LEFT ;
     static LOGFONT    lf ;
     static PRINTDLG   pd ;
     static TCHAR      szText[] = { 
                              TEXT ("Call me Ishmael. Some years ago -- never ")
                              TEXT ("mind how long precisely -- having little ")
                              TEXT ("or no money in my purse, and nothing ")
                              TEXT ("particular to interest me on shore, I ")
                              TEXT ("thought I would sail about a little and ")
                              TEXT ("see the watery part of the world. It is ")
                              TEXT ("a way I have of driving off the spleen, ")
                              TEXT ("and regulating the circulation. Whenever ")
                              TEXT ("I find myself growing grim about the ")
                              TEXT ("mouth; whenever it is a damp, drizzly ")
                              TEXT ("November in my soul; whenever I find ")
                              TEXT ("myself involuntarily pausing before ")
                              TEXT ("coffin warehouses, and bringing up the ")
                              TEXT ("rear of every funeral I meet; and ")
                              TEXT ("especially whenever my hypos get such an ")
                              TEXT ("upper hand of me, that it requires a ")
                              TEXT ("strong moral principle to prevent me ")
                              TEXT ("from deliberately stepping into the ")
                              TEXT ("street, and methodically knocking ")
                              TEXT ("people's hats off -- then, I account it ")
                              TEXT ("high time to get to sea as soon as I ")
                              TEXT ("can. This is my substitute for pistol ")
                              TEXT ("and ball. With a philosophical flourish ")
                              TEXT ("Cato throws himself upon his sword; I ")
                              TEXT ("quietly take to the ship. There is ")
                              TEXT ("nothing surprising in this. If they but ")
                              TEXT ("knew it, almost all men in their degree, ")
                              TEXT ("some time or other, cherish very nearly ")
                              TEXT ("the same feelings towards the ocean with ")
                              TEXT ("me.") } ;
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
          hdc = GetDC (hwnd) ;
          lf.lfHeight = - GetDeviceCaps (hdc, LOGPIXELSY) / 6 ;
          lf.lfOutPrecision = OUT_TT_ONLY_PRECIS ;
          lstrcpy (lf.lfFaceName, TEXT ("Times New Roman")) ;
          ReleaseDC (hwnd, hdc) ;

          cf.lStructSize    = sizeof (CHOOSEFONT) ;
          cf.hwndOwner      = hwnd ;
          cf.hDC            = NULL ;
          cf.lpLogFont      = &lf ;
          cf.iPointSize     = 120 ;

               // définit les flags uniquement pour TrueType !

          cf.Flags          = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | 
                              CF_TTONLY | CF_EFFECTS ;
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
                    // définit les marges pour OUTWIDTH 

               rect.left  = (GetDeviceCaps (hdcPrn, PHYSICALWIDTH) -
                             GetDeviceCaps (hdcPrn, LOGPIXELSX) * OUTWIDTH) / 2 
                           - GetDeviceCaps (hdcPrn, PHYSICALOFFSETX) ;
               
               rect.right = rect.left + 
                             GetDeviceCaps (hdcPrn, LOGPIXELSX) * OUTWIDTH ;

                    // définit les marges du haut et du bas à un pouce

               rect.top    = GetDeviceCaps (hdcPrn, LOGPIXELSY) -
                             GetDeviceCaps (hdcPrn, PHYSICALOFFSETY) ;

               rect.bottom = GetDeviceCaps (hdcPrn, PHYSICALHEIGHT) - 
                             GetDeviceCaps (hdcPrn, LOGPIXELSY) -
                             GetDeviceCaps (hdcPrn, PHYSICALOFFSETY) ;

                    // Imprime le texte

               SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
               ShowCursor (TRUE) ;

               fSuccess = FALSE ;

               if ((StartDoc (hdcPrn, &di) > 0) && (StartPage (hdcPrn) > 0))
               {
                         // sélectionne la police utilisant lfHeight ajusté

                    iSavePointSize = lf.lfHeight ;
                    lf.lfHeight = -(GetDeviceCaps (hdcPrn, LOGPIXELSY) *
                                         cf.iPointSize) / 720 ;

                    SelectObject (hdcPrn, CreateFontIndirect (&lf)) ;
                    lf.lfHeight = iSavePointSize ;

                         // définit la couleur du texte 

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
          rect.right = rect.left + OUTWIDTH * GetDeviceCaps (hdc, LOGPIXELSX) ;

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


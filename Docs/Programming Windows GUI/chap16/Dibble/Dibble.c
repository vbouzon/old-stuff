/*----------------------------------------
   DIBBLE.C -- Programme avec bitmaps et palettes
               (c) Charles Petzold, 1998
  ----------------------------------------*/

#include <windows.h>
#include "dibhelp.h"
#include "dibpal.h"
#include "dibconv.h"
#include "resource.h"

#define WM_USER_SETSCROLLS    (WM_USER + 1)
#define WM_USER_DELETEDIB     (WM_USER + 2)
#define WM_USER_DELETEPAL     (WM_USER + 3)
#define WM_USER_CREATEPAL     (WM_USER + 4)

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

TCHAR szAppName[] = TEXT ("Dibble") ;

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

     hwnd = CreateWindow (szAppName, szAppName,
                          WS_OVERLAPPEDWINDOW | WM_VSCROLL | WM_HSCROLL,
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

/*-------------------------------------------------------------
   DisplayDib: Affiche ou imprime une DIB dans sa taille d'origine
               ou adaptée à la fenêtre, selon la commande sélectionnée
  -------------------------------------------------------------*/

int DisplayDib (HDC hdc, HBITMAP hBitmap, int x, int y, 
                int cxClient, int cyClient, 
                WORD wShow, BOOL fHalftonePalette)
{
     BITMAP bitmap ;
     HDC    hdcMem ; 
     int    cxBitmap, cyBitmap, iReturn ;

     GetObject (hBitmap, sizeof (BITMAP), &bitmap) ;
     cxBitmap = bitmap.bmWidth ;
     cyBitmap = bitmap.bmHeight ;

     SaveDC (hdc) ;

     if (fHalftonePalette)
          SetStretchBltMode (hdc, HALFTONE) ;
     else
          SetStretchBltMode (hdc, COLORONCOLOR) ;

     hdcMem = CreateCompatibleDC (hdc) ;
     SelectObject (hdcMem, hBitmap) ;

     switch (wShow)
     {
     case IDM_SHOW_NORMAL:
          if (fHalftonePalette)
               iReturn = StretchBlt (hdc,    0, 0, 
                                             min (cxClient, cxBitmap - x), 
                                             min (cyClient, cyBitmap - y), 
                                     hdcMem, x, y, 
                                             min (cxClient, cxBitmap - x), 
                                             min (cyClient, cyBitmap - y), 
                                     SRCCOPY);
          else
               iReturn = BitBlt (hdc,    0, 0, 
                                         min (cxClient, cxBitmap - x), 
                                         min (cyClient, cyBitmap - y),
                                 hdcMem, x, y, SRCCOPY) ;
          break ;
               
     case IDM_SHOW_CENTER:
          if (fHalftonePalette)
               iReturn = StretchBlt (hdc, (cxClient - cxBitmap) / 2,
                                          (cyClient - cyBitmap) / 2, 
                                          cxBitmap, cyBitmap,
                                     hdcMem, 0, 0, cxBitmap, cyBitmap, SRCCOPY);
          else
               iReturn = BitBlt (hdc, (cxClient - cxBitmap) / 2,
                                      (cyClient - cyBitmap) / 2, 
                                      cxBitmap, cyBitmap,
                                 hdcMem, 0, 0, SRCCOPY) ;
          break ;

     case IDM_SHOW_STRETCH:
          iReturn = StretchBlt (hdc,    0, 0, cxClient, cyClient, 
                                hdcMem, 0, 0, cxBitmap, cyBitmap, SRCCOPY) ;
          break ;

     case IDM_SHOW_ISOSTRETCH:
          SetMapMode (hdc, MM_ISOTROPIC) ;
          SetWindowExtEx (hdc, cxBitmap, cyBitmap, NULL) ;
          SetViewportExtEx (hdc, cxClient, cyClient, NULL) ;
          SetWindowOrgEx (hdc, cxBitmap / 2, cyBitmap / 2, NULL) ;
          SetViewportOrgEx (hdc, cxClient / 2, cyClient / 2, NULL) ;

          iReturn = StretchBlt (hdc,    0, 0, cxBitmap, cyBitmap, 
                                hdcMem, 0, 0, cxBitmap, cyBitmap, SRCCOPY) ;
          break ;
     }
     DeleteDC (hdcMem) ;
     RestoreDC (hdc, -1) ;
     return iReturn ;
}

/*--------------------------------------------------------------------
   DibFlipHorizontal: Appelle DibSetPixel et DibGetPixel non optimisées
  --------------------------------------------------------------------*/

HDIB DibFlipHorizontal (HDIB hdibSrc)
{
     HDIB hdibDst ;
     int  cx, cy, x, y ;

     if (!DibIsAddressable (hdibSrc))
          return NULL ;

     if (NULL == (hdibDst = DibCopy (hdibSrc, FALSE)))
          return NULL ;

     cx = DibWidth  (hdibSrc) ;
     cy = DibHeight (hdibSrc) ;
     
     for (x = 0 ; x < cx ; x++)
     for (y = 0 ; y < cy ; y++)
     {
          DibSetPixel (hdibDst, x, cy - 1 - y, DibGetPixel (hdibSrc, x, y)) ;
     }
     return hdibDst ;
}

/*---------------------------------------------------------------
   DibRotateRight: Appelle DibSetPixelx et DibGetPixelx optimisées
  ---------------------------------------------------------------*/

HDIB DibRotateRight (HDIB hdibSrc)
{
     HDIB hdibDst ;
     int  cx, cy, x, y ;

     if (!DibIsAddressable (hdibSrc))
          return NULL ;

     if (NULL == (hdibDst = DibCopy (hdibSrc, TRUE)))
          return NULL ;

     cx = DibWidth (hdibSrc) ;
     cy = DibHeight (hdibSrc) ;

     switch (DibBitCount (hdibSrc))
     {
     case  1:  
          for (x = 0 ; x < cx ; x++)
          for (y = 0 ; y < cy ; y++)
               DibSetPixel1 (hdibDst, cy - y - 1, x, 
                    DibGetPixel1 (hdibSrc, x, y)) ;
          break ;

     case  4:  
          for (x = 0 ; x < cx ; x++)
          for (y = 0 ; y < cy ; y++)
               DibSetPixel4 (hdibDst, cy - y - 1, x, 
                    DibGetPixel4 (hdibSrc, x, y)) ;
          break ;

     case  8:
          for (x = 0 ; x < cx ; x++)
          for (y = 0 ; y < cy ; y++)
               DibSetPixel8 (hdibDst, cy - y - 1, x, 
                    DibGetPixel8 (hdibSrc, x, y)) ;
          break ;

     case 16:  
          for (x = 0 ; x < cx ; x++)
          for (y = 0 ; y < cy ; y++)
               DibSetPixel16 (hdibDst, cy - y - 1, x, 
                    DibGetPixel16 (hdibSrc, x, y)) ;
          break ;

     case 24:
          for (x = 0 ; x < cx ; x++)
          for (y = 0 ; y < cy ; y++)
               DibSetPixel24 (hdibDst, cy - y - 1, x, 
                    DibGetPixel24 (hdibSrc, x, y)) ;
          break ;

     case 32:  
          for (x = 0 ; x < cx ; x++)
          for (y = 0 ; y < cy ; y++)
               DibSetPixel32 (hdibDst, cy - y - 1, x, 
                    DibGetPixel32 (hdibSrc, x, y)) ;
          break ;
     }
     return hdibDst ;
}

/*----------------------------------------------------------
   PaletteMenu: Affiche et supprime les coches des commandes 
                du menu de la palette
  ----------------------------------------------------------*/

void PaletteMenu (HMENU hMenu, WORD wItemNew)
{
     static WORD wItem = IDM_PAL_NONE ;

     CheckMenuItem (hMenu, wItem, MF_UNCHECKED) ;
     wItem = wItemNew ;
     CheckMenuItem (hMenu, wItem, MF_CHECKED) ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static BOOL         fHalftonePalette ;
     static DOCINFO      di = { sizeof (DOCINFO), TEXT ("Dibble: Impression") } ;
     static HBITMAP      hBitmap ;
     static HDIB         hdib ;
     static HMENU        hMenu ;
     static HPALETTE     hPalette ;
     static int          cxClient, cyClient, iVscroll, iHscroll ;
     static OPENFILENAME ofn ;
     static PRINTDLG     printdlg = { sizeof (PRINTDLG) } ;
     static TCHAR        szFileName [MAX_PATH], szTitleName [MAX_PATH] ;
     static TCHAR        szFilter[] = TEXT ("Fichiers bitmap (*.BMP)\0*.bmp\0")
                                      TEXT ("Tous les fichiers (*.*)\0*.*\0\0") ;
     static TCHAR      * szCompression[] = { 
                           TEXT ("BI_RGB"), TEXT ("BI_RLE8"), TEXT ("BI_RLE4"), 
                           TEXT ("BI_BITFIELDS"), TEXT ("Inconnu") } ;
     static WORD         wShow = IDM_SHOW_NORMAL ;
     BOOL                fSuccess ;
     BYTE              * pGlobal ;
     HDC                 hdc, hdcPrn ;
     HGLOBAL             hGlobal ;
     HDIB                hdibNew ;
     int                 iEnable, cxPage, cyPage, iConvert ;
     PAINTSTRUCT         ps ;
     SCROLLINFO          si ;
     TCHAR               szBuffer [256] ;

     switch (message)
     {
     case WM_CREATE:
         
               // Stocke l'identificateur du menu dans une variable statique

          hMenu = GetMenu (hwnd) ;

               // Initialise la structure OPENFILENAME pour les boîtes
               //   de dialogue Ouvrir en Enregistrer.

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
          ofn.Flags             = OFN_OVERWRITEPROMPT ;
          ofn.nFileOffset       = 0 ;
          ofn.nFileExtension    = 0 ;
          ofn.lpstrDefExt       = TEXT ("bmp") ;
          ofn.lCustData         = 0 ;
          ofn.lpfnHook          = NULL ;
          ofn.lpTemplateName    = NULL ;
          return 0 ;

     case WM_DISPLAYCHANGE:
          SendMessage (hwnd, WM_USER_DELETEPAL, 0, 0) ;
          SendMessage (hwnd, WM_USER_CREATEPAL, TRUE, 0) ;
          return 0 ;

     case WM_SIZE:
               // Enregistre largeur et hauteur de la zone client
               // dans des variables statiques.

          cxClient = LOWORD (lParam) ;
          cyClient = HIWORD (lParam) ;

          wParam = FALSE ;
                                             // échoue

               // WM_USER_SETSCROLLS: Message défini par le développeur !
               // Définit les barres de défilement. Si le mode d'affichage
               // n'est pas normal, les rendre invisibles.
               //   Si wParam est TRUE, réinitialiser la position
               //   de la barre de défilement.

     case WM_USER_SETSCROLLS:
          if (hdib == NULL || wShow != IDM_SHOW_NORMAL)
          {
               si.cbSize = sizeof (SCROLLINFO) ;
               si.fMask  = SIF_RANGE ;
               si.nMin   = 0 ;
               si.nMax   = 0 ;
               SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;
               SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ;
          }
          else
          {
                    // D'abord la barre de défilement verticale

               si.cbSize = sizeof (SCROLLINFO) ;
               si.fMask  = SIF_ALL ;

               GetScrollInfo (hwnd, SB_VERT, &si) ;
               si.nMin  = 0 ;
               si.nMax  = DibHeight (hdib) ;
               si.nPage = cyClient ;

               if ((BOOL) wParam)
                    si.nPos = 0 ;

               SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;
               GetScrollInfo (hwnd, SB_VERT, &si) ;

               iVscroll = si.nPos ;

                    // Ensuite la barre de défilement horizontale

               GetScrollInfo (hwnd, SB_HORZ, &si) ;
               si.nMin  = 0 ;
               si.nMax  = DibWidth (hdib) ;
               si.nPage = cxClient ;
          
               if ((BOOL) wParam)
                    si.nPos = 0 ;

               SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ;
               GetScrollInfo (hwnd, SB_HORZ, &si) ;

               iHscroll = si.nPos ;
          }
          return 0 ;

          // WM_VSCROLL: Défilement vertical de la DIB

     case WM_VSCROLL:
          si.cbSize = sizeof (SCROLLINFO) ;
          si.fMask  = SIF_ALL ;
          GetScrollInfo (hwnd, SB_VERT, &si) ;
          
          iVscroll = si.nPos ;

          switch (LOWORD (wParam))
          {
          case SB_LINEUP:      si.nPos -= 1 ;             break ;
          case SB_LINEDOWN:    si.nPos += 1 ;             break ;
          case SB_PAGEUP:      si.nPos -= si.nPage ;      break ;
          case SB_PAGEDOWN:    si.nPos += si.nPage ;      break ;
          case SB_THUMBTRACK:  si.nPos  = si.nTrackPos ;  break ;
          default:                                        break ;
          }
          si.fMask = SIF_POS ;
          SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;
          GetScrollInfo (hwnd, SB_VERT, &si) ;
          if (si.nPos != iVscroll)
          {
               ScrollWindow (hwnd, 0, iVscroll - si.nPos, NULL, NULL) ;
               iVscroll = si.nPos ;
               UpdateWindow (hwnd) ;
          }
          return 0 ;

          // WM_HSCROLL: Défilement horizontal de la DIB

     case WM_HSCROLL:
          si.cbSize = sizeof (SCROLLINFO) ;
          si.fMask  = SIF_ALL ;
          GetScrollInfo (hwnd, SB_HORZ, &si) ;
          
          iHscroll = si.nPos ;

          switch (LOWORD (wParam))
          {
          case SB_LINELEFT:    si.nPos -= 1 ;             break ;
          case SB_LINERIGHT:   si.nPos += 1 ;             break ;
          case SB_PAGELEFT:    si.nPos -= si.nPage ;      break ;
          case SB_PAGERIGHT:   si.nPos += si.nPage ;      break ;
          case SB_THUMBTRACK:  si.nPos  = si.nTrackPos ;  break ;
          default:                                        break ;
          }

          si.fMask = SIF_POS ;
          SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ;
          GetScrollInfo (hwnd, SB_HORZ, &si) ;

          if (si.nPos != iHscroll)
          {
               ScrollWindow (hwnd, iHscroll - si.nPos, 0, NULL, NULL) ;
               iHscroll = si.nPos ;
               UpdateWindow (hwnd) ;
          }
          return 0 ;

          // WM_INITMENUPOPUP: Affiche commandes activées ou en gris

     case WM_INITMENUPOPUP:
          if (hdib)
               iEnable = MF_ENABLED ;
          else
               iEnable = MF_GRAYED ;
          EnableMenuItem (hMenu, IDM_FILE_SAVE,       iEnable) ;
          EnableMenuItem (hMenu, IDM_FILE_PRINT,      iEnable) ;
          EnableMenuItem (hMenu, IDM_FILE_PROPERTIES, iEnable) ;
          EnableMenuItem (hMenu, IDM_EDIT_CUT,        iEnable) ;
          EnableMenuItem (hMenu, IDM_EDIT_COPY,       iEnable) ;
          EnableMenuItem (hMenu, IDM_EDIT_DELETE,     iEnable) ;

          if (DibIsAddressable (hdib))
               iEnable = MF_ENABLED ;
          else
               iEnable = MF_GRAYED ;

          EnableMenuItem (hMenu, IDM_EDIT_ROTATE,    iEnable) ;
          EnableMenuItem (hMenu, IDM_EDIT_FLIP,      iEnable) ;
          EnableMenuItem (hMenu, IDM_CONVERT_01,     iEnable) ;
          EnableMenuItem (hMenu, IDM_CONVERT_04,     iEnable) ;
          EnableMenuItem (hMenu, IDM_CONVERT_08,     iEnable) ;
          EnableMenuItem (hMenu, IDM_CONVERT_16,     iEnable) ;
          EnableMenuItem (hMenu, IDM_CONVERT_24,     iEnable) ;
          EnableMenuItem (hMenu, IDM_CONVERT_32,     iEnable) ;

          switch (DibBitCount (hdib))
          {
          case  1: EnableMenuItem (hMenu, IDM_CONVERT_01, MF_GRAYED) ; break ;
          case  4: EnableMenuItem (hMenu, IDM_CONVERT_04, MF_GRAYED) ; break ;
          case  8: EnableMenuItem (hMenu, IDM_CONVERT_08, MF_GRAYED) ; break ;
          case 16: EnableMenuItem (hMenu, IDM_CONVERT_16, MF_GRAYED) ; break ;
          case 24: EnableMenuItem (hMenu, IDM_CONVERT_24, MF_GRAYED) ; break ;
          case 32: EnableMenuItem (hMenu, IDM_CONVERT_32, MF_GRAYED) ; break ;
          }

          if (hdib && DibColorSize (hdib) > 0)
               iEnable = MF_ENABLED ;
          else
               iEnable = MF_GRAYED ;

          EnableMenuItem (hMenu, IDM_PAL_DIBTABLE,    iEnable) ;
          if (DibIsAddressable (hdib) && DibBitCount (hdib) > 8)
               iEnable = MF_ENABLED ;
          else
               iEnable = MF_GRAYED ;

          EnableMenuItem (hMenu, IDM_PAL_OPT_POP4,   iEnable) ;
          EnableMenuItem (hMenu, IDM_PAL_OPT_POP5,   iEnable) ;
          EnableMenuItem (hMenu, IDM_PAL_OPT_POP6,   iEnable) ;
          EnableMenuItem (hMenu, IDM_PAL_OPT_MEDCUT, iEnable) ;
          EnableMenuItem (hMenu, IDM_EDIT_PASTE, 
               IsClipboardFormatAvailable (CF_DIB) ? MF_ENABLED : MF_GRAYED) ;

          return 0 ;

          // WM_COMMAND: Traite toutes les commandes des menus.

     case WM_COMMAND:
          iConvert = 0 ;

          switch (LOWORD (wParam))
          {
          case IDM_FILE_OPEN:

                    // Affiche la boîte de dialogue Ouvrir

               if (!GetOpenFileName (&ofn))
                    return 0 ;
               
                    // Supprimer DIB et palette éventuelles

               SendMessage (hwnd, WM_USER_DELETEDIB, 0, 0) ;
               
                    // Charger la DIB dans la mémoire

               SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
               ShowCursor (TRUE) ;

               hdib = DibFileLoad (szFileName) ;

               ShowCursor (FALSE) ;
               SetCursor (LoadCursor (NULL, IDC_ARROW)) ;

                    // Réinitialise les barres de défilement

               SendMessage (hwnd, WM_USER_SETSCROLLS, TRUE, 0) ;

                    // Crée la palette et la DDB

               SendMessage (hwnd, WM_USER_CREATEPAL, TRUE, 0) ;

               if (!hdib)
               {
                    MessageBox (hwnd, TEXT ("Impossible de charger le fichier DIB!"), 
                                szAppName, MB_OK | MB_ICONEXCLAMATION) ;
               }
               InvalidateRect (hwnd, NULL, TRUE) ;
               return 0 ;

          case IDM_FILE_SAVE:

                    // Affiche la boîte de dialogue Enregistrer

               if (!GetSaveFileName (&ofn))
                    return 0 ;

                    // Copie la DIB dans la mémoire

               SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
               ShowCursor (TRUE) ;

               fSuccess = DibFileSave (hdib, szFileName) ;

               ShowCursor (FALSE) ;
               SetCursor (LoadCursor (NULL, IDC_ARROW)) ;

               if (!fSuccess)
                    MessageBox (hwnd, TEXT ("Impossible d'enregistrer le fichier DIB !"),
                                szAppName, MB_OK | MB_ICONEXCLAMATION) ;
               return 0 ;

          case IDM_FILE_PRINT:
               if (!hdib)
                    return 0 ;

                    // Obtention du contexte de périphérique de l'imprimante

               printdlg.Flags = PD_RETURNDC | PD_NOPAGENUMS | PD_NOSELECTION ;

               if (!PrintDlg (&printdlg))
                    return 0 ;

               if (NULL == (hdcPrn = printdlg.hDC))
               {
                    MessageBox (hwnd, TEXT ("Impossible d'obtenir le contexte de périphérique de l'imprimante"),
                                szAppName, MB_ICONEXCLAMATION | MB_OK) ;
                    return 0 ;
               }
                    // Vérifie que l'imprimante peut imprimer les images bitmap

               if (!(RC_BITBLT & GetDeviceCaps (hdcPrn, RASTERCAPS)))
               {
                    DeleteDC (hdcPrn) ;
                    MessageBox (hwnd, TEXT ("L'imprimante ne peut pas imprimer les images bitmap"),
                                szAppName, MB_ICONEXCLAMATION | MB_OK) ;
                    return 0 ;
               }
                    // Obtention de la taille de la zone imprimable sur la page

               cxPage = GetDeviceCaps (hdcPrn, HORZRES) ;
               cyPage = GetDeviceCaps (hdcPrn, VERTRES) ;

               fSuccess = FALSE ;

                    // Envoie la DIB à l'imprimante

               SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
               ShowCursor (TRUE) ;

               if ((StartDoc (hdcPrn, &di) > 0) && (StartPage (hdcPrn) > 0))
               {
                    DisplayDib (hdcPrn, DibBitmapHandle (hdib), 0, 0, 
                                cxPage, cyPage, wShow, FALSE) ;
                    
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
                    MessageBox (hwnd, TEXT ("Impossible d'imprimer l'image bitmap"),
                                szAppName, MB_ICONEXCLAMATION | MB_OK) ;
               return 0 ;

          case IDM_FILE_PROPERTIES:
               if (!hdib)
                    return 0 ;

               wsprintf (szBuffer, TEXT ("Largeur de point :\t%i\n")
                                   TEXT ("Hauteur de point :\t%i\n")
                                   TEXT ("Bits par point :\t%i\n")
                                   TEXT ("Nombre de couleurs :\t%i\n")
                                   TEXT ("Compression :\t%s\n"),
                         DibWidth (hdib), DibHeight (hdib),
                         DibBitCount (hdib), DibNumColors (hdib),
                         szCompression [min (3, DibCompression (hdib))]) ;

               MessageBox (hwnd, szBuffer, szAppName, 
                           MB_ICONEXCLAMATION | MB_OK) ;
               return 0 ;

          case IDM_APP_EXIT:
               SendMessage (hwnd, WM_CLOSE, 0, 0) ;
               return 0 ;

          case IDM_EDIT_COPY:
          case IDM_EDIT_CUT:
               if (!(hGlobal = DibCopyToPackedDib (hdib, TRUE)))
                    return 0 ;

               OpenClipboard (hwnd) ;
               EmptyClipboard () ;
               SetClipboardData (CF_DIB, hGlobal) ;
               CloseClipboard () ;

               if (LOWORD (wParam) == IDM_EDIT_COPY)
                    return 0 ;
                                   // échoue si IDM_EDIT_CUT
          case IDM_EDIT_DELETE:
               SendMessage (hwnd, WM_USER_DELETEDIB, 0, 0) ;
               InvalidateRect (hwnd, NULL, TRUE) ;
               return 0 ;

          case IDM_EDIT_PASTE:
               OpenClipboard (hwnd) ;

               hGlobal = GetClipboardData (CF_DIB) ;
               pGlobal = GlobalLock (hGlobal) ;

                    // Supprimer DIB palette éventuelles.
                    // Convertir ensuite la DIB compressée en HDIB.

               if (pGlobal)
               {
                    SendMessage (hwnd, WM_USER_DELETEDIB, 0, 0) ;
                    hdib = DibCopyFromPackedDib ((BITMAPINFO *) pGlobal) ;
                    SendMessage (hwnd, WM_USER_CREATEPAL, TRUE, 0) ;
               }

               GlobalUnlock (hGlobal) ;
               CloseClipboard () ;

                    // Réinitialise les barres de défilement

               SendMessage (hwnd, WM_USER_SETSCROLLS, TRUE, 0) ;
               InvalidateRect (hwnd, NULL, TRUE) ;
               return 0 ;

          case IDM_EDIT_ROTATE:
               if (hdibNew = DibRotateRight (hdib))
               {
                    DibDelete (hdib) ;
                    DeleteObject (hBitmap) ;
                    hdib = hdibNew ;
                    hBitmap = DibCopyToDdb (hdib, hwnd, hPalette) ;
                    SendMessage (hwnd, WM_USER_SETSCROLLS, TRUE, 0) ;
                    InvalidateRect (hwnd, NULL, TRUE) ;
               }
               else
               {
                    MessageBox (hwnd, TEXT ("Mémoire insuffisante"),
                                szAppName, MB_OK | MB_ICONEXCLAMATION) ;
               }
               return 0 ;

          case IDM_EDIT_FLIP:
               if (hdibNew = DibFlipHorizontal (hdib))
               {
                    DibDelete (hdib) ;
                    DeleteObject (hBitmap) ;
                    hdib = hdibNew ;
                    hBitmap = DibCopyToDdb (hdib, hwnd, hPalette) ;
                    InvalidateRect (hwnd, NULL, TRUE) ;
               }
               else
               {
                    MessageBox (hwnd, TEXT ("Mémoire insuffisante"),
                                szAppName, MB_OK | MB_ICONEXCLAMATION) ;
               }
               return 0 ;

          case IDM_SHOW_NORMAL:
          case IDM_SHOW_CENTER:
          case IDM_SHOW_STRETCH:
          case IDM_SHOW_ISOSTRETCH:
               CheckMenuItem (hMenu, wShow, MF_UNCHECKED) ;
               wShow = LOWORD (wParam) ;
               CheckMenuItem (hMenu, wShow, MF_CHECKED) ;
               SendMessage (hwnd, WM_USER_SETSCROLLS, FALSE, 0) ;

               InvalidateRect (hwnd, NULL, TRUE) ;
               return 0 ;

          case IDM_CONVERT_32:  iConvert += 8 ;
          case IDM_CONVERT_24:  iConvert += 8 ;   
          case IDM_CONVERT_16:  iConvert += 8 ;
          case IDM_CONVERT_08:  iConvert += 4 ;
          case IDM_CONVERT_04:  iConvert += 3 ;
          case IDM_CONVERT_01:  iConvert += 1 ;
               SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
               ShowCursor (TRUE) ;

               hdibNew = DibConvert (hdib, iConvert) ;

               ShowCursor (FALSE) ;
               SetCursor (LoadCursor (NULL, IDC_ARROW)) ;

               if (hdibNew)
               {
                    SendMessage (hwnd, WM_USER_DELETEDIB, 0, 0) ;
                    hdib = hdibNew ;
                    SendMessage (hwnd, WM_USER_CREATEPAL, TRUE, 0) ;
                    InvalidateRect (hwnd, NULL, TRUE) ;
               }
               else
               {
                    MessageBox (hwnd, TEXT ("Mémoire insuffisante"),
                                szAppName, MB_OK | MB_ICONEXCLAMATION) ;
               }
               return 0 ;

          case IDM_APP_ABOUT:
               MessageBox (hwnd, TEXT ("Dibble (c) Charles Petzold, 1998"),
                           szAppName, MB_OK | MB_ICONEXCLAMATION) ;
               return 0 ;
          }
     
               // tous les autres messages WM_COMMAND viennent des éléments 
               //   de la palette. Toute palette éventuelle est supprimée, et
               //   le pointeur souris prend la forme d'un sablier.

          SendMessage (hwnd, WM_USER_DELETEPAL, 0, 0) ;
          SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
          ShowCursor (TRUE) ;

               // Notez que tous les messages destinés aux éléments de la 
               //   palette se terminent par break et non par return. Cela
               //   permet un traitement ultérieur.

          switch (LOWORD (wParam))
          {
          case IDM_PAL_DIBTABLE: 
               hPalette = DibPalDibTable (hdib) ; 
               break ;

          case IDM_PAL_HALFTONE: 
               hdc = GetDC (hwnd) ;

               if (hPalette = CreateHalftonePalette (hdc))
                    fHalftonePalette = TRUE ;

               ReleaseDC (hwnd, hdc) ;
               break ;

          case IDM_PAL_ALLPURPOSE: 
               hPalette = DibPalAllPurpose () ; 
               break ;

          case IDM_PAL_GRAY2:   hPalette = DibPalUniformGrays (  2) ; break ;
          case IDM_PAL_GRAY3:   hPalette = DibPalUniformGrays (  3) ; break ;
          case IDM_PAL_GRAY4:   hPalette = DibPalUniformGrays (  4) ; break ;
          case IDM_PAL_GRAY8:   hPalette = DibPalUniformGrays (  8) ; break ;
          case IDM_PAL_GRAY16:  hPalette = DibPalUniformGrays ( 16) ; break ;
          case IDM_PAL_GRAY32:  hPalette = DibPalUniformGrays ( 32) ; break ;
          case IDM_PAL_GRAY64:  hPalette = DibPalUniformGrays ( 64) ; break ;
          case IDM_PAL_GRAY128: hPalette = DibPalUniformGrays (128) ; break ;
          case IDM_PAL_GRAY256: hPalette = DibPalUniformGrays (256) ; break ;

          case IDM_PAL_RGB222: hPalette = DibPalUniformColors (2,2,2); break;
          case IDM_PAL_RGB333: hPalette = DibPalUniformColors (3,3,3); break;
          case IDM_PAL_RGB444: hPalette = DibPalUniformColors (4,4,4); break;
          case IDM_PAL_RGB555: hPalette = DibPalUniformColors (5,5,5); break;
          case IDM_PAL_RGB666: hPalette = DibPalUniformColors (6,6,6); break;
          case IDM_PAL_RGB775: hPalette = DibPalUniformColors (7,7,5); break;
          case IDM_PAL_RGB757: hPalette = DibPalUniformColors (7,5,7); break;
          case IDM_PAL_RGB577: hPalette = DibPalUniformColors (5,7,7); break;
          case IDM_PAL_RGB884: hPalette = DibPalUniformColors (8,8,4); break;
          case IDM_PAL_RGB848: hPalette = DibPalUniformColors (8,4,8); break;
          case IDM_PAL_RGB488: hPalette = DibPalUniformColors (4,8,8); break;

          case IDM_PAL_OPT_POP4:  
               hPalette = DibPalPopularity (hdib, 4) ;
               break ;

          case IDM_PAL_OPT_POP5:  
               hPalette = DibPalPopularity (hdib, 5) ;
               break ;

          case IDM_PAL_OPT_POP6:  
               hPalette = DibPalPopularity (hdib, 6) ;
               break ;                   

          case IDM_PAL_OPT_MEDCUT:
               hPalette = DibPalMedianCut (hdib, 6) ;
               break ;
          }

               // Après traitement des éléments Palette du menu, le pointeur
               //   souris retrouve sa forme de flèche, la commande est cochée,
               //   et la fenêtre est invalidée.

          hBitmap = DibCopyToDdb (hdib, hwnd, hPalette) ;

          ShowCursor (FALSE) ;
          SetCursor (LoadCursor (NULL, IDC_ARROW)) ;

          if (hPalette)
               PaletteMenu (hMenu, (LOWORD (wParam))) ;

          InvalidateRect (hwnd, NULL, TRUE) ;
          return 0 ;

          // Ce message défini par le développeur supprime une DIB existante
          //   pour faire place à une nouvelle. Invoqué pendant les commandes
          //   Fichier, Ouvrir, Edition, Coller, entre autres.

     case WM_USER_DELETEDIB:
          if (hdib)
          {
               DibDelete (hdib) ;
               hdib = NULL ;
          }
          SendMessage (hwnd, WM_USER_DELETEPAL, 0, 0) ;
          return 0 ;

          // Ce message défini par le développeur supprime une palette
          //   existante pour faire place à une nouvelle.

     case WM_USER_DELETEPAL:
          if (hPalette)
          {
               DeleteObject (hPalette) ;
               hPalette = NULL ;
               fHalftonePalette = FALSE ;
               PaletteMenu (hMenu, IDM_PAL_NONE) ;
          }
          if (hBitmap)
               DeleteObject (hBitmap) ;

          return 0 ;

          // message défini par le développeur pour créer une nouvelle
          //   palette basée sur une nouvelle DIB.
          //  Si wParam == TRUE, crée aussi une DDB.

     case WM_USER_CREATEPAL:
          if (hdib)
          {
               hdc = GetDC (hwnd) ;

               if (!(RC_PALETTE & GetDeviceCaps (hdc, RASTERCAPS)))
               {
                    PaletteMenu (hMenu, IDM_PAL_NONE) ;
               }
               else if (hPalette = DibPalDibTable (hdib))
               {
                    PaletteMenu (hMenu, IDM_PAL_DIBTABLE) ;
               }
               else if (hPalette = CreateHalftonePalette (hdc))
               {
                    fHalftonePalette = TRUE ;
                    PaletteMenu (hMenu, IDM_PAL_HALFTONE) ;
               }
               ReleaseDC (hwnd, hdc) ;

               if ((BOOL) wParam)
                    hBitmap = DibCopyToDdb (hdib, hwnd, hPalette) ;
          }
          return 0 ;

     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;

          if (hPalette)
          {
               SelectPalette (hdc, hPalette, FALSE) ;
               RealizePalette (hdc) ;
          }
          if (hBitmap)
          {
               DisplayDib (hdc, 
                           fHalftonePalette ? DibBitmapHandle (hdib) : hBitmap, 
                           iHscroll, iVscroll, 
                           cxClient, cyClient,
                           wShow, fHalftonePalette) ;
          }
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
          if (hdib)
               DibDelete (hdib) ;

          if (hBitmap)
               DeleteObject (hBitmap) ;

          if (hPalette)
               DeleteObject (hPalette) ;

          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

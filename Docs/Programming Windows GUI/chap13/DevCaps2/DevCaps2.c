/*------------------------------------------------------------------
   DEVCAPS2.C - Affiche les informations sur les possibilités d'un périphérique (Version 2)
                 (c) Charles Petzold, 1998
------------------------------------------------------------------*/

#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
void DoBasicInfo    (HDC, HDC, int, int) ;
void DoOtherInfo    (HDC, HDC, int, int) ;
void DoBitCodedCaps (HDC, HDC, int, int, int) ;

typedef struct
{
     int     iMask ;
     TCHAR * szDesc ;
}
BITS ;

#define IDM_DEVMODE      1000

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("DevCaps2") ;
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
     wndclass.lpszMenuName  = szAppName ;
     wndclass.lpszClassName = szAppName ;
     
     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Ce programme fonctionne exclusivement sous Windows NT!"),
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName, NULL,
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
     static TCHAR            szDevice[32], szWindowText[64] ;
     static int              cxChar, cyChar, nCurrentDevice = IDM_SCREEN,
                                             nCurrentInfo   = IDM_BASIC ;
     static DWORD            dwNeeded, dwReturned ;
     static PRINTER_INFO_4 * pinfo4 ;
     static PRINTER_INFO_5 * pinfo5 ;
     DWORD                   i ;
     HDC                     hdc, hdcInfo ;
     HMENU                   hMenu ;
     HANDLE                  hPrint ;
     PAINTSTRUCT             ps ;
     TEXTMETRIC              tm ;
     
     switch (message)
     {
     case WM_CREATE :
          hdc = GetDC (hwnd) ;
          SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;
          GetTextMetrics (hdc, &tm) ;
          cxChar = tm.tmAveCharWidth ;
          cyChar = tm.tmHeight + tm.tmExternalLeading ;
          ReleaseDC (hwnd, hdc) ;
                                           // échoue
     case WM_SETTINGCHANGE:
          hMenu = GetSubMenu (GetMenu (hwnd), 0) ;
          
          while (GetMenuItemCount (hMenu) > 1)
               DeleteMenu (hMenu, 1, MF_BYPOSITION) ;

               // Demande liste de toutes les imprimantes locales et distantes
               // 
               // Commençons par déterminer la taille du tableau requis ; cet
               //   appel échouera, mais nous donnera la taille requise dans dwNeeded
               //
               // Allouons ensuite la mémoire nécessaire au tableau et
               //   copions ses données
               // Placer les noms des imprimantes dans le menu

          if (GetVersion () & 0x80000000)         // Windows 98
          {
               EnumPrinters (PRINTER_ENUM_LOCAL, NULL, 5, NULL, 
                             0, &dwNeeded, &dwReturned) ;

               pinfo5 = malloc (dwNeeded) ;

               EnumPrinters (PRINTER_ENUM_LOCAL, NULL, 5, (PBYTE) pinfo5,
                             dwNeeded, &dwNeeded, &dwReturned) ;

               for (i = 0 ; i < dwReturned ; i++)
               {
                    AppendMenu (hMenu, (i+1) % 16 ? 0 : MF_MENUBARBREAK, i + 1, 
                                pinfo5[i].pPrinterName) ;
               }
               free (pinfo5) ;
          }
          else                                    // Windows NT
          {
               EnumPrinters (PRINTER_ENUM_LOCAL, NULL, 4, NULL, 
                             0, &dwNeeded, &dwReturned) ;

               pinfo4 = malloc (dwNeeded) ;

               EnumPrinters (PRINTER_ENUM_LOCAL, NULL, 4, (PBYTE) pinfo4,
                             dwNeeded, &dwNeeded, &dwReturned) ;

               for (i = 0 ; i < dwReturned ; i++)
               {
                    AppendMenu (hMenu, (i+1) % 16 ? 0 : MF_MENUBARBREAK, i + 1, 
                                pinfo4[i].pPrinterName) ;
               }
               free (pinfo4) ;
          }
          
          AppendMenu (hMenu, MF_SEPARATOR, 0, NULL) ;
          AppendMenu (hMenu, 0, IDM_DEVMODE, TEXT ("Propriétés")) ;
          
          wParam = IDM_SCREEN ;
                                             // échoue
     case WM_COMMAND :
          hMenu = GetMenu (hwnd) ;
          
          if (LOWORD (wParam) == IDM_SCREEN ||         // IDM_SCREEN & imprimantes
              LOWORD (wParam) < IDM_DEVMODE)       
          {
               CheckMenuItem (hMenu, nCurrentDevice, MF_UNCHECKED) ;
               nCurrentDevice = LOWORD (wParam) ;
               CheckMenuItem (hMenu, nCurrentDevice, MF_CHECKED) ;
          }
          else if (LOWORD (wParam) == IDM_DEVMODE)     // Sélection des propriétés
          {
               GetMenuString (hMenu, nCurrentDevice, szDevice,
                              sizeof (szDevice) / sizeof (TCHAR), MF_BYCOMMAND);
               
               if (OpenPrinter (szDevice, &hPrint, NULL))
               {
                    PrinterProperties (hwnd, hPrint) ;
                    ClosePrinter (hPrint) ;
               }
          }
          else                               // éléments du menu info
          {
               CheckMenuItem (hMenu, nCurrentInfo, MF_UNCHECKED) ;
               nCurrentInfo = LOWORD (wParam) ;
               CheckMenuItem (hMenu, nCurrentInfo, MF_CHECKED) ;
          }
          InvalidateRect (hwnd, NULL, TRUE) ;
          return 0 ;
          
     case WM_INITMENUPOPUP :
          if (lParam == 0)
               EnableMenuItem (GetMenu (hwnd), IDM_DEVMODE,
                    nCurrentDevice == IDM_SCREEN ? MF_GRAYED : MF_ENABLED) ;
          return 0 ;
          
     case WM_PAINT :
          lstrcpy (szWindowText, TEXT ("Possibilités du périphérique : ")) ;
          
          if (nCurrentDevice == IDM_SCREEN)
          {
               lstrcpy (szDevice, TEXT ("ÉCRAN")) ;
               hdcInfo = CreateIC (szDevice, NULL, NULL, NULL) ;
          }
          else
          {
               hMenu = GetMenu (hwnd) ;
               GetMenuString (hMenu, nCurrentDevice, szDevice,
                              sizeof (szDevice), MF_BYCOMMAND) ;
               hdcInfo = CreateIC (NULL, szDevice, NULL, NULL) ;
          }
          
          lstrcat (szWindowText, szDevice) ;
          SetWindowText (hwnd, szWindowText) ;
          
          hdc = BeginPaint (hwnd, &ps) ;
          SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;
          
          if (hdcInfo)
          {
               switch (nCurrentInfo)
               {
               case IDM_BASIC :
                    DoBasicInfo (hdc, hdcInfo, cxChar, cyChar) ;
                    break ;
                    
               case IDM_OTHER :
                    DoOtherInfo (hdc, hdcInfo, cxChar, cyChar) ;
                    break ;
                    
               case IDM_CURVE :
               case IDM_LINE :
               case IDM_POLY :
               case IDM_TEXT :
                    DoBitCodedCaps (hdc, hdcInfo, cxChar, cyChar,
                                    nCurrentInfo - IDM_CURVE) ;
                    break ;
               }
               DeleteDC (hdcInfo) ;
          }
          
          EndPaint (hwnd, &ps) ;
          return 0 ;
          
     case WM_DESTROY :
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}
     
void DoBasicInfo (HDC hdc, HDC hdcInfo, int cxChar, int cyChar)
{
     static struct
     {
          int     nIndex ;
          TCHAR * szDesc ;
     }
     info[] =
     {
          HORZSIZE,        TEXT ("HORZSIZE        Largeur en millimètres :"),
          VERTSIZE,        TEXT ("VERTSIZE        Hauteur en millimètres :"),
          HORZRES,         TEXT ("HORZRES         Largeur en pixels :"),
          VERTRES,         TEXT ("VERTRES         Hauteur en  lignes  tramées :"),
          BITSPIXEL,       TEXT ("BITSPIXEL       Bits couleur par pixel :"),
          PLANES,          TEXT ("PLANES          Nombre de plans de couleurs :"),
          NUMBRUSHES,      TEXT ("NUMBRUSHES      Nombre de pinceaux de périphérique :"),
          NUMPENS,         TEXT ("NUMPENS         Nombre de crayons de périphérique :"),
          NUMMARKERS,      TEXT ("NUMMARKERS      Nombre de marqueurs de périphériques :"),
          NUMFONTS,        TEXT ("NUMFONTS        Nombre de polices de périphérique :"),
          NUMCOLORS,       TEXT ("NUMCOLORS       Nombre de couleurs de périphérique :"),
          PDEVICESIZE,     TEXT ("PDEVICESIZE     Taille de la structure du périphérique :"),
          ASPECTX,         TEXT ("ASPECTX         Largeur relative d'un pixel :"),
          ASPECTY,         TEXT ("ASPECTY         Hauteur relative d'un pixel :"),
          ASPECTXY,        TEXT ("ASPECTXY        Diagonale relative d'un pixel :"),
          LOGPIXELSX,      TEXT ("LOGPIXELSX      Points horizontaux par pouce :"),
          LOGPIXELSY,      TEXT ("LOGPIXELSY      Points verticaux par pouce :"),
          SIZEPALETTE,     TEXT ("SIZEPALETTE     Nombre d'entrées de la palette :"),
          NUMRESERVED,     TEXT ("NUMRESERVED     Entrées réservées de la palette :"),
          COLORRES,        TEXT ("COLORRES        Résolution couleur actuelle :"),
          PHYSICALWIDTH,   TEXT ("PHYSICALWIDTH   Largeur des pixels de la page imprimante :"),
          PHYSICALHEIGHT,  TEXT ("PHYSICALHEIGHT  Hauteur des pixels de la page imprimante :"),
          PHYSICALOFFSETX, TEXT ("PHYSICALOFFSETX Décalage x de la page imprimante :"),
          PHYSICALOFFSETY, TEXT ("PHYSICALOFFSETY Décalage y de la page imprimante :") 
     } ;
     int   i ;
     TCHAR szBuffer[80] ;
          
     for (i = 0 ; i < sizeof (info) / sizeof (info[0]) ; i++)
          TextOut (hdc, cxChar, (i + 1) * cyChar, szBuffer,
               wsprintf (szBuffer, TEXT ("%-45s%8d"), info[i].szDesc,
                    GetDeviceCaps (hdcInfo, info[i].nIndex))) ;
}
    
void DoOtherInfo (HDC hdc, HDC hdcInfo, int cxChar, int cyChar)
{
     static BITS clip[] =
     {
          CP_RECTANGLE,    TEXT ("CP_RECTANGLE    Peut couper vers rectangle:")
     } ; 
          
     static BITS raster[] =
     {
          RC_BITBLT,       TEXT ("RC_BITBLT       Capable de BitBlt simple  :"),
          RC_BANDING,      TEXT ("RC_BANDING      Demande prise en charge bandes :"),
          RC_SCALING,      TEXT ("RC_SCALING      Demande prise en charge échelle :"),
          RC_BITMAP64,     TEXT ("RC_BITMAP64     Prise en charge bitmaps >64K :"),
          RC_GDI20_OUTPUT, TEXT ("RC_GDI20_OUTPUT Possède appels 2.0 de sortie :"),
          RC_DI_BITMAP,    TEXT ("RC_DI_BITMAP    Prise en charge DIB vers mémoire :"),
          RC_PALETTE,      TEXT ("RC_PALETTE      Prise en charge palette:"),
          RC_DIBTODEV,     TEXT ("RC_DIBTODEV     Prise en charge conversion bitmap :"),
          RC_BIGFONT,      TEXT ("RC_BIGFONT      Prise en charge  bitmap  >64K :"),
          RC_STRETCHBLT,   TEXT ("RC_STRETCHBLT   Prise en charge  StretchBlt :"),
          RC_FLOODFILL,    TEXT ("RC_FLOODFILL    Prise en charge  FloodFill :"),
          RC_STRETCHDIB,   TEXT ("RC_STRETCHDIB   Prise en charge  StretchDIBits :")
     } ;
          
     static TCHAR * szTech[] = { TEXT ("DT_PLOTTER (Table traçante vectorielle)"),
                                 TEXT ("DT_RASDISPLAY (Affichage tramé)"),
                                 TEXT ("DT_RASPRINTER (Imprimante matricielle)"),
                                 TEXT ("DT_RASCAMERA (Caméra matricielle)"),
                                 TEXT ("DT_CHARSTREAM (Train de caractères)"),
                                 TEXT ("DT_METAFILE (Fichier Metafile)"),
                                 TEXT ("DT_DISPFILE (Fichier Display)") } ;
     int            i ;
     TCHAR          szBuffer[80] ;
          
     TextOut (hdc, cxChar, cyChar, szBuffer,
          wsprintf (szBuffer, TEXT ("%-24s%04XH"), TEXT ("DRIVERVERSION:"),
               GetDeviceCaps (hdcInfo, DRIVERVERSION))) ;
          
     TextOut (hdc, cxChar, 2 * cyChar, szBuffer,
          wsprintf (szBuffer, TEXT ("%-24s%-40s"), TEXT ("TECHNOLOGY:"), 
               szTech[GetDeviceCaps (hdcInfo, TECHNOLOGY)])) ;
          
     TextOut (hdc, cxChar, 4 * cyChar, szBuffer,
          wsprintf (szBuffer, TEXT ("CLIPCAPS (Possibilités clipping)"))) ;
          
     for (i = 0 ; i < sizeof (clip) / sizeof (clip[0]) ; i++)
          TextOut (hdc, 9 * cxChar, (i + 6) * cyChar, szBuffer,
               wsprintf (szBuffer, TEXT ("%-45s %3s"), clip[i].szDesc,
                    GetDeviceCaps (hdcInfo, CLIPCAPS) & clip[i].iMask ?
                         TEXT ("Oui") : TEXT ("Non"))) ;
          
     TextOut (hdc, cxChar, 8 * cyChar, szBuffer,
          wsprintf (szBuffer, TEXT ("RASTERCAPS (Possibilités tramage)"))) ;
      
     for (i = 0 ; i < sizeof (raster) / sizeof (raster[0]) ; i++)
          TextOut (hdc, 9 * cxChar, (i + 10) * cyChar, szBuffer,
               wsprintf (szBuffer, TEXT ("%-45s %3s"), raster[i].szDesc,
                    GetDeviceCaps (hdcInfo, RASTERCAPS) & raster[i].iMask ?
                         TEXT ("Oui") : TEXT ("Non"))) ;
}

void DoBitCodedCaps (HDC hdc, HDC hdcInfo, int cxChar, int cyChar, int iType)
{
     static BITS curves[] =
     {
          CC_CIRCLES,    TEXT ("CC_CIRCLES    Peut tracer cercles:"),
          CC_PIE,        TEXT ("CC_PIE        Peut tracer graphiques circulaires :"),
          CC_CHORD,      TEXT ("CC_CHORD      Peut tracer cordes d'arcs :"),
          CC_ELLIPSES,   TEXT ("CC_ELLIPSES   Peut tracer ellipses :"),
          CC_WIDE,       TEXT ("CC_WIDE       Peut tracer bordures larges :"),
          CC_STYLED,     TEXT ("CC_STYLED     Peut tracer bordures avec styles :"),
          CC_WIDESTYLED, TEXT ("CC_WIDESTYLED Peut tracer bordures larges et avec style :"),
          CC_INTERIORS,  TEXT ("CC_INTERIORS  Peut tracer intérieurs:")
     } ; 
     
     static BITS lines[] =
     {
          LC_POLYLINE,   TEXT ("LC_POLYLINE   Peut tracer ligne brisée :"),
          LC_MARKER,     TEXT ("LC_MARKER     Peut tracer marqueurs :"),
          LC_POLYMARKER, TEXT ("LC_POLYMARKER Peut tracer polymarqueurs"),
          LC_WIDE,       TEXT ("LC_WIDE       Peut tracer lignes larges:"),
          LC_STYLED,     TEXT ("LC_STYLED     Peut tracer lignes avec style :"),
          LC_WIDESTYLED, TEXT ("LC_WIDESTYLED Peut tracer lignes larges et avec style :"),
          LC_INTERIORS,  TEXT ("LC_INTERIORS  Peut tracer intérieurs:")
     } ;
     
     static BITS poly[] =
     {
          PC_POLYGON,     
               TEXT ("PC_POLYGON     Peut tracer polygone rempli alterné :"),
          PC_RECTANGLE,   TEXT ("PC_RECTANGLE   Peut tracer rectangle:"),
          PC_WINDPOLYGON, 
               TEXT ("PC_WINDPOLYGON Peut tracer number fill polygon :"),
          PC_SCANLINE,    TEXT ("PC_SCANLINE    Peut tracer scanlines :"),
          PC_WIDE,        TEXT ("PC_WIDE        Peut tracer bordures larges :"),
          PC_STYLED,      TEXT ("PC_STYLED      Peut tracer bordures avec style :"),
          PC_WIDESTYLED,  
               TEXT ("PC_WIDESTYLED  Peut tracer bordures larges avec style :"),
          PC_INTERIORS,   TEXT ("PC_INTERIORS   Peut tracer intérieurs :")
     } ;
     
     static BITS text[] =
     {
          TC_OP_CHARACTER, 
               TEXT ("TC_OP_CHARACTER Peut tracer character output precision :"),
          TC_OP_STROKE,    
               TEXT ("TC_OP_STROKE    Peut tracer stroke output precision :"),
          TC_CP_STROKE,    
               TEXT ("TC_CP_STROKE    Peut tracer stroke clip precision:"),
          TC_CR_90,        
               TEXT ("TC_CP_90        Peut tracer rotation caractères 90 degrés :"),
          TC_CR_ANY,       
               TEXT ("TC_CR_ANY       Peut tracer toute rotation de caractère :"),
          TC_SF_X_YINDEP,  
               TEXT ("TC_SF_X_YINDEP  Peut tracer échelle indépendante de X et Y:"),
          TC_SA_DOUBLE,    
               TEXT ("TC_SA_DOUBLE    Peut tracer doubled character for scaling :"),
          TC_SA_INTEGER,   
               TEXT ("TC_SA_INTEGER   Peut tracer multiples entiers pour échelle :"),
          TC_SA_CONTIN,    
               TEXT ("TC_SA_CONTIN    Peut tracer tous multiples pour échelle exacte :"),
          TC_EA_DOUBLE,    
               TEXT ("TC_EA_DOUBLE    Peut tracer caractères double graisse :"),
          TC_IA_ABLE,      TEXT ("TC_IA_ABLE      Peut tracer italiques:"),
          TC_UA_ABLE,      TEXT ("TC_UA_ABLE      Peut tracer souligné :"),
          TC_SO_ABLE,      TEXT ("TC_SO_ABLE      Peut tracer barré :"),
          TC_RA_ABLE,      TEXT ("TC_RA_ABLE      Peut tracer polices matricielles :"),
          TC_VA_ABLE,      TEXT ("TC_VA_ABLE      Peut tracer polices vectorielles :")
     } ;
     
     static struct
     {
          int     iIndex ;
          TCHAR * szTitle ;
          BITS    (*pbits)[] ;
          int     iSize ;
     }
     bitinfo[] =
     {
          CURVECAPS,  TEXT ("CURVCAPS (Possibilités courbes)"),
               (BITS (*)[]) curves, sizeof (curves) / sizeof (curves[0]),
          LINECAPS,   TEXT ("LINECAPS (Possibilités lignes)"),
               (BITS (*)[]) lines, sizeof (lines) / sizeof (lines[0]),
          POLYGONALCAPS, TEXT ("POLYGONALCAPS (Possibilités polygones)"),
               (BITS (*)[]) poly, sizeof (poly) / sizeof (poly[0]),
          TEXTCAPS,   TEXT ("TEXTCAPS (Possibilités texte)"),
               (BITS (*)[]) text, sizeof (text) / sizeof (text[0])
     } ;
     
     static TCHAR szBuffer[80] ;
     BITS         (*pbits)[] = bitinfo[iType].pbits ;
     int          i, iDevCaps = GetDeviceCaps (hdcInfo, bitinfo[iType].iIndex) ;
     
     TextOut (hdc, cxChar, cyChar, bitinfo[iType].szTitle,
              lstrlen (bitinfo[iType].szTitle)) ;
     
     for (i = 0 ; i < bitinfo[iType].iSize ; i++)
          TextOut (hdc, cxChar, (i + 3) * cyChar, szBuffer,
               wsprintf (szBuffer, TEXT ("%-55s %3s"), (*pbits)[i].szDesc,
                    iDevCaps & (*pbits)[i].iMask ? TEXT ("Oui") : TEXT ("Non")));
}

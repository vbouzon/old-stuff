/*------------------------------------------------
   UPDDEMO.C -- Démonstration d'accès FTP Anonyme
                (c) Charles Petzold, 1998
  ------------------------------------------------*/

#include <windows.h>
#include <wininet.h>
#include <process.h>
#include "resource.h"

     // Messages utilisateur utilisés dans WndProc

#define WM_USER_CHECKFILES (WM_USER + 1)
#define WM_USER_GETFILES   (WM_USER + 2)

     // Information pour le téléchargement FTP

#define FTPSERVER TEXT ("ftp.cpetzold.com")
#define DIRECTORY TEXT ("cpetzold.com/ProgWin/UpdDemo")
#define TEMPLATE  TEXT ("UD??????.TXT")

     // Structures utilisées pour stocker les noms de fichier et les contenus
typedef struct
{
     TCHAR * szFilename ;
     char  * szContents ;
}
FILEINFO ;

typedef struct
{
     int      iNum ;
     FILEINFO info[1] ;
}
FILELIST ;

     // Structure utilisée pour le second thread

typedef struct
{
     BOOL bContinue ;
     HWND hwnd ;
}
PARAMS ;

     // Déclarations de toutes les fonctions du programme

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
BOOL    CALLBACK DlgProc (HWND, UINT, WPARAM, LPARAM) ;
VOID             FtpThread (PVOID) ;
VOID             ButtonSwitch (HWND, HWND, TCHAR *) ;
FILELIST *       GetFileList (VOID) ;
int              Compare (const FILEINFO *, const FILEINFO *) ;

     // Un couple de variables globales

HINSTANCE hInst ;
TCHAR     szAppName[] = TEXT ("UpdDemo") ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;

     hInst = hInstance ;

     wndclass.style         = 0 ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = NULL ;
     wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Le programme nécessite Windows NT !"), 
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName, TEXT ("Démo de mise à jour avec FTP Anonyme"),
                          WS_OVERLAPPEDWINDOW | WS_VSCROLL,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;

     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;

          // Après l'affichage de la fenêtre, vérifie si le dernier fichier existe

     SendMessage (hwnd, WM_USER_CHECKFILES, 0, 0) ;

     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static FILELIST * plist ;
     static int        cxClient, cyClient, cxChar, cyChar ;
     HDC               hdc ;
     int               i ;
     PAINTSTRUCT       ps ;
     SCROLLINFO        si ;
     SYSTEMTIME        st ;
     TCHAR             szFilename [MAX_PATH] ;

     switch (message)
     {
     case WM_CREATE:
          cxChar = LOWORD (GetDialogBaseUnits ()) ;
          cyChar = HIWORD (GetDialogBaseUnits ()) ;
          return 0 ;

     case WM_SIZE:
          cxClient = LOWORD (lParam) ;
          cyClient = HIWORD (lParam) ;

          si.cbSize = sizeof (SCROLLINFO) ;
          si.fMask  = SIF_RANGE | SIF_PAGE ;
          si.nMin   = 0 ;
          si.nMax   = plist ? plist->iNum - 1 : 0 ;
          si.nPage  = cyClient / cyChar ;

          SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;
          return 0 ;

     case WM_VSCROLL:
          si.cbSize = sizeof (SCROLLINFO) ;
          si.fMask  = SIF_POS | SIF_RANGE | SIF_PAGE ;
          GetScrollInfo (hwnd, SB_VERT, &si) ;

          switch (LOWORD (wParam))
          {
          case SB_LINEDOWN:       si.nPos += 1 ;              break ;
          case SB_LINEUP:         si.nPos -= 1 ;              break ;
          case SB_PAGEDOWN:       si.nPos += si.nPage ;       break ;
          case SB_PAGEUP:         si.nPos -= si.nPage ;       break ;
          case SB_THUMBPOSITION:  si.nPos = HIWORD (wParam) ; break ;
          default:                return 0 ;
          }
          si.fMask = SIF_POS ;
          SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;
          InvalidateRect (hwnd, NULL, TRUE) ;
          return 0 ;

     case WM_USER_CHECKFILES:
               // récupère la date système et forme le nom du fichier avec l'année et le mois

          GetSystemTime (&st) ;
          wsprintf (szFilename, TEXT ("UD%04i%02i.TXT"), st.wYear, st.wMonth) ;

               // Vérifie si le fichier existe; si oui, lecture de tous les fichiers

          if (GetFileAttributes (szFilename) != (DWORD) -1)
          {
               SendMessage (hwnd, WM_USER_GETFILES, 0, 0) ;
               return 0 ;
          }
               // Autrement, récupérer les fichiers sur Internet.
               // Mais vérifions que nous n'allons pas copier les fichiers sur un CD-ROM !

          if (GetDriveType (NULL) == DRIVE_CDROM)
          {
               MessageBox (hwnd, TEXT ("Impossible d'exécuter le programme à partir du CD-ROM !"),
                                 szAppName, MB_OK | MB_ICONEXCLAMATION) ;
               return 0 ;
          }
               // Demander à l'utilisateur s'il souhaite établir une connexion Internet

          if (IDYES == MessageBox (hwnd, 
                                  TEXT ("Mise à jour à partir d'Internet ?"),
                                  szAppName, MB_YESNO | MB_ICONQUESTION))

               // Appelle la boîte de dialogue 

          DialogBox (hInst, szAppName, hwnd, DlgProc) ;

               // Met à jour l'affichage

          SendMessage (hwnd, WM_USER_GETFILES, 0, 0) ;
          return 0 ;

     case WM_USER_GETFILES:
          SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
          ShowCursor (TRUE) ;

               // Lit dans tous les fichiers du disque

          plist = GetFileList () ;

          ShowCursor (FALSE) ;
          SetCursor (LoadCursor (NULL, IDC_ARROW)) ;

               // Simule un message WM_SIZE pour altérer la barre de défilement et repeindre

          SendMessage (hwnd, WM_SIZE, 0, MAKELONG (cxClient, cyClient)) ;
          InvalidateRect (hwnd, NULL, TRUE) ;
          return 0 ;

     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;
          SetTextAlign (hdc, TA_UPDATECP) ;

          si.cbSize = sizeof (SCROLLINFO) ;
          si.fMask  = SIF_POS ;
          GetScrollInfo (hwnd, SB_VERT, &si) ;

          if (plist)
          {
               for (i = 0 ; i < plist->iNum ; i++)
               {
                    MoveToEx (hdc, cxChar, (i - si.nPos) * cyChar, NULL) ;
                    TextOut  (hdc, 0, 0, plist->info[i].szFilename,                                          
                                lstrlen (plist->info[i].szFilename)) ;
                    TextOut  (hdc, 0, 0, TEXT (": "), 2) ;
                    TextOutA (hdc, 0, 0, plist->info[i].szContents, 
                                 strlen (plist->info[i].szContents)) ;
               }
          }
          EndPaint (hwnd, &ps) ;
          return 0 ;

     case WM_DESTROY:
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

BOOL CALLBACK DlgProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static PARAMS params ;

     switch (message)
     {
     case WM_INITDIALOG:
          params.bContinue = TRUE ;
          params.hwnd = hwnd ;

          _beginthread (FtpThread, 0, &params) ;
          return TRUE ;

     case WM_COMMAND:
          switch (LOWORD (wParam))
          {
          case IDCANCEL:           // Bouton pour annuler le téléchargement
               params.bContinue = FALSE ;
               return TRUE ;

          case IDOK:               // Bouton pour fermer la boîte de dialogue
               EndDialog (hwnd, 0) ;
               return TRUE ;
          }
     }
     return FALSE ;
}

/*----------------------------------------------------------------------
   FtpThread: Lecture des fichiers du serveur FTP et copie sur le disque local
  ----------------------------------------------------------------------*/

void FtpThread (PVOID parg)
{
     BOOL            bSuccess ;
     HINTERNET       hIntSession, hFtpSession, hFind ;
     HWND            hwndStatus, hwndButton ;
     PARAMS        * pparams ;
     TCHAR           szBuffer [64] ;
     WIN32_FIND_DATA finddata ;

     pparams = parg ;
     hwndStatus = GetDlgItem (pparams->hwnd, IDC_STATUS) ;
     hwndButton = GetDlgItem (pparams->hwnd, IDCANCEL) ;

          // Ouvre une session Internet
     
     hIntSession = InternetOpen (szAppName, INTERNET_OPEN_TYPE_PRECONFIG,
                                 NULL, NULL, INTERNET_FLAG_ASYNC) ;

     if (hIntSession == NULL)
     {
          wsprintf (szBuffer, TEXT ("InternetOpen error %i"), GetLastError ()) ;
          ButtonSwitch (hwndStatus, hwndButton, szBuffer) ;
          _endthread () ;
     }

     SetWindowText (hwndStatus, TEXT ("Internet session opened...")) ;

          // Vérifie si l'utilisateur a appuyé sur Annuler

     if (!pparams->bContinue)
     {
          InternetCloseHandle (hIntSession) ;
          ButtonSwitch (hwndStatus, hwndButton, NULL) ;
          _endthread () ;
     }

          // Ouvre une session FTP

     hFtpSession = InternetConnect (hIntSession, FTPSERVER,
                                    INTERNET_DEFAULT_FTP_PORT,
                                    NULL, NULL, INTERNET_SERVICE_FTP, 0, 0) ;
     if (hFtpSession == NULL)
     {
          InternetCloseHandle (hIntSession) ;
          wsprintf (szBuffer, TEXT ("InternetConnect error %i"), 
                              GetLastError ()) ;
          ButtonSwitch (hwndStatus, hwndButton, szBuffer) ;
          _endthread () ;
     }

     SetWindowText (hwndStatus, TEXT ("FTP Session opened...")) ;
     
          // Vérifie si l'utilisateur a appuyé sur Annuler

     if (!pparams->bContinue)
     {
          InternetCloseHandle (hFtpSession) ;
          InternetCloseHandle (hIntSession) ;
          ButtonSwitch (hwndStatus, hwndButton, NULL) ;
          _endthread () ;
     }

          // Définit le dossier
     
     bSuccess = FtpSetCurrentDirectory (hFtpSession, DIRECTORY) ;

     if (!bSuccess)
     {
          InternetCloseHandle (hFtpSession) ;
          InternetCloseHandle (hIntSession) ;
          wsprintf (szBuffer, TEXT ("Cannot set directory to %s"),
                              DIRECTORY) ;
          ButtonSwitch (hwndStatus, hwndButton, szBuffer) ;
          _endthread () ;
     }

     SetWindowText (hwndStatus, TEXT ("Directory found...")) ;

          // Vérifie si l'utilisateur a appuyé sur Annuler

     if (!pparams->bContinue)
     {
          InternetCloseHandle (hFtpSession) ;
          InternetCloseHandle (hIntSession) ;
          ButtonSwitch (hwndStatus, hwndButton, NULL) ;
          _endthread () ;
     }

          // Récupère le premier fichier correspondant au modèle

     hFind = FtpFindFirstFile (hFtpSession, TEMPLATE, 
                               &finddata, 0, 0) ;

     if (hFind == NULL)
     {
          InternetCloseHandle (hFtpSession) ;
          InternetCloseHandle (hIntSession) ;
          ButtonSwitch (hwndStatus, hwndButton, TEXT ("Cannot find files")) ;
          _endthread () ;
     }

     do 
     {
               // Vérifie si l'utilisateur a appuyé sur Annuler

          if (!pparams->bContinue)
          {
               InternetCloseHandle (hFind) ;
               InternetCloseHandle (hFtpSession) ;
               InternetCloseHandle (hIntSession) ;
               ButtonSwitch (hwndStatus, hwndButton, NULL) ;
               _endthread () ;
          }
               // Copie le fichier Internet sur le disque local, 
               // mais échec si le fichier exoiste déjà en local.

          wsprintf (szBuffer, TEXT ("Reading file %s..."), finddata.cFileName) ;
          SetWindowText (hwndStatus, szBuffer) ;

          FtpGetFile (hFtpSession, 
                      finddata.cFileName, finddata.cFileName, TRUE, 
                      FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY, 0) ;
     }
     while (InternetFindNextFile (hFind, &finddata)) ;

     InternetCloseHandle (hFind) ;
     InternetCloseHandle (hFtpSession) ;
     InternetCloseHandle (hIntSession) ;

     ButtonSwitch (hwndStatus, hwndButton, TEXT ("Internet Download Complete"));
}

/*-----------------------------------------------------------------------
   ButtonSwitch:  Affiche le message final et passe Annuler en OK
  -----------------------------------------------------------------------*/

VOID ButtonSwitch (HWND hwndStatus, HWND hwndButton, TCHAR * szText) 
{
     if (szText)
          SetWindowText (hwndStatus, szText) ;
     else
          SetWindowText (hwndStatus, TEXT ("Internet Session Cancelled")) ;

     SetWindowText (hwndButton, TEXT ("OK")) ;
     SetWindowLong (hwndButton, GWL_ID, IDOK) ;
}

/*-----------------------------------------------------------------------
   GetFileList: Lit les fichiers sur le disque et sauvegarde leurs noms et contenus
  -----------------------------------------------------------------------*/

FILELIST * GetFileList (void)
{
     DWORD           dwRead ;
     FILELIST      * plist ;
     HANDLE          hFile, hFind ;
     int             iSize, iNum  ;
     WIN32_FIND_DATA finddata ;

     hFind = FindFirstFile (TEMPLATE, &finddata) ;

     if (hFind == INVALID_HANDLE_VALUE)
          return NULL ;
     
     plist = NULL ;
     iNum  = 0 ;

     do
     {
               // Ouvre le fichier et récupère sa taille

          hFile = CreateFile (finddata.cFileName, GENERIC_READ, FILE_SHARE_READ,
                              NULL, OPEN_EXISTING, 0, NULL) ;

          if (hFile == INVALID_HANDLE_VALUE)
               continue ;
     
          iSize = GetFileSize (hFile, NULL) ;

          if (iSize == (DWORD) -1)
          {
               CloseHandle (hFile) ;
               continue ;
          }
               // Réalloue la structure FILELIST pour une nouvelle entrée 

          plist = realloc (plist, sizeof (FILELIST) + iNum * sizeof (FILEINFO));

               // Alloue de l'espace et sauvegarde le nom du fichier 

          plist->info[iNum].szFilename = malloc (lstrlen (finddata.cFileName) +
                                                 sizeof (TCHAR)) ;
          lstrcpy (plist->info[iNum].szFilename, finddata.cFileName) ;

               // Alloue de l'espace et sauvegarde le contenu

          plist->info[iNum].szContents = malloc (iSize + 1) ;
          ReadFile (hFile, plist->info[iNum].szContents, iSize, &dwRead, NULL);
          plist->info[iNum].szContents[iSize] = 0 ;

          CloseHandle (hFile) ;
          iNum ++ ;
     }
     while (FindNextFile (hFind, &finddata)) ;

     FindClose (hFind) ;

          // Trie les fichiers par nom

     qsort (plist->info, iNum, sizeof (FILEINFO), Compare) ;

     plist->iNum = iNum ;

     return plist ;
}

/*----------------------------
   Compare la fonction pour qsort
  ----------------------------*/

int Compare (const FILEINFO * pinfo1, const FILEINFO * pinfo2)
{
     return lstrcmp (pinfo2->szFilename, pinfo1->szFilename) ;
}


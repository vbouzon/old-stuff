/*----------------------------------------
   TESTMCI.C -- Testeur de chaîne de commande MCI
                (c) Charles Petzold, 1998
  ----------------------------------------*/

#include <windows.h>
#include "resource.h"

#define ID_TIMER    1

BOOL CALLBACK DlgProc (HWND, UINT, WPARAM, LPARAM) ;

TCHAR szAppName [] = TEXT ("TestMci") ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     if (-1 == DialogBox (hInstance, szAppName, NULL, DlgProc))
     {
          MessageBox (NULL, TEXT ("Le programme nécessite Windows NT !"),
                      szAppName, MB_ICONERROR) ;
     }
     return 0 ;
}

BOOL CALLBACK DlgProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static HWND hwndEdit ;
     int         iCharBeg, iCharEnd, iLineBeg, iLineEnd, iChar, iLine, iLength ;
     MCIERROR    error ;
     RECT        rect ;
     TCHAR       szCommand [1024], szReturn [1024], 
                 szError [1024], szBuffer [32] ;

     switch (message)
     {
     case WM_INITDIALOG:
               // Centrer la fenêtre à l'écran

          GetWindowRect (hwnd, &rect) ;
          SetWindowPos (hwnd, NULL, 
               (GetSystemMetrics (SM_CXSCREEN) - rect.right + rect.left) / 2,
               (GetSystemMetrics (SM_CYSCREEN) - rect.bottom + rect.top) / 2,
               0, 0, SWP_NOZORDER | SWP_NOSIZE) ;

          hwndEdit = GetDlgItem (hwnd, IDC_MAIN_EDIT) ;
          SetFocus (hwndEdit) ;
          return FALSE ;

     case WM_COMMAND:
          switch (LOWORD (wParam))
          {
          case IDOK:
                    // Trouver le nombre de lignes en fonction de la sélection

               SendMessage (hwndEdit, EM_GETSEL, (WPARAM) &iCharBeg, 
                                                 (LPARAM) &iCharEnd) ;

               iLineBeg = SendMessage (hwndEdit, EM_LINEFROMCHAR, iCharBeg, 0) ;
               iLineEnd = SendMessage (hwndEdit, EM_LINEFROMCHAR, iCharEnd, 0) ;

                    // Boucle sur toutes les lignes               

               for (iLine = iLineBeg ; iLine <= iLineEnd ; iLine++)
               {
                         // Trouver la ligne et la terminer; ignorer si vide

                    * (WORD *) szCommand = sizeof (szCommand) / sizeof (TCHAR) ;

                    iLength = SendMessage (hwndEdit, EM_GETLINE, iLine, 
                                                     (LPARAM) szCommand) ;
                    szCommand [iLength] = '\0' ;

                    if (iLength == 0)
                         continue ;

                         // Envoyer la commande MCI

                    error = mciSendString (szCommand, szReturn, 
                              sizeof (szReturn) / sizeof (TCHAR), hwnd) ;

                         // Définir la champ de la chaîne retournée

                    SetDlgItemText (hwnd, IDC_RETURN_STRING, szReturn) ;

                         // Définir le champ de la chaîne d'erreur (même s'il n'y a pas d'erreur)

                    mciGetErrorString (error, szError, 
                                       sizeof (szError) / sizeof (TCHAR)) ;

                    SetDlgItemText (hwnd, IDC_ERROR_STRING, szError) ;
               }
                    // Envoyer le caret à la fin de la dernière ligne sélectionnée

               iChar  = SendMessage (hwndEdit, EM_LINEINDEX,  iLineEnd, 0) ;
               iChar += SendMessage (hwndEdit, EM_LINELENGTH, iCharEnd, 0) ;
               SendMessage (hwndEdit, EM_SETSEL, iChar, iChar) ;
               
                    // Insérer une combinaison de retour chariot et de fin de ligne

               SendMessage (hwndEdit, EM_REPLACESEL, FALSE, 
                                      (LPARAM) TEXT ("\r\n")) ;
               SetFocus (hwndEdit) ;
               return TRUE ;

          case IDCANCEL:
               EndDialog (hwnd, 0) ;
               return TRUE ;

          case IDC_MAIN_EDIT:
               if (HIWORD (wParam) == EN_ERRSPACE)
               {
                    MessageBox (hwnd, TEXT ("Contrôle d'erreur en dehors de l'espace."),
                                szAppName, MB_OK | MB_ICONINFORMATION) ;
                    return TRUE ;
               }
               break ;
          }
          break ;

     case MM_MCINOTIFY:
          EnableWindow (GetDlgItem (hwnd, IDC_NOTIFY_MESSAGE), TRUE) ;

          wsprintf (szBuffer, TEXT ("ID du périphérique = %i"), lParam) ;
          SetDlgItemText (hwnd, IDC_NOTIFY_ID, szBuffer) ;
          EnableWindow (GetDlgItem (hwnd, IDC_NOTIFY_ID), TRUE) ;

          EnableWindow (GetDlgItem (hwnd, IDC_NOTIFY_SUCCESSFUL),
                              wParam & MCI_NOTIFY_SUCCESSFUL) ;
          
          EnableWindow (GetDlgItem (hwnd, IDC_NOTIFY_SUPERSEDED),
                              wParam & MCI_NOTIFY_SUPERSEDED) ;

          EnableWindow (GetDlgItem (hwnd, IDC_NOTIFY_ABORTED),
                              wParam & MCI_NOTIFY_ABORTED) ;

          EnableWindow (GetDlgItem (hwnd, IDC_NOTIFY_FAILURE),
                              wParam & MCI_NOTIFY_FAILURE) ;

          SetTimer (hwnd, ID_TIMER, 5000, NULL) ;
          return TRUE ;

     case WM_TIMER:
          KillTimer (hwnd, ID_TIMER) ;

          EnableWindow (GetDlgItem (hwnd, IDC_NOTIFY_MESSAGE), FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_NOTIFY_ID), FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_NOTIFY_SUCCESSFUL), FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_NOTIFY_SUPERSEDED), FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_NOTIFY_ABORTED), FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_NOTIFY_FAILURE), FALSE) ;
          return TRUE ;

     case WM_SYSCOMMAND:
          switch (LOWORD (wParam))
          {
          case SC_CLOSE:
               EndDialog (hwnd, 0) ;
               return TRUE ;
          }
          break ;
     }
     return FALSE ;
}

/*----------------------------------------
   RECORD3.C -- Enregistreur de sons
                (c) Charles Petzold, 1998
  ----------------------------------------*/

#include <windows.h>
#include "resource.h"

BOOL CALLBACK DlgProc (HWND, UINT, WPARAM, LPARAM) ;

TCHAR szAppName [] = TEXT ("Record3") ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     if (-1 == DialogBox (hInstance, TEXT ("Record"), NULL, DlgProc))
     {
          MessageBox (NULL, TEXT ("Le programme nécessite Windows NT !"),
                      szAppName, MB_ICONERROR) ;
     }
     return 0 ;
}

BOOL mciExecute (LPCTSTR szCommand)
{
     MCIERROR error ;
     TCHAR    szErrorStr [1024] ;

     if (error = mciSendString (szCommand, NULL, 0, NULL))
     {
          mciGetErrorString (error, szErrorStr, 
                             sizeof (szErrorStr) / sizeof (TCHAR)) ;
          MessageBeep (MB_ICONEXCLAMATION) ;
          MessageBox (NULL, szErrorStr, TEXT ("Erreur MCI"), 
                      MB_OK | MB_ICONEXCLAMATION) ;
     }
     return error == 0 ;
}

BOOL CALLBACK DlgProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static BOOL bRecording, bPlaying, bPaused ;
     
     switch (message)
     {
     case WM_COMMAND:
          switch (wParam)
          {
          case IDC_RECORD_BEG:
                    // Supprime le fichier son existant
               
               DeleteFile (TEXT ("record3.wav")) ;
               
                    // Ouverture audio et enregistrement
               
               if (!mciExecute (TEXT ("open new type waveaudio alias mysound")))
                    return TRUE ;
               
               mciExecute (TEXT ("record mysound")) ;
               
                    // Boutons d'activation et de désactivation
               
               EnableWindow (GetDlgItem (hwnd, IDC_RECORD_BEG), FALSE);
               EnableWindow (GetDlgItem (hwnd, IDC_RECORD_END), TRUE) ;
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_BEG),   FALSE);
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_PAUSE), FALSE);
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_END),   FALSE);
               SetFocus (GetDlgItem (hwnd, IDC_RECORD_END)) ;
               
               bRecording = TRUE ;
               return TRUE ;
               
          case IDC_RECORD_END:
                    // Arrêt, sauvegarde et fermeture de l'enregistrement
               
               mciExecute (TEXT ("stop mysound")) ;
               mciExecute (TEXT ("save mysound record3.wav")) ;
               mciExecute (TEXT ("close mysound")) ;
               
                    // Boutons d'activation et de désactivation
               
               EnableWindow (GetDlgItem (hwnd, IDC_RECORD_BEG), TRUE) ;
               EnableWindow (GetDlgItem (hwnd, IDC_RECORD_END), FALSE);
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_BEG),   TRUE) ;
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_PAUSE), FALSE);
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_END),   FALSE);
               SetFocus (GetDlgItem (hwnd, IDC_PLAY_BEG)) ;
               
               bRecording = FALSE ;
               return TRUE ;
               
          case IDC_PLAY_BEG:
                    // Ouverture audio et reproduction
               
               if (!mciExecute (TEXT ("open record3.wav alias mysound")))
                    return TRUE ;
               
               mciExecute (TEXT ("play mysound")) ;
               
                    // Boutons d'activation et de désactivation
               
               EnableWindow (GetDlgItem (hwnd, IDC_RECORD_BEG), FALSE);
               EnableWindow (GetDlgItem (hwnd, IDC_RECORD_END), FALSE);
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_BEG),   FALSE);
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_PAUSE), TRUE) ;
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_END),   TRUE) ;
               SetFocus (GetDlgItem (hwnd, IDC_PLAY_END)) ;
               
               bPlaying = TRUE ;
               return TRUE ;
               
          case IDC_PLAY_PAUSE:
               if (!bPaused)
                         // Pause dans la reproduction
               {
                    mciExecute (TEXT ("pause mysound")) ;
                    SetDlgItemText (hwnd, IDC_PLAY_PAUSE, TEXT ("Reprendre")) ;
                    bPaused = TRUE ;
               }
               else
                         // Reprise de la reproduction
               {
                    mciExecute (TEXT ("play mysound")) ;
                    SetDlgItemText (hwnd, IDC_PLAY_PAUSE, TEXT ("Pause")) ;
                    bPaused = FALSE ;
               }
               
               return TRUE ;
               
          case IDC_PLAY_END:
                    // Stop and close
               
               mciExecute (TEXT ("stop mysound")) ;
               mciExecute (TEXT ("close mysound")) ;
               
                    // Boutons d'activation et de désactivation
               
               EnableWindow (GetDlgItem (hwnd, IDC_RECORD_BEG), TRUE) ;
               EnableWindow (GetDlgItem (hwnd, IDC_RECORD_END), FALSE);
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_BEG),   TRUE) ;
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_PAUSE), FALSE);
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_END),   FALSE);
               SetFocus (GetDlgItem (hwnd, IDC_PLAY_BEG)) ;
               
               bPlaying = FALSE ;
               bPaused  = FALSE ;
               return TRUE ;
          }
          break ;
     
     case WM_SYSCOMMAND:
          switch (wParam)
          {
          case SC_CLOSE:
               if (bRecording)
                    SendMessage (hwnd, WM_COMMAND, IDC_RECORD_END, 0L);
               
               if (bPlaying)
                    SendMessage (hwnd, WM_COMMAND, IDC_PLAY_END, 0L) ;
               
               EndDialog (hwnd, 0) ;
               return TRUE ;
          }
          break ;
     }
     return FALSE ;
}

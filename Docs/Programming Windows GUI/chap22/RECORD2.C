/*----------------------------------------
   RECORD2.C -- Engeristreur de sons
                (c) Charles Petzold, 1998
------------------------------------------*/

#include <windows.h>
#include "resource.h"

BOOL CALLBACK DlgProc (HWND, UINT, WPARAM, LPARAM) ;

TCHAR szAppName [] = TEXT ("Record2") ;

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

void ShowError (HWND hwnd, DWORD dwError)
{
     TCHAR szErrorStr [1024] ;
     
     mciGetErrorString (dwError, szErrorStr, 
                        sizeof (szErrorStr) / sizeof (TCHAR)) ;
     MessageBeep (MB_ICONEXCLAMATION) ;
     MessageBox (hwnd, szErrorStr, szAppName, MB_OK | MB_ICONEXCLAMATION) ;
}

BOOL CALLBACK DlgProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static BOOL       bRecording, bPlaying, bPaused ;
     static TCHAR      szFileName[] = TEXT ("record2.wav") ;
     static WORD       wDeviceID ;
     DWORD             dwError ;
     MCI_GENERIC_PARMS mciGeneric ;
     MCI_OPEN_PARMS    mciOpen ;
     MCI_PLAY_PARMS    mciPlay ;
     MCI_RECORD_PARMS  mciRecord ;
     MCI_SAVE_PARMS    mciSave ;
     
     switch (message)
     {
     case WM_COMMAND:
          switch (wParam)
          {
          case IDC_RECORD_BEG:
                    // supprime le fichier son existant
               
               DeleteFile (szFileName) ;
               
                    // Ouverture audio
               
               mciOpen.dwCallback       = 0 ;
               mciOpen.wDeviceID        = 0 ;
               mciOpen.lpstrDeviceType  = TEXT ("waveaudio") ;
               mciOpen.lpstrElementName = TEXT ("") ; 
               mciOpen.lpstrAlias       = NULL ;
               
               dwError = mciSendCommand (0, MCI_OPEN, 
                                   MCI_WAIT | MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
                                   (DWORD) (LPMCI_OPEN_PARMS) &mciOpen) ;
               if (dwError != 0)
               {
                    ShowError (hwnd, dwError) ;
                    return TRUE ;
               }
                    // enregistre l'ID du périphérique
               
               wDeviceID = mciOpen.wDeviceID ;
               
                    // Début de l'enregistrement 
               
               mciRecord.dwCallback = (DWORD) hwnd ;
               mciRecord.dwFrom     = 0 ;
               mciRecord.dwTo       = 0 ;
               
               mciSendCommand (wDeviceID, MCI_RECORD, MCI_NOTIFY,
                               (DWORD) (LPMCI_RECORD_PARMS) &mciRecord) ;
               
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
                    // Arrête l'enregisrement
               
               mciGeneric.dwCallback = 0 ;
               
               mciSendCommand (wDeviceID, MCI_STOP, MCI_WAIT,
                               (DWORD) (LPMCI_GENERIC_PARMS) &mciGeneric) ;
               
                    // Enregistre le fichier

               mciSave.dwCallback = 0 ;
               mciSave.lpfilename = szFileName ;
               
               mciSendCommand (wDeviceID, MCI_SAVE, MCI_WAIT | MCI_SAVE_FILE,
                               (DWORD) (LPMCI_SAVE_PARMS) &mciSave) ;
               
                    // Ferme le périphérique audio
               
               mciSendCommand (wDeviceID, MCI_CLOSE, MCI_WAIT,
                               (DWORD) (LPMCI_GENERIC_PARMS) &mciGeneric) ;
               
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
                    // Ouverture audio
               
               mciOpen.dwCallback       = 0 ;
               mciOpen.wDeviceID        = 0 ;
               mciOpen.lpstrDeviceType  = NULL ;
               mciOpen.lpstrElementName = szFileName ;
               mciOpen.lpstrAlias       = NULL ;
               
               dwError = mciSendCommand (0, MCI_OPEN,
                                         MCI_WAIT | MCI_OPEN_ELEMENT,
                                         (DWORD) (LPMCI_OPEN_PARMS) &mciOpen) ;
               
               if (dwError != 0)
               {
                    ShowError (hwnd, dwError) ;
                    return TRUE ;
               }
                    // Enregistre l'ID du périphérique
               
               wDeviceID = mciOpen.wDeviceID ;
               
                    // Début de la reproduction
               
               mciPlay.dwCallback = (DWORD) hwnd ;
               mciPlay.dwFrom     = 0 ;
               mciPlay.dwTo       = 0 ;
               
               mciSendCommand (wDeviceID, MCI_PLAY, MCI_NOTIFY,
                               (DWORD) (LPMCI_PLAY_PARMS) &mciPlay) ;
               
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
                    mciGeneric.dwCallback = 0 ;
                    
                    mciSendCommand (wDeviceID, MCI_PAUSE, MCI_WAIT,
                                    (DWORD) (LPMCI_GENERIC_PARMS) & mciGeneric);
                    
                    SetDlgItemText (hwnd, IDC_PLAY_PAUSE, TEXT ("Reprendre")) ;
                    bPaused = TRUE ;
               }
               else
                         // reprise de la reproduction
               {
                    mciPlay.dwCallback = (DWORD) hwnd ;
                    mciPlay.dwFrom     = 0 ;
                    mciPlay.dwTo       = 0 ;
                    
                    mciSendCommand (wDeviceID, MCI_PLAY, MCI_NOTIFY,
                                    (DWORD) (LPMCI_PLAY_PARMS) &mciPlay) ;
                    
                    SetDlgItemText (hwnd, IDC_PLAY_PAUSE, TEXT ("Pause")) ;
                    bPaused = FALSE ;
               }
               
               return TRUE ;
               
          case IDC_PLAY_END:
                    // Arrêt et fermeture
               
               mciGeneric.dwCallback = 0 ;
               
               mciSendCommand (wDeviceID, MCI_STOP, MCI_WAIT,
                               (DWORD) (LPMCI_GENERIC_PARMS) &mciGeneric) ;
               
               mciSendCommand (wDeviceID, MCI_CLOSE, MCI_WAIT,
                               (DWORD) (LPMCI_GENERIC_PARMS) &mciGeneric) ;
               
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
               
     case MM_MCINOTIFY:
          switch (wParam)
          {
          case MCI_NOTIFY_SUCCESSFUL:
               if (bPlaying)
                    SendMessage (hwnd, WM_COMMAND, IDC_PLAY_END, 0) ;
               
               if (bRecording)
                    SendMessage (hwnd, WM_COMMAND, IDC_RECORD_END, 0);
               
               return TRUE ;
          }
          break ;
     
     case WM_SYSCOMMAND:
          switch (wParam)
          {
          case SC_CLOSE:
               if (bRecording)
                    SendMessage (hwnd, WM_COMMAND, IDC_RECORD_END, 0L) ;
               
               if (bPlaying)
                    SendMessage (hwnd, WM_COMMAND, IDC_PLAY_END, 0L) ;
               
               EndDialog (hwnd, 0) ;
               return TRUE ;
          }
          break ;
     }
     return FALSE ;
}

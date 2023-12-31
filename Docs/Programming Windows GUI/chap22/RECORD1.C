/*----------------------------------------
   RECORD1.C -- Enregistreur de sons
                (c) Charles Petzold, 1998
  ----------------------------------------*/

#include <windows.h>
#include "resource.h"

#define INP_BUFFER_SIZE 16384

BOOL CALLBACK DlgProc (HWND, UINT, WPARAM, LPARAM) ;

TCHAR szAppName [] = TEXT ("Record1") ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     if (-1 == DialogBox (hInstance, TEXT ("Record"), NULL, DlgProc))
     {
          MessageBox (NULL, TEXT ("Le programme n�cessite Windows NT !"),
                      szAppName, MB_ICONERROR) ;
     }
     return 0 ;
}

void ReverseMemory (BYTE * pBuffer, int iLength)
{
     BYTE b ;
     int  i ;
     
     for (i = 0 ; i < iLength / 2 ; i++)
     {
          b = pBuffer [i] ;
          pBuffer [i] = pBuffer [iLength - i - 1] ;
          pBuffer [iLength - i - 1] = b ;
     }
}

BOOL CALLBACK DlgProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static BOOL         bRecording, bPlaying, bReverse, bPaused,
                         bEnding, bTerminating ;
     static DWORD        dwDataLength, dwRepetitions = 1 ;
     static HWAVEIN      hWaveIn ;
     static HWAVEOUT     hWaveOut ;
     static PBYTE        pBuffer1, pBuffer2, pSaveBuffer, pNewBuffer ;
     static PWAVEHDR     pWaveHdr1, pWaveHdr2 ;
     static TCHAR        szOpenError[] = TEXT ("Erreur en ouverture !");
     static TCHAR        szMemError [] = TEXT ("Erreur d'allocation m�moire !") ;
     static WAVEFORMATEX waveform ;
     
     switch (message)
     {
     case WM_INITDIALOG:
               // Allocation de m�moire pour l'en-t�te
          
          pWaveHdr1 = malloc (sizeof (WAVEHDR)) ;
          pWaveHdr2 = malloc (sizeof (WAVEHDR)) ;
          
               // Allocation de m�moire pour le tampon de sauvegarde
          
          pSaveBuffer = malloc (1) ;
          return TRUE ;
          
     case WM_COMMAND:
          switch (LOWORD (wParam))
          {
          case IDC_RECORD_BEG:
                    // Allocation de la m�moire tampon

               pBuffer1 = malloc (INP_BUFFER_SIZE) ;
               pBuffer2 = malloc (INP_BUFFER_SIZE) ;
               
               if (!pBuffer1 || !pBuffer2)
               {
                    if (pBuffer1) free (pBuffer1) ;
                    if (pBuffer2) free (pBuffer2) ;

                    MessageBeep (MB_ICONEXCLAMATION) ;
                    MessageBox (hwnd, szMemError, szAppName,
                                      MB_ICONEXCLAMATION | MB_OK) ;
                    return TRUE ;
               }
               
                    // Ouverture audio pour l'enregistrement
               
               waveform.wFormatTag      = WAVE_FORMAT_PCM ;
               waveform.nChannels       = 1 ;
               waveform.nSamplesPerSec  = 11025 ;
               waveform.nAvgBytesPerSec = 11025 ;
               waveform.nBlockAlign     = 1 ;
               waveform.wBitsPerSample  = 8 ;
               waveform.cbSize          = 0 ;
               
               if (waveInOpen (&hWaveIn, WAVE_MAPPER, &waveform, 
                               (DWORD) hwnd, 0, CALLBACK_WINDOW))
               {
                    free (pBuffer1) ;
                    free (pBuffer2) ;
                    MessageBeep (MB_ICONEXCLAMATION) ;
                    MessageBox (hwnd, szOpenError, szAppName,
                                      MB_ICONEXCLAMATION | MB_OK) ;
               }
                    // D�finit les en-t�tes et les pr�pare
               
               pWaveHdr1->lpData          = pBuffer1 ;
               pWaveHdr1->dwBufferLength  = INP_BUFFER_SIZE ;
               pWaveHdr1->dwBytesRecorded = 0 ;
               pWaveHdr1->dwUser          = 0 ;
               pWaveHdr1->dwFlags         = 0 ;
               pWaveHdr1->dwLoops         = 1 ;
               pWaveHdr1->lpNext          = NULL ;
               pWaveHdr1->reserved        = 0 ;

               waveInPrepareHeader (hWaveIn, pWaveHdr1, sizeof (WAVEHDR)) ;
          
               pWaveHdr2->lpData          = pBuffer2 ;
               pWaveHdr2->dwBufferLength  = INP_BUFFER_SIZE ;
               pWaveHdr2->dwBytesRecorded = 0 ;
               pWaveHdr2->dwUser          = 0 ;
               pWaveHdr2->dwFlags         = 0 ;
               pWaveHdr2->dwLoops         = 1 ;
               pWaveHdr2->lpNext          = NULL ;
               pWaveHdr2->reserved        = 0 ;
          
               waveInPrepareHeader (hWaveIn, pWaveHdr2, sizeof (WAVEHDR)) ;
               return TRUE ;
               
          case IDC_RECORD_END:
                    // R�initialise l'enregistrement pour retourner le dernier tampon
               
               bEnding = TRUE ;
               waveInReset (hWaveIn) ;
               return TRUE ;
               
          case IDC_PLAY_BEG:
                    // Ouverture audio pour la sortie
               
               waveform.wFormatTag      = WAVE_FORMAT_PCM ;
               waveform.nChannels       = 1 ;
               waveform.nSamplesPerSec  = 11025 ;
               waveform.nAvgBytesPerSec = 11025 ;
               waveform.nBlockAlign     = 1 ;
               waveform.wBitsPerSample  = 8 ;
               waveform.cbSize          = 0 ;
               
               if (waveOutOpen (&hWaveOut, WAVE_MAPPER, &waveform, 
                                (DWORD) hwnd, 0, CALLBACK_WINDOW))
               {
                    MessageBeep (MB_ICONEXCLAMATION) ;
                    MessageBox (hwnd, szOpenError, szAppName,
                         MB_ICONEXCLAMATION | MB_OK) ;
               }
               return TRUE ;
               
          case IDC_PLAY_PAUSE:
                    // Pause ou red�marrage
               
               if (!bPaused)
               {
                    waveOutPause (hWaveOut) ;
                    SetDlgItemText (hwnd, IDC_PLAY_PAUSE, TEXT ("Reprendre")) ;
                    bPaused = TRUE ;
               }
               else
               {
                    waveOutRestart (hWaveOut) ;
                    SetDlgItemText (hwnd, IDC_PLAY_PAUSE, TEXT ("Pause")) ;
                    bPaused = FALSE ;
               }
               return TRUE ;
               
          case IDC_PLAY_END:
                    // R�initialise la sortie pour pr�paration de la fermeture
               
               bEnding = TRUE ;
               waveOutReset (hWaveOut) ;
               return TRUE ;
               
          case IDC_PLAY_REV:
                    // Inverse le tampon enregistrer et jouer
               
               bReverse = TRUE ;
               ReverseMemory (pSaveBuffer, dwDataLength) ;
               
               SendMessage (hwnd, WM_COMMAND, IDC_PLAY_BEG, 0) ;
               return TRUE ;
               
          case IDC_PLAY_REP:
                    // D�finit des r�p�titions infinies
               
               dwRepetitions = -1 ;
               SendMessage (hwnd, WM_COMMAND, IDC_PLAY_BEG, 0) ;
               return TRUE ;
               
          case IDC_PLAY_SPEED:
                    // Ouverture audio pour sortie rapide
               
               waveform.wFormatTag      = WAVE_FORMAT_PCM ;
               waveform.nChannels       = 1 ;
               waveform.nSamplesPerSec  = 22050 ;
               waveform.nAvgBytesPerSec = 22050 ;
               waveform.nBlockAlign     = 1 ;
               waveform.wBitsPerSample  = 8 ;
               waveform.cbSize          = 0 ;
               
               if (waveOutOpen (&hWaveOut, 0, &waveform, (DWORD) hwnd, 0,
                                           CALLBACK_WINDOW))
               {
                    MessageBeep (MB_ICONEXCLAMATION) ;
                    MessageBox (hwnd, szOpenError, szAppName,
                                      MB_ICONEXCLAMATION | MB_OK) ;
               }
               return TRUE ;
          }
          break ;
               
     case MM_WIM_OPEN:
               // Vide le tampon d'enregistrement
          
          pSaveBuffer = realloc (pSaveBuffer, 1) ;
          
               // Boutons d'activation et de d�sactivation
          
          EnableWindow (GetDlgItem (hwnd, IDC_RECORD_BEG), FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_RECORD_END), TRUE)  ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_BEG),   FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_PAUSE), FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_END),   FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_REV),   FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_REP),   FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_SPEED), FALSE) ;
          SetFocus (GetDlgItem (hwnd, IDC_RECORD_END)) ;

               // Ajouter les tampons
          
          waveInAddBuffer (hWaveIn, pWaveHdr1, sizeof (WAVEHDR)) ;
          waveInAddBuffer (hWaveIn, pWaveHdr2, sizeof (WAVEHDR)) ;
          
               // D�but d'�chantillon
          
          bRecording = TRUE ;
          bEnding = FALSE ;
          dwDataLength = 0 ;
          waveInStart (hWaveIn) ;
          return TRUE ;
          
     case MM_WIM_DATA:
         
               // R�alloue la m�moire du tampon d'enregistrement
          
          pNewBuffer = realloc (pSaveBuffer, dwDataLength +
                                   ((PWAVEHDR) lParam)->dwBytesRecorded) ;
          
          if (pNewBuffer == NULL)
          {
               waveInClose (hWaveIn) ;
               MessageBeep (MB_ICONEXCLAMATION) ;
               MessageBox (hwnd, szMemError, szAppName,
                                 MB_ICONEXCLAMATION | MB_OK) ;
               return TRUE ;
          }
          
          pSaveBuffer = pNewBuffer ;
          CopyMemory (pSaveBuffer + dwDataLength, ((PWAVEHDR) lParam)->lpData,
                         ((PWAVEHDR) lParam)->dwBytesRecorded) ;
          
          dwDataLength += ((PWAVEHDR) lParam)->dwBytesRecorded ;
          
          if (bEnding)
          {
               waveInClose (hWaveIn) ;
               return TRUE ;
          }
          
               // Envoie un nouveau tampon
          
          waveInAddBuffer (hWaveIn, (PWAVEHDR) lParam, sizeof (WAVEHDR)) ;
          return TRUE ;
          
     case MM_WIM_CLOSE:
               // Lib�re le tampon m�moire

          waveInUnprepareHeader (hWaveIn, pWaveHdr1, sizeof (WAVEHDR)) ;
          waveInUnprepareHeader (hWaveIn, pWaveHdr2, sizeof (WAVEHDR)) ;

          free (pBuffer1) ;
          free (pBuffer2) ;
          
               // Boutons d'activation et de d�sactivation
          
          EnableWindow (GetDlgItem (hwnd, IDC_RECORD_BEG), TRUE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_RECORD_END), FALSE) ;
          SetFocus (GetDlgItem (hwnd, IDC_RECORD_BEG)) ;
          
          if (dwDataLength > 0)
          {
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_BEG),   TRUE)  ;
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_PAUSE), FALSE) ;
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_END),   FALSE) ;
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_REP),   TRUE)  ;
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_REV),   TRUE)  ;
               EnableWindow (GetDlgItem (hwnd, IDC_PLAY_SPEED), TRUE)  ;
               SetFocus (GetDlgItem (hwnd, IDC_PLAY_BEG)) ;
          }
          bRecording = FALSE ;
          
          if (bTerminating)
               SendMessage (hwnd, WM_SYSCOMMAND, SC_CLOSE, 0L) ;
          
          return TRUE ;
          
     case MM_WOM_OPEN:
               // Boutons d'activation et de d�sactivation
          
          EnableWindow (GetDlgItem (hwnd, IDC_RECORD_BEG), FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_RECORD_END), FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_BEG),   FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_PAUSE), TRUE)  ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_END),   TRUE)  ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_REP),   FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_REV),   FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_SPEED), FALSE) ;
          SetFocus (GetDlgItem (hwnd, IDC_PLAY_END)) ;
          
               // D�finit l'en-t�te
          
          pWaveHdr1->lpData          = pSaveBuffer ;
          pWaveHdr1->dwBufferLength  = dwDataLength ;
          pWaveHdr1->dwBytesRecorded = 0 ;
          pWaveHdr1->dwUser          = 0 ;
          pWaveHdr1->dwFlags         = WHDR_BEGINLOOP | WHDR_ENDLOOP ;
          pWaveHdr1->dwLoops         = dwRepetitions ;
          pWaveHdr1->lpNext          = NULL ;
          pWaveHdr1->reserved        = 0 ;
          
               // Pr�pare et �crit
          
          waveOutPrepareHeader (hWaveOut, pWaveHdr1, sizeof (WAVEHDR)) ;
          waveOutWrite (hWaveOut, pWaveHdr1, sizeof (WAVEHDR)) ;
          
          bEnding = FALSE ;
          bPlaying = TRUE ;
          return TRUE ;
          
     case MM_WOM_DONE:
          waveOutUnprepareHeader (hWaveOut, pWaveHdr1, sizeof (WAVEHDR)) ;
          waveOutClose (hWaveOut) ;
          return TRUE ;
          
     case MM_WOM_CLOSE:
               // Boutons d'activation et de d�sactivation
          
          EnableWindow (GetDlgItem (hwnd, IDC_RECORD_BEG), TRUE)  ;
          EnableWindow (GetDlgItem (hwnd, IDC_RECORD_END), TRUE)  ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_BEG),   TRUE)  ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_PAUSE), FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_END),   FALSE) ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_REV),   TRUE)  ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_REP),   TRUE)  ;
          EnableWindow (GetDlgItem (hwnd, IDC_PLAY_SPEED), TRUE)  ;
          SetFocus (GetDlgItem (hwnd, IDC_PLAY_BEG)) ;
          
          SetDlgItemText (hwnd, IDC_PLAY_PAUSE, TEXT ("Pause")) ;
          bPaused = FALSE ;
          dwRepetitions = 1 ;
          bPlaying = FALSE ;
          
          if (bReverse)
          {
               ReverseMemory (pSaveBuffer, dwDataLength) ;
               bReverse = FALSE ;
          }
          
          if (bTerminating)
               SendMessage (hwnd, WM_SYSCOMMAND, SC_CLOSE, 0L) ;
          
          return TRUE ;
          
     case WM_SYSCOMMAND:
          switch (LOWORD (wParam))
          {
          case SC_CLOSE:
               if (bRecording)
               {
                    bTerminating = TRUE ;
                    bEnding = TRUE ;
                    waveInReset (hWaveIn) ;
                    return TRUE ;
               }
               
               if (bPlaying)
               {
                    bTerminating = TRUE ;
                    bEnding = TRUE ;
                    waveOutReset (hWaveOut) ;
                    return TRUE ;
               }
               
               free (pWaveHdr1) ;
               free (pWaveHdr2) ;
               free (pSaveBuffer) ;
               EndDialog (hwnd, 0) ;
               return TRUE ;
          }
          break ;
     }
     return FALSE ;
}

/*-----------------------------------------
   DRUMFILE.C -- routines pour DRUM
                 (c) Charles Petzold, 1998
  -----------------------------------------*/

#include <windows.h>
#include "drumtime.h"

#define minmax(a,x,b) (min (max (x, a), b))

#define TIMER_RES   5

void CALLBACK DrumTimerFunc (UINT, UINT, DWORD, DWORD, DWORD) ;

BOOL     bSequenceGoing, bEndSequence ;
DRUM     drum ;
HMIDIOUT hMidiOut ;
HWND     hwndNotify ;
int      iIndex ;
UINT     uTimerRes, uTimerID ;

DWORD MidiOutMessage (HMIDIOUT hMidi, int iStatus, int iChannel,
                      int iData1, int iData2)
{
     DWORD dwMessage ;
     
     dwMessage = iStatus | iChannel | (iData1 << 8) | (iData2 << 16) ;
     
     return midiOutShortMsg (hMidi, dwMessage) ;
}

void DrumSetParams (PDRUM pdrum)
{
     CopyMemory (&drum, pdrum, sizeof (DRUM)) ;
}

BOOL DrumBeginSequence (HWND hwnd)
{
     TIMECAPS tc ;
     
     hwndNotify = hwnd ;           // Enregistre le handle fenêtre pour notification
     DrumEndSequence (TRUE) ;      // Arrête la séquence si elle est en cours
     
          // Ouvre le port de sortie MIDI Mapper
     
     if (midiOutOpen (&hMidiOut, MIDIMAPPER, 0, 0, 0))
          return FALSE ;
     
          // Envoie les messages pour les canaux 9 et 0
     
     MidiOutMessage (hMidiOut, 0xC0, 9, 0, 0) ;
     MidiOutMessage (hMidiOut, 0xC0, 0, 0, 0) ;
     
          // Commence la séquence en définissant un événement horloge
     
     timeGetDevCaps (&tc, sizeof (TIMECAPS)) ;
     uTimerRes = minmax (tc.wPeriodMin, TIMER_RES, tc.wPeriodMax) ;
     timeBeginPeriod (uTimerRes) ;
     
     uTimerID = timeSetEvent (max ((UINT) uTimerRes, (UINT) drum.iMsecPerBeat),
                              uTimerRes, DrumTimerFunc, 0, TIME_ONESHOT) ;
     
     if (uTimerID == 0)
     {
          timeEndPeriod (uTimerRes) ;
          midiOutClose (hMidiOut) ;
          return FALSE ;
     }
     
     iIndex = -1 ;
     bEndSequence = FALSE ;
     bSequenceGoing = TRUE ;
     
     return TRUE ;
}

void DrumEndSequence (BOOL bRightAway)
{
     if (bRightAway)
     {
          if (bSequenceGoing)
          {
                    // arrête l'horloge
               if (uTimerID)
                    timeKillEvent (uTimerID) ;
               timeEndPeriod (uTimerRes) ;

                    // annule toutes les notes
               MidiOutMessage (hMidiOut, 0xB0, 9, 123, 0) ;
               MidiOutMessage (hMidiOut, 0xB0, 0, 123, 0) ;
               
                    // ferme le port MIDI
               midiOutClose (hMidiOut) ;
               bSequenceGoing = FALSE ;
          }
     }
     else
          bEndSequence = TRUE ;
}

void CALLBACK DrumTimerFunc (UINT  uID, UINT uMsg, DWORD dwUser,
                             DWORD dw1, DWORD dw2)
{
     static DWORD dwSeqPercLast [NUM_PERC], dwSeqPianLast [NUM_PERC] ;
     int          i ;
     
         // Efface les notes des canaux 9 et 0
     
     if (iIndex != -1)
     {
          for (i = 0 ; i < NUM_PERC ; i++)
          {
               if (dwSeqPercLast[i] & 1 << iIndex)
                    MidiOutMessage (hMidiOut, 0x80, 9, i + 35, 0) ;
               
               if (dwSeqPianLast[i] & 1 << iIndex) 
                    MidiOutMessage (hMidiOut, 0x80, 0, i + 35, 0) ;
          }
     }
     
          // Incrémente l'index
     
     iIndex = (iIndex + 1) % drum.iNumBeats ;
     PostMessage (hwndNotify, WM_USER_NOTIFY, iIndex, timeGetTime ()) ;
     
          // Vérifie la fin de séquence
     
     if (bEndSequence && iIndex == 0)
     {
          PostMessage (hwndNotify, WM_USER_FINISHED, 0, 0L) ;
          return ;
     }
     
          // récupère les messages pour les canaux 9 et 0
     
     for (i = 0 ; i < NUM_PERC ; i++)
     {
          if (drum.dwSeqPerc[i] & 1 << iIndex)
               MidiOutMessage (hMidiOut, 0x90, 9, i + 35, drum.iVelocity) ;
          
          if (drum.dwSeqPian[i] & 1 << iIndex)
               MidiOutMessage (hMidiOut, 0x90, 0, i + 35, drum.iVelocity) ;
          
          dwSeqPercLast[i] = drum.dwSeqPerc[i] ;
          dwSeqPianLast[i] = drum.dwSeqPian[i] ;
     }
          // définit un nouvel événement horloge
     
     uTimerID = timeSetEvent (max ((int) uTimerRes, drum.iMsecPerBeat),
                              uTimerRes, DrumTimerFunc, 0, TIME_ONESHOT) ;
     
     if (uTimerID == 0)
     {
          PostMessage (hwndNotify, WM_USER_ERROR, 0, 0) ;
     }
}

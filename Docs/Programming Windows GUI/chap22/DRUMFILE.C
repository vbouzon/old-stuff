/*------------------------------------------
   DRUMFILE.C -- Routines E/S pour DRUM
                 (c) Charles Petzold, 1998
  ------------------------------------------*/

#include <windows.h>
#include <commdlg.h>
#include "drumtime.h"
#include "drumfile.h"

OPENFILENAME ofn = { sizeof (OPENFILENAME) } ;

TCHAR * szFilter[] = { TEXT ("Drum Files (*.DRM)"),  
                       TEXT ("*.drm"), TEXT ("") } ;

TCHAR szDrumID   [] = TEXT ("DRUM") ;
TCHAR szListID   [] = TEXT ("LIST") ;
TCHAR szInfoID   [] = TEXT ("INFO") ;
TCHAR szSoftID   [] = TEXT ("ISFT") ;
TCHAR szDateID   [] = TEXT ("ISCD") ;
TCHAR szFmtID    [] = TEXT ("fmt ") ;
TCHAR szDataID   [] = TEXT ("data") ;
char  szSoftware [] = "DRUM par Charles Petzold, Programmer sous Windows" ;

TCHAR szErrorNoCreate    [] = TEXT ("Le fichier %s n'a pas pu être ouvert en écriture.");
TCHAR szErrorCannotWrite [] = TEXT ("Le fichier %s n'a pas pu être modifié.") ;
TCHAR szErrorNotFound    [] = TEXT ("Le fichier %s n'a pas pu être trouvé ou ouvert.") ;
TCHAR szErrorNotDrum     [] = TEXT ("Le fichier %s n'est pas un fichier DRUM standard.") ;
TCHAR szErrorUnsupported [] = TEXT ("Le fichier %s n'est pas reconnu comme fichier DRUM.") ;
TCHAR szErrorCannotRead  [] = TEXT ("Le fichier %s ne peut être lu.") ;

BOOL DrumFileOpenDlg (HWND hwnd, TCHAR * szFileName, TCHAR * szTitleName)
{
     ofn.hwndOwner         = hwnd ;
     ofn.lpstrFilter       = szFilter [0] ;
     ofn.lpstrFile         = szFileName ;
     ofn.nMaxFile          = MAX_PATH ;
     ofn.lpstrFileTitle    = szTitleName ;
     ofn.nMaxFileTitle     = MAX_PATH ;
     ofn.Flags             = OFN_CREATEPROMPT ;
     ofn.lpstrDefExt       = TEXT ("drm") ;
     
     return GetOpenFileName (&ofn) ;
}

BOOL DrumFileSaveDlg (HWND hwnd, TCHAR * szFileName, TCHAR * szTitleName)
{
     ofn.hwndOwner         = hwnd ;
     ofn.lpstrFilter       = szFilter [0] ;
     ofn.lpstrFile         = szFileName ;
     ofn.nMaxFile          = MAX_PATH ;
     ofn.lpstrFileTitle    = szTitleName ;
     ofn.nMaxFileTitle     = MAX_PATH ;
     ofn.Flags             = OFN_OVERWRITEPROMPT ;
     ofn.lpstrDefExt       = TEXT ("drm") ;
     
     return GetSaveFileName (&ofn) ;
}

TCHAR * DrumFileWrite (DRUM * pdrum, TCHAR * szFileName)
{
     char        szDateBuf [16] ;
     HMMIO       hmmio ;
     int         iFormat = 2 ;
     MMCKINFO    mmckinfo [3] ;
     SYSTEMTIME  st ;
     WORD        wError = 0 ;
     
     memset (mmckinfo, 0, 3 * sizeof (MMCKINFO)) ;
     
          // recréer le fichier pour écriture
     
     if ((hmmio = mmioOpen (szFileName, NULL,
               MMIO_CREATE | MMIO_WRITE | MMIO_ALLOCBUF)) == NULL)
          return szErrorNoCreate ;
     
          // Créer un  "RIFF" avec un type "CPDR"
     
     mmckinfo[0].fccType = mmioStringToFOURCC (szDrumID, 0) ;
     
     wError |= mmioCreateChunk (hmmio, &mmckinfo[0], MMIO_CREATERIFF) ;
     
          // Créer un  "LIST" avec un type "INFO" 
     
     mmckinfo[1].fccType = mmioStringToFOURCC (szInfoID, 0) ;
     
     wError |= mmioCreateChunk (hmmio, &mmckinfo[1], MMIO_CREATELIST) ;
     
          // créer un  "ISFT" 
     
     mmckinfo[2].ckid = mmioStringToFOURCC (szSoftID, 0) ;
     
     wError |= mmioCreateChunk (hmmio, &mmckinfo[2], 0) ;
     wError |= (mmioWrite (hmmio, szSoftware, sizeof (szSoftware)) !=
                                              sizeof (szSoftware)) ;
     wError |= mmioAscend (hmmio, &mmckinfo[2], 0) ;
     
          // créer une chaîne de durée

     GetLocalTime (&st) ;
     
     wsprintfA (szDateBuf, "%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay) ;
     
          // créer un  "ISCD" 
     
     mmckinfo[2].ckid = mmioStringToFOURCC (szDateID, 0) ;
     
     wError |= mmioCreateChunk (hmmio, &mmckinfo[2], 0) ;
     wError |= (mmioWrite (hmmio, szDateBuf, (strlen (szDateBuf) + 1)) !=
                                       (int) (strlen (szDateBuf) + 1)) ;
     wError |= mmioAscend (hmmio, &mmckinfo[2], 0) ;
     wError |= mmioAscend (hmmio, &mmckinfo[1], 0) ;
     
          // créer un "fmt " 
     
     mmckinfo[1].ckid = mmioStringToFOURCC (szFmtID, 0) ;
     
     wError |= mmioCreateChunk (hmmio, &mmckinfo[1], 0) ;
     wError |= (mmioWrite (hmmio, (PSTR) &iFormat, sizeof (int)) !=
                                                   sizeof (int)) ;
     wError |= mmioAscend (hmmio, &mmckinfo[1], 0) ;
     
          // créer un  "data"
     
     mmckinfo[1].ckid = mmioStringToFOURCC (szDataID, 0) ;
     
     wError |= mmioCreateChunk (hmmio, &mmckinfo[1], 0) ;
     wError |= (mmioWrite (hmmio, (PSTR) pdrum, sizeof (DRUM)) !=
                                                sizeof (DRUM)) ;
     wError |= mmioAscend (hmmio, &mmckinfo[1], 0) ;
     wError |= mmioAscend (hmmio, &mmckinfo[0], 0) ;
     
          // effacer et retourner
     
     wError |= mmioClose (hmmio, 0) ;
     
     if (wError)
     {
          mmioOpen (szFileName, NULL, MMIO_DELETE) ;
          return szErrorCannotWrite ;
     }
     return NULL ;
}

TCHAR * DrumFileRead (DRUM * pdrum, TCHAR * szFileName)
{
     DRUM     drum ;
     HMMIO    hmmio ;
     int      i, iFormat ;
     MMCKINFO mmckinfo [3] ;
     
     ZeroMemory (mmckinfo, 2 * sizeof (MMCKINFO)) ;
     
         // ouvrir le fichier
     
     if ((hmmio = mmioOpen (szFileName, NULL, MMIO_READ)) == NULL)
          return szErrorNotFound ;
     
          // localise un  "RIFF" avec un type "DRUM" 
     
     mmckinfo[0].ckid = mmioStringToFOURCC (szDrumID, 0) ;
     
     if (mmioDescend (hmmio, &mmckinfo[0], NULL, MMIO_FINDRIFF))
     {
          mmioClose (hmmio, 0) ;
          return szErrorNotDrum ;
     }
     
          // localise, lit et vérifie "fmt " 
     
     mmckinfo[1].ckid = mmioStringToFOURCC (szFmtID, 0) ;
     
     if (mmioDescend (hmmio, &mmckinfo[1], &mmckinfo[0], MMIO_FINDCHUNK))
     {
          mmioClose (hmmio, 0) ;
          return szErrorNotDrum ;
     }
     
     if (mmckinfo[1].cksize != sizeof (int))
     {
          mmioClose (hmmio, 0) ;
          return szErrorUnsupported ;
     }
     
     if (mmioRead (hmmio, (PSTR) &iFormat, sizeof (int)) != sizeof (int))
     {
          mmioClose (hmmio, 0) ;
          return szErrorCannotRead ;
     }
     
     if (iFormat != 1 && iFormat != 2)
     {
          mmioClose (hmmio, 0) ;
          return szErrorUnsupported ;
     }
     
          // va jusqu'à la fin de  "fmt " 
     
     mmioAscend (hmmio, &mmckinfo[1], 0) ;
     
          // localise, lit et vérifie "data" 
     
     mmckinfo[1].ckid = mmioStringToFOURCC (szDataID, 0) ;
  
     if (mmioDescend (hmmio, &mmckinfo[1], &mmckinfo[0], MMIO_FINDCHUNK))
     {
          mmioClose (hmmio, 0) ;
          return szErrorNotDrum ;
     }
     
     if (mmckinfo[1].cksize != sizeof (DRUM))
     {
          mmioClose (hmmio, 0) ;
          return szErrorUnsupported ;
     }
     
     if (mmioRead (hmmio, (LPSTR) &drum, sizeof (DRUM)) != sizeof (DRUM))
     {
          mmioClose (hmmio, 0) ;
          return szErrorCannotRead ;
     }
     
          // ferme le fichier
     
     mmioClose (hmmio, 0) ;

          // convertit le format 1 en format 2 et copie la structure DRUM 

     if (iFormat == 1)
     {
          for (i = 0 ; i < NUM_PERC ; i++)
          {
               drum.dwSeqPerc [i] = drum.dwSeqPian [i] ;
               drum.dwSeqPian [i] = 0 ;
          }
     }
     
     memcpy (pdrum, &drum, sizeof (DRUM)) ;
     return NULL ;
}

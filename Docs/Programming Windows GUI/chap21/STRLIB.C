/*------------------------------------------------
   STRLIB.C -- Module bibliothèque pour le programme STRPROG
               (c) Charles Petzold, 1998
  ------------------------------------------------*/

#include <windows.h>
#include <wchar.h>       // Pour les fonctions en caractères étendus
#include "strlib.h"

     // section mémoire partagée (nécessite /SECTION:shared,RWS dans les options de liaison)

#pragma data_seg ("shared")
int   iTotal = 0 ;
WCHAR szStrings [MAX_STRINGS][MAX_LENGTH + 1] = { '\0' } ;
#pragma data_seg ()

#pragma comment(linker,"/SECTION:shared,RWS")

int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
     return TRUE ;
}

EXPORT BOOL CALLBACK AddStringA (PCSTR pStringIn)
{
     BOOL  bReturn ;
     int   iLength ;
     PWSTR pWideStr ;

          // convertit une chaîne en Unicode et appelle AddStringW

     iLength = MultiByteToWideChar (CP_ACP, 0, pStringIn, -1, NULL, 0) ;
     pWideStr = malloc (iLength) ;
     MultiByteToWideChar (CP_ACP, 0, pStringIn, -1, pWideStr, iLength) ;
     bReturn = AddStringW (pWideStr) ;
     free (pWideStr) ;

     return bReturn ;
}

EXPORT BOOL CALLBACK AddStringW (PCWSTR pStringIn)
{
     PWSTR pString ;
     int   i, iLength ;
     
     if (iTotal == MAX_STRINGS - 1)
          return FALSE ;
     
     if ((iLength = wcslen (pStringIn)) == 0)
          return FALSE ;

          // Alloue de la mémoire pour stocker la chaîne, la copie, et la convertit en majuscules

     pString = malloc (sizeof (WCHAR) * (1 + iLength)) ;
     wcscpy (pString, pStringIn) ;
     _wcsupr (pString) ;

          // alphabétise les chaînes
     
     for (i = iTotal ; i > 0 ; i--)
     {
          if (wcscmp (pString, szStrings[i - 1]) >= 0)
               break ;
          
          wcscpy (szStrings[i], szStrings[i - 1]) ;
     }
     wcscpy (szStrings[i], pString) ;
     iTotal++ ;

     free (pString) ;
     return TRUE ;
}

EXPORT BOOL CALLBACK DeleteStringA (PCSTR pStringIn)
{
     BOOL  bReturn ;
     int   iLength ;
     PWSTR pWideStr ;

          // convertit la chaîne en Unicode et appelle DeleteStringW

     iLength = MultiByteToWideChar (CP_ACP, 0, pStringIn, -1, NULL, 0) ;
     pWideStr = malloc (iLength) ;
     MultiByteToWideChar (CP_ACP, 0, pStringIn, -1, pWideStr, iLength) ;
     bReturn = DeleteStringW (pWideStr) ;
     free (pWideStr) ;

     return bReturn ;
}

EXPORT BOOL CALLBACK DeleteStringW (PCWSTR pStringIn)
{
     int i, j ;
     
     if (0 == wcslen (pStringIn))
          return FALSE ;
     
     for (i = 0 ; i < iTotal ; i++)
     {
          if (_wcsicmp (szStrings[i], pStringIn) == 0)
               break ;
     }
          // si la chaîne n'est pas dans la liste, retourne sans rien entreprendre
     
     if (i == iTotal)
          return FALSE ;
     
          // autrement ajuste la liste vers le bas
     
     for (j = i ; j < iTotal ; j++)
          wcscpy (szStrings[j], szStrings[j + 1]) ;
     
     szStrings[iTotal--][0] = '\0' ;
     return TRUE ;
}

EXPORT int CALLBACK GetStringsA (GETSTRCB pfnGetStrCallBack, PVOID pParam)
{
     BOOL bReturn ;
     int  i, iLength ;
     PSTR pAnsiStr ;

     for (i = 0 ; i < iTotal ; i++)
     {
               // convertit la chaîne à partir d'Unicode

          iLength = WideCharToMultiByte (CP_ACP, 0, szStrings[i], -1, NULL, 0,
                                         NULL, NULL) ;
          pAnsiStr = malloc (iLength) ;
          WideCharToMultiByte (CP_ACP, 0, szStrings[i], -1, pAnsiStr, iLength,
                                          NULL, NULL) ;

               // appelle la fonction callback

          bReturn = pfnGetStrCallBack (pAnsiStr, pParam) ;
          
          if (bReturn == FALSE)
               return i + 1 ;

          free (pAnsiStr) ;
     }
     return iTotal ;
}

EXPORT int CALLBACK GetStringsW (GETSTRCB pfnGetStrCallBack, PVOID pParam)
{
     BOOL bReturn ;
     int  i ;
     
     for (i = 0 ; i < iTotal ; i++)
     {
          bReturn = pfnGetStrCallBack (szStrings[i], pParam) ;
          
          if (bReturn == FALSE)
               return i + 1 ;
     }
     return iTotal ;
}

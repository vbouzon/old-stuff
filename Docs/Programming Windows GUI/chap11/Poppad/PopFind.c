/*--------------------------------------------------------
   POPFIND.C - Fonctions de recherche et de remplacement de l'�diteur Popup
  --------------------------------------------------------*/

#include <windows.h>
#include <commdlg.h>
#include <tchar.h>            // pour _tcsstr (strstr pour Unicode & non-Unicode)

#define MAX_STRING_LEN   256

static TCHAR szFindText [MAX_STRING_LEN] ;
static TCHAR szReplText [MAX_STRING_LEN] ;

HWND PopFindFindDlg (HWND hwnd)
{
     static FINDREPLACE fr ;       // statique pour bo�te de dialogue non modale !!!
     
     fr.lStructSize      = sizeof (FINDREPLACE) ;
     fr.hwndOwner        = hwnd ;
     fr.hInstance        = NULL ;
     fr.Flags            = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD ;
     fr.lpstrFindWhat    = szFindText ;
     fr.lpstrReplaceWith = NULL ;
     fr.wFindWhatLen     = MAX_STRING_LEN ;
     fr.wReplaceWithLen  = 0 ;
     fr.lCustData        = 0 ;
     fr.lpfnHook         = NULL ;
     fr.lpTemplateName   = NULL ;
     
     return FindText (&fr) ;
}

HWND PopFindReplaceDlg (HWND hwnd)
{
     static FINDREPLACE fr ;       // statique pour bo�te de dialogue non modale !!!
     
     fr.lStructSize      = sizeof (FINDREPLACE) ;
     fr.hwndOwner        = hwnd ;
     fr.hInstance        = NULL ;
     fr.Flags            = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD ;
     fr.lpstrFindWhat    = szFindText ;
     fr.lpstrReplaceWith = szReplText ;
     fr.wFindWhatLen     = MAX_STRING_LEN ;
     fr.wReplaceWithLen  = MAX_STRING_LEN ;
     fr.lCustData        = 0 ;
     fr.lpfnHook         = NULL ;
     fr.lpTemplateName   = NULL ;
     
     return ReplaceText (&fr) ;
}

BOOL PopFindFindText (HWND hwndEdit, int * piSearchOffset, LPFINDREPLACE pfr)
{
     int    iLength, iPos ;
     PTSTR  pstrDoc, pstrPos ;
     
          // Lecture dans le document modifi�
     
     iLength = GetWindowTextLength (hwndEdit) ;
     
     if (NULL == (pstrDoc = (PTSTR) malloc ((iLength + 1) * sizeof (TCHAR))))
          return FALSE ;
     
     GetWindowText (hwndEdit, pstrDoc, iLength + 1) ;
     
          // Recherche de la cha�ne dans le document
     
     pstrPos = _tcsstr (pstrDoc + * piSearchOffset, pfr->lpstrFindWhat) ;
     free (pstrDoc) ;
     
          // Renvoie code d'erreur si cha�ne introuvable
     
     if (pstrPos == NULL)
          return FALSE ;
     
          // Trouve la position dans le document et le nouveau d�calage de d�but
     
     iPos = pstrPos - pstrDoc ;
     * piSearchOffset = iPos + lstrlen (pfr->lpstrFindWhat) ;
     
          // S�lectionner le texte trouv�
     
     SendMessage (hwndEdit, EM_SETSEL, iPos, * piSearchOffset) ;
     SendMessage (hwndEdit, EM_SCROLLCARET, 0, 0) ;
     
     return TRUE ;
}

BOOL PopFindNextText (HWND hwndEdit, int * piSearchOffset)
{
     FINDREPLACE fr ;
     
     fr.lpstrFindWhat = szFindText ;
     
     return PopFindFindText (hwndEdit, piSearchOffset, &fr) ;
}

BOOL PopFindReplaceText (HWND hwndEdit, int * piSearchOffset, LPFINDREPLACE pfr)
{
          // Rechercher le texte
     
     if (!PopFindFindText (hwndEdit, piSearchOffset, pfr))
          return FALSE ;
     
          // Le remplacer
     
     SendMessage (hwndEdit, EM_REPLACESEL, 0, (LPARAM) pfr->lpstrReplaceWith) ;
     
     return TRUE ;
}

BOOL PopFindValidFind (void)
{
     return * szFindText != '\0' ;
}

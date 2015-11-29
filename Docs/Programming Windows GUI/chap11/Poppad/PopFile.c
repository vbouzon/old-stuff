/*------------------------------------------
   POPFILE.C -- Fonctions de fichiers de l'�diteur Popup
  ------------------------------------------*/

#include <windows.h>
#include <commdlg.h>

static OPENFILENAME ofn ;

void PopFileInitialize (HWND hwnd)
{
     static TCHAR szFilter[] = TEXT ("Fichiers texte (*.TXT)\0*.txt\0")  \
                               TEXT ("Fichiers ASCII (*.ASC)\0*.asc\0") \
                               TEXT ("Tous les fichiers (*.*)\0*.*\0\0") ;
     
     ofn.lStructSize       = sizeof (OPENFILENAME) ;
     ofn.hwndOwner         = hwnd ;
     ofn.hInstance         = NULL ;
     ofn.lpstrFilter       = szFilter ;
     ofn.lpstrCustomFilter = NULL ;
     ofn.nMaxCustFilter    = 0 ;
     ofn.nFilterIndex      = 0 ;
     ofn.lpstrFile         = NULL ;          // D�fini dans les fonctions Ouvrir et Fermer
     ofn.nMaxFile          = MAX_PATH ;
     ofn.lpstrFileTitle    = NULL ;          // D�fini dans les fonctions Ouvrir et Fermer
     ofn.nMaxFileTitle     = MAX_PATH ;
     ofn.lpstrInitialDir   = NULL ;
     ofn.lpstrTitle        = NULL ;
     ofn.Flags             = 0 ;             // D�fini dans les fonctions Ouvrir et Fermer
     ofn.nFileOffset       = 0 ;
     ofn.nFileExtension    = 0 ;
     ofn.lpstrDefExt       = TEXT ("txt") ;
     ofn.lCustData         = 0L ;
     ofn.lpfnHook          = NULL ;
     ofn.lpTemplateName    = NULL ;
}

BOOL PopFileOpenDlg (HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName)
{
     ofn.hwndOwner         = hwnd ;
     ofn.lpstrFile         = pstrFileName ;
     ofn.lpstrFileTitle    = pstrTitleName ;
     ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;
     
     return GetOpenFileName (&ofn) ;
}

BOOL PopFileSaveDlg (HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName)
{
     ofn.hwndOwner         = hwnd ;
     ofn.lpstrFile         = pstrFileName ;
     ofn.lpstrFileTitle    = pstrTitleName ;
     ofn.Flags             = OFN_OVERWRITEPROMPT ;
     
     return GetSaveFileName (&ofn) ;
}

BOOL PopFileRead (HWND hwndEdit, PTSTR pstrFileName)
{
     BYTE   bySwap ;
     DWORD  dwBytesRead ;
     HANDLE hFile ;
     int    i, iFileLength, iUniTest ;
     PBYTE  pBuffer, pText, pConv ;

          // Overture du fichier.

     if (INVALID_HANDLE_VALUE == 
               (hFile = CreateFile (pstrFileName, GENERIC_READ, FILE_SHARE_READ,
                                    NULL, OPEN_EXISTING, 0, NULL)))
          return FALSE ;

          // lecture de la taille du fichier en octets et allocation m�moire pour lecture.
          // Ajout de deux octets pour z�ro final.
                    
     iFileLength = GetFileSize (hFile, NULL) ; 
     pBuffer = malloc (iFileLength + 2) ;

          // Lecture du fichier et insertion des z�ros finaux.
     
     ReadFile (hFile, pBuffer, iFileLength, &dwBytesRead, NULL) ;
     CloseHandle (hFile) ;
     pBuffer[iFileLength] = '\0' ;
     pBuffer[iFileLength + 1] = '\0' ;

          // D�terminer si le texte est Unicode

     iUniTest = IS_TEXT_UNICODE_SIGNATURE | IS_TEXT_UNICODE_REVERSE_SIGNATURE ;
     
     if (IsTextUnicode (pBuffer, iFileLength, &iUniTest))
     {
          pText = pBuffer + 2 ;
          iFileLength -= 2 ;

          if (iUniTest & IS_TEXT_UNICODE_REVERSE_SIGNATURE)
          {
               for (i = 0 ; i < iFileLength / 2 ; i++)
               {
                    bySwap = ((BYTE *) pText) [2 * i] ;
                    ((BYTE *) pText) [2 * i] = ((BYTE *) pText) [2 * i + 1] ;
                    ((BYTE *) pText) [2 * i + 1] = bySwap ;
               }
          }

               // Allocation m�moire pour cha�ne �ventuellement convertie

          pConv = malloc (iFileLength + 2) ;

               // Si le contr�le d'�dition n'est pas Unicode, convertir texte Unicode
               // en non-Unicode (en caract�re large g�n�ral).

#ifndef UNICODE
          WideCharToMultiByte (CP_ACP, 0, (PWSTR) pText, -1, pConv, 
                               iFileLength + 2, NULL, NULL) ;

               // Si le contr�le d'�dition est Unicode, copier la cha�ne
#else
          lstrcpy ((PTSTR) pConv, (PTSTR) pText) ;
#endif

     }
     else      // fichier non-Unicode
     {
          pText = pBuffer ;

               // Allocation m�moire pour cha�ne �ventuellement convertie.

          pConv = malloc (2 * iFileLength + 2) ;

               // Si le contr�le d'�dition est Unicode, convertir texte ASCII.

#ifdef UNICODE
          MultiByteToWideChar (CP_ACP, 0, pText, -1, (PTSTR) pConv, 
                               iFileLength + 1) ;

               // Si non-Unicode, copier le tampon
#else
          lstrcpy ((PTSTR) pConv, (PTSTR) pText) ;
#endif
     }
     
     SetWindowText (hwndEdit, (PTSTR) pConv) ;
     free (pBuffer) ;
     free (pConv) ;
   
     return TRUE ;
}

BOOL PopFileWrite (HWND hwndEdit, PTSTR pstrFileName)
{
     DWORD  dwBytesWritten ;
     HANDLE hFile ;
     int    iLength ;
     PTSTR  pstrBuffer ;
     WORD   wByteOrderMark = 0xFEFF ;

          // Ouvrir le fichier en le cr�ant si n�cessaire
     
     if (INVALID_HANDLE_VALUE == 
               (hFile = CreateFile (pstrFileName, GENERIC_WRITE, 0, 
                                    NULL, CREATE_ALWAYS, 0, NULL)))
          return FALSE ;

          // Lire le nombre de caract�res dans contr�le d'�dition et 
          // allouer la m�moire n�cessaire.
     
     iLength = GetWindowTextLength (hwndEdit) ;
     pstrBuffer = (PTSTR) malloc ((iLength + 1) * sizeof (TCHAR)) ;
     
     if (!pstrBuffer)
     {
          CloseHandle (hFile) ;
          return FALSE ;
     }

          // Si le contr�le d'�dition doit renvoyer du texte Unicode,
          // �crire la marque d'ordre d'octets dans le fichier.

#ifdef UNICODE
     WriteFile (hFile, &wByteOrderMark, 2, &dwBytesWritten, NULL) ;
#endif

          // Lire le tampon d'�dition et l'�crire dans le fichier.
     
     GetWindowText (hwndEdit, pstrBuffer, iLength + 1) ;
     WriteFile (hFile, pstrBuffer, iLength * sizeof (TCHAR), 
                &dwBytesWritten, NULL) ;
     
     if ((iLength * sizeof (TCHAR)) != (int) dwBytesWritten)
     {
          CloseHandle (hFile) ;
          free (pstrBuffer) ;
          return FALSE ;
     }
     
     CloseHandle (hFile) ;
     free (pstrBuffer) ;
     
     return TRUE ;
}

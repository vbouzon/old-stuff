/*---------------------------------------
   PRINT1.C -- Impression minimaliste
               (c) Charles Petzold, 1998
  ---------------------------------------*/

#include <windows.h>

HDC  GetPrinterDC (void) ;              // dans GETPRNDC.C
void PageGDICalls (HDC, int, int) ;     // dans PRINT.C

HINSTANCE hInst ;
TCHAR     szAppName[] = TEXT ("Print1") ;
TCHAR     szCaption[] = TEXT ("Programme Print 1") ;

BOOL PrintMyPage (HWND hwnd)
{
     static DOCINFO di = { sizeof (DOCINFO), TEXT ("Print1 : impression en cours") } ;
     BOOL           bSuccess = TRUE ;
     HDC            hdcPrn ;
     int            xPage, yPage ;
     
     if (NULL == (hdcPrn = GetPrinterDC ()))
          return FALSE ;

     xPage = GetDeviceCaps (hdcPrn, HORZRES) ;
     yPage = GetDeviceCaps (hdcPrn, VERTRES) ;
     
     if (StartDoc (hdcPrn, &di) > 0)
     {
          if (StartPage (hdcPrn) > 0)
          {
               PageGDICalls (hdcPrn, xPage, yPage) ;
               
               if (EndPage (hdcPrn) > 0)
                    EndDoc (hdcPrn) ;
               else
                    bSuccess = FALSE ;
          }
     }
     else
          bSuccess = FALSE ;
     
     DeleteDC (hdcPrn) ;
     return bSuccess ;
}

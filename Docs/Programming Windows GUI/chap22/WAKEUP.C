/*---------------------------------------
   WAKEUP.C -- Programme de réveil 
               (c) Charles Petzold, 1998
  ---------------------------------------*/

#include <windows.h>
#include <commctrl.h>

     // Valeurs d'ID pour trois fenêtres enfant

#define ID_TIMEPICK 0
#define ID_CHECKBOX 1
#define ID_PUSHBTN  2

     // ID de l'horloge

#define ID_TIMER    1

     // Nombre d'incréments de 100 nanosecondes (de cadences FILETIME) en une heure

#define FTTICKSPERHOUR (60 * 60 * (LONGLONG) 10000000)

     // Définition et strcuture du "fichier" son

#define SAMPRATE  11025
#define NUMSAMPS  (3 * SAMPRATE)
#define HALFSAMPS (NUMSAMPS / 2) 

typedef struct
{
     char  chRiff[4] ;
     DWORD dwRiffSize ;
     char  chWave[4] ;
     char  chFmt [4] ;
     DWORD dwFmtSize ;
     PCMWAVEFORMAT pwf ;
     char  chData[4] ;
     DWORD dwDataSize ;
     BYTE  byData[0] ;
}
WAVEFORM ;

     // La procédure fenêtre et la procédure sous-classe

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK SubProc (HWND, UINT, WPARAM, LPARAM) ;

     // Adresse de la procédure fenêtre originale pour les fenêtres de sous-classe

WNDPROC SubbedProc [3] ;

     // La fenêtre enfant courante avec le focus d'entrée

HWND hwndFocus ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInst,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName [] = TEXT ("Réveil") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;

     wndclass.style         = 0 ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) (1 + COLOR_BTNFACE) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Le programme nécessite Windows NT !"),
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }

     hwnd = CreateWindow (szAppName, szAppName,
                          WS_OVERLAPPED | WS_CAPTION | 
                                          WS_SYSMENU | WS_MINIMIZEBOX,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;

     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static HWND          hwndDTP, hwndCheck, hwndPush ;
     static WAVEFORM      waveform = { "RIFF", NUMSAMPS + 0x24, "WAVE", "fmt ", 
                                       sizeof (PCMWAVEFORMAT), 1, 1, SAMPRATE, 
                                       SAMPRATE, 1, 8, "data", NUMSAMPS } ;
     static WAVEFORM    * pwaveform ;
     FILETIME             ft ;
     HINSTANCE            hInstance ;
     INITCOMMONCONTROLSEX icex ;
     int                  i, cxChar, cyChar ;
     LARGE_INTEGER        li ;
     SYSTEMTIME           st ;

     switch (message)
     {
     case WM_CREATE:
               // Quelques questions d'initialisation

          hInstance = (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE) ;

          icex.dwSize = sizeof (icex) ;
          icex.dwICC  = ICC_DATE_CLASSES ;
          InitCommonControlsEx (&icex) ;

               // Création du fichier son 

          pwaveform = malloc (sizeof (WAVEFORM) + NUMSAMPS) ;
          * pwaveform = waveform ;

          for (i = 0 ; i < HALFSAMPS ; i++)
               if (i % 600 < 300)
                    if (i % 16 < 8)
                         pwaveform->byData[i] = 25 ;
                    else
                         pwaveform->byData[i] = 230 ;
               else
                    if (i % 8 < 4)
                         pwaveform->byData[i] = 25 ;
                    else
                         pwaveform->byData[i] = 230 ;

               // Récupère la taille des caractères et définit une taille de fenêtre fixe

          cxChar = LOWORD (GetDialogBaseUnits ()) ;
          cyChar = HIWORD (GetDialogBaseUnits ()) ;

          SetWindowPos (hwnd, NULL, 0, 0, 
                        42 * cxChar, 
                        10 * cyChar / 3 + 2 * GetSystemMetrics (SM_CYBORDER) +
                                              GetSystemMetrics (SM_CYCAPTION),
                        SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE) ; 

               // Crée les trois fenêtres enfant

          hwndDTP = CreateWindow (DATETIMEPICK_CLASS, TEXT (""), 
                         WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_TIMEFORMAT,
                         2 * cxChar, cyChar, 12 * cxChar, 4 * cyChar / 3, 
                         hwnd, (HMENU) ID_TIMEPICK, hInstance, NULL) ;

          hwndCheck = CreateWindow (TEXT ("Button"), TEXT ("Heure"),
                         WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                         16 * cxChar, cyChar, 12 * cxChar, 4 * cyChar / 3,
                         hwnd, (HMENU) ID_CHECKBOX, hInstance, NULL) ;

          hwndPush = CreateWindow (TEXT ("Button"), TEXT ("Éteindre"),
                         WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_DISABLED,
                         28 * cxChar, cyChar, 12 * cxChar, 4 * cyChar / 3,
                         hwnd, (HMENU) ID_PUSHBTN, hInstance, NULL) ;

          hwndFocus = hwndDTP ;

               // Définir les sous-classes des trois fenêtres enfant

          SubbedProc [ID_TIMEPICK] = (WNDPROC) 
                         SetWindowLong (hwndDTP, GWL_WNDPROC, (LONG) SubProc) ;
          SubbedProc [ID_CHECKBOX] = (WNDPROC) 
                         SetWindowLong (hwndCheck, GWL_WNDPROC, (LONG) SubProc);
          SubbedProc [ID_PUSHBTN] = (WNDPROC) 
                         SetWindowLong (hwndPush, GWL_WNDPROC, (LONG) SubProc) ;
          
               // Fixe l'heure à l'heure courante
               // plus 9 heures arrondies à l'heure inférieure
          
          GetLocalTime (&st) ;
          SystemTimeToFileTime (&st, &ft) ;
          li = * (LARGE_INTEGER *) &ft ;
          li.QuadPart += 9 * FTTICKSPERHOUR ; 
          ft = * (FILETIME *) &li ;
          FileTimeToSystemTime (&ft, &st) ;
          st.wMinute = st.wSecond = st.wMilliseconds = 0 ;
          SendMessage (hwndDTP, DTM_SETSYSTEMTIME, 0, (LPARAM) &st) ;
          return 0 ;

     case WM_SETFOCUS:
          SetFocus (hwndFocus) ;
          return 0 ;

     case WM_COMMAND:
          switch (LOWORD (wParam))      //  ID du contrôle
          {
          case ID_CHECKBOX:
               
                    // Quand l'utilisateur active l'alarme, récupère
                    // l'heure et y soustrait 
                    // l'heure courante du PC.

               if (SendMessage (hwndCheck, BM_GETCHECK, 0, 0))
               {
                    SendMessage (hwndDTP, DTM_GETSYSTEMTIME, 0, (LPARAM) &st) ;
                    SystemTimeToFileTime (&st, &ft) ;
                    li = * (LARGE_INTEGER *) &ft ;

                    GetLocalTime (&st) ;
                    SystemTimeToFileTime (&st, &ft) ;
                    li.QuadPart -= ((LARGE_INTEGER *) &ft)->QuadPart ;

                         // Pour être sûr que l'heure est bien entre 0 et 24 !
                         // Ces petits ajustements nous permettent d'ignorer
                         // la partie date des structures SYSTEMTIME.

                    while (li.QuadPart < 0)
                         li.QuadPart += 24 * FTTICKSPERHOUR ;

                    li.QuadPart %= 24 * FTTICKSPERHOUR ;

                         

                    SetTimer (hwnd, ID_TIMER, (int) (li.QuadPart / 10000), 0) ;
               }
                    // Si le bouton n'est pas activé, détruire le réveil.

               else
                    KillTimer (hwnd, ID_TIMER) ;

               return 0 ;

               // Le bouton Éteindre arrête la sonnerie et 
               // désactive la case à cocher.

          case ID_PUSHBTN:
               PlaySound (NULL, NULL, 0) ;
               SendMessage (hwndCheck, BM_SETCHECK, 0, 0) ;
               EnableWindow (hwndDTP, TRUE) ;
               EnableWindow (hwndCheck, TRUE) ;
               EnableWindow (hwndPush, FALSE) ;
               SetFocus (hwndDTP) ;
               return 0 ;
          }
          return 0 ;

               // Le message WM_NOTIFY provient de la mise à l'heure.
               // Si l'utilisateur a activé l'alarme et veut  
               // changer l'heure de réveil, il pourrait y avoir un décalage
               // entre l'heure affichée et l'heure de réveil. Le programme
               // désactive l'alarme et détruit un réveil existant.

     case WM_NOTIFY:
          switch (wParam)          //  ID du contrôle
          {
          case ID_TIMEPICK:
               switch (((NMHDR *) lParam)->code)       // code de notification
               {
               case DTN_DATETIMECHANGE:
                    if (SendMessage (hwndCheck, BM_GETCHECK, 0, 0))
                    {
                         KillTimer (hwnd, ID_TIMER) ;
                         SendMessage (hwndCheck, BM_SETCHECK, 0, 0) ;
                    }
                    return 0 ;
               }
          }
          return 0 ;

          // Le message WM_COMMAND vient des deux boutons. 

     case WM_TIMER:

               // Quand le message arrive de l'horloge, détruit l'horloge 
               // parce que nous ne voulons qu'un réveil, et commence à sonner. 

          KillTimer (hwnd, ID_TIMER) ;
          PlaySound ((PTSTR) pwaveform,  NULL, 
                     SND_MEMORY | SND_LOOP | SND_ASYNC);

               // Permet à l'utilisateur d'éteindre la sonnerie avec la barre d'espace.
               // Si la fenêtre est réduite, elle est restaurée
               // et le bouton disponible reçoit le focus d'entrée.

          EnableWindow (hwndDTP, FALSE) ;
          EnableWindow (hwndCheck, FALSE) ;
          EnableWindow (hwndPush, TRUE) ;

          hwndFocus = hwndPush ;
          ShowWindow (hwnd, SW_RESTORE) ;
          SetForegroundWindow (hwnd) ;
          return 0 ;

          // Efface si l'alarme sonne ou si le réveil est toujours programmé.

     case WM_DESTROY:
          free (pwaveform) ;

          if (IsWindowEnabled (hwndPush))
               PlaySound (NULL, NULL, 0) ;

          if (SendMessage (hwndCheck, BM_GETCHECK, 0, 0))
               KillTimer (hwnd, ID_TIMER) ;

          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

LRESULT CALLBACK SubProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     int idNext, id = GetWindowLong (hwnd, GWL_ID) ;
         
     switch (message)
     {
     case WM_CHAR:
          if (wParam == '\t')
          {
               idNext = id ;

               do
                    idNext = (idNext + 
                         (GetKeyState (VK_SHIFT) < 0 ? 2 : 1)) % 3 ;
               while (!IsWindowEnabled (GetDlgItem (GetParent (hwnd), idNext)));

               SetFocus (GetDlgItem (GetParent (hwnd), idNext)) ;
               return 0 ;
          }
          break ;

     case WM_SETFOCUS:
          hwndFocus = hwnd ;
          break ;
     }
     return CallWindowProc (SubbedProc [id], hwnd, message, wParam, lParam) ;
}

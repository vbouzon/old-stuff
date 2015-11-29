/*---------------------------------------
   KBMIDI.C -- Clavier MIDI 
               (c) Charles Petzold, 1998
  ---------------------------------------*/

#include <windows.h>

// Defines for Menu IDs
// --------------------

#define IDM_OPEN    0x100
#define IDM_CLOSE   0x101
#define IDM_DEVICE  0x200
#define IDM_CHANNEL 0x300
#define IDM_VOICE   0x400

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

TCHAR    szAppName [] = TEXT (" Clavier Midi") ;
HMIDIOUT hMidiOut ;
int      iDevice = MIDIMAPPER, iChannel = 0, iVoice = 0, iVelocity = 64 ;
int      cxCaps, cyChar, xOffset, yOffset ;

     // Structures et données pour afficher les familles et les instruments dans le menu
     // ----------------------------------------------------------------

typedef struct
{
     TCHAR * szInst ;
     int    iVoice ;
}
INSTRUMENT ;

typedef struct
{
     TCHAR      * szFam ;
     INSTRUMENT   inst [8] ;
}
FAMILY ;

FAMILY fam [16] = { 
     
     TEXT ("Piano"),

          TEXT ("Grand Piano acoustique"),        0,
          TEXT ("Piano acoutisque large"),       1,
          TEXT ("Grand Piano électrique"),        2,
          TEXT ("Piano Honky-tonk"),            3,
          TEXT ("Piano Rhodes"),                4,
          TEXT ("Piano Chorus"),              5,
          TEXT ("Harpe"),                 6,
          TEXT ("Clavecin"),                    7,

     TEXT ("Percussion chromatique"),

          TEXT ("Celesta"),                     8,
          TEXT ("Glockenspiel"),                9,
          TEXT ("Music Box"),                   10,
          TEXT ("Vibraphone"),                  11,
          TEXT ("Marimba"),                     12,
          TEXT ("Xylophone"),                   13,
          TEXT ("Tubular Bells"),               14,
          TEXT ("Dulcimer"),                    15,

     TEXT ("Orgue"),

          TEXT ("Orgue Hammond"),               16,
          TEXT ("Orgue à percussion"),            17,
          TEXT ("Orgue Rock"),                  18,
          TEXT ("Orgue d'église"),                19,
          TEXT ("Orgue à anche"),                  20,
          TEXT ("Accordéon"),                   21,
          TEXT ("Harmonica"),                   22,
          TEXT ("Accordéon tango"),             23,

     TEXT ("Guitare"),

          TEXT ("Guitare acoustique (nylon)"),     24,
          TEXT ("Guitare acoustique (métal)"),     25,
          TEXT ("Guitare électrctique (jazz)"),      26,
          TEXT ("Guitare électrique (propre)"),     27,
          TEXT ("Guitare électrique (muette)"),     28,
          TEXT ("Guitare saturée"),           29,
          TEXT ("Guitare distortion"),           30,
          TEXT ("Guitare harmonique"),            31,

     TEXT ("Basses"),

          TEXT ("Basse acoustique"),               32,
          TEXT ("Basse électrique (doigt)"),      33,
          TEXT ("Basse électrique (onglet)"),        34,
          TEXT ("Basse sans touchette"),               35,
          TEXT ("Basse Slap 1"),                 36,
          TEXT ("Basse Slap 2"),                 37,
          TEXT ("Basse Synth 1"),                38,
          TEXT ("Basse Synt 2"),                39,

     TEXT ("Cordes"),

          TEXT ("Violon"),                      40,
          TEXT ("Alto"),                       41,
          TEXT ("Violoncelle"),                       42,
          TEXT ("Contrebasse"),                  43,
          TEXT ("Tremolo"),             44,
          TEXT ("Pizzicato"),           45,
          TEXT ("Harpe orchestrale"),             46,
          TEXT ("Timbales"),                     47,

     TEXT ("Ensemble"),

          TEXT ("Ensemble 1"),           48,
          TEXT ("Ensemble 2"),           49,
          TEXT ("Synth 1"),             50,
          TEXT ("Synth  2"),             51,
          TEXT ("Aahs"),                  52,
          TEXT ("Oohs"),                  53,
          TEXT ("Voix synth"),                 54,
          TEXT ("Orchestra Hit"),               55,

     TEXT ("Cuivres"),

          TEXT ("Trompette"),                     56,
          TEXT ("Trombone"),                    57,
          TEXT ("Tuba"),                        58,
          TEXT ("Trompette bouchée"),               59,
          TEXT ("Cor d'harmonie"),                 60,
          TEXT ("Section cuivres"),               61,
          TEXT ("Cuivres synth 1"),               62,
          TEXT ("Cuivres synth 2"),               63,

     TEXT ("Anches"),

          TEXT ("Sax Soprano"),                 64,
          TEXT ("Sax Alto"),                    65,
          TEXT ("Sax Ténor"),                   66,
          TEXT ("Sax Baryton"),                67,
          TEXT ("Hautbois"),                        68,
          TEXT ("Cor anglais"),                69,
          TEXT ("Basson"),                     70,
          TEXT ("Clarinette"),                    71,

     TEXT ("Flûtes"),

          TEXT ("Piccolo"),                     72,
          TEXT ("Flûte traversière"),                       73,
          TEXT ("Recorder"),                    74,
          TEXT ("Flûte de Pan"),                   75,
          TEXT ("Bottle Blow"),                 76,
          TEXT ("Shakuhachi"),                  77,
          TEXT ("Whistle"),                     78,
          TEXT ("Ocarina"),                     79,

     TEXT ("Synth Lead"),

          TEXT ("Lead 1 (square)"),             80,
          TEXT ("Lead 2 (sawtooth)"),           81,
          TEXT ("Lead 3 (caliope lead)"),       82,
          TEXT ("Lead 4 (chiff lead)"),         83,
          TEXT ("Lead 5 (charang)"),            84,
          TEXT ("Lead 6 (voice)"),              85,
          TEXT ("Lead 7 (fifths)"),             86,
          TEXT ("Lead 8 (brass + lead)"),       87,

     TEXT ("Synth Pad"),

          TEXT ("Pad 1 (new age)"),             88,
          TEXT ("Pad 2 (warm)"),                89,
          TEXT ("Pad 3 (polysynth)"),           90,
          TEXT ("Pad 4 (choir)"),               91,
          TEXT ("Pad 5 (bowed)"),               92,
          TEXT ("Pad 6 (metallic)"),            93,
          TEXT ("Pad 7 (halo)"),                94,
          TEXT ("Pad 8 (sweep)"),               95,

     TEXT ("Effets synth"),

          TEXT ("FX 1 (pluie)"),                 96,
          TEXT ("FX 2 (soundtrack)"),           97,
          TEXT ("FX 3 (cristal)"),              98,
          TEXT ("FX 4 (atmosphère)"),           99,
          TEXT ("FX 5 (luminosité)"),           100,
          TEXT ("FX 6 (goblins)"),              101,
          TEXT ("FX 7 (echoes)"),               102,
          TEXT ("FX 8 (sci-fi)"),               103,

     TEXT ("Ethnique"),

          TEXT ("Sitar"),                       104,
          TEXT ("Banjo"),                       105,
          TEXT ("Shamisen"),                    106,
          TEXT ("Koto"),                        107,
          TEXT ("Kalimba"),                     108,
          TEXT ("Bagpipe"),                     109,
          TEXT ("Fiddle"),                      110,
          TEXT ("Shanai"),                      111,

     TEXT ("Percussive"),

          TEXT ("Tinkle Bell"),                 112,
          TEXT ("Agogo"),                       113,
          TEXT ("Steel Drums"),                 114,
          TEXT ("Woodblock"),                   115,
          TEXT ("Taiko Drum"),                  116,
          TEXT ("Melodic Tom"),                 117,
          TEXT ("Synth Drum"),                  118,
          TEXT ("Reverse Cymbal"),              119,

     TEXT ("Effets sonores"),

          TEXT ("Touchette"),           120,
          TEXT ("Respiration"),                121,
          TEXT ("Marée"),                    122,
          TEXT ("Chant d'oiseau"),                  123,
          TEXT ("Sonnerie de téléphone"),              124,
          TEXT ("Hélicoptère"),                  125,
          TEXT ("Applaudissements"),                    126,
          TEXT ("Coup de feu"),                     127 } ;

     // Données pour traduire les codes en octaves et notes
     // ----------------------------------------------------

#define NUMSCANS    (sizeof key / sizeof key[0])

struct
{
     int     iOctave ;
     int     iNote ;
     int     yPos ;
     int     xPos ;
     TCHAR * szKey ;
}
key [] =
{
                                         // Code  Cara  Oct  Note
                                         // ----  ----  ---  ----
         -1, -1, -1, -1, NULL,           //   0   Sans
         -1, -1, -1, -1, NULL,           //   1   Echa
         -1, -1,  0,  0, TEXT (""),      //   2     1
          5,  1,  0,  2, TEXT ("C#"),    //   3     2    5    C#
          5,  3,  0,  4, TEXT ("D#"),    //   4     3    5    D#
         -1, -1,  0,  6, TEXT (""),      //   5     4
          5,  6,  0,  8, TEXT ("F#"),    //   6     5    5    F#
          5,  8,  0, 10, TEXT ("G#"),    //   7     6    5    G#
          5, 10,  0, 12, TEXT ("A#"),    //   8     7    5    A#
         -1, -1,  0, 14, TEXT (""),      //   9     8
          6,  1,  0, 16, TEXT ("C#"),    //  10     9    6    C#
          6,  3,  0, 18, TEXT ("D#"),    //  11     0    6    D#
         -1, -1,  0, 20, TEXT (""),      //  12     -
          6,  6,  0, 22, TEXT ("F#"),    //  13     =    6    F#
         -1, -1, -1, -1, NULL,           //  14    Reto
          
         -1, -1, -1, -1, NULL,           //  15    Tab
          5,  0,  1,  1, TEXT ("C"),     //  16     q    5    C
          5,  2,  1,  3, TEXT ("D"),     //  17     w    5    D
          5,  4,  1,  5, TEXT ("E"),     //  18     e    5    E
          5,  5,  1,  7, TEXT ("F"),     //  19     r    5    F
          5,  7,  1,  9, TEXT ("G"),     //  20     t    5    G
          5,  9,  1, 11, TEXT ("A"),     //  21     y    5    A
          5, 11,  1, 13, TEXT ("B"),     //  22     u    5    B
          6,  0,  1, 15, TEXT ("C"),     //  23     i    6    C
          6,  2,  1, 17, TEXT ("D"),     //  24     o    6    D
          6,  4,  1, 19, TEXT ("E"),     //  25     p    6    E
          6,  5,  1, 21, TEXT ("F"),     //  26     [    6    F
          6,  7,  1, 23, TEXT ("G"),     //  27     ]    6    G
         -1, -1, -1, -1, NULL,           //  28    Ent
          
         -1, -1, -1, -1, NULL,           //  29    Ctrl
          3,  8,  2,  2, TEXT ("G#"),    //  30     a    3    G#
          3, 10,  2,  4, TEXT ("A#"),    //  31     s    3    A#
         -1, -1,  2,  6, TEXT (""),      //  32     d
          4,  1,  2,  8, TEXT ("C#"),    //  33     f    4    C#
          4,  3,  2, 10, TEXT ("D#"),    //  34     g    4    D#
         -1, -1,  2, 12, TEXT (""),      //  35     h
          4,  6,  2, 14, TEXT ("F#"),    //  36     j    4    F#
          4,  8,  2, 16, TEXT ("G#"),    //  37     k    4    G#
          4, 10,  2, 18, TEXT ("A#"),    //  38     l    4    A#
         -1, -1,  2, 20, TEXT (""),      //  39     ;
          5,  1,  2, 22, TEXT ("C#"),    //  40     '    5    C#
         -1, -1, -1, -1, NULL,           //  41     `
          
         -1, -1, -1, -1, NULL,           //  42    Maj
         -1, -1, -1, -1, NULL,           //  43     \  (pas de continuaion de ligne)
          3,  9,  3,  3, TEXT ("A"),     //  44     z    3    A
          3, 11,  3,  5, TEXT ("B"),     //  45     x    3    B
          4,  0,  3,  7, TEXT ("C"),     //  46     c    4    C
          4,  2,  3,  9, TEXT ("D"),     //  47     v    4    D
          4,  4,  3, 11, TEXT ("E"),     //  48     b    4    E
          4,  5,  3, 13, TEXT ("F"),     //  49     n    4    F
          4,  7,  3, 15, TEXT ("G"),     //  50     m    4    G
          4,  9,  3, 17, TEXT ("A"),     //  51     ,    4    A
          4, 11,  3, 19, TEXT ("B"),     //  52     .    4    B
          5,  0,  3, 21, TEXT ("C")      //  53     /    5    C
} ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     MSG      msg;
     HWND     hwnd ;
     WNDCLASS wndclass ;
     
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
     
     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Le programme nécessite Windows NT !"),
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName, TEXT ("Clavier MIDI"),
                          WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;

     if (!hwnd)
          return 0 ;
     
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd); 
     
     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

// Crée le menu (appelé de WndProc, WM_CREATE)
// ----------------------------------------------------------

HMENU CreateTheMenu (int iNumDevs)
{
     TCHAR       szBuffer [32] ;
     HMENU       hMenu, hMenuPopup, hMenuSubPopup ;
     int         i, iFam, iIns ;
     MIDIOUTCAPS moc ;
     
     hMenu = CreateMenu () ;
     
          // Crée le menu "État" 
     
     hMenuPopup = CreateMenu () ;
     
     AppendMenu (hMenuPopup, MF_STRING, IDM_OPEN, TEXT ("&Ouvert")) ; 
     AppendMenu (hMenuPopup, MF_STRING | MF_CHECKED, IDM_CLOSE, 
                             TEXT ("&Fermé")) ;
     
     AppendMenu (hMenu, MF_STRING | MF_POPUP, (UINT) hMenuPopup, 
                        TEXT ("&État")) ;
     
          // Crée le menu "Périphérique"
     
     hMenuPopup = CreateMenu () ;
     
          // Place le  MIDI Mapper dans le menu s'il est installé
     
     if (!midiOutGetDevCaps (MIDIMAPPER, &moc, sizeof (moc)))
          AppendMenu (hMenuPopup, MF_STRING, IDM_DEVICE + (int) MIDIMAPPER,
                         moc.szPname) ;
     else
          iDevice = 0 ;
     
          // Ajoute le reste des périphériques MIDI
     
     for (i = 0 ; i < iNumDevs ; i++)
     {
          midiOutGetDevCaps (i, &moc, sizeof (moc)) ;
          AppendMenu (hMenuPopup, MF_STRING, IDM_DEVICE + i, moc.szPname) ;
     }
     
     CheckMenuItem (hMenuPopup, 0, MF_BYPOSITION | MF_CHECKED) ;
     AppendMenu (hMenu, MF_STRING | MF_POPUP, (UINT) hMenuPopup, 
                        TEXT ("&Périphérique")) ;
     
          // Crée le menu  "Canal"
     
     hMenuPopup = CreateMenu () ;
     
     for (i = 0 ; i < 16 ; i++)
     {
          wsprintf (szBuffer, TEXT ("%d"), i + 1) ;
          AppendMenu (hMenuPopup, MF_STRING | (i ? MF_UNCHECKED : MF_CHECKED),
                                  IDM_CHANNEL + i, szBuffer) ;
     }
     
     AppendMenu (hMenu, MF_STRING | MF_POPUP, (UINT) hMenuPopup, 
                        TEXT ("&Canal")) ;
     
          // Crée le menu  "Voix" 
     
     hMenuPopup = CreateMenu () ;
     
     for (iFam = 0 ; iFam < 16 ; iFam++)
     {
          hMenuSubPopup = CreateMenu () ;
          
          for (iIns = 0 ; iIns < 8 ; iIns++)
          {
               wsprintf (szBuffer, TEXT ("&%d.\t%s"), iIns + 1,
                                   fam[iFam].inst[iIns].szInst) ;
               AppendMenu (hMenuSubPopup,
                           MF_STRING | (fam[iFam].inst[iIns].iVoice ?
                                             MF_UNCHECKED : MF_CHECKED),
                           fam[iFam].inst[iIns].iVoice + IDM_VOICE,
                           szBuffer) ;
          }
          
          wsprintf (szBuffer, TEXT ("&%c.\t%s"), 'A' + iFam,
                              fam[iFam].szFam) ;
          AppendMenu (hMenuPopup, MF_STRING | MF_POPUP, (UINT) hMenuSubPopup,
                                  szBuffer) ;
     }
     AppendMenu (hMenu, MF_STRING | MF_POPUP, (UINT) hMenuPopup, 
                        TEXT ("&Voix")) ;
     return hMenu ;
}

// Routines pour simplifier les sorties MIDI
// ------------------------------------

DWORD MidiOutMessage (HMIDIOUT hMidi, int iStatus, int iChannel,
                      int iData1,  int iData2)
{
     DWORD dwMessage ;
     
     dwMessage = iStatus | iChannel | (iData1 << 8) | (iData2 << 16) ;
     
     return midiOutShortMsg (hMidi, dwMessage) ;
}

DWORD MidiNoteOff (HMIDIOUT hMidi, int iChannel, int iOct, int iNote, int iVel)
{
     return MidiOutMessage (hMidi, 0x080, iChannel, 12 * iOct + iNote, iVel) ;
}

DWORD MidiNoteOn (HMIDIOUT hMidi, int iChannel, int iOct, int iNote, int iVel)
{
     return MidiOutMessage (hMidi, 0x090, iChannel, 12 * iOct + iNote, iVel) ;
}

DWORD MidiSetPatch (HMIDIOUT hMidi, int iChannel, int iVoice)
{
     return MidiOutMessage (hMidi, 0x0C0, iChannel, iVoice, 0) ;
}

DWORD MidiPitchBend (HMIDIOUT hMidi, int iChannel, int iBend)
{
     return MidiOutMessage (hMidi, 0x0E0, iChannel, iBend & 0x7F, iBend >> 7) ;
}

// Dessine une simple touche dans la fenêtre
// ---------------------------

VOID DrawKey (HDC hdc, int iScanCode, BOOL fInvert)
{
     RECT rc ;
     
     rc.left   = 3 * cxCaps * key[iScanCode].xPos / 2 + xOffset ;
     rc.top    = 3 * cyChar * key[iScanCode].yPos / 2 + yOffset ;
     rc.right  = rc.left + 3 * cxCaps ;
     rc.bottom = rc.top  + 3 * cyChar / 2 ;
     
     SetTextColor (hdc, fInvert ? 0x00FFFFFFul : 0x00000000ul) ;
     SetBkColor   (hdc, fInvert ? 0x00000000ul : 0x00FFFFFFul) ;
     
     FillRect (hdc, &rc, GetStockObject (fInvert ? BLACK_BRUSH : WHITE_BRUSH)) ;
     
     DrawText (hdc, key[iScanCode].szKey, -1, &rc,
                    DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
     
     FrameRect (hdc, &rc, GetStockObject (BLACK_BRUSH)) ;
}

// Traite le message Key Up ou Key Down  
// ------------------------------------

VOID ProcessKey (HDC hdc, UINT message, LPARAM lParam)
{
     int iScanCode, iOctave, iNote ;
     
     iScanCode = 0x0FF & HIWORD (lParam) ;
     
     if (iScanCode >= NUMSCANS)                       // pas de code au-delà de 53
          return ;
     
     if ((iOctave = key[iScanCode].iOctave) == -1)    // Pas de touche musicale
          return ;
     
     if (GetKeyState (VK_SHIFT) < 0)
          iOctave += 0x20000000 & lParam ? 2 : 1 ;
     
     if (GetKeyState (VK_CONTROL) < 0)
          iOctave -= 0x20000000 & lParam ? 2 : 1 ;
     
     iNote = key[iScanCode].iNote ;
     
     if (message == WM_KEYUP)                           // Pour Key Up
     {
          MidiNoteOff (hMidiOut, iChannel, iOctave, iNote, 0) ;   // Note off
          DrawKey (hdc, iScanCode, FALSE) ;
          return ;
     }
     
     if (0x40000000 & lParam)                          // ignore typematics
          return ;
     
     MidiNoteOn (hMidiOut, iChannel, iOctave, iNote, iVelocity) ; // Note on
     DrawKey (hdc, iScanCode, TRUE) ;                 // Dessine la clé inversée
}

// La procédure fenêtre
// ----------------

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static BOOL bOpened = FALSE ;
     HDC         hdc ;
     HMENU       hMenu ;
     int         i, iNumDevs, iPitchBend, cxClient, cyClient ;
     MIDIOUTCAPS moc ;
     PAINTSTRUCT ps ;
     SIZE        size ;
     TCHAR       szBuffer [16] ;
     
     switch (message)
     {
     case WM_CREATE:
               // Récupère la taille des majuscules dans la police système
          
          hdc = GetDC (hwnd) ;
          
          GetTextExtentPoint (hdc, TEXT ("M"), 1, &size) ;
          cxCaps = size.cx ;
          cyChar = size.cy ;
          
          ReleaseDC (hwnd, hdc) ;
          
               // Initialise la barre "Volume" 
          
          SetScrollRange (hwnd, SB_HORZ, 1, 127, FALSE) ;
          SetScrollPos   (hwnd, SB_HORZ, iVelocity, TRUE) ;
          
               // Initialise la barre "Pitch Bend" 
          
          SetScrollRange (hwnd, SB_VERT, 0, 16383, FALSE) ;
          SetScrollPos   (hwnd, SB_VERT, 8192, TRUE) ;
          
               // Récupère le nombre de périphériques de sortie MIDI  et définit le menu
          
          if (0 == (iNumDevs = midiOutGetNumDevs ()))
          {
               MessageBeep (MB_ICONSTOP) ;
               MessageBox (hwnd, TEXT ("Aucun périphérique de sortie MIDI !"),
                                 szAppName, MB_OK | MB_ICONSTOP) ;
               return -1 ;
          }
          SetMenu (hwnd, CreateTheMenu (iNumDevs)) ;
          return 0 ;
          
     case WM_SIZE:
          cxClient = LOWORD (lParam) ;
          cyClient = HIWORD (lParam) ;
          
          xOffset = (cxClient - 25 * 3 * cxCaps / 2) / 2 ;
          yOffset = (cyClient - 11 * cyChar) / 2 + 5 * cyChar ;
          return 0 ;
          
     case WM_COMMAND:
          hMenu = GetMenu (hwnd) ;
          
              // Commande "Ouvrir" du menu
          
          if (LOWORD (wParam) == IDM_OPEN && !bOpened)
          {
               if (midiOutOpen (&hMidiOut, iDevice, 0, 0, 0))
               {
                    MessageBeep (MB_ICONEXCLAMATION) ;
                    MessageBox (hwnd, TEXT ("Impossible d'ouvrir le périphérique MIDI"),
                                szAppName, MB_OK | MB_ICONEXCLAMATION) ;
               }
               else
               {
                    CheckMenuItem (hMenu, IDM_OPEN,  MF_CHECKED) ;
                    CheckMenuItem (hMenu, IDM_CLOSE, MF_UNCHECKED) ;
                    
                    MidiSetPatch (hMidiOut, iChannel, iVoice) ;
                    bOpened = TRUE ;
               }
          }
          
               // La commande "Fermer" du menu
          
          else if (LOWORD (wParam) == IDM_CLOSE && bOpened)
          {
               CheckMenuItem (hMenu, IDM_OPEN,  MF_UNCHECKED) ;
               CheckMenuItem (hMenu, IDM_CLOSE, MF_CHECKED) ;
               
                    // Efface toutes les touches et ferme le périphérique
               
               for (i = 0 ; i < 16 ; i++)
                    MidiOutMessage (hMidiOut, 0xB0, i, 123, 0) ;
               
               midiOutClose (hMidiOut) ;
               bOpened = FALSE ;
          }
          
               // Change la commande "périphérique" MIDI 
          
          else if (LOWORD (wParam) >= IDM_DEVICE - 1 && 
                   LOWORD (wParam) <  IDM_CHANNEL)
          {
               CheckMenuItem (hMenu, IDM_DEVICE + iDevice, MF_UNCHECKED) ;
               iDevice = LOWORD (wParam) - IDM_DEVICE ;
               CheckMenuItem (hMenu, IDM_DEVICE + iDevice, MF_CHECKED) ;
               
                    // Ferme et ouvre à nouveau le périphérique MIDI
               
               if (bOpened)
               {
                    SendMessage (hwnd, WM_COMMAND, IDM_CLOSE, 0L) ;
                    SendMessage (hwnd, WM_COMMAND, IDM_OPEN,  0L) ;
               }
          }
          
               // Change la commande "Canal" MIDI 
          
          else if (LOWORD (wParam) >= IDM_CHANNEL && 
                   LOWORD (wParam) <  IDM_VOICE)
          {
               CheckMenuItem (hMenu, IDM_CHANNEL + iChannel, MF_UNCHECKED);
               iChannel = LOWORD (wParam) - IDM_CHANNEL ;
               CheckMenuItem (hMenu, IDM_CHANNEL + iChannel, MF_CHECKED) ;
               
               if (bOpened)
                    MidiSetPatch (hMidiOut, iChannel, iVoice) ;
          }
          
               // Change la commande "Voix" MIDI 
          
          else if (LOWORD (wParam) >= IDM_VOICE)
          {
               CheckMenuItem (hMenu, IDM_VOICE + iVoice, MF_UNCHECKED) ;
               iVoice = LOWORD (wParam) - IDM_VOICE ;
               CheckMenuItem (hMenu, IDM_VOICE + iVoice, MF_CHECKED) ;
               
               if (bOpened)
                    MidiSetPatch (hMidiOut, iChannel, iVoice) ;
          }
          
          InvalidateRect (hwnd, NULL, TRUE) ;
          return 0 ;
          
          // traite un message Key Up ou Key Down 
          
     case WM_KEYUP:
     case WM_KEYDOWN:
          hdc = GetDC (hwnd) ;
          
          if (bOpened)
               ProcessKey (hdc, message, lParam) ;
          
          ReleaseDC (hwnd, hdc) ;
          return 0 ;
          
          // Pour Échap, efface toutes les notes et repeint.
          
     case WM_CHAR:
          if (bOpened && wParam == 27)
          {
               for (i = 0 ; i < 16 ; i++)
                    MidiOutMessage (hMidiOut, 0xB0, i, 123, 0) ;
               
               InvalidateRect (hwnd, NULL, TRUE) ;
          }
          return 0 ;
          
          // Rapidité du défilement horizontal
          
     case WM_HSCROLL:
          switch (LOWORD (wParam))
          {
          case SB_LINEUP:         iVelocity -= 1 ;  break ;
          case SB_LINEDOWN:       iVelocity += 1 ;  break ;
          case SB_PAGEUP:         iVelocity -= 8 ;  break ;
          case SB_PAGEDOWN:       iVelocity += 8 ;  break ;
          case SB_THUMBPOSITION:  iVelocity = HIWORD (wParam) ;  break ;
          default:                return 0 ;
          }
          iVelocity = max (1, min (iVelocity, 127)) ;
          SetScrollPos (hwnd, SB_HORZ, iVelocity, TRUE) ;
          return 0 ;
          
          // Pitch Bend du défilement vertical
     
     case WM_VSCROLL:
          switch (LOWORD (wParam))
          {
          case SB_THUMBTRACK:    iPitchBend = 16383 - HIWORD (wParam) ;  break ;
          case SB_THUMBPOSITION: iPitchBend = 8191 ;                     break ;
          default:               return 0 ;
          }
          iPitchBend = max (0, min (iPitchBend, 16383)) ;
          SetScrollPos (hwnd, SB_VERT, 16383 - iPitchBend, TRUE) ;
          
          if (bOpened)
               MidiPitchBend (hMidiOut, iChannel, iPitchBend) ;
          return 0 ;
     
     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;
          
          for (i = 0 ; i < NUMSCANS ; i++)
               if (key[i].xPos != -1)
                    DrawKey (hdc, i, FALSE) ;
               
          midiOutGetDevCaps (iDevice, &moc, sizeof (MIDIOUTCAPS)) ;
          wsprintf (szBuffer, TEXT ("Channel %i"), iChannel + 1) ;
     
          TextOut (hdc, cxCaps, 1 * cyChar, 
                        bOpened ? TEXT ("Open") : TEXT ("Closed"),
                        bOpened ? 4 : 6) ;
          TextOut (hdc, cxCaps, 2 * cyChar, moc.szPname,
                        lstrlen (moc.szPname)) ;
          TextOut (hdc, cxCaps, 3 * cyChar, szBuffer, lstrlen (szBuffer)) ;
          TextOut (hdc, cxCaps, 4 * cyChar,
                        fam[iVoice / 8].inst[iVoice % 8].szInst,
               lstrlen (fam[iVoice / 8].inst[iVoice % 8].szInst)) ;
     
          EndPaint (hwnd, &ps) ;
          return 0 ;
               
     case WM_DESTROY :
          SendMessage (hwnd, WM_COMMAND, IDM_CLOSE, 0L) ;
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

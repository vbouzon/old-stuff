#include <windows.h>
#include <stdlib.h>
#include <commdlg.h>
#include "resource.h"
#include <stdio.h>
#include <conio.h>

IMAGE_DOS_HEADER ff;
IMAGE_NT_HEADERS pe;
IMAGE_SECTION_HEADER *section;
OPENFILENAME ofn;
HINSTANCE hInst;
HWND hDialog, hTmp;
HANDLE source, result;
DWORD bytes;
char filename[1000], *tampon;
int tableS[256], tableK[256];
char tableau[256];
char *clef;
char buffer[256];

long Alignement(long taille, long alignement)
{
	if (taille % alignement != 0)
		taille = (taille - taille % alignement) + alignement;
	return taille;
}

long FindMinOffset()
{
	long a, min = section[0].PointerToRawData;
	for (a = 0; a < pe.FileHeader.NumberOfSections; a++)
	{
		if (section[a].PointerToRawData < min && section[a].PointerToRawData != 0)
		{
			min = section[a].PointerToRawData;
		}
	}
	return min;
}

long FindNextRVA()
{
	long a, max = 0, next = 0;
	for (a = 0; a < pe.FileHeader.NumberOfSections; a++)
	{
		if (section[a].VirtualAddress > max)
		{
			max = section[a].VirtualAddress;
			next = max + Alignement(section[a].Misc.VirtualSize, pe.OptionalHeader.SectionAlignment);
		}
	}
	return next;
}

long FindNextOffset()
{
	long a, max = 0, next = 0;
	for (a = 0; a < pe.FileHeader.NumberOfSections; a++)
	{
		if (section[a].PointerToRawData > max)
		{
			max = section[a].PointerToRawData;
			next = max + Alignement(section[a].SizeOfRawData, pe.OptionalHeader.FileAlignment);
		}
	}
	return next;
}

void swaper(int i, int j)
// On permute les elements d'indices i et j de TableS
{
	int temp;
	temp = tableS[i];
	tableS[i] = tableS[j];
	tableS[j] = temp;
}

int Crypte(char *tampon, long taille)
{
	char t;
	long i, j, c;

	// Creation de la table //
	for (i = 0; i < 256; i++)
		tableS[i] = i;
	for (i = 0; i < 256; i++)
		tableK[i] = clef[(i % sizeof(clef)) % 256];

	j = 0;
	for (i = 0; i < 256; i++)
	{
		j = (j + tableS[i] + tableK[i]) % 256;
		swaper(i, j);
	}
	for (i = 0; i < 256; i++)
		tableau[i] = tableS[i];

	// Cryptage //
	i = 0;
	j = 0;
	c = 0;

	do
	{
		i = (i + 1) % 256;
		j = (j + tableS[i]) % 256;
		swaper(i, j);
		t = tableS[(tableS[i] + tableS[j]) % 256];
		tampon[c] = tampon[c] ^ t;
		c++;
		taille--;
	} while (taille > 0);
	return 0;
}

void Reset()
{
	SendDlgItemMessage(hDialog, IDC_SECTION, CB_RESETCONTENT, 0, 0);
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ff, sizeof(IMAGE_DOS_HEADER));
	ZeroMemory(&pe, sizeof(IMAGE_NT_HEADERS));
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
}

int read()
{
	Reset();
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hDialog; // handle de ton appli ou fenêtre
	ofn.hInstance = NULL;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = 1000;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 1000;
	ofn.lpstrTitle = NULL;
	ofn.Flags = 0;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = "exe";
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	ofn.lpstrFilter = "Application (*.exe)\0*.exe\0";
	ofn.lpstrTitle = "Choisir l'application";

	if (!(GetOpenFileName(&ofn)))
	{
		MessageBox(NULL, TEXT("Impossible d'ouvrir le fichier !"), TEXT("Erreur"), MB_OK | MB_ICONWARNING);
		hTmp = GetDlgItem(hDialog, IDC_ECRIRE);
		EnableWindow(hTmp, FALSE);
		CloseHandle(source);
		return 0;
	}

	source = CreateFile(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);

	if (source == INVALID_HANDLE_VALUE)
		return 0;

	/*Lecture de l'entete MSDOS*/
	ReadFile(source, &ff, sizeof(IMAGE_DOS_HEADER), &bytes, 0);

	if (ff.e_magic != IMAGE_DOS_SIGNATURE)
	{
		MessageBox(NULL, TEXT("La signature MS DOS n'est pas bonne"), TEXT("Erreur"), MB_OK | MB_ICONWARNING);
		hTmp = GetDlgItem(hDialog, IDC_ECRIRE);
		EnableWindow(hTmp, FALSE);
		CloseHandle(source);
		return 0;
	}

	/*Lecture de l'entete NT*/
	SetFilePointer(source, ff.e_lfanew, 0, 0);

	ReadFile(source, &pe, sizeof(IMAGE_NT_HEADERS), &bytes, 0);

	if (pe.Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(NULL, TEXT("La signature PE n'est pas bonne"), TEXT("Erreur"), MB_OK | MB_ICONWARNING);
		hTmp = GetDlgItem(hDialog, IDC_ECRIRE);
		EnableWindow(hTmp, FALSE);
		CloseHandle(source);
		return 0;
	}

	section = new IMAGE_SECTION_HEADER[pe.FileHeader.NumberOfSections + 1];

	for (int s = 0; s < (pe.FileHeader.NumberOfSections); s++)
	{
		char nom[9] = "";
		ReadFile(source, &section[s], sizeof(IMAGE_SECTION_HEADER), &bytes, 0);
		if (!strncmp((char *)section[s].Name, (char *)".l0k", 8))
		{
			MessageBox(NULL, TEXT("Cet Appliaction PE a deja ete packl0ker"), TEXT("Erreur"), MB_OK | MB_ICONWARNING);
			hTmp = GetDlgItem(hDialog, IDC_ECRIRE);
			EnableWindow(hTmp, FALSE);
			CloseHandle(source);
			return 0;
		}
	}
	for (int a = 0; a < (pe.FileHeader.NumberOfSections); a++)
		SendDlgItemMessage(hDialog, IDC_SECTION, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)section[a].Name);

	hTmp = GetDlgItem(hDialog, IDC_ECRIRE);
	EnableWindow(hTmp, TRUE);
	CloseHandle(source);
	return 0;
}

int write()
{
	char section_a_crypter[7];
	GetDlgItemText(hDialog, IDC_SECTION, section_a_crypter, 7);

	for (int s = 0; s < (pe.FileHeader.NumberOfSections); s++)
	{
		if (!strncmp((char *)section[s].Name, (char *)section_a_crypter, 8))
			section[s].Characteristics = 0xC0000020;
	}

	// On modifie notre session
	strncpy((char *)&section[pe.FileHeader.NumberOfSections].Name, ".l0k", 8);
	section[pe.FileHeader.NumberOfSections].Misc.VirtualSize = pe.OptionalHeader.SectionAlignment;
	section[pe.FileHeader.NumberOfSections].VirtualAddress = FindNextRVA();
	section[pe.FileHeader.NumberOfSections].SizeOfRawData = pe.OptionalHeader.FileAlignment;
	section[pe.FileHeader.NumberOfSections].PointerToRawData = FindNextOffset();
	section[pe.FileHeader.NumberOfSections].PointerToRelocations = NULL;
	section[pe.FileHeader.NumberOfSections].PointerToLinenumbers = NULL;
	section[pe.FileHeader.NumberOfSections].NumberOfRelocations = NULL;
	section[pe.FileHeader.NumberOfSections].NumberOfLinenumbers = NULL;
	section[pe.FileHeader.NumberOfSections].Characteristics = 0xC0000020;

	// Sauvegarde L Original Entry Point
	unsigned long OEP;
	OEP = pe.OptionalHeader.AddressOfEntryPoint + pe.OptionalHeader.ImageBase;
	// Modifie L Entry Point
	pe.OptionalHeader.AddressOfEntryPoint = section[pe.FileHeader.NumberOfSections].VirtualAddress + 256;
	// On rajoute 1 au nb de sections
	pe.FileHeader.NumberOfSections += 1;
	// On rajoute la taille de notre section a la taille totale
	pe.OptionalHeader.SizeOfImage += pe.OptionalHeader.SectionAlignment;
	long addrtableau;
	addrtableau = pe.OptionalHeader.ImageBase + section[pe.FileHeader.NumberOfSections - 1].VirtualAddress;

	// Regarde si on a decaler ou pas
	long offset_min, header_size;
	offset_min = FindMinOffset();
	header_size = ff.e_lfanew + sizeof(IMAGE_NT_HEADERS) + sizeof(IMAGE_SECTION_HEADER) * pe.FileHeader.NumberOfSections;
	result = CreateFile("result.exe", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	WriteFile(result, &ff, sizeof(IMAGE_DOS_HEADER), &bytes, 0);
	SetFilePointer(result, ff.e_lfanew, 0, 0);
	WriteFile(result, &pe, sizeof(IMAGE_NT_HEADERS), &bytes, 0);
	WriteFile(result, section, sizeof(IMAGE_SECTION_HEADER) * (pe.FileHeader.NumberOfSections), &bytes, 0);

	unsigned long debut, taille, fin;
	source = CreateFile(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);

	// Pour chaque section on se placer a son debut et on lit la session puis on la reecrit dans l'autre fichier au meme endroit
	for (int a = 0; a < pe.FileHeader.NumberOfSections - 1; a++)
	{
		tampon = (char *)malloc(Alignement(section[a].SizeOfRawData, pe.OptionalHeader.FileAlignment));
		SetFilePointer(source, section[a].PointerToRawData, 0, 0);
		ReadFile(source, tampon, Alignement(section[a].SizeOfRawData, pe.OptionalHeader.FileAlignment), &bytes, 0);
		if (!strncmp((char *)section[a].Name, (char *)section_a_crypter, 8))
		{
			Crypte(tampon, Alignement(section[a].SizeOfRawData, pe.OptionalHeader.FileAlignment));
			debut = pe.OptionalHeader.ImageBase + section[a].VirtualAddress;
			taille = Alignement(section[a].SizeOfRawData, pe.OptionalHeader.FileAlignment);
			fin = debut + taille;
		}
		SetFilePointer(result, section[a].PointerToRawData, 0, 0);
		WriteFile(result, tampon, Alignement(section[a].SizeOfRawData, pe.OptionalHeader.FileAlignment), &bytes, 0);
		free(tampon);
	}

	// On rempli la taille de notre section par des 0
	for (a = section[pe.FileHeader.NumberOfSections - 1].PointerToRawData; a < (section[pe.FileHeader.NumberOfSections - 1].PointerToRawData + Alignement(section[pe.FileHeader.NumberOfSections - 1].SizeOfRawData, pe.OptionalHeader.FileAlignment)); a++)
		WriteFile(result, "\x00", 1, &bytes, 0);
	// On se place au debut de notre section et on ecrit notre session
	SetFilePointer(result, section[pe.FileHeader.NumberOfSections - 1].PointerToRawData, 0, 0);
	for (int i = 0; i < 256; i++)
		WriteFile(result, &tableau[i], 1, &bytes, 0);

	WriteFile(result, "\x6A\x00\x6A\x00\x33\xC9\xB9", 7, &bytes, 0);
	WriteFile(result, &debut, sizeof(debut), &bytes, 0);
	WriteFile(result, "\x81\xF9", 2, &bytes, 0);
	WriteFile(result, &fin, sizeof(fin), &bytes, 0);
	WriteFile(result, "\x5B\x58\x74\x43\xFE\xC0\x02\x98", 8, &bytes, 0);
	WriteFile(result, &addrtableau, sizeof(addrtableau), &bytes, 0);
	WriteFile(result, "\x51\x33\xC9\x8A\x8B", 5, &bytes, 0);
	WriteFile(result, &addrtableau, sizeof(addrtableau), &bytes, 0);
	WriteFile(result, "\x8A\x90", 2, &bytes, 0);
	WriteFile(result, &addrtableau, sizeof(addrtableau), &bytes, 0);
	WriteFile(result, "\x88\x93", 2, &bytes, 0);
	WriteFile(result, &addrtableau, sizeof(addrtableau), &bytes, 0);
	WriteFile(result, "\x88\x88", 2, &bytes, 0);
	WriteFile(result, &addrtableau, sizeof(addrtableau), &bytes, 0);
	WriteFile(result, "\x59\x33\xD2\x02\x90", 5, &bytes, 0);
	WriteFile(result, &addrtableau, sizeof(addrtableau), &bytes, 0);
	WriteFile(result, "\x02\x93", 2, &bytes, 0);
	WriteFile(result, &addrtableau, sizeof(addrtableau), &bytes, 0);
	WriteFile(result, "\x50\x53\x8A\x01\x8A\x9A", 6, &bytes, 0);
	WriteFile(result, &addrtableau, sizeof(addrtableau), &bytes, 0);
	WriteFile(result, "\x33\xC3\x88\x01\x41\xEB\xB3\x33\xC0\xB8", 10, &bytes, 0);
	WriteFile(result, &OEP, sizeof(OEP), &bytes, 0);
	WriteFile(result, "\xFF\xE0", 2, &bytes, 0);
	CloseHandle(result);
	CloseHandle(source);
	return 0;
}

LRESULT CALLBACK Mainform(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int command;
	hDialog = hDlg;
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		command = LOWORD(wParam);
		switch (command)
		{
		case IDC_OUVRIR:
			read();
			return TRUE;

		case IDC_ECRIRE:
			int i;
			GetDlgItemText(hDialog, IDC_CLE, buffer, 256);
			for (i = 0; buffer[i] != '\x00' && i < 256; i++)
			{
			}

			if (i != 0)
			{
				clef = (char *)malloc(i);
				strncpy(clef, buffer, i);
				write();
			}
			else
			{
				MessageBox(NULL, TEXT("Veuillez mettre une clé SVP"), TEXT("Erreur"), MB_OK | MB_ICONWARNING);
			}

			return TRUE;
		}
		break;

	case WM_CLOSE:
		exit(0);
		return TRUE;
	}
	return FALSE;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateDialog(hInst, (LPCTSTR)IDD_MAINFORM, NULL, (DLGPROC)Mainform);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{
	// TODO: Place code here.
	MSG msg;
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
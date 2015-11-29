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
HANDLE hFile;
DWORD Bytes;
char FileName[256];

void DoAnalyseFile()
{
	OPENFILENAME ofn;

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
			DoAnalyseFile();
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
	hInst = hInstance; // Store instance handle in our global variable
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

	return 666;
}
// ScanPort.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#pragma comment(lib, "wsock32.lib")

// Global Variables:
HINSTANCE hInst;
HWND hDialog, hTmp;
char host[200], portsscannedc[6];
int debut, fin, timer, portsscanned;
char buf[100];

// Foward declarations of functions included in this code module:
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK Mainform(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI ConnectPort(LPVOID p)
{

	int port = (int)p;
	portsscanned++;
	itoa(portsscanned, portsscannedc, 10);
	SetDlgItemText(hDialog, IDC_PORTSCANNED, (LPCTSTR)portsscannedc);
	struct sockaddr_in mysock;
	struct hostent *hoste;
	char portcourant[10], service[50];
	SendDlgItemMessage(hDialog, IDC_PROG, PBM_STEPIT, 0, 0);
	itoa((int)p, portcourant, 10);
	SetDlgItemText(hDialog, IDC_SCANNE, portcourant);
	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&mysock, 0, sizeof(mysock));
	if (!(hoste = gethostbyname(host)))
	{
		mysock.sin_addr.s_addr = inet_addr(host);
	}
	else
	{
		memcpy(&(mysock.sin_addr), hoste->h_addr, hoste->h_length);
	}
	mysock.sin_family = AF_INET;
	mysock.sin_port = htons(port);
	int test = connect(sock, (struct sockaddr *)&mysock, sizeof(mysock));
	if (test >= 0)
	{

		GetPrivateProfileString("SERVICES", portcourant, "Inconnu", service, sizeof(service), ".\\config.ini");
		char portservice[60];
		strcpy(portservice, portcourant);
		strcat(portservice, "-->");
		strcat(portservice, service);
		SendDlgItemMessage(hDialog, IDC_PORTSOPEN, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)portservice);
	}
	closesocket(sock);
	return 0;
}

DWORD WINAPI run(LPVOID arg)
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(1, 0), &WSAData);
	hTmp = GetDlgItem(hDialog, IDC_SCAN);
	EnableWindow(hTmp, FALSE);
	portsscanned = 0;
	for (debut; debut <= fin; debut++)
	{
		if (!(CreateThread(NULL, 0, ConnectPort, (LPVOID)debut, 0, NULL)))
		{
			MessageBox(hDialog, "Création du thread impossible...", "Erreur", 16);
		}
		Sleep(timer);
	}
	hTmp = GetDlgItem(hDialog, IDC_SCAN);
	EnableWindow(hTmp, TRUE);
	return 0;
}

DWORD WINAPI runalea(LPVOID arg)
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(1, 0), &WSAData);
	hTmp = GetDlgItem(hDialog, IDC_SCAN);
	EnableWindow(hTmp, FALSE);

	srand(time(NULL) * GetTickCount());
	int port = rand() % fin;
	if (port % 2)
	{
		port++;
	}

	srand(time(NULL));
	int pas = rand() % fin;
	if (!(pas % 2) || pas == 0)
	{
		pas++;
	}

	portsscanned = 0;

	for (int nbport = 0; nbport < fin; nbport++)
	{

		if (!(CreateThread(NULL, 0, ConnectPort, (LPVOID)port, 0, NULL)))
		{
			MessageBox(hDialog, "Création du thread impossible...", "Erreur", 16);
		}
		port += pas;
		if (port > fin)
			port = port - fin;
		Sleep(timer);
	}

	hTmp = GetDlgItem(hDialog, IDC_SCAN);
	EnableWindow(hTmp, TRUE);
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{
	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	InitCommonControls();
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SCANPORT);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
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

void scan()
{
	SendDlgItemMessage(hDialog, IDC_PORTSOPEN, LB_RESETCONTENT, 0, 0);

	GetDlgItemText(hDialog, IDC_IP, host, 100);
	WritePrivateProfileString("CONFIG", "Host", host, ".\\config.ini");

	GetDlgItemText(hDialog, IDC_PORTBEGIN, buf, 100);
	debut = atoi(buf);
	WritePrivateProfileString("CONFIG", "PortDebut", buf, ".\\config.ini");

	GetDlgItemText(hDialog, IDC_PORTEND, buf, 100);
	fin = atoi(buf);
	WritePrivateProfileString("CONFIG", "PortFin", buf, ".\\config.ini");

	GetDlgItemText(hDialog, IDC_TIMER, buf, 4);
	timer = atoi(buf);
	WritePrivateProfileString("CONFIG", "Timer", buf, ".\\config.ini");

	SendDlgItemMessage(hDialog, IDC_PROG, PBM_SETRANGE, 0, MAKELPARAM(((debut - 1) * 10), (fin * 10)));
	SendDlgItemMessage(hDialog, IDC_PROG, PBM_SETSTEP, (WPARAM)10, 0);

	LRESULT lResult = SendDlgItemMessage(hDialog, IDC_ALEA, BM_GETSTATE, 0, 0);

	if (lResult == BST_CHECKED)
	{
		if (!(CreateThread(NULL, 0, runalea, NULL, 0, NULL)))
		{
			MessageBox(hDialog, "Création du thread impossible...", "Erreur", 16);
		}
	}
	else
	{
		if (!(CreateThread(NULL, 0, run, NULL, 0, NULL)))
		{
			MessageBox(hDialog, "Création du thread impossible...", "Erreur", 16);
		}
	}
}

LRESULT CALLBACK Mainform(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int command;
	hDialog = hDlg;
	switch (message)
	{
	case WM_INITDIALOG:
		GetPrivateProfileString("CONFIG", "Host", "", buf, sizeof(buf), ".\\config.ini");
		SetDlgItemText(hDlg, IDC_IP, (LPCTSTR)buf);

		GetPrivateProfileString("CONFIG", "PortDebut", "", buf, sizeof(buf), ".\\config.ini");
		SetDlgItemText(hDlg, IDC_PORTBEGIN, (LPCTSTR)buf);

		GetPrivateProfileString("CONFIG", "PortFin", "", buf, sizeof(buf), ".\\config.ini");
		SetDlgItemText(hDlg, IDC_PORTEND, (LPCTSTR)buf);

		GetPrivateProfileString("CONFIG", "Timer", "", buf, sizeof(buf), ".\\config.ini");
		SetDlgItemText(hDlg, IDC_TIMER, (LPCTSTR)buf);

		hTmp = GetDlgItem(hDialog, IDC_PROG);

		SendDlgItemMessage(hDialog, IDC_PROG, PBM_SETBKCOLOR, 0, (LPARAM)(COLORREF)0x000000);
		SendDlgItemMessage(hDialog, IDC_PROG, PBM_SETBARCOLOR, 0, (LPARAM)(COLORREF)RGB(193, 190, 51));

		return TRUE;

	case WM_COMMAND:
		command = LOWORD(wParam);
		switch (command)
		{
		case IDC_SCAN:
			if (HIWORD(wParam) == BN_CLICKED)
				scan();
			return TRUE;

		case IDC_IP:
			return TRUE;

		case IDC_ALEA:
			return TRUE;

		case IDC_PORTBEGIN:
			return TRUE;

		case IDC_PORTEND:
			return TRUE;
		}
		break;

	case WM_CLOSE:
		WSACleanup();
		exit(0);
		return TRUE;
	}
	return FALSE;
}

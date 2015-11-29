#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>

char host[200];
unsigned int debut, fin;
HANDLE hThread;
/* _____   _   _   _____        _____   _____       ___       ___  ___
/*|  _  \ | | | | /  ___/      |_   _| | ____|     /   |     /   |/   |
/*| | | | | |_| | | |___         | |   | |__      / /| |    / /|   /| |
/*| | | | |  _  | \___  \        | |   |  __|    / / | |   / / |__/ | |
/*| |_| | | | | |  ___| |        | |   | |___   / /  | |  / /       | |
/*|_____/ |_| |_| /_____/        |_|   |_____| /_/   |_| /_/        |_|
/**********************************************************************/
/* Simple scanner de ports avec la methode Connect assez rapide car   */
/* le code est multithread.J'etais  à l'époque dans une team nommée TDF*/
/* comme vous pouvez le voir ;) Merci. http://www.dhs-team.org -b0l0k-*/
/**********************************************************************/

DWORD WINAPI ConnectPort(LPVOID p)
{
        int port = (int)p;
        struct sockaddr_in mysock;
        struct hostent *hoste;
        int sock;

        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock < 0)
        {
                return 0;
        };
        memset(&mysock, 0, sizeof(mysock));
        mysock.sin_family = AF_INET;
        if (!(hoste = gethostbyname(host)))
        {
                mysock.sin_addr.s_addr = inet_addr(host);
        }
        else
        {
                memcpy(&(mysock.sin_addr), hoste->h_addr, hoste->h_length);
        }
        mysock.sin_port = htons(port);
        int test = connect(sock, (struct sockaddr *)&mysock, sizeof(mysock));
        if (test < 0)
        {
                // printf("TCP Port %d ferme\n",port);
        }
        else
        {
                printf("TCP Port %d ouvert\n", port);
        }
        closesocket(sock);
        return 0;
}

int main(int argc, char *argv[])
{
        printf("_____________________________________________________\n"
               "|    TTTTTTTTTTT       DDDDD           FFFFFFFFF    |\n"
               "|    TTTTTTTTTTT       DD  DD          FFFFFFFFF    |\n"
               "|        TTT           DD   DD         FFF          |\n"
               "|        TTT           DD    DD        FFFFFFFFF    |\n"
               "|        TTT           DD    DD        FFFFFFFFF    |\n"
               "|        TTT           DD   DD         FFF          |\n"
               "|        TTT           DD  DD          FFF          |\n"
               "|________TTT___________DDDDD___________FFF__________|\n\n"
               "                  ScanPort by b0l0k                 \n"
               "          Usage: %s <host> (PortDebut) (Portfin)   \n"
               "      Port debut default = 1 && Port fin default = 1\n\n",
               argv[0]);

        startWSA();
        if (argc < 2)
                return 0;
        if (argc < 3)
        {
                debut = 1;
                fin = 1000;
        }
        else
        {
                debut = atoi(argv[2]);
                fin = atoi(argv[3]);
        }
        snprintf(host, 200, "%s", argv[1]);
        int nb_port = (fin - debut) + 1;
        for (debut; debut <= fin; debut++)
        {
                CreateThread(NULL, 0, ConnectPort, (LPVOID)debut, 0, NULL);
                sleep(1);
        }
        WSACleanup();
        printf("Le scan est terminé avec succes. Nb de ports scanne: %d", nb_port);
        return 0;
}

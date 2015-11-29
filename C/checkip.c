#include <winsock.h>
#include <stdio.h>
#include <string.h>

char ip[20];
char buffer[5000];

/* _____   _   _   _____        _____   _____       ___       ___  ___
/*|  _  \ | | | | /  ___/      |_   _| | ____|     /   |     /   |/   |
/*| | | | | |_| | | |___         | |   | |__      / /| |    / /|   /| |
/*| | | | |  _  | \___  \        | |   |  __|    / / | |   / / |__/ | |
/*| |_| | | | | |  ___| |        | |   | |___   / /  | |  / /       | |
/*|_____/ |_| |_| /_____/        |_|   |_____| /_/   |_| /_/        |_|
/**********************************************************************/
/* Un programme qui trouve votre ip avec laquelle vous vous connectez */
/* à internet grace à http://checkip.dyndns.org. Celui ci à été codé  */
/* pour un projet exterieur.          http://www.dhs-team.org -b0l0k- */
/**********************************************************************/

int main()
{
      int sockip;
      int test;
      struct sockaddr_in mysockip;
      struct hostent *hoste;
      WORD wVersionRequested;
      WSADATA WSAData;
      wVersionRequested = MAKEWORD(1, 1);
      WSAStartup(wVersionRequested, &WSAData);
      sockip = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
      memset(&mysockip, 0, sizeof(mysockip));
      mysockip.sin_family = AF_INET;
      hoste = gethostbyname("checkip.dyndns.org");
      memcpy(&(mysockip.sin_addr), hoste->h_addr, hoste->h_length);
      mysockip.sin_port = htons(80);
      test = connect(sockip, (struct sockaddr *)&mysockip, sizeof(mysockip));
      if (test == 0)
      {
            strcpy(buffer, "GET index.php HTTP/1.1\r\n\r\n\r\n");
            send(sockip, buffer, strlen(buffer), 0);
            memset(buffer, '\0', 5000);
            recv(sockip, buffer, 5000, 0);
            memset(buffer, '\0', 5000);
            recv(sockip, buffer, 5000, 0);

            int i = 76; //<html><head><title>Current IP Check</title></head><body>Current IP Address:
            int len = 0;
            int stop = 1;
            while (stop) // calcul du nombre de caractere a l'ip
            {
                  i++;
                  len++;
                  if (buffer[i] == '<')
                        stop = 0;
            }
            i = 76;
            int j = 0;
            for (len; len > 0; len--) // rempli char IP[20]
            {
                  ip[j] = buffer[i];
                  j++;
                  i++;
            }
      }
      closesocket(sockip);
      WSACleanup();
      printf("VOTRE IP: %s\n", ip);
      system("PAUSE");
      return 0;
}

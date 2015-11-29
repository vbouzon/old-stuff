#include <winsock.h>
#include <stdio.h>
#include <string.h>

/* _____   _   _   _____        _____   _____       ___       ___  ___
/*|  _  \ | | | | /  ___/      |_   _| | ____|     /   |     /   |/   |
/*| | | | | |_| | | |___         | |   | |__      / /| |    / /|   /| |
/*| | | | |  _  | \___  \        | |   |  __|    / / | |   / / |__/ | |
/*| |_| | | | | |  ___| |        | |   | |___   / /  | |  / /       | |
/*|_____/ |_| |_| /_____/        |_|   |_____| /_/   |_| /_/        |_|
/******************************************************************************/
/* Exemple d'un bot irc. Ce code a été mon "vrai" premier code en C depuis je */
/* ne l'ai pas corrigé. Soyez indulgent. Des failles sont peut etre          */
/* presentes. NE PAS OUBLIEZ de linker la lib ws2_32 -b0l0k-                  */
/******************************************************************************/

int sock;
char buffer[5000], buffer2[5000];
int test, test2, test3;
struct sockaddr_in mysock;
struct hostent *hoste;
char host[20] = "irc.quakenet.org", user[20] = "USER", chan[10] = "#mdfk";
int port, connected, joinchan;

char *google(char search[100])
{
  char buf[5000];
  static char reponse[1000] = "";
  int sockip;
  int test;
  struct sockaddr_in mysockip;
  sockip = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  memset(&mysockip, 0, sizeof(mysockip));
  mysockip.sin_family = AF_INET;
  hoste = gethostbyname("google.fr");
  memcpy(&(mysockip.sin_addr), hoste->h_addr, hoste->h_length);
  mysockip.sin_port = htons(80);
  test = connect(sockip, (struct sockaddr *)&mysockip, sizeof(mysockip));
  if (test == 0)
  {
    printf("Recherche.....\n");
    strcpy(buf, "GET ./search?hl=fr&ie=UTF-8&q=");
    strcat(buf, search);
    strcat(buf, "&meta=lr%3Dlang_fr&btnI=yes HTTP/1.1\n\n");
    send(sockip, buf, strlen(buf), 0);
    recv(sockip, buf, 5000, 0);
    printf("Reponse parse\n");
    if ((strstr(buf, "Location: ")) == NULL)
      return ("Pas de reponse");
    char *ptr = strstr(buf, "Location: ");
    char *ptr2 = strchr(ptr, '\n');
    int nb = (ptr2 - ptr) - 10;
    snprintf(reponse, nb, "%s", ptr + 10);
    return (reponse);
  }
  closesocket(sockip);
}

char *trouvearg(char buf[500])
{
  if ((strchr((strchr((strchr(buf, ':') + 1), ':')), ' ')) == NULL)
    return ("Aucun arg\n");
  char *ptr = strchr((strchr((strchr(buf, ':') + 1), ':')), ' ');
  char *ptr2 = strchr((strchr((strchr(buf, ':')), ':')), '\n');
  int nb = (ptr2 - ptr) - 2;
  if (nb <= 0)
    return ("Aucun arg\n");
  static char arg[100] = "";
  snprintf(arg, nb, "%s", ptr + 1);
  return (arg);
}

int verifident(char pseudo[30])
{
  if (strstr("b0l0k", pseudo) != NULL)
  {
    printf("oui.\nIdentifie:");
    strcpy(buffer2, "WHOIS ");
    strcat(buffer2, pseudo);
    strcat(buffer2, "\n");
    send(sock, buffer2, strlen(buffer2), 0);
    memset(buffer2, '\0', 5000);
    recv(sock, buffer2, 5000, 0);
    if ((strstr(((strstr(buffer2, chan)) + 3), chan)) != NULL)
    {
      printf("Tentative de bypassez l'authentification\n\n");
      return 0;
    }
    if ((strstr((strstr(buffer2, chan)), "is an identified user")) != NULL)
    {
      printf("oui\n\n");
      return 1;
    }
    printf("non.\n\n");
    return 0;
  }
  printf("non.\n\n");
  return 0;
}

char *trouvepseudo(char buf[500])
{
  char *ptr = strchr(buf, '!');
  int nb = (ptr - buf) - 1;
  static char pseudo[15] = "";
  memset(pseudo, 0, sizeof(pseudo));
  int i = 1;
  for (nb; nb > 0; nb--)
  {
    pseudo[i - 1] = buf[i];
    i++;
  }
  return (pseudo);
}

// FONCTION VOID
void notice(char pseudo[30], char message[1024])
{
  strcpy(buffer2, "NOTICE ");
  strcat(buffer2, pseudo);
  strcat(buffer2, " ");
  strcat(buffer2, message);
  strcat(buffer2, "\n");
  send(sock, buffer2, strlen(buffer2), 0);
  printf("Notice a %s : %s", pseudo, message);
}
void privmsgall(char message[5000])
{
  strcpy(buffer2, "PRIVMSG ");
  strcat(buffer2, chan);
  strcat(buffer2, " :");
  strcat(buffer2, message);
  strcat(buffer2, "\n");
  send(sock, buffer2, strlen(buffer2), 0);
  printf("PRIVMSG %s %s", chan, buffer2);
}

void giveop(char pseudo[30])
{
  strcpy(buffer2, "MODE ");
  strcat(buffer2, chan);
  strcat(buffer2, " +o ");
  strcat(buffer2, pseudo);
  strcat(buffer2, "\n");
  send(sock, buffer2, strlen(buffer2), 0);
}

void takeop(char pseudo[30])
{
  strcpy(buffer2, "MODE ");
  strcat(buffer2, chan);
  strcat(buffer2, " -o ");
  strcat(buffer2, pseudo);
  strcat(buffer2, "\n");
  send(sock, buffer2, strlen(buffer2), 0);
}

void givevoice(char pseudo[30])
{
  strcpy(buffer2, "MODE ");
  strcat(buffer2, chan);
  strcat(buffer2, " +v ");
  strcat(buffer2, pseudo);
  strcat(buffer2, "\n");
  send(sock, buffer2, strlen(buffer2), 0);
}

void quit(char msg[30])
{
  strcpy(buffer2, "QUIT :");
  strcat(buffer2, msg);
  strcat(buffer2, "\n");
  send(sock, buffer2, strlen(buffer2), 0);
}

// MAIN
int main()
{

  // WSA
  WORD wVersionRequested;
  WSADATA WSAData;
  int err;
  wVersionRequested = MAKEWORD(1, 1);
  err = WSAStartup(wVersionRequested, &WSAData);
  if (err == 0)
  {
    fprintf(stdout, "[+] Winsock a bien ete initialise\n");
  }
  else
  {
    fprintf(stdout, "[-] Erreur lors de l'initialisation de Winsock\n");
    return (-1);
  }

  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0)
  {
    return 1;
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
  mysock.sin_port = htons(6667);
  test = connect(sock, (struct sockaddr *)&mysock, sizeof(mysock));
  connected = 1;
  joinchan = 0;
  if (test < 0)
  {
    printf("[-] Impossible de se connecter => ");
    printf("2eme tentative dans 10s\n");
    Sleep(5000);
    test2 = connect(sock, (struct sockaddr *)&mysock, sizeof(mysock));
    if (test2 < 0)
    {
      printf("[-] Impossible de se connecter => ");
      printf("3eme tentative dans 10s\n");
      Sleep(5000);
      test3 = connect(sock, (struct sockaddr *)&mysock, sizeof(mysock));
      if (test3 < 0)
      {
        connected = 0;
      }
    }
  };

  if (connected == 1)
  {
    printf("[+] Connection reussie\n");
    strcpy(buffer, "NICK ");
    strcat(buffer, user);
    strcat(buffer, "\r\nUSER xxx xxx");
    strcat(buffer, host);
    strcat(buffer, " :ident\r\n");
    printf("[+] IDENT envoye\n");
    send(sock, buffer, strlen(buffer), 0);
    strcpy(buffer, "JOIN ");
    strcat(buffer, chan);
    strcat(buffer, "\r\n");
    send(sock, buffer, strlen(buffer), 0);
    printf("[+] CHAN rejoint\n");

    // boucle principale
    while (connected == 1)
    {
    depart:
      memset(buffer, '\0', 5000);
      memset(buffer2, '\0', 5000);
      recv(sock, buffer, 5000, 0);

      if (joinchan == 1)
      {

        if ((strstr(buffer, "JOIN :#CHAN")) != NULL)
        {
          char pseudo[50];
          strcpy(pseudo, trouvepseudo(buffer));
          printf("\n%s nous a rejoint.\n", pseudo);
          notice(pseudo, "Bienvenue~~~~~~~~Welcome.\n");
          printf("%s va etre voice\n", pseudo);
          givevoice(pseudo);
          goto depart;
        }

        if ((strstr(buffer, ":!op")) != NULL)
        {
          char pseudo[50];
          printf("\nDemande de Op par ");
          strcpy(pseudo, trouvepseudo(buffer));
          printf("%s.\nAdmin:", pseudo);
          if (verifident(pseudo) == 1)
          {
            char arg[100];
            strcpy(arg, trouvearg(buffer));
            if (strstr("Aucun arg\n", arg) != NULL)
            {
              printf("%s va etre op\n", pseudo);
              giveop(pseudo);
            }
            else
            {
              printf("%s va etre op\n", arg);
              giveop(arg);
            }
          }
          goto depart;
        }

        if ((strstr(buffer, ":!deop")) != NULL)
        {
          char pseudo[50];
          printf("\nDemande de Deop par ");
          strcpy(pseudo, trouvepseudo(buffer));
          printf("%s.\nAdmin:", pseudo);
          if (verifident(pseudo) == 1)
          {
            char arg[100];
            strcpy(arg, trouvearg(buffer));
            if (strstr("Aucun arg\n", arg) != NULL)
            {
              printf("%s va etre deop\n", pseudo);
              takeop(pseudo);
            }
            else
            {
              printf("%s va etre deop\n", arg);
              takeop(arg);
            }
          }
          goto depart;
        }

        if ((strstr(buffer, ":!voice")) != NULL)
        {
          char pseudo[50];
          printf("\nDemande de voice par ");
          strcpy(pseudo, trouvepseudo(buffer));
          printf("%s.\nAdmin:", pseudo);
          if (verifident(pseudo) == 1)
          {
            char arg[100];
            strcpy(arg, trouvearg(buffer));
            if (strstr("Aucun arg\n", arg) != NULL)
            {
              printf("%s va etre voice\n", pseudo);
              givevoice(pseudo);
            }
            else
            {
              printf("%s va etre voice\n", arg);
              givevoice(arg);
            }
          }
          goto depart;
        }

        if ((strstr(buffer, ":!google")) != NULL)
        {
          char pseudo[50];
          printf("\nDemande de recherche par ");
          strcpy(pseudo, trouvepseudo(buffer));
          printf("%s.\nAdmin:", pseudo);
          if (verifident(pseudo) == 1)
          {
            char demande[100];
            strcpy(demande, trouvearg(buffer));
            printf("Demande: %s\n", demande);
            char reponse[100];
            strcpy(reponse, google(demande));
            privmsgall(reponse);
          }
          goto depart;
        }

        if ((strstr(buffer, ":!quit")) != NULL)
        {

          char pseudo[50];
          char *ptr;
          ptr = (char *)&pseudo;
          printf("\nInsulte par ");
          strcpy(pseudo, trouvepseudo(buffer));
          printf("%s.\nAdmin:", pseudo);
          if (verifident(pseudo) == 1)
          {
            printf("Alors je part :-(...\n");
            quit("Message de quit");
            connected == 0;
          }
        }
      }

      if (!strncasecmp(buffer, "PING", 4))
      {
        buffer[1] = 'O';
        send(sock, buffer, strlen(buffer), 0);
        printf("PONG %s", buffer);
        goto depart;
      }

      if ((strstr(buffer, "JOIN :#TDF")) != NULL && joinchan == 0)
      {
        joinchan = 1;
        printf("[+]Canal Rejoint!\n");
      }
    }
    printf("Disconnect");
  };
  closesocket(sock);
  WSACleanup();
  system("PAUSE");
  return 0;
}

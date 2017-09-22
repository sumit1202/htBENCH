#include <stdio.h>  /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <string.h> /* memcpy, memset */
#include <unistd.h> /* read, write, close */

#include <sys/types.h>
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */

// Method that calls perror method and print out the error
void error(const char *msg)
{
  perror(msg);
  exit(0);
}

// Buffer to store the respons message
#define BUFFER_SIZE 6387162

int main(int argc, char *argv[])
{
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[BUFFER_SIZE];
  if (argc < 3)
  {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  server = gethostbyname(argv[1]);
  if (server == NULL)
  {
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
  }

  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(portno);

  if (connect(sockfd,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  n = write(sockfd, "GET /\r\n", strlen("GET /\r\n"));

  if (n < 0)
    error("ERROR writing to socket");

  bzero(buffer, BUFFER_SIZE);

  n = read(sockfd, buffer, BUFFER_SIZE - 1);

  if (n < 0)
    error("ERROR reading from socket");

  printf("%s\n", buffer);

  shutdown(sockfd, SHUT_RDWR);
  close(sockfd);
  return 0;
}

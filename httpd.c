#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define perror(s) write(2, s, sizeof(s))

int main(void) {
  struct sockaddr_in sockaddr = {0};
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  sockaddr.sin_port = htons(80);

  int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0) {
    perror("socket");
    return 1;
  }

  if (bind(sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0 ) {
    perror("bind");
    return 1;
  }

  if (listen(sock, 1024) < 0) {
    perror("listen");
    return 1;
  }

  while (1) {
    struct sockaddr clientAddr = {0};
    socklen_t clientAddrSize = sizeof(struct sockaddr);
    int clientSock = accept(sock, &clientAddr, &clientAddrSize);
    if (clientSock < 0) {
      perror("accept");
      return 1;
    }

    #define CONTENT "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 4\r\n\r\nYay!"
    send(clientSock, CONTENT, sizeof(CONTENT), 0);

    close(clientSock);
  }

  return 0;
}

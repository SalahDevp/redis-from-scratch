#include <arpa/inet.h>  // IP address conversion stuff
#include <netinet/in.h> // Internet-specific socket address structures
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h> // Core socket functions and data types
#include <sys/types.h>  // Definitions of data types used in system calls
#include <unistd.h>     // Miscellaneous POSIX definitions
//
#define MAX_MSG_LEN 255

const int SERVER_PORT = 8000;
const char SERVER_ADDRESS[] = "127.0.0.1";

int main(int argc, char *argv[]) {
  int clsfd = socket(AF_INET, SOCK_STREAM, 0);

  // set server address
  struct sockaddr_in srv_addr;
  bzero(&srv_addr, sizeof(srv_addr));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = htons(SERVER_PORT);
  inet_aton(SERVER_ADDRESS, &srv_addr.sin_addr);

  uint32_t msg_ln;
  char buf[MAX_MSG_LEN + 1];

  // connect to server
  if (connect(clsfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
    printf("error connecting to server\n");
    exit(1);
  }

  printf("enter your message:\n");
  fgets(buf, MAX_MSG_LEN + 1, stdin);
  msg_ln = strlen(buf);
  // remove \n
  if (buf[msg_ln - 1] == '\n') {
    buf[--msg_ln] = '\0';
  }

  // send length
  write(clsfd, &msg_ln, sizeof(msg_ln));
  // send msg to server
  if (write(clsfd, buf, msg_ln) != msg_ln) {
    printf("couldn't send message\n");
    exit(1);
  }
  printf("message: %s sent successfuly to %s:%d\n", buf, SERVER_ADDRESS,
         SERVER_PORT);

  close(clsfd);
  return 0;
}

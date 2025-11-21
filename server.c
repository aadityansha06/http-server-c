#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
 int main(int argc, char *argv[])
{
   
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Socket creation failed");
    return -1;
  }
  printf("Socket created successfully with file descriptor: %d\n", sockfd);
  struct sockaddr_in servaddr ,  client_addr;
  servaddr.sin_family = AF_INET; // ipv4
  servaddr.sin_addr.s_addr = INADDR_ANY; //allow any connection 
  servaddr.sin_port = htons (8080); // port is 8080


  if(bind(sockfd,(struct sockaddr*)&servaddr, sizeof(servaddr))>0){
    perror("Bind failed");
    return -1;  
  }
  printf("Bind successful\n");
   listen(sockfd, 5);
   printf("Listening on port 8080\n");
   socklen_t len = sizeof(client_addr);  // Use socklen_t, not long unsigned int

   
int clientfd =   accept(sockfd,(struct sockaddr *)&client_addr,&len);

if (clientfd<0)
{
    perror("Accept failed");
    return -1;  
}

printf("Connection accepted with client file descriptor: %d\n", clientfd);

char recived_msg[1024],method[100],file[200];
char requested_page[100];
 int n = recv(clientfd, recived_msg, sizeof(recived_msg)-1, 0);
 if (n < 0) {
    perror("Receive failed");
    return -1;
  }
  recived_msg[n] = '\0'; // Null-terminate the received string
  printf("Received message: %s\n", recived_msg);

  sscanf(recived_msg, "%s /%s HTTP/1.1", method,requested_page);

  snprintf(file,sizeof(file),"%s.html",requested_page);

 FILE *fp = fopen(file,"r");
  if (fp==NULL)
  {
    /* code */
    char *not_found_response = "HTTP/1.1 404 Not Found\r\n"
                              "Content-Type: text/html\r\n"
                              "\r\n"
                              "<html><body><h1>404 Not Found</h1></body></html>";
    send(clientfd, not_found_response, strlen(not_found_response), 0);
  }
  else
  {
    char response[4096];
    char buffer[1024];
    size_t bytes_read;

    // Read file content
    bytes_read = fread(buffer, 1, sizeof(buffer)-1, fp);
    buffer[bytes_read] = '\0'; // Null-terminate the file content

    // Create HTTP response
    snprintf(response, sizeof(response),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "\r\n"
             "%s", buffer);

    // Send response to client
    send(clientfd, response, strlen(response), 0);

    fclose(fp);
  }
    return 0;
}

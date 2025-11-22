#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>   // for close()
#include <signal.h>

int sockfd; 

void cleanup_handler(int sig) {
    printf("\nShutting down...\n");
    close(sockfd);  // This releases the port!
    exit(0);
}

int main(int argc, char *argv[])
{
   signal(SIGINT,cleanup_handler); // cathes ctrl+c for terminating and do cleanup before it 
  signal(SIGTERM,cleanup_handler);// Catches kill command
  signal(SIGTSTP, cleanup_handler); // cathes ctrl+z do cleanup
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Socket creation failed");
    return -1;
  }
  printf("Socket created successfully with file descriptor: %d\n", sockfd);





int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(sockfd);
        return -1;
    }
    printf("SO_REUSEADDR enabled\n");

  struct sockaddr_in servaddr ,  client_addr;
  servaddr.sin_family = AF_INET; // ipv4
  servaddr.sin_addr.s_addr = INADDR_ANY; //allow any connection 
  servaddr.sin_port = htons (8080); // port is 8080

  if(bind(sockfd,(struct sockaddr*)&servaddr, sizeof(servaddr))<0){
    perror("Bind failed");
    close(sockfd);         
    return -1;  
  }
  printf("Bind successful\n");
   listen(sockfd, 5);
   printf("Listening on port 8080\n");
   socklen_t len = sizeof(client_addr);  // Use socklen_t, not long unsigned int
   int clientfd ;
while (1)
{
   
clientfd =   accept(sockfd,(struct sockaddr *)&client_addr,&len);

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

 char username[50],password[50];
 size_t content_length = 0;


char *cl_ptr = strstr(recived_msg, "Content-Length:");
if (cl_ptr != NULL) {
    sscanf(cl_ptr, "Content-Length: %zu", &content_length);
}

  char *body = strstr(recived_msg, "\r\n\r\n");
  if (body != NULL) {
    body += 4; // Move past the "\r\n\r\n"
    // Now 'body' points to the start of the body content
    // Extract username and password from body
    sscanf(body,"username=%49[^&]&password=%49s", username, password);
  }





  /*
  %49s means:

Read a string

But read at most 49 characters

Then add a \0 terminator automatically

So the destination buffer should be at least 50 bytes
  */
  
    char response[4096];
    char buffer[1024];
    size_t bytes_read;
  char response_paylod[1024];

  snprintf(response_paylod,sizeof(response_paylod),
           "<html><body><h1>Login Successful</h1>"
           "<p>Welcome  %s</p>"
           
           "</body></html>",username);
           size_t payload_length = strlen(response_paylod);
    // Read file content
    // bytes_read = fread(buffer, 1, sizeof(buffer)-1, fp);
    // buffer[bytes_read] = '\0'; // Null-terminate the file content

    // Create HTTP response
     snprintf(response, sizeof(response),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html\r\n"
                 "Access-Control-Allow-Origin: *\r\n"
                 "Access-Control-Allow-Headers: *\r\n"
                 "Content-Length: %zu\r\n"
                 "Connection: close\r\n"  // Fixed: Added \r\n
                 "\r\n"
                 "%s",
                 payload_length, response_paylod);

    // Send response to client
    send(clientfd, response, strlen(response), 0);
 close(clientfd);


}

     
 


    return 0;
}

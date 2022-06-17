 #include <stdio.h>
 #include <errno.h>
 #include <netinet/in.h> 
 #include <netdb.h> 

 //#define DATA_BUFFER "Mona Lisa was painted by Leonardo da Vinci"

 char buffer[1024];

 int main () {
    struct sockaddr_in saddr;
    int fd, ret_val;
    struct hostent *local_host; /* need netdb.h for this */

    /* Step1: create a TCP socket */
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (fd == -1) {
        fprintf(stderr, "socket failed [%s]\n", strerror(errno));
        return -1;
    }
    printf("Created a socket with fd: %d\n", fd);

    /* Let us initialize the server address structure */
    saddr.sin_family = AF_INET;         
    saddr.sin_port = htons(5000);     
    local_host = gethostbyname("127.0.0.1");
    saddr.sin_addr = *((struct in_addr *)local_host->h_addr);

    /* Step2: connect to the TCP server socket */
    ret_val = connect(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (ret_val == -1) {
        fprintf(stderr, "connect failed [%s]\n", strerror(errno));
        close(fd);
        return -1;
    }
    printf("The Socket is now connected\n");

    sprintf(buffer,"frequency 4\n");
    //sleep(5);

    /* Next step: send some data */
    ret_val = send(fd,buffer, sizeof(buffer), 0);
    printf("Successfully sent data (len %d bytes): %s\n", 
                ret_val, buffer);

    /* Last step: close the socket */
    close(fd);
    return 0;
 }

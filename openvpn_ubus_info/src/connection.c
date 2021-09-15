#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <fcntl.h>

#define DATA_SIZE 1000
static int unix_socket;

int connect_unix_socket(char *server_path, char *server_name)
{
    int rc, len;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char buf[256];
    char *client_path;

    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));

    //Create a UNIX domain stream socket
    unix_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (unix_socket == -1){
        syslog(LOG_ERR, "Failed to create UNIX socket");
        exit(1);
    }

    client_sockaddr.sun_family = AF_UNIX;
    asprintf(&client_path, "/var/run/manage_%s.sock", server_name);
    strcpy(client_sockaddr.sun_path, client_path);
    len = sizeof(client_sockaddr);

    unlink(client_path);
    rc = bind(unix_socket, (struct sockaddr *)&client_sockaddr, len);
    if (rc == -1){
        syslog(LOG_ERR, "Failed to bind");
        close(unix_socket);
        exit(1);
    }

    // Set up the UNIX sockaddr structure for the server socket and connect to it.
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, server_path);
    rc = connect(unix_socket, (struct sockaddr *)&server_sockaddr, len);
    if (rc == -1){
        syslog(LOG_ERR, "Failed to connect");
        close(unix_socket);
        exit(1);
    }
    return 0;
}

// check if connection to socket is still up
int check_socket_connection_status(void)
{
    int error = 0;
    socklen_t len = sizeof(error);
    int retval = getsockopt(unix_socket, SOL_SOCKET, SO_ERROR, &error, &len);

    if (retval != 0){
        syslog(LOG_ERR, "Failed to get socket");
        return 1;
    }
    if (error != 0){
        syslog(LOG_ERR, "Socket error");
        return 1;
    }

    return 0;
}

//Send data to server, handle partial send
int send_managment_command(char *buffer, int *len)
{
    if (send(unix_socket, buffer, *len, 0) >=0)
        return 0;
    return -1;
}

//Receive data from server. If mode is set to 1 flush data
char *receve_data_from_server(int mode)
{
    char *data;

    if (fcntl(unix_socket, F_SETFL, O_NONBLOCK) == -1)
        return NULL;

    data = (char *)malloc(sizeof(char) * DATA_SIZE);
    if (data == NULL)
        return NULL;


    recv(unix_socket, data, DATA_SIZE, 0);
    if (mode == 1 && data != NULL){
        free(data);
        data == NULL;
    }
    
    return data;
}

void connection_close(){
    close(unix_socket);
}
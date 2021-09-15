#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>

#include "openvpn_ubus.h"
#include "main.h"
#include "connection.h"

int main(int argc , char *argv[])
{   
    int rc = 0; 
    sleep(1); //Wait for server to startup
    rc = check_arguments(argc, argv);
    if (rc != 0)
    return 1;

    rc = connect_unix_socket(server_config.server_path,server_config.name); //connect to socket
    if (rc == 1)
            return 2;

    else if (rc == 2)
            goto cleanup;

    rc = process_ubus();    //start ubus

    cleanup:
    connection_close();
    return rc;
}

static int check_arguments(int argc , char *argv[])
{
    if (argc < 2) {
        syslog(LOG_ERR,"Not enough arguments");
        return 1;
    }
    if (argc >= 2) {
        sprintf(server_config.name, "openvpn.%s", argv[1]);
        sprintf(server_config.server_path, argv[2]);
    }
    return 0;
}
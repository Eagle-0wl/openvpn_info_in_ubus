#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>

#include "ubus.h"
#include "main.h"
#include "connection.h"

int main(int argc , char *argv[])
{   
    int rc = 0; 
    sleep(1); //Wait for server to startup
    rc = check_arguments(argc, argv);
    if (rc != 0)
    return 1;

    rc = connect_unix_socket(server_settings.server_path);
    if (rc == 1)
            return 2;

    else if (rc == 2)
            goto cleanup;

    rc = process_ubus();

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
        sprintf(server_settings.name, "openvpn.%s", argv[1]);
        sprintf(server_settings.server_path, argv[2]);
    }
    return 0;
}
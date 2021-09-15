#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ubus.h"
#include "helpers.h"
#include "main.h"
#include "linked_list.h"

//remove \n and \r from string
static void remove_char(char *s)
{
    int writer = 0, reader = 0;

    while (s[reader])
    {
        if (s[reader]!= '\n' && s[reader] != '\r') 
        {   
        s[writer++] = s[reader];
        }
        reader++;    
    }
    s[writer]=0;
}

//   split line into parts
//   0 - find clients that disconnect and delete them from linked list
//   1 - only update clients informaion
static void line_split_into_parts(char *string, int mode)
{
    char temp_string[80];
    char *token = NULL;
    char *rest = NULL;
    int counter = 0;
    struct Clients *new_client;
    struct Clients temp_client;
    strcpy(temp_string, string);

    for (token = strtok_r(temp_string, ",", &rest);
            token != NULL;
            token = strtok_r(NULL, ",", &rest)) {
            remove_char(token);
            if (counter == 0) 
                    strcpy(temp_client.name, token);
            else if (counter == 1)
                    strcpy(temp_client.address, token);
            else if (counter == 2)
                    strcpy(temp_client.bytes_received, token);
            else if (counter == 3)
                    strcpy(temp_client.bytes_sent, token);
            else if (counter == 4)
                    strcpy(temp_client.date, token); 
            counter++;  
    }
    new_client = create_client(temp_client);
    push_client(&clients, new_client);
}


//split message at every new line
static void message_split_on_newline(char *string, int mode)
{
    char *token;

    while((token = strtok_r(string, "\n", &string))) {
        line_split_into_parts(token, mode);
    }
}

int check_if_not_exist(struct Clients temp_client)
{
    struct Clients *point_to_first = NULL; //!< temporary node, helps us always start loop from first element
    point_to_first = clients;
    while (point_to_first != NULL) {
        if (strcmp(temp_client.address, point_to_first->address) == 0) {
                return 0;
        }
        point_to_first = point_to_first->next;
    }
    return 1;
}




//counts clients from the response message.
static int client_count(char *string)
{
    int count = 0;
    string = strchr(string, '\n');

    while (string != NULL) {
            string = strchr(string+1, '\n');
            count++;
    }

    return (count-8)/2;
}

//parse received message and remove unnecessary characters
static int parse_message(char *message)
{
    char *message_beginning = NULL;
    char *message_end = NULL;
    char *client_info = NULL;
    char *line;
    int clients_count = 0;
    static int prev_clients_count = 0;
    
    clients_count = client_count(message);
    // if (clients_count == 0) {
    //         if (clients != NULL) {
    //                 delete_list();
    //                 clients = NULL;
    //         }
    //         prev_clients_count = 0;
    //         return 1;
    // }
    delete_list();
    message_beginning = strstr(message, "Since\r\n")+7;
    message_end = strstr(message, "ROUTING TABLE\r\n");

    if(message_beginning == NULL || message_end == NULL)
            return 2;
    client_info  = (char*) malloc ( ((strlen(message)-293)*sizeof(char)) * (clients_count)); //we will be removing -294 character +1 character for \0 
    if(client_info == NULL)
            return 3;

    memmove(client_info, message_beginning, message_end - message_beginning-1);

    client_info[message_end - message_beginning-1] = '\0';
    if (prev_clients_count < clients_count){
        while((line = strtok_r(client_info, "\n", &client_info))) {
            line_split_into_parts(line, 0);
        }
    }else {
        while((line = strtok_r(client_info, "\n", &client_info))) {
            line_split_into_parts(line, 1);
        }
    }
    free(client_info);
    prev_clients_count = clients_count;
    return 0;
}

//request status from server,receve server responce call parsing function to parse data, 
int gather_status()
{
    char *send_message;
    char *received_message;
    int len = 0;
    int rc = 0; //return code
    receve_data_from_server(1); //catch unwanted server responses
    send_message = (char *) malloc(sizeof("status\n"));
    strcpy(send_message, "status\n");
    len = strlen(send_message);
    if (send_message == NULL){
        syslog(LOG_ERR, "Failed to allocate memmory for message");
        return 3;
    }
    send_managment_command(send_message, &len);
    received_message = receve_data_from_server(0);
    if (received_message == NULL) {
            rc = 4;
            syslog(LOG_ERR, "Failed to allocate memmory for receve message");
            goto cleanup_1;
    }
        
    rc = parse_message(received_message);
    free(received_message);
    cleanup_1:
            free(send_message);
    return rc;
}
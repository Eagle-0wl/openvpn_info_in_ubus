#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
static void parse_data_from_line(char *string)
{
    char temp_string[80];
    char *token = NULL;
    char *rest = NULL;
    int counter = 0;
    struct Clients tmp_client;
    strcpy(temp_string, string);

    for (token = strtok_r(temp_string, ",", &rest);
            token != NULL;
            token = strtok_r(NULL, ",", &rest)) {
            remove_char(token);
            if (counter == 0) 
                    strcpy(tmp_client.name, token);
            else if (counter == 1)
                    strcpy(tmp_client.address, token);
            else if (counter == 2)
                    strcpy(tmp_client.bytes_received, token);
            else if (counter == 3)
                    strcpy(tmp_client.bytes_sent, token);
            else if (counter == 4)
                    strcpy(tmp_client.date, token); 
            counter++;  
    }
    insert_client_into_linked_list(tmp_client); 
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
    delete_list();
    message_beginning = strstr(message, "Since\r\n")+7;
    message_end = strstr(message, "ROUTING TABLE\r\n");

    if(message_beginning == NULL || message_end == NULL)
            return 2;
    client_info = (char*) malloc (((strlen(message)-280)*sizeof(char)) * (clients_count)); //we will be removing -281 character +1 character for \0 
    if(client_info == NULL)
            return 3;

    memmove(client_info, message_beginning, message_end - message_beginning-1);

    client_info[message_end - message_beginning-1] = '\0';
    while((line = strtok_r(client_info, "\n", &client_info))) {
        parse_data_from_line(line);
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
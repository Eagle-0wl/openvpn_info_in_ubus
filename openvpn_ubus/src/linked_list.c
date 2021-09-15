#include <string.h>
#include <stdlib.h>

#include "ubus.h"
#include "linked_list.h"


// delete list
void delete_list()
{
struct Clients* tmp = clients;
while(clients){ 
        clients = clients->next;
        free (tmp);
        tmp = clients;
    }
    clients = NULL;
}

/**
 * push client to linked list
 */
void push_client(struct Clients **client, struct Clients *new)
{
    if (*client != NULL) {
            new->next = *client;
    }
    *client = new;
}

/**
 * allocate memory for linked list
 */
struct Clients *create_client(struct Clients temp_client)
{
    struct Clients *client;
    client = (struct Clients *) malloc(sizeof(struct Clients));
    if (client == NULL)
            return client;
    strcpy(client->name, temp_client.name);
    strcpy(client->address, temp_client.address);
    strcpy(client->bytes_received, temp_client.bytes_received);
    strcpy(client->bytes_sent, temp_client.bytes_sent);
    strcpy(client->date, temp_client.date);
    client->next = NULL;

    return client;
}
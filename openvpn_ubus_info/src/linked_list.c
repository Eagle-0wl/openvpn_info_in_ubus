#include <string.h>
#include <stdlib.h>

#include "linked_list.h"
//add client to linked list
void insert_client_into_linked_list(struct Clients temp_client) {
    struct Clients* link = (struct Clients*)malloc(sizeof(struct Clients));
    strcpy(link->name, temp_client.name);
    strcpy(link->address, temp_client.address);
    strcpy(link->bytes_received, temp_client.bytes_received);
    strcpy(link->bytes_sent, temp_client.bytes_sent);
    strcpy(link->date, temp_client.date);
    
    link->next = clients;
    clients = link;
}
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
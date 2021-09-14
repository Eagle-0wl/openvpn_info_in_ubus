#include <string.h>
#include <stdlib.h>

#include "ubus.h"
#include "linked_list.h"

void insert_client_into_linked_list(struct Clients temp_client) {

    struct Clients *link = (struct Clients*) malloc(sizeof(struct Clients));
    strcpy(link->name, temp_client.name);
    strcpy(link->address, temp_client.address);
    strcpy(link->bytes_received, temp_client.bytes_received);
    strcpy(link->bytes_sent, temp_client.bytes_sent);
    strcpy(link->date, temp_client.date);
    link->next = clients; //point it to old first node
    clients = link;       //point first to new first node

    link->next = NULL;
}
/**
 * delete linked list
 */
void delete_list()
{
struct Clients* tmp = clients;

while(clients != NULL) { 
        free(clients->name);
        free(clients->address);       
        free(clients->bytes_received);  
        free(clients->bytes_sent);  
        free(clients->date);           
        tmp = clients;
        clients = clients->next;
        free (tmp);
    }
}

//    struct Clients* next;
 
//    while (current != NULL)
//    {
//        next = current->next;
//        free(current);
//        current = next;
//    }
   //clients = NULL;

//     // struct Clients *temp = clients;
//     struct Clients *tmp;
//     // while (temp != NULL) {
//     //         next = temp->next;
//     //         free(temp);
//     //         temp = next;
//     // }

//     while(clients != NULL) { 
//         free(clients->name);
//         free(clients->address);       
//         free(clients->bytes_received);  
//         free(clients->bytes_sent);  
//         free(clients->date);           
//         tmp= clients;
//         clients = clients->next;
//         free (tmp);
//     }


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

void delete_node(struct Clients** head_ref, char *string)
{
    struct Clients *temp = *head_ref, *prev;

    if (temp != NULL && strcmp(temp->address, string) == 0 || strcmp(temp->name, string) == 0) {
        *head_ref = temp->next;
        free(temp);
        return;
    }
    
    while (temp != NULL && strcmp(temp->address, string) != 0 && strcmp(temp->name, string) != 0) {
        prev = temp;
        temp = temp->next;
    }
 
    if (temp == NULL)
        return;
 
    prev->next = temp->next;
 
    free(temp);
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
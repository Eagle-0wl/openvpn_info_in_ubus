struct Clients {
        char name[40];
        char address[40];
        char bytes_received[25];
        char bytes_sent[25];
        char date[40];
        struct Clients *next;
};

struct Clients *clients;
void push_client(struct Clients **client, struct Clients *new);
struct Clients *create_client(struct Clients temp_client);
void delete_list();
void delete_node(struct Clients** head_ref, char *ip);
void insert_client_into_linked_list(struct Clients temp_client);

struct Clients {
        char name[40];
        char address[40];
        char bytes_received[25];
        char bytes_sent[25];
        char date[40];
        struct Clients *next;
};

struct Clients *clients;
struct Clients *create_client(struct Clients temp_client);
void delete_list();
void insert_client_into_linked_list(struct Clients temp_client);
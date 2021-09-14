
int connect_unix_socket();
int check_socket_connection_status(void);
int send_managment_command(char *buffer, int *len);
char *receve_data_from_server(int mode);
void connection_close();
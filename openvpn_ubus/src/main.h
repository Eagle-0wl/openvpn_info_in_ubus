struct settings{
    char name[20];      //you can't input more than 8 symbols in website
    char server_path[35]; //name cant be longer than 20, and predetermined path is /var/run/
};

struct settings server_settings;
static int check_arguments(int argc, char *argv[]);
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "state.h"
#include "utils.h"

int get_res_code(res_code *res)
{
    char scode[3] = {res->n1, res->n2, res->n3};
    return atoi(scode);
}

void logging(logtype logtype, const char *format, ...)
{
    char *logs[] = {"", "LOG", "SVR"};
    va_list args;

    va_start(args, format);

    if (logtype != NONE)
        printf("\n[%s]\t", logs[logtype]); // prints a log prefix

    vprintf(format, args); // calls printf with format + args

    va_end(args);
    fflush(stdout); // flushes out the printed stuff
}

ftp_uri *parse_arguments(char *uri)
{

    ftp_uri *data = malloc(sizeof(ftp_uri));
    memset(data, 0, sizeof(ftp_uri));

    // parses the args uri
    int res = sscanf(uri, "ftp://%[^:]:%[^@]@%[^/]/%s",
                     data->username,
                     data->password,
                     data->host,
                     data->path);

    if (res != 4) // assumes username and password were not provided and tries again
    {

        strcpy(data->username, "anonymous"); // default username value if not provided
        strcpy(data->password, "password");  // default password value if not provided

        res = sscanf(uri, "ftp://%[^/]/%s",
                     data->host,
                     data->path);

        if (res != 2) // if another error occurred then finally aborts
        {
            logging(LOG, "Wrong arguments format\n");
            exit(1);
        }
    }

    int n = strlen(data->path);
    char *suffix = data->path + n; // parses the filename as a suffix of the path

    while (0 < n && data->path[--n] != '/')
        ;

    if (data->path[n] == '/')
    {
        suffix = data->path + n + 1;
        data->path[n] = '\0';
    }

    if (strlen(suffix) > 0)
        strcpy(data->filename, suffix);
    else
        strcpy(data->filename, data->path);

    logging(NONE, "\tUsername: %s\n\tPassword: %s\n\tHost: %s\n\tPath: %s\n\tFilename: %s\n",
            data->username,
            data->password,
            data->host,
            data->path,
            data->filename);

    return data;
}

struct hostent *get_ip(char *host)
{
    struct hostent *host_entity = malloc(sizeof(struct hostent));
    memset(host_entity, 0, sizeof(struct hostent));

    if ((host_entity = gethostbyname(host)) == NULL) // gets host ip address by its name
    {
        herror("Error calling gethostbyname function");
        exit(1);
    }

    logging(NONE, "\tHost Ip Address: %s\n",
            inet_ntoa(
                *(
                    (struct in_addr *)
                        host_entity->h_addr)));

    return host_entity;
}

int get_response(int socket_fd, res_code *code)
{
    char c;
    state st = INIT;

    printf("\n");

    while (st != STOP)
    {
        if (read(socket_fd, &c, 1) != 1) // reads char by char the server response
        {
            logging(LOG, "Error reading response from ftp server\n");
            return 1;
        }

        printf("%c", c);

        st = getState(c, st, code); // evaluates the server response and returns a new state
    }

    printf("\n");

    return 0;
}

int send_command(int socket_fd, char *cmd, char *buffer, res_code *code)
{
    int res = 0;

    do
    {
        if (cmd != NULL)
            write(socket_fd, cmd, strlen(cmd)); // sends a command to the server

        logging(SERVER, "Server Response:");

        if (buffer != NULL) // if buffer is not NULL, it will be filled with the server response
        {
            freopen("/dev/null", "a", stdout);
            setbuf(stdout, buffer); // sends stdout data to buffer
        }

        res = get_response(socket_fd, code);

        if (buffer != NULL)
        {
            freopen("/dev/tty", "a", stdout); // restores stdout
            printf("%s", buffer);
        }
    } while (code->n1 == '4'); // server code to ask for resending command

    if (code->n1 == '5') // server code to abort operation
        return 1;

    return res;
}

int parse_connection(char *buffer, char *ip_address)
{
    char dummy[100];
    char ip1[10], ip2[10], ip3[10], ip4[10];
    char p1[10], p2[10];
    char ip_addr[50];
    int port1, port2;

    int res = sscanf(buffer, "%[^(](%[^,],%[^,],%[^,],%[^,],%[^,],%[^)])", dummy, ip1, ip2, ip3, ip4, p1, p2);

    sprintf(ip_addr, "%s.%s.%s.%s", ip1, ip2, ip3, ip4);

    if (res != 7 || strcmp(ip_addr, ip_address) != 0)
        return -1;
    else
        return atoi(p1) * 256 + atoi(p2); // calculates the new port number
}

int server_init(int socket_fd)
{
    logging(SERVER, "Server Welcome Response:\n");

    res_code code;
    if (get_response(socket_fd, &code) != 0 && get_res_code(&code) != 220)
        return 1;

    logging(LOG, "Server is ready\n");
    return 0;
}

int download_file(int socket_client_fd, char *filename)
{

    int fd = open(filename, O_CREAT | O_RDWR, 0777);

    char buffer[1000];
    int bytes;
    while ((bytes = read(socket_client_fd, buffer, 1000)) > 0)
    {
        bytes = write(fd, buffer, bytes);
    }

    close(fd);
    return 0;
}
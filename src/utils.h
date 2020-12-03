#ifndef UTILS_H
#define UTILS_H

#define MAXLEN 100
#define PORT 21

/**
 * Logging Types
 */
typedef enum
{
	NONE,
	LOG,
	SERVER
} logtype;

/**
 * Struct to store username credentials and request info
 */
typedef struct
{
	char username[MAXLEN];
	char password[MAXLEN];
	char host[MAXLEN];
	char path[MAXLEN];
	char filename[MAXLEN];
} ftp_uri;

/**
 * Struct to store server 3 digit response code
 */
typedef struct
{
    char n1;
    char n2;
    char n3;
} res_code;

/**
 * Server response evaluation states
 */
typedef enum
{
    INIT,
    N1,
    N2,
    N3,
    LAST,
    STOP
} state;

/**
 * Gets the server response code string as an integer
 * @param res server response code struct
 * @return integer representation of the response code
*/
int get_res_code(res_code *res);

/**
 * printf Wrapper for logging
 * @param logtype
 * @param format printf first arg
 * @param ...
*/
void logging(logtype logtype, const char *format, ...);

/**
 * Parses the command line arguments
 * @param uri ftp url syntax-like string
 * @return pointer to a ftp_uri data struct
*/
ftp_uri *parse_arguments(char *uri);


/**
 * Gets the host entity info
 * @param host ftp host
 * @return pointer to a hostent struct
*/
struct hostent *get_ip(char *host);


/**
 * Gets a server response
 * @param socket_fd socket file descriptor to be read
 * @param code 3 digit response code
 * @return 0 if success, 1 otherwise
*/
int get_response(int socket_fd, res_code *code);


/**
 * Sends a command to the server and waits for a response
 * @param socket_fd socket file descriptor to be read
 * @param cmd command to be sent to the server
 * @param buffer to fill with the entire server response string
 * @param code 3 digit response code
 * @return 0 if success, 1 otherwise
*/
int send_command(int socket_fd, char *cmd, char *buffer, res_code *code);

/**
 * Parses the buffer string with the address of the new connection
 * for downloading the file
 * @param buffer string containing an address of the form (ip1,ip2,ip3,ip4,p1,p2)
 * @param ip_address original ip address to compare with the response
 * @return new port number if success, -1 otherwise
*/
int parse_connection(char *buffer, char *ip_address);

/**
 * Reads the server welcome message
 * @param socket_fd socket file descriptor to be read
 * @return 0 if successful, 1 otherwise
*/
int server_init(int socket_fd);

/**
 * Downloads the file from socket_client_fd
 * @param socket_client_fd stream transmitting the file
 * @param filename name of the file to be downloaded
 * @return 0 if successful
*/
int download_file(int socket_client_fd, char *filename);

#endif

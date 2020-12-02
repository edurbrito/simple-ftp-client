#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "state.h"
#include "utils.h"

int main(int argc, char **argv)
{

	if (argc != 2)
	{
		logging(LOG, "Wrong number of arguments");
		exit(1);
	}

	struct hostent *host_entity;

	logging(LOG, "FTP CLIENT - Input Data:\n-------------------------------------------\n");

	ftp_uri *data = parse_arguments(argv[1]);
	host_entity = get_ip(data->host);

	logging(NONE, "-------------------------------------------\n");

	int socket_fd;
	int socket_client_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in server_addr_client;

	// server address handling
	bzero((char *)&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	// 32 bit Internet address network byte ordered
	server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)host_entity->h_addr))); 
	// server TCP port must be network byte ordered
	server_addr.sin_port = htons(PORT);

	// opens a TCP socket
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket()");
		exit(0);
	}

	// connects to the server
	if (connect(socket_fd,
				(struct sockaddr *)&server_addr,
				sizeof(server_addr)) < 0)
	{
		perror("connect()");
		exit(0);
	}

	if (server_init(socket_fd) != 0)
	{
		logging(LOG, "Server is not ready\n");
		exit(1);
	}

	res_code code;

	char user_cmd[100];
	char pass_cmd[100];
	sprintf(user_cmd, "user %s\n", data->username);
	sprintf(pass_cmd, "pass %s\n", data->password);

	logging(LOG, "Sending Username");
	if (send_command(socket_fd, user_cmd, NULL, &code) != 0 || get_res_code(&code) != 331)
	{
		logging(LOG, "Could not set username\n");
		exit(1);
	}
	else
	{
		logging(LOG, "Sending Password");
		if (send_command(socket_fd, pass_cmd, NULL, &code) != 0 || get_res_code(&code) != 230)
		{
			logging(LOG, "Could not set password\n");
			exit(1);
		}
		logging(LOG, "Password Received\n");
	}

	char pasv_cmd[] = "pasv\n";
	char *buffer = malloc(1000 * sizeof(char));

	logging(LOG, "Requesting Passive mode");
	if (send_command(socket_fd, pasv_cmd, buffer, &code) != 0 || get_res_code(&code) != 227)
	{
		logging(LOG, "Could not enter passive mode\n");
		exit(1);
	}

	logging(LOG, "In Passive mode\n");

	int port;

	if ((port = parse_connection(buffer, inet_ntoa(*((struct in_addr *)host_entity->h_addr)))) < 0)
	{
		logging(LOG, "Could not get new port in the same address\n");
		exit(1);
	}

	// server address handling
	bzero((char *)&server_addr_client, sizeof(server_addr_client));

	server_addr_client.sin_family = AF_INET;
	// 32 bit Internet address network byte ordered
	server_addr_client.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)host_entity->h_addr))); 
	// server TCP port must be network byte ordered
	server_addr_client.sin_port = htons(port); 

	// opens a TCP socket
	if ((socket_client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket()");
		exit(1);
	}

	// connects to the server
	if (connect(socket_client_fd,
				(struct sockaddr *)&server_addr_client,
				sizeof(server_addr_client)) < 0)
	{
		perror("connect()");
		exit(1);
	}

	char retr_cmd[200];
	sprintf(retr_cmd, "retr %s/%s\n", data->path, data->filename);

	logging(LOG, "Requesting Retrieve File");
	if (send_command(socket_fd, retr_cmd, NULL, &code) != 0 || get_res_code(&code) != 150)
	{
		logging(LOG, "Could not request File\n");
		exit(1);
	}
	logging(LOG, "Started Downloading File");

	if(download_file(socket_client_fd, data->filename) != 0)
	{
		logging(LOG, "Could not download File\n");
		exit(1);
	}

	logging(LOG, "Completed File Download");
	if (send_command(socket_fd, NULL, NULL, &code) != 0 || get_res_code(&code) != 226)
	{
		logging(LOG, "An error occurred. Terminating Connection\n");
	}

	logging(LOG, "Closing Connection\n");

	fflush(stdout);
	free(data);
	free(buffer);
	close(socket_fd);
	close(socket_client_fd);
	exit(0);
}

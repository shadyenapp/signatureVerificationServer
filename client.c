/****************************
 *  Hayden Sapp
 *  2024-02-25 -> 2024-03-15
 *  client.c
 ****************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 50

/*Send data to server*/
void send_file(FILE *fp, int sockfd){
    int n;
    unsigned char data[SIZE] = {};
    /*While the tarball is not the end of file*/
    while(!feof(fp)) {
        /*read one hex byte from the tarball*/
        fread(data, 1, 1, fp);
        /*Send the byte*/
        if (send(sockfd, data, sizeof(data), 0) == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
        /*Zero out the buffer*/
        bzero(data, SIZE);
    }
}

int main(){
    /*localhost*/
    char *ip = "127.0.0.1";
    /*Port 8080 as default*/
    int port = 8080;
    int e;
    /*socket structures*/
    int sockfd;
    struct sockaddr_in server_addr;

    /*tarball*/
    FILE *fp;
    char *filename = "files.tar.gz";

    /*Make a socket*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Client socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    /*connect to the bound server socket*/
    e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e == -1) {
        perror("[-]Error in socket");
        exit(1);
    }
	printf("[+]Connected to Server.\n");

    /*open the tarball listed in filename*/
    fp = fopen(filename, "rb+");
    if (fp == NULL) {
        perror("[-]Error in reading file.");
        exit(1);
    }
    /*send data to server*/
    send_file(fp, sockfd);
    printf("[+]File data sent successfully.\n");
    /*close connection*/
	printf("[+]Closing the connection.\n");
    /*clost socket*/
    close(sockfd);
    /*close file pointer*/
    fclose(fp);
    return 0;
}
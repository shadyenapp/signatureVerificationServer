/****************************
 *  Hayden Sapp
 *  2024-02-25 -> 2024-03-15
 *  server.c
 ****************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SIZE 50

/*Write to File*/
void write_file(int sockfd){
    int n;
    FILE *fp;
    char *filename = "files.tar.gz";
    unsigned char buffer[SIZE] = {};
    /*open an empty file to write the tarball to*/
    fp = fopen(filename, "wb+");
    while (1) {
        /*recieve data from client*/
        n = recv(sockfd, buffer, SIZE, 0);
        if (n <= 0){
            break;
            return;
        }
        /*Write one hex byte at a time*/
        fwrite(buffer, 1, 1, fp);
        /*Clear buffer for next byte*/
        bzero(buffer, sizeof(buffer));
     }
    /*close file for future use*/
    fclose(fp);
    return;
}
/*unzip tarball*/
void unzip(void){
    printf("[+]Unzipping File\n");
    /*Unzips the only tar.gz in the directory (regardless of name)*/
    /*If there are multiple tarballs change $(ls | grep gz) to the name of the tarball*/
    system("tar -xvzf $(ls | grep gz) > /dev/null");
    printf("[+]File Unzipped\n");
}
/*Signature verification and running of script*/
void verifySignature(void) {
    FILE *fp;
    char buffer[12];
    int value;
    printf("[+]Verifying Signature\n");
    /*Verify script from signature using openssl and save it to a file called verification.txt*/
    system("openssl dgst -sha256 -verify public.pem -signature sig script.sh > verification.txt");
    fp = fopen("verification.txt", "r");
    fgets(buffer, 12, fp);
    /*If the result from the openssl command contains Verified OK, it will run the script*/
    value = strcmp(buffer, "Verified OK");
    if (value == 0) {
        printf("[+]Script is Verified to be OK\n");
        printf("[+]Running Script-------\n\n");
        /*Add execute commands to script in case*/
        system("chmod +x script.sh > /dev/null");
        /*run the script*/
        system("./script.sh");
        printf("\n[+]Script Complete-----\n");
    }
    else {
        printf("[-]Script is not Verified\n");
    }
}

int main(){
    /*localhost*/
    char *ip = "127.0.0.1";
    /*port 8080 as default*/
    int port = 8080;
    int e;
    /*socket structures*/
    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    /*bind socket to IP*/
    e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e < 0) {
        perror("[-]Error in bind");
        exit(1);
    }
    printf("[+]Binding successfull.\n");

    if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}
    else{
		perror("[-]Error in listening");
        exit(1);
	}
    /*accept the connection*/
    addr_size = sizeof(new_addr);    
    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
    /*write data from client*/
    write_file(new_sock);
    printf("[+]Data written in the file successfully.\n");
    /*unzip data from client*/
    unzip();
    /*Verify and run data*/
    verifySignature();
    return 0;
}

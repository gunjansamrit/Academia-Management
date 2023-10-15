#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<fcntl.h>
char takeOption(char* prompts,int client_socket)
{
    char buffer[1024];
    char command_buffer[1024];
    int buffer_index = 0;
    for(int i=0;i<1;i++) 
    {
        send(client_socket,prompts,strlen(prompts),0);
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) 
        {
            break;
        }
        for (int i = 0; i < bytes_received; i++) 
        {
            if (buffer[i] == '\n') 
            {
                command_buffer[buffer_index] = '\0';  
                buffer_index = 0;
            } 
            else 
            {
                command_buffer[buffer_index++] = buffer[i];
            }
        }
    }
    return command_buffer[0];
}
// void notify(int client_socket,char *prompt)
// {
//     send(client_socket,prompt,strlen(prompt),0);
//     return;
// }

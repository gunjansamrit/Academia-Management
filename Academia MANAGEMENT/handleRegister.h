#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<fcntl.h>
#include "components.h"
int getNextStudentID() 
{
    int fd = open("student.txt", O_RDONLY);
    if (fd == -1) 
    {
        return 0; 
    }
    off_t offset = lseek(fd, 0, SEEK_END); 
    int numStudents = offset / sizeof(struct Student);
    close(fd);
    return numStudents; 
}
int getNextFacultyID() 
{
    int fd = open("faculty.txt", O_RDONLY);
    if (fd == -1) 
    {
        return 0; 
    }
    off_t offset = lseek(fd, 0, SEEK_END); 
    int numStudents = offset / sizeof(struct Faculty);
    close(fd);
    return numStudents; 
}


void convertIndexToID(int index, char* id) 
{
        snprintf(id, 4, "%03d", index); 
}
void writeFacultyToFile(int client_socket,const struct Faculty* fac) 
{
    int fd = open("faculty.txt", O_WRONLY | O_APPEND);
    if (fd == -1) 
    {
        perror("Error opening file");
        return;
    }

    if (write(fd, fac, sizeof(struct Faculty)) != sizeof(struct Faculty)) 
    {
        perror("Error writing to file");
        return;
    }
    char *prompt="Registered Successfully\n";
    send(client_socket,prompt,strlen(prompt),0);
    close(fd);
}
void registerFaculty(int client_socket)
{
    struct Faculty fac;
    char buffer[1024];
    char command_buffer[1024];
    int buffer_index = 0;
    char *prompts[4]={"Enter Name : \n","Enter department : \n","Enter email : \n","Enter password : \n"};
    for(int k=0;k<4;k++) 
    {
        send(client_socket,prompts[k],strlen(prompts[k]),0);
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) 
        {
            break;
        }
        for (int i = 0; i < bytes_received; i++) 
        {
            if (buffer[i] == '\n') 
            {
                fac.details[k][buffer_index] = '\0';  
                buffer_index = 0;
            } 
            else 
            {
                fac.details[k][buffer_index++] = buffer[i];
            }
        }
    }
    char id[4];
    int nextId=getNextFacultyID();
    if(nextId>=1000)
    {
        send(client_socket,"Maximum faculty limit reached.\nCannot Register\n",strlen("Maximum faculty limit reached.\nCannot Register\n"),0);
        return;
    }
    convertIndexToID(nextId,id);
    id[3]='\0';
    strncpy(fac.details[4],id,4);
    writeFacultyToFile(client_socket,&fac);
}
void writeStudentToFile(int client_socket,const struct Student* stu) 
{
    int fd = open("student.txt", O_WRONLY | O_APPEND);
    if (fd == -1) 
    {
        perror("Error opening file");
        return;
    }

    if (write(fd, stu, sizeof(struct Student)) != sizeof(struct Student)) 
    {
        perror("Error writing to file");
    }
    char *prompt="Regisetred Successfully\n";
    send(client_socket,prompt,strlen(prompt),0);
    close(fd);
}
void registerStudent(int client_socket)
{
    struct Student stu;
    char buffer[1024];
    char command_buffer[1024];
    int buffer_index = 0;
    char *prompts[4]={"Enter Name : \n","Enter department : \n","Enter email : \n","Enter password : \n"};
    for(int k=0;k<4;k++) 
    {
        send(client_socket,prompts[k],strlen(prompts[k]),0);
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) 
        {
            break;
        }
        for (int i = 0; i < bytes_received; i++) 
        {
            if (buffer[i] == '\n') 
            {
                stu.details[k][buffer_index] = '\0';  
                buffer_index = 0;
            } 
            else 
            {
                stu.details[k][buffer_index++] = buffer[i];
            }
        }
    }
    char id[4];
    int nextId=getNextStudentID();
    if(nextId>=1000)
    {
        send(client_socket,"Maximum students limit reached.\nCannot Register\n",strlen("Maximum students limit reached.\nCannot Register\n"),0);
        return;
    }
    convertIndexToID(nextId,id);
    id[3]='\0';
    strncpy(stu.details[4],id,4);
    writeStudentToFile(client_socket,&stu);
}


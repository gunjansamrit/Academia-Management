#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<fcntl.h>
void addCourse()
{
    printf("add course\n");
}
void editCourse()
{
    printf("edit course\n");
}
void changePasswordFaculty(int client_socket,struct Faculty *fac)
{
    char buffer[1024];
    char command_buffer[1024];
    char passwords[2][1024];
    int buffer_index = 0;
    char *prompts[2]={"Enter New Password\n","Confirm New Password\n"};
    while(1)
    {
        for(int k=0;k<2;k++) 
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
                    passwords[k][buffer_index] = '\0';  
                    buffer_index = 0;
                } 
                else 
                {
                    passwords[k][buffer_index++] = buffer[i];
                }
            }
        }
        if(strcmp(passwords[0],passwords[1])==0)
        {
            strcpy(fac->details[3],passwords[0]);
            int fd = open("faculty.txt", O_RDWR);
            int index = atoi(fac->details[4]);
            off_t offset = index * sizeof(struct Faculty);
            if (fd == -1||lseek(fd, offset, SEEK_SET) == -1) 
            {
                char *prompt="Failed\n";
                send(client_socket,prompt,strlen(prompt),0);
                break;
            }
            struct Faculty temp;
            // off_t offset = 0;
            // int updated = 0;
            // while (read(fd, &temp, sizeof(struct Faculty)) == sizeof(struct Faculty)) 
            // {
            //     if (strcmp(temp.details[4], fac->details[4]) == 0) 
            //     {
            //         lseek(fd, offset, SEEK_SET);
            //         write(fd, fac, sizeof(struct Faculty));
            //         updated = 1;
            //         break;
            //     }
            //     offset = lseek(fd, 0, SEEK_CUR);
            // }

            // close(fd);
            // char *prompt="Password changed Succefully\n";
            // send(client_socket,prompt,strlen(prompt),0);
            // break;
            char *prompt;
            if (read(fd, &temp, sizeof(struct Faculty)) == sizeof(struct Faculty)) 
            {
                if (strcmp(temp.details[4], fac->details[4]) == 0) 
                {
                    lseek(fd, offset, SEEK_SET);
                    if(write(fd, fac, sizeof(struct Faculty))==sizeof(struct Faculty))
                    {
                        prompt="Password changed Succefully\n";
                    }
                    else
                    {
                        prompt="Failed\n";
                    }
                }
                else
                {
                    prompt="Invaild name\n";
                }
            }
            else
            {
                prompt="Failed\n";
            }
            send(client_socket,prompt,strlen(prompt),0);
            close(fd);
            break;
        }
        else
        {
            char *prompt="Passwords do not match\n";
            send(client_socket,prompt,strlen(prompt),0);
        }
    }
}
void viewCourses()
{
    printf("view courses\n");
}
void handleFacultyLoginSuccess(int client_socket,struct Faculty *fac)
{
    send(client_socket,"Login Successfull.....\n",strlen("Login Successfull.....\n"),0);
    char *prompt="What would you like to do??\nAdd Course(Enter 1)\nEdit Course(Enter 2)\nChange Password(Enter 3)\nView Courses(Enter 4)\nLogOut(Enter 0)\n";
    int login=1;
    while(login)
    {
        char role=takeOption(prompt,client_socket);
        switch(role)
        {
            case '1':
                addCourse();
                break;
            case '2':
                editCourse();
                break;
            case '3':
                changePasswordFaculty(client_socket,fac);
                break;
            case '4':
                viewCourses();
                break;
            case '0':
                login=0;
                break;
            default:
                break;
        }
    }
}
void handleFacultyLoginFail(int client_socket)
{
    //notify(client_socket,"Login Fail\n");
    char *prompt="Login Fail\n";
    send(client_socket,prompt,strlen(prompt),0);
    return;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<fcntl.h>
void enrollCourse()
{
    printf("enroll course\n");
}
void deenrollCourse()
{
    printf("de enroll course\n");
}
void changePasswordStudent(int client_socket,struct Student *stu)
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
            strcpy(stu->details[3],passwords[0]);
            int fd = open("student.txt", O_RDWR);
            int index = atoi(stu->details[4]);
            off_t offset = index * sizeof(struct Student);
            if (fd == -1||lseek(fd, offset, SEEK_SET) == -1) 
            {
                char *prompt="Failed\n";
                send(client_socket,prompt,strlen(prompt),0);
                break;
            }
            struct Student temp;
            char *prompt;
            if (read(fd, &temp, sizeof(struct Student)) == sizeof(struct Student)) 
            {
                if (strcmp(temp.details[4], stu->details[4]) == 0) 
                {
                    lseek(fd, offset, SEEK_SET);
                    if(write(fd, stu, sizeof(struct Student))==sizeof(struct Student))
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
void viewRegisteredCourses()
{
    printf("view registered courses\n");
}
void modifyDetails()
{
    printf("Modify details\n");
}
void handleStudentLoginSuccess(int client_socket,struct Student *stu)
{
    send(client_socket,"Login Successfull.....\n",strlen("Login Successfull.....\n"),0);
    char *prompt="What would you like to do??\nEnroll to a Course(Enter 1)\nDe-enroll to a Course(Enter 2)\nChange Password(Enter 3)\nView registered Courses(Enter 4)\nModify your details(Enter 5)\nLogOut(Enter 0)\n";
    int login=1;
    while(login)
    {
        char role=takeOption(prompt,client_socket);
        switch(role)
        {
            case '1':
                enrollCourse();
                break;
            case '2':
                deenrollCourse();
                break;
            case '3':
                changePasswordStudent(client_socket,stu);
                break;
            case '4':
                viewRegisteredCourses();
                break;
            case '5':
                modifyDetails();
                break;
            case '0':
                login=0;
                break;
            default:
                break;
        }
    }
}
void handleStudentLoginFail(int client_socket)
{
    //notify(client_socket,"Login Fail\n");
    char *prompt="Login Fail\n";
    send(client_socket,prompt,strlen(prompt),0);
    return;
}

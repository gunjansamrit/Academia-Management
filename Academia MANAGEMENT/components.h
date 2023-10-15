#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX 50
struct course
{
    char name[MAX];
    // struct Faculty facultyOffering;
    int maxSeats;
    int availableSeats;
    int credits;
};
struct Admin
{
    char username[MAX];
    char password[MAX];
};
struct Faculty
{
    // char name[MAX];
    // char department[MAX];
    // char email[MAX];
    // char password[MAX];
    // char facultyId[MAX];
    // 0-name
    // 1-department
    // 2-email
    // 3-password
    // 4-facultyId
    char details[5][MAX];
    struct course coursesOffered[MAX];
    int coursesCount;
};
struct Student
{
    // char name[MAX];
    // char department[MAX];
    // char email[MAX];
    // char studentId[MAX];
    // char password[MAX];
    // 0-name
    // 1-department
    // 2-email
    // 3-password
    // 4-studentId
    char details[5][MAX];
    struct course coursesRegeistered[MAX];
};

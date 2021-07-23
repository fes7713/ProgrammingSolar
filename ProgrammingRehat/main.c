#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 1

typedef struct course
{
    char department[15];
    int courseNumber;
    char courseTitle[30];
    short credits;
} Course;

void inputCourse(Course * pointer);
void printCourse(Course * pointer);
void printCourseRow(Course * pointer);
void inputAllCourses(Course * pointer[], int size);
void printAllCourses(Course * pointer[], int size);

void saveAllCoursesText(Course * pointer[], int size);
void loadAllCourseText(Course *pointer[]);
void saveAllCourseData(Course * pointer [], int size);
void loadAllCoursesData(Course * pointer[]);

int main()
{
    Course * inputArray[SIZE];
    Course * array[SIZE];
    Course * array2[SIZE];
                    
    for(int i = 0; i < SIZE; i ++)
    {
        inputArray[i] = (Course * ) malloc(sizeof(Course));
        array[i] = (Course * ) malloc(sizeof(Course));
        array2[i] = (Course * ) malloc(sizeof(Course));
    }

    inputAllCourses(inputArray, SIZE);
    
    // Text
    saveAllCoursesText(inputArray, SIZE);
    loadAllCourseText(array);
    printAllCourses(array, SIZE);

    // Binary
    saveAllCourseData(inputArray, SIZE);
    loadAllCoursesData(array2);
    printAllCourses(array2, SIZE);
    
    for(int i = 0; i < SIZE; i++)
    {
        free(array[i]);
        free(array2[i]);
    }
    
    
}

void inputCourse(Course * pointer)
{
    //Department
    printf("Enter Department: ");
    fgets(pointer -> department, 15, stdin);
    fflush(stdin);
    pointer -> department[strlen(pointer -> department) - 1] = '\0';
    
    //Course Number
    printf("Enter Course Number: ");
    scanf("%d", &pointer -> courseNumber);
    fflush(stdin);
    
    //Course Title
    printf("Enter Course Title: ");
    fgets(pointer -> courseTitle, 30, stdin);
    fflush(stdin);
    pointer -> courseTitle[strlen(pointer -> courseTitle) - 1] = '\0';
    
    //Credits
    printf("Enter Credits: ");
    scanf("%hd", &pointer -> credits);
    fflush(stdin);
}

void printCourse(Course * pointer)
{
    //Department
    printf("%s\t%30s\n", "Department: ", pointer -> department);
    
    //Course Number
    printf("%s\t%29.4d\n", "Course Number: ", pointer -> courseNumber);
    
    //Course Title
    printf("%s\t%30s\n", "Course Title: ", pointer -> courseTitle);
    
    //Credits
    printf("%s\t%33hd\n", "Credits: ", pointer -> credits);
}

void printCourseRow(Course * pointer)
{
    printf("%-15s\t%04d\t\t\t%-30s%-4d\n", pointer -> department, pointer -> courseNumber, pointer -> courseTitle, pointer -> credits);
}

//Exercise 5
void inputAllCourses(Course * pointer[], int size)
{
    for(int i = 0; i < size; i ++)
    {
        inputCourse(pointer[i]);
    }
}

void printAllCourses(Course * pointer[], int size)
{
    printf("%-15s\t%-4s\t%-30s%-s\n", "Department", "Course Number", "Course Title", "Credits");
    for(int i = 0; i < size; i ++)
    {
        printCourseRow(pointer[i]);
    }
}

//Exercise 1
void saveAllCoursesText(Course * pointer[], int size)
{
    FILE * cfPtr;
    
    if((cfPtr = fopen("course.txt", "w")) == NULL)
    {
        puts("File could not be opened");
    }
    
    else
    {
        for(int i = 0; i < size; i ++)
        {
            fprintf(cfPtr, "%15s%4d%30s%4d\n", pointer[i] -> department, pointer[i] -> courseNumber, pointer[i] -> courseTitle, pointer[i] -> credits);
        }
    }
    
    fclose(cfPtr);
}
    
//Exercise 2
void loadAllCourseText(Course * pointer[])
{
    FILE * cfPtr;
    
    if((cfPtr = fopen("course.txt", "r")) == NULL)
    {
        puts("File could not be opened");
    }
    
    else
    {
        int i = 0;
        
        while(!feof(cfPtr))
        {
            fscanf(cfPtr, "%15s%4d%30s%4d", pointer[i] -> department, &pointer[i] -> courseNumber, pointer[i] -> courseTitle, &pointer[i] -> credits);
        }
    }
    
    fclose(cfPtr);
}

//Exercise 3
void saveAllCourseData(Course * pointer [], int size)
{
    FILE * cfPtr;
    
    if((cfPtr = fopen("course.dat", "wb")) == NULL)
    {
        puts("File could not be opened");
    }
    
    else
    {
        for(int i = 0; i < size; i ++)
        {
            fwrite(pointer[i], sizeof(Course), 1, cfPtr);
        }
    }
    
    fclose(cfPtr);
}

//Exercise 4
void loadAllCoursesData(Course * pointer[])
{
    FILE * cfPtr;
    
    if((cfPtr = fopen("course.dat", "rb")) == NULL)
    {
        puts("File could not be opened");
    }
    
    else
    {
        int i = 0;
        
        fread(pointer[i], sizeof(Course), 1, cfPtr);
        while(!feof(cfPtr))
        {
            i++;
            fread(pointer[i], sizeof(Course), 1, cfPtr);
        }
    }
    
    fclose(cfPtr);
}

    
    

#define  _CTR_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct _Student {
    char name[30];
    char lastname[30];
    int points;
} Student;

int main() {
    FILE* filePointer = fopen("data.txt", "r");
    if (!filePointer) {
        printf("Error opening file\n");
        return 1;
    }

    char buffer[50] = { 0 };
    int numberOfStudents = 0;

    while (fgets(buffer, 50, filePointer) != NULL) {
        numberOfStudents++;
    }
    printf("broj studenata: %d\n", numberOfStudents);

    if (numberOfStudents == 0) {
        printf("File is empty.\n");
        fclose(filePointer);
        return 0;
    }

    Student* students = (Student*)malloc(numberOfStudents * sizeof(Student));
    if (students == NULL) {
        printf("Error: Memory allocation failed!\n");
        fclose(filePointer);
        return -1;
    }

    rewind(filePointer);

    for (int i = 0; i < numberOfStudents; i++) {
        fscanf(filePointer, "%29s %29s %d",
            students[i].name,
            students[i].lastname,
            &students[i].points);
    }

    int maxPoints = students[0].points;
    for (int i = 1; i < numberOfStudents; i++) {
        if (students[i].points > maxPoints) {
            maxPoints = students[i].points;
        }
    }

    if (maxPoints == 0) {
        maxPoints = 1;
    }

    for (int i = 0; i < numberOfStudents; i++) {
        float relativePoints = (float)students[i].points / maxPoints * 100;
        printf("%s %s - %d bodova, %.2f%% \n", students[i].name, students[i].lastname, students[i].points, relativePoints);
    }

    free(students);
    fclose(filePointer);

    return 0;
}
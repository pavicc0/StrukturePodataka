
#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 

typedef struct Cvor { 
    int broj; 
    struct Cvor* next; 
} Cvor;

// Funkcija koja stavlja broj na stog
int Push(Cvor** vrh, int broj) {
    Cvor* novi = (Cvor*)malloc(sizeof(Cvor)); 
    if (novi == NULL) return 0; 
    novi->broj = broj; 
    novi->next = *vrh; 
    *vrh = novi; 
    return 1; 
}

// Funkcija koja skida broj sa stoga
int Pop(Cvor** vrh, int* broj) {
    Cvor* pomocni; 
    if (*vrh == NULL) return 0; 
    pomocni = *vrh; 
    *broj = pomocni->broj; 
    *vrh = pomocni->next; 
    free(pomocni); 
    return 1; 
}

int main() { 
    FILE* dat; 
    Cvor* vrh = NULL; 
    char izraz[100]; 
    char* token; 
    int a, b, rezultat; 
    int greska = 0; 

    dat = fopen("izraz.txt", "r"); 
    if (dat == NULL) { 
        printf("Greska pri otvaranju datoteke!"); 
        return 1; 
    }

    if (fscanf(dat, "%99[^\n]", izraz) != 1) { 
        printf("Datoteka je prazna!"); 
        fclose(dat); 
        return 1; 
    }

    fclose(dat); 

    token = strtok(izraz, " \t"); 

    while (token != NULL && greska == 0) { 

        if (isdigit((unsigned char)token[0]) || (token[0] == '-' && isdigit((unsigned char)token[1]))) { 
            if (!Push(&vrh, atoi(token))) greska = 1; 
        }

        else { 
            if (!Pop(&vrh, &b) || !Pop(&vrh, &a)) greska = 1; 

            else { 

                if (strcmp(token, "+") == 0) rezultat = a + b; 
                else if (strcmp(token, "-") == 0) rezultat = a - b; 
                else if (strcmp(token, "*") == 0) rezultat = a * b; 
                else if (strcmp(token, "/") == 0 && b != 0) rezultat = a / b;
                else { greska = 1; rezultat = 0; } 

                if (greska == 0 && !Push(&vrh, rezultat)) greska = 1; 
            }
        }

        token = strtok(NULL, " \t"); 
    }

    if (greska == 0 && vrh != NULL && vrh->next == NULL) { 
        Pop(&vrh, &rezultat); 
        printf("Rezultat je: %d", rezultat);
    }
    else { 
        printf("Neispravan postfiksni izraz!"); 
    }

    while (vrh != NULL) { 
        Pop(&vrh, &rezultat); 
    }

    return 0; 
}


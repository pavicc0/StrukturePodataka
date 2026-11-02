
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct _Polinom {
    int koef;             
    int exp;               
    struct _Polinom* next;  
} Polinom, * Position;


void dodaj(Position* head, int koef, int exp);
void citaj(Position* head, char* ime);
void ispisi(Position head);
void zbroji(Position p1, Position p2, Position* rez);
void pomnozi(Position p1, Position p2, Position* rez);
void obrisi(Position* head);


void dodaj(Position* head, int koef, int exp) {
    Position temp = *head;

   
    if (koef == 0) return;

   
    while (temp != NULL) {
        if (temp->exp == exp) {
            temp->koef += koef; 
            return;
        }
        temp = temp->next;
    }

    Position novi = (Position)malloc(sizeof(Polinom));

  
    if (novi == NULL) return;

    novi->koef = koef;
    novi->exp = exp;
    novi->next = *head;

    
    *head = novi;
}


void citaj(Position* head, const char* ime) {
    FILE* f;
    int koef, exp;

  
    if (fopen_s(&f, ime, "r") != 0) {
        printf("Greska pri otvaranju datoteke!\n");
        return;
    }

    while (fscanf_s(f, "%d %d", &koef, &exp) == 2) {
        dodaj(head, koef, exp);
    }

    fclose(f);
}


void ispisi(Position head) {
    int prvi = 1;

    
    while (head != NULL) {

        // Ispisuje predznak
        if (!prvi && head->koef > 0)
            printf("+");

        printf("%dx^%d", head->koef, head->exp);

        prvi = 0;
        head = head->next;
    }

    
    if (prvi)
        printf("0");

    printf("\n");
}


void zbroji(Position p1, Position p2, Position* rez) {

    // Prepisuje sve članove prvog polinoma
    while (p1 != NULL) {
        dodaj(rez, p1->koef, p1->exp);
        p1 = p1->next;
    }

    // Dodaje članove drugog polinoma
    while (p2 != NULL) {
        dodaj(rez, p2->koef, p2->exp);
        p2 = p2->next;
    }
}


void pomnozi(Position p1, Position p2, Position* rez) {
    Position temp;

   
    for (temp = p1; temp != NULL; temp = temp->next) {
        Position q = p2;

        while (q != NULL) {

            
            int koef = temp->koef * q->koef;

            
            int exp = temp->exp + q->exp;

            // Dodaje rezultat u novi polinom
            dodaj(rez, koef, exp);

            q = q->next;
        }
    }
}


void obrisi(Position* head) {
    Position temp;

    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}


int main(void) {
    Position P1 = NULL;  
    Position P2 = NULL;  
    Position Z = NULL;   
    Position M = NULL;   

    
    citaj(&P1, "p1.txt");
    citaj(&P2, "p2.txt");

    printf("Prvi polinom: ");
    ispisi(P1);

    printf("Drugi polinom: ");
    ispisi(P2);

    zbroji(P1, P2, &Z);

    printf("Zbroj polinoma: ");
    ispisi(Z);

    pomnozi(P1, P2, &M);

    printf("Umnozak polinoma: ");
    ispisi(M);

    
    obrisi(&P1);
    obrisi(&P2);
    obrisi(&Z);
    obrisi(&M);

    return 0;
}




#define _CRT_SECURE_NO_WARNINGS // Omogućuje korištenje funkcija poput strcpy bez upozorenja kompajlera
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. DEFINICIJA STRUKTURE I TIPA PODATAKA
// Uveden je novi tip podataka "Position" koji je pokazivač na strukturu _Person.
// To nam olakšava pisanje koda jer ne moramo stalno pisati "struct _Person*".
struct _Person {
    char name[50];       // Polje za ime osobe
    char lastName[50];   // Polje za prezime osobe
    int year;            // Godina rođenja osobe
    struct _Person* next;// Pokazivač na idući element u vezanoj listi
};

typedef struct _Person Person;
typedef Person* Position; // Position je pokazivač tipa Person*

// Deklaracije funkcija
int insertBegin(Position head, char* name, char* lastName, int year); // Pod A
int printList(Position first);                                      // Pod B
int insertEnd(Position head, char* name, char* lastName, int year);  // Pod C
Position findLName(char* lastName, Position first);                  // Pod D
int deletePerson(char* lastName, Position head);                     // Pod e

int main() {
    // Stvaranje glave (head) liste - početni čvor koji ne sadrži podatke o osobi, 
    // nego služi isključivo kao sidro (početak) liste. 
    // Pokazivač 'next' postavljamo na NULL jer je lista na početku prazna.
    Person Head = { "", "", 0, NULL };

    // --- POD A: Dinamičko dodavanje na početak liste ---
    insertBegin(&Head, "Ana", "Anic", 2001);
    insertBegin(&Head, "Ivan", "Ivic", 1998);

    // --- POD B: Ispis liste ---
    printf("--- Trenutna lista nakon dodavanja na pocetak ---\n");
    printList(Head.next); // Prosljeđujemo element iza Heada (prvi pravi element)

    // --- POD C: Dinamičko dodavanje na kraj liste ---
    insertEnd(&Head, "Marko", "Markic", 1995);

    printf("\n--- Trenutna lista nakon dodavanja na kraj ---\n");
    printList(Head.next);

    // --- POD D: Pronalaženje elementa u listi po prezimenu ---
    printf("\n--- Pretrazivanje osobe po prezimenu 'Ivic' ---\n");
    Position found = findLName("Ivic", Head.next);
    if (found != NULL) {
        printf("Pronađena osoba: %s %s, rođen %d. godine.\n", found->name, found->lastName, found->year);
    }
    else {
        printf("Osoba nije pronađena.\n");
    }

    // --- POD E: Brisanje određenog elementa iz liste po prezimenu ---
    printf("\n--- Brisanje osobe s prezimenom 'Ivic' ---\n");
    deletePerson("Ivic", &Head);

    printf("\n--- Lista nakon brisanja ---\n");
    printList(Head.next);

    // Oslobađanje preostale memorije na kraju programa (dobra praksa)
    while (Head.next != NULL) {
        deletePerson(Head.next->lastName, &Head);
    }

    return 0;
}


// --- IMPLEMENTACIJA FUNKCIJA ---

// Funkcija A: Dodaje novi element na početak liste
int insertBegin(Position head, char* name, char* lastName, int year) {
    // 1. Alokacija memorije: malloc vraća pokazivač na novourezervirani prostor u memoriji veličine strukture Person.
    // (Position služi za castanje u odgovarajući tip pokazivača).
    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Greška: Alokacija memorije nije uspjela!\n");
        return -1; // Izlaz iz funkcije ako nema slobodne memorije
    }

    // 2. Upis podataka u novu strukturu pomoću funkcije strcpy jer su u pitanju stringovi (nizovi znakova)
    strcpy(newPerson->name, name);
    strcpy(newPerson->lastName, lastName);
    newPerson->year = year;

    // 3. Povezivanje u listu: 
    // Novi element kao svoj 'next' postavlja ono na što je do tada pokazivao head (stari prvi element).
    newPerson->next = head->next;

    // Head sada pokazuje na naš novi element, čime on postaje službeno prvi element u listi.
    head->next = newPerson;

    return 0;
}


// Funkcija B: Ispisuje cijelu listu
int printList(Position first) {
    Position current = first; // Pomoćni pokazivač kreće od prvog elementa

    if (current == NULL) {
        printf("Lista je prazna.\n");
        return 0;
    }

    // Vrtimo petlju dok god ne dođemo do kraja liste (dok je pokazivač različit od NULL)
    while (current != NULL) {
        printf("Ime: %s, Prezime: %s, Godina rođenja: %d\n", current->name, current->lastName, current->year);
        current = current->next; // Pomičemo pokazivač na sljedeći čvor u listi
    }

    return 0;
}


// Funkcija C: Dodaje novi element na kraj liste
int insertEnd(Position head, char* name, char* lastName, int year) {
    // 1. Alokacija memorije za novi čvor
    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Greška pri alokaciji memorije!\n");
        return -1;
    }

    // Popunjavamo podatke
    strcpy(newPerson->name, name);
    strcpy(newPerson->lastName, lastName);
    newPerson->year = year;
    newPerson->next = NULL; // Budući da je ovo kraj liste, njegov 'next' mora biti NULL (kraj lanca)

    // 2. Pronalaženje posljednjeg elementa
    Position current = head;
    while (current->next != NULL) { // Idemo sve dok ne nađemo element čiji je 'next' jednak NULL
        current = current->next;
    }

    // 3. Spajanje: Posljednji element u listi sada pokazuje na naš novi element umjesto na NULL
    current->next = newPerson;

    return 0;
}


// Funkcija D: Pronalazi element u listi po prezimenu
Position findLName(char* lastName, Position first) {
    Position current = first; // Pomoćni pokazivač kreće od prvog elementa

    // Prolazimo kroz listu čvor po čvor
    while (current != NULL) {
        // strcmp uspoređuje dva stringa. Ako vrati 0, znači da su prezimena potpuno identična.
        if (strcmp(current->lastName, lastName) == 0) {
            return current; // Vraćamo pokazivač na pronađenu osobu
        }
        current = current->next; // Idemo na sljedeći čvor
    }

    return NULL; // Ako petlja završi, a nismo našli osobu, vraćamo NULL
}


// Funkcija E: Brise određeni element iz liste po prezimenu
int deletePerson(char* lastName, Position head) {
    Position prev = head;           // Pokazivač koji uvijek drži adresu prethodnog elementa
    Position curr = head->next;     // Pokazivač na element koji trenutno provjeravamo

    // Tražimo element čije prezime odgovara traženom, pazeći da ne izađemo izvan liste (curr != NULL)
    while (curr != NULL && strcmp(curr->lastName, lastName) != 0) {
        prev = curr;
        curr = curr->next;
    }

    // Ako je curr došao do kraja (NULL), znači da tražena osoba ne postoji u listi
    if (curr == NULL) {
        printf("Osoba s prezimenom '%s' nije pronađena.\n", lastName);
        return -1;
    }

    // Premošćivanje (izbacivanje čvora iz lanca):
    // Prethodni element sada preskače trenutni i spaja se direktno s onim iza trenutnog čvora (`curr->next`).
    prev->next = curr->next;

    // Oslobađamo memoriju obrisanog elementa iz RAM-a pomoću funkcije free() (sprječavamo curenje memorije)
    free(curr);

    return 0;
}
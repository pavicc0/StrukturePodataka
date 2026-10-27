
#define _CRT_SECURE_NO_WARNINGS // Iskljucuje upozorenja za neke C funkcije u Visual Studiu
#include <iostream>  // Omogucuje cout
#include <cstdio>    // Omogucuje printf, fopen, fclose, fprintf i fscanf
#include <cstdlib>   // Omogucuje malloc i free
#include <cstring>   // Omogucuje strcpy i strcmp
using namespace std; // Omogucuje pisanje cout bez std::

// Definicija strukture koja predstavlja jednu osobu
struct _Person {
    char name[50];       // Niz znakova za ime osobe
    char lastName[50];   // Niz znakova za prezime osobe
    int year;            // Godina rodenja
    struct _Person* next; // Pokazivac na sljedeci element liste
};

typedef struct _Person Person; // Person je skraceni naziv za struct _Person
typedef Person* Position;      // Position je pokazivac na strukturu Person

// Deklaracije funkcija
int insertBegin(Position head, char* name, char* lastName, int year);
int printList(Position first);
int insertEnd(Position head, char* name, char* lastName, int year);
Position findLName(char* lastName, Position first);
int deletePerson(char* lastName, Position head);
int insertAfter(Position head, char* lastName, char* name, char* newLastName, int year);
int insertBefore(Position head, char* lastName, char* name, char* newLastName, int year);
int sortList(Position head);
int saveFile(Position first, char* filename);
int readFile(Position head, char* filename);

int main() {
    // Stvaramo glavu liste koja ne sadrzi podatke o osobi
    // Head.next = NULL znaci da je lista na pocetku prazna
    Person Head = { "", "", 0, NULL };

    // Dodajemo Anu na pocetak liste
    insertBegin(&Head, (char*)"Ana", (char*)"Anic", 2001);

    // Dodajemo Ivana na pocetak, pa Ivan postaje prvi element
    insertBegin(&Head, (char*)"Ivan", (char*)"Ivic", 1998);

    // Dodajemo Marka na kraj liste
    insertEnd(&Head, (char*)"Marko", (char*)"Markic", 1995);

    // Ispisujemo trenutnu listu
    cout << "Pocetna lista:\n";
    printList(Head.next); // Prosljedujemo prvi stvarni element liste

    // A. Dodajemo Petru iza osobe s prezimenom Anic
    insertAfter(&Head, (char*)"Anic", (char*)"Petra", (char*)"Peric", 2000);

    // B. Dodajemo Luku ispred osobe s prezimenom Ivic
    insertBefore(&Head, (char*)"Ivic", (char*)"Luka", (char*)"Lukic", 2002);

    // C. Sortiramo listu abecedno po prezimenima
    sortList(&Head);

    // Ispisujemo sortiranu listu
    cout << "\nSortirana lista:\n";
    printList(Head.next);

    // D. Zapisujemo sve osobe u datoteku osobe.txt
    saveFile(Head.next, (char*)"osobe.txt");

    // Brisemo sve elemente liste prije ponovnog ucitavanja
    // Time sprjecavamo da se osobe dodaju dvaput
    while (Head.next != NULL)
        deletePerson(Head.next->lastName, &Head);

    // E. Citamo osobe iz datoteke i dodajemo ih u listu
    readFile(&Head, (char*)"osobe.txt");

    // Ispisujemo listu procitanu iz datoteke
    cout << "\nLista procitana iz datoteke:\n";
    printList(Head.next);

    // Oslobadamo memoriju svih preostalih elemenata
    while (Head.next != NULL)
        deletePerson(Head.next->lastName, &Head);

    return 0; // Zavrsavamo program
}

// Funkcija koja dodaje osobu na pocetak liste
int insertBegin(Position head, char* name, char* lastName, int year) {
    // Rezerviramo memoriju za novi element
    Position q = (Position)malloc(sizeof(Person));

    // Provjeravamo je li alokacija memorije uspjela
    if (q == NULL) return -1;

    // Kopiramo ime i prezime u novi element
    strcpy(q->name, name);
    strcpy(q->lastName, lastName);

    // Upisujemo godinu rodenja
    q->year = year;

    // Novi element pokazuje na dosadasnji prvi element
    q->next = head->next;

    // Glava sada pokazuje na novi prvi element
    head->next = q;

    return 0; // Uspjesno dodavanje
}

// Funkcija koja ispisuje sve elemente liste
int printList(Position first) {
    // Prolazimo kroz listu dok ne dodemo do kraja
    while (first != NULL) {
        // Ispisujemo podatke trenutne osobe
        printf("%s %s %d\n", first->name, first->lastName, first->year);

        // Prelazimo na sljedeci element
        first = first->next;
    }

    return 0; // Zavrsavamo funkciju
}

// Funkcija koja dodaje osobu na kraj liste
int insertEnd(Position head, char* name, char* lastName, int year) {
    // Rezerviramo memoriju za novi element
    Position q = (Position)malloc(sizeof(Person));

    // Ako nema memorije, vracamo -1
    if (q == NULL) return -1;

    // Upisujemo podatke nove osobe
    strcpy(q->name, name);
    strcpy(q->lastName, lastName);
    q->year = year;

    // Novi element je posljednji, zato next postavljamo na NULL
    q->next = NULL;

    // Dolazimo do posljednjeg elementa liste
    while (head->next != NULL)
        head = head->next;

    // Posljednji element sada pokazuje na novi element
    head->next = q;

    return 0; // Uspjesno dodavanje
}

// Funkcija koja pronalazi osobu po prezimenu
Position findLName(char* lastName, Position first) {
    // Prolazimo kroz sve elemente liste
    while (first != NULL) {

        // strcmp vraca 0 ako su prezimena jednaka
        if (strcmp(first->lastName, lastName) == 0)
            return first; // Vraca pokazivac na pronadenu osobu

        // Ako nije pronadena, prelazimo na sljedecu osobu
        first = first->next;
    }

    return NULL; // Osoba nije pronadena
}

// Funkcija koja brise osobu po prezimenu
int deletePerson(char* lastName, Position head) {
    // prev pokazuje na prethodni element
    Position prev = head;

    // curr pokazuje na prvi stvarni element liste
    Position curr = head->next;

    // Trazenje osobe sa zadanim prezimenom
    while (curr != NULL && strcmp(curr->lastName, lastName) != 0) {
        prev = curr;       // Prethodni postaje trenutni
        curr = curr->next; // Prelazimo na sljedeci element
    }

    // Ako smo dosli do kraja, osoba ne postoji
    if (curr == NULL) return -1;

    // Preskacemo element koji zelimo obrisati
    prev->next = curr->next;

    // Oslobadamo memoriju obrisanog elementa
    free(curr);

    return 0; // Uspjesno brisanje
}

// A. Funkcija koja dodaje osobu iza odredene osobe
int insertAfter(Position head, char* lastName, char* name, char* newLastName, int year) {
    // Pronalazimo osobu iza koje zelimo dodati novi element
    Position p = findLName(lastName, head->next);

    // Ako osoba ne postoji, prekidamo funkciju
    if (p == NULL) return -1;

    // Rezerviramo memoriju za novi element
    Position q = (Position)malloc(sizeof(Person));
    if (q == NULL) return -1;

    // Upisujemo podatke nove osobe
    strcpy(q->name, name);
    strcpy(q->lastName, newLastName);
    q->year = year;

    // Novi element pokazuje na element koji je prije bio iza p
    q->next = p->next;

    // Element p sada pokazuje na novi element
    p->next = q;

    return 0; // Uspjesno dodavanje
}

// B. Funkcija koja dodaje osobu ispred odredene osobe
int insertBefore(Position head, char* lastName, char* name, char* newLastName, int year) {
    // Krecemo od glave jer trazimo prethodni element
    Position p = head;

    // Trazenje elementa prije osobe sa zadanim prezimenom
    while (p->next != NULL && strcmp(p->next->lastName, lastName) != 0)
        p = p->next; // Prelazimo na sljedeci element

    // Ako osoba nije pronadena, prekidamo funkciju
    if (p->next == NULL) return -1;

    // Rezerviramo memoriju za novi element
    Position q = (Position)malloc(sizeof(Person));
    if (q == NULL) return -1;

    // Upisujemo podatke nove osobe
    strcpy(q->name, name);
    strcpy(q->lastName, newLastName);
    q->year = year;

    // Novi element pokazuje na pronadenu osobu
    q->next = p->next;

    // Prethodni element sada pokazuje na novi element
    p->next = q;

    return 0; // Uspjesno dodavanje
}

// C. Funkcija koja sortira listu po prezimenima
int sortList(Position head) {
    // p i q koristimo za usporedivanje elemenata
    Position p, q;

    // Pomocne varijable za zamjenu podataka
    char name[50], lastName[50];
    int year;

    // Vanjska petlja prolazi kroz listu
    for (p = head->next; p != NULL; p = p->next)

        // Unutarnja petlja usporeduje p sa svim elementima iza njega
        for (q = p->next; q != NULL; q = q->next)

            // Ako je prezime p abecedno iza prezimena q, zamjenjujemo podatke
            if (strcmp(p->lastName, q->lastName) > 0) {

                // Spremamo podatke osobe p
                strcpy(name, p->name);
                strcpy(lastName, p->lastName);
                year = p->year;

                // U osobu p kopiramo podatke osobe q
                strcpy(p->name, q->name);
                strcpy(p->lastName, q->lastName);
                p->year = q->year;

                // U osobu q vracamo prethodno spremljene podatke osobe p
                strcpy(q->name, name);
                strcpy(q->lastName, lastName);
                q->year = year;
            }

    return 0; // Sortiranje je zavrseno
}

// D. Funkcija koja zapisuje listu u datoteku
int saveFile(Position first, char* filename) {
    // Otvaramo datoteku za pisanje; stari sadrzaj se brise
    FILE* f = fopen(filename, "w");

    // Ako se datoteka ne moze otvoriti, vracamo -1
    if (f == NULL) return -1;

    // Prolazimo kroz sve elemente liste
    while (first != NULL) {

        // Zapisujemo ime, prezime i godinu u jedan redak datoteke
        fprintf(f, "%s %s %d\n",
            first->name, first->lastName, first->year);

        // Prelazimo na sljedecu osobu
        first = first->next;
    }

    // Zatvaramo datoteku nakon zapisivanja
    fclose(f);

    return 0; // Uspjesan zapis
}

// E. Funkcija koja cita podatke iz datoteke u listu
int readFile(Position head, char* filename) {
    // Otvaramo datoteku za citanje
    FILE* f = fopen(filename, "r");

    // Ako datoteka ne postoji ili se ne moze otvoriti, vracamo -1
    if (f == NULL) return -1;

    // Pomocne varijable za ucitavanje podataka
    char name[50], lastName[50];
    int year;

    // Ucitavamo podatke dok postoje sva tri podatka u retku
    while (fscanf(f, "%49s %49s %d", name, lastName, &year) == 3)

        // Svaku procitanu osobu dodajemo na kraj liste
        insertEnd(head, name, lastName, year);

    // Zatvaramo datoteku
    fclose(f);

    return 0; // Uspjesno citanje
}

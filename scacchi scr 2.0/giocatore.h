#ifndef GIOCATORE_H
#define GIOCATORE_H
#include <termios.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

/* STRUTTURA DATI */
struct giocatore
{
    char username[20];
    char password[20];
    int elo;
    float punteggio;
    int partite;
    struct giocatore *next;
};

static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo);

/* Restore old terminal i/o settings */
void resetTermios(void);

/* Read 1 character - echo defines echo mode */
char getch_(int echo);

/* Read 1 character without echo */
char getch(void);

/* Read 1 character with echo */
char getche(void);

void ins_psw();

/* OPERAZIONI */
void inserisciGiocatore(struct giocatore **head,char username[],char password[],int ELO, float Pt, int partite);
void caricaGiocatori(struct giocatore **head);
void cancellaGiocatore(struct giocatore** head, char* nome);
void trova_e_sostituisci (struct giocatore** head, char* nomevinc, char* nomeperd, int patta);
char* Registrazione(struct giocatore **head);
char* Login(struct giocatore *head);
void salvaUtenti(struct giocatore* head);
void stampa_classifica(struct giocatore* head);
void svuota_lista_giocatori (struct giocatore** head);
#endif

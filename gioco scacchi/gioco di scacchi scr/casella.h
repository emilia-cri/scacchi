#ifndef CASELLA_H
#define CASELLA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

/* descrizione di una casella della scacchiera */
typedef struct {
	int Colore;
	int Pezzo;
} Casella;

struct nodo_Casella
{
  Casella dato;
  struct nodo_Casella *next;
};

/* descrizione di una mossa identificatico della casa di partenza e di arrivo */
typedef struct {
	int Da;
	int A;
	int cat; //0 se catturo 1 altrimenti
} Mossa;

struct nodo_Mossa
{
  Mossa dato;
  struct nodo_Mossa *next;
};


struct nodo_Casella* Inizializza(struct nodo_Casella* head);
void trova_e_sostituisci_casella (struct nodo_Casella* head, int c, int p, int index);
int case_x_colore (struct nodo_Casella* head, int index);
int case_x_pezzo (struct nodo_Casella* head, int index);
int CasaAttaccata (struct nodo_Casella* head,int r, int ChiMuove);
void svuota_lista_casella (struct nodo_Casella** head);
void svuota_lista_mosse (struct nodo_Mossa** head);
void SalvaMossa(struct nodo_Mossa** head, int d, int a, int c);
void StampaMossa(struct nodo_Mossa* head,int NumeroMosse);
struct nodo_Mossa *GeneraMosse(struct nodo_Casella* head,int ChiMuove);
int InScacco(struct nodo_Casella* head, int ChiMuove);
void Matto(struct nodo_Casella* head, int ChiMuove);
int Input(struct nodo_Casella* head,char * Input, int ChiMuove);
void VisualizzaScacchiera(struct nodo_Casella* head, int ChiMuove);

#endif

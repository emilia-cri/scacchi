#ifndef ANNOTAZIONE_H
#define ANNOTAZIONE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

/* informazioni per annotare una mossa */
struct nodo_Storia
{
	char Da_c;
	char Da_r;
	char A_c;
	char A_r;
	int cat;
	char l;
	char promozione;
	struct nodo_Storia *next;
};

/* OPERAZIONI */
void inserisci_storia (struct nodo_Storia** head,char* da_c,char* da_r, char* a_c, char* a_r, int c, char* let, char* prom);
void stampa(struct nodo_Storia** head,int matto);
void salvaAnnotazione(struct nodo_Storia** head,int matto, char* bianco, char* nero);
void svuota_lista_crono (struct nodo_Storia** head);

#endif

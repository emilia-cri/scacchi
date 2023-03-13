#include "giocatore.h"
#include "casella.h"
#include "annotazione.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

#define RESET "\x1b[H\x1b[2J"

#define BOOL			int
#define TRUE			1
#define FALSE			0

#define NUMEROCASE		64
#define NUMEROPEZZI		6
#define MAXMOSSE		1024


#define BIANCO			0
#define NERO			1

#define PEDONE			0
#define CAVALLO		1
#define ALFIERE		2
#define TORRE			3
#define DONNA			4
#define RE			5

#define VUOTO			6


#define RIGA(x)		(((unsigned)x)>>3)
#define COLONNA(x)		(x & 7)


/* id di alcune caselle importanti */
#define A1				56
#define B1				57
#define C1				58
#define D1				59
#define E1				60
#define F1				61
#define G1				62
#define H1				63
#define A8				0
#define B8				1
#define C8				2
#define D8				3
#define E8				4
#define F8				5
#define G8				6
#define H8				7



/* posizione iniziale della scacchiera */

const int ColoriIniziali[NUMEROCASE] = {
	NERO,   NERO,   NERO,   NERO,   NERO,   NERO,   NERO,   NERO,
	NERO,   NERO,   NERO,   NERO,   NERO,   NERO,   NERO,   NERO,
	VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,
	VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,
	VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,
	VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,
	BIANCO, BIANCO, BIANCO, BIANCO, BIANCO, BIANCO, BIANCO, BIANCO,
	BIANCO, BIANCO, BIANCO, BIANCO, BIANCO, BIANCO, BIANCO, BIANCO
};

const int PezziIniziali[NUMEROCASE] = {
	TORRE,  CAVALLO, ALFIERE, DONNA, RE,     ALFIERE, CAVALLO, TORRE,
	PEDONE, PEDONE,  PEDONE,  PEDONE, PEDONE, PEDONE,  PEDONE,  PEDONE,
	VUOTO,  VUOTO,   VUOTO,   VUOTO,  VUOTO,  VUOTO,   VUOTO,   VUOTO,
	VUOTO,  VUOTO,   VUOTO,   VUOTO,  VUOTO,  VUOTO,   VUOTO,   VUOTO,
	VUOTO,  VUOTO,   VUOTO,   VUOTO,  VUOTO,  VUOTO,   VUOTO,   VUOTO,
	VUOTO,  VUOTO,   VUOTO,   VUOTO,  VUOTO,  VUOTO,   VUOTO,   VUOTO,
	PEDONE, PEDONE,  PEDONE,  PEDONE, PEDONE, PEDONE,  PEDONE,  PEDONE,
	TORRE,  CAVALLO, ALFIERE, DONNA, RE,     ALFIERE, CAVALLO, TORRE
};



void trova_e_sostituisci_casella (struct nodo_Casella* head, int c, int p, int index){
    int i=0;
   
    while ((head != NULL) && (i<index)){
        //scorro la lista
        head = head->next;
        i++;
    }
    
    (head->dato).Colore = c;
    (head->dato).Pezzo = p;
}


int case_x_colore (struct nodo_Casella* head, int index){
    int i=0;
    int c=0;
    while ((head != NULL) && (i<index)){
        //scorro la lista
        head = head->next;
        i++;
    }
    c=(head->dato).Colore;
    return c;
}

int case_x_pezzo (struct nodo_Casella* head, int index){
    int i=0;
    int p=0;
    while ((head != NULL) && (i<index)){
        //scorro la lista
        head = head->next;
        i++;
    }
    p=(head->dato).Pezzo;
    return p;
}

void svuota_lista_casella (struct nodo_Casella** head){
struct nodo_Casella* t = *head; //precedente 
struct nodo_Casella* w = *head; //successivo 
while(t!=NULL){
w=t->next;
free(t);
t=w;}
t=NULL;
}



void svuota_lista_mosse (struct nodo_Mossa** head){
struct nodo_Mossa* t = *head; //precedente 
struct nodo_Mossa* w = *head; //successivo 
while(t!=NULL){
w=t;
t=t->next;
free(w);
}
free(t);
}

void inserisci_storia (struct nodo_Storia** head,char* da_c,char* da_r, char* a_c, char* a_r, int c, char* let, char* prom){
    
	struct nodo_Storia* s;
    	
    	//alloco il nuovo nodo
    	struct nodo_Storia* new_head;
    	new_head = (struct nodo_Storia*)malloc(sizeof(struct nodo_Storia));
	strcpy(&new_head->Da_c,da_c);
	strcpy(&new_head->Da_r,da_r);
	strcpy(&new_head->A_c,a_c);
	strcpy(&new_head->A_r,a_r);
    	new_head->cat = c;
    	strcpy(&new_head->l,let);
    	strcpy(&new_head->promozione,prom);
    	//lui diventa il primo nodo e quindi deve puntare all'attuale testa
    	new_head->next = NULL;
    	//se la lista e' vuota lui sara' anche il primo quindi la testa deve puntare a lui
    	if(*head==NULL){  
        	*head=new_head;
            }
    	 else{
              s=*head;
    	 while ((s!=NULL) && (s->next != NULL)){
        	s=s->next;}

    	s->next = new_head;}
	
}

//stampa la cronologia delle mosse
void stampa(struct nodo_Storia** head,int matto){
    int i=1,k=1;
    if(*head!=NULL){
   	struct nodo_Storia* s=*head;
    while ((s != NULL)){
    if(i%2!=0)printf("%d   ",k);
    if((s->l=='R')&&(s->Da_c=='e')&&(s->A_c=='g')) printf("   O-O");
    else if((s->l=='R')&&(s->Da_c=='e')&&(s->A_c=='c')) printf("   O-O-O");     
    else if(s->cat==0) printf("   %c%c%cx%c%c%c", s->l,s->Da_c,s->Da_r,s->A_c,s->A_r,s->promozione);
    else if(s->cat==1) printf("   %c%c%c-%c%c%c", s->l,s->Da_c,s->Da_r,s->A_c,s->A_r,s->promozione);
        s =s->next;
        if((s==NULL)&&(matto==1)) printf("#");
        if(i%2==0) {
        printf("\n");
        k++;}
        i++;
        
    }}
    printf("\n");
}

void salvaAnnotazione(struct nodo_Storia** head,int matto, char* bianco, char* nero){
    FILE *f;
    f=NULL;
    f=fopen("partita.txt","w");
    int i=1,k=1;

    //Se il file viene aperto correttamente
    if(f!=NULL)
    {
	fprintf(f, "Gioco di scacchi, annotazione classica.  \n\n");
	fprintf(f, "      BIANCO= %s     NERO=%s \n\n",bianco,nero);
	fprintf(f, "        ♙             ♟ \n\n");
    if(*head!=NULL){
   	struct nodo_Storia* s=*head;
    while ((s != NULL)){
    if(i%2!=0)fprintf(f,"%d   ",k);
    if((s->l=='R')&&(s->Da_c=='e')&&(s->A_c=='g')) fprintf(f,"    O-O");
    else if((s->l=='R')&&(s->Da_c=='e')&&(s->A_c=='c')) fprintf(f,"    O-O-O");     
    else if(s->cat==0) fprintf(f,"   %c%c%cx%c%c%c", s->l,s->Da_c,s->Da_r,s->A_c,s->A_r,s->promozione);
    else if(s->cat==1) fprintf(f,"   %c%c%c-%c%c%c", s->l,s->Da_c,s->Da_r,s->A_c,s->A_r,s->promozione);
        s =s->next;
        if((s==NULL)&&(matto==1)) fprintf(f,"#");
        if(i%2==0) {
        fprintf(f,"\n");
        k++;}
        i++;
        
    }}
    fprintf(f,"\n");
         }
    else
    {
         //Se il file non è aperto correttamente
         printf("Errore scrittura file.\n");
         return;
    }
    //Chiusura del file
    fclose(f);
   
}

void svuota_lista_crono (struct nodo_Storia** head){
struct nodo_Storia* t = *head; //precedente 
struct nodo_Storia* w = *head; //successivo 
while(t!=NULL){
w=t->next;
free(t);
t=w;}
t=NULL;
}


/* SalvaMossa inserisce una mossa nello stack per poi esaminarle e vedere se quella inserita dall'utente e' tra queste */
void SalvaMossa(struct nodo_Mossa** head, int d, int a, int c){
    
    struct nodo_Mossa *mossa;
    mossa=(struct nodo_Mossa*)malloc(sizeof(struct nodo_Mossa));
    mossa->dato.Da = d;
    mossa->dato.A = a;
    mossa->dato.cat = c;
    mossa->next=(*head);
    *head=mossa;
 
}





/* GeneraMosse genera mosse pseudolegali per la posizione corrente */
struct nodo_Mossa *GeneraMosse(struct nodo_Casella* head,int ChiMuove) {
	int CasaCorrente, CasaDestinazione, Direzione;
	int t,r;
	int col;
	int avv;
	struct nodo_Mossa* head_mossa=NULL;
  
  	if(ChiMuove==BIANCO)
  	avv = NERO;
  	else	avv = BIANCO;    /* lato di chi attacca */

	/* cerca tutte le case per pezzi del colore che deve muovere */
	for (CasaCorrente = 0; CasaCorrente < NUMEROCASE; CasaCorrente++) {
	if (case_x_colore(head,CasaCorrente) == ChiMuove) {
			switch (case_x_pezzo(head,CasaCorrente)) {
			case PEDONE:
				if (ChiMuove == BIANCO) {
					/* mosse per il pedone bianco */
					if (COLONNA(CasaCorrente)!=0 && case_x_colore(head,CasaCorrente-9) == NERO){
						/* mangia in avanti a sinistra */
						SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente - 9),0);
						}
						
					if (COLONNA(CasaCorrente) != 7 && case_x_colore(head,CasaCorrente-7) == NERO){
						/* mangia in avanti a destra */
						SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente - 7),0);
						}
						
					if (case_x_colore(head,CasaCorrente-8) == VUOTO) {{
						/* avanti una casa */
						SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente - 8),1);
						}
					if (CasaCorrente >=48 && case_x_colore(head,CasaCorrente-16) == VUOTO){
						SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente - 16),1);
						}
							
					}
				}
				else {
					/* mosse per il pedone nero */
					if (COLONNA(CasaCorrente) !=0 && case_x_colore(head,CasaCorrente+7) == BIANCO){
						SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente + 7),0);
						}
					if (COLONNA(CasaCorrente) != 7 && case_x_colore(head,CasaCorrente+9) == BIANCO){
						SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente + 9),0);
						}
					if (case_x_colore(head,CasaCorrente+8) == VUOTO) {{
						SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente + 8),1);
						}
					if (CasaCorrente<=15 && case_x_colore(head,CasaCorrente+16) == VUOTO){
						SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente + 16),1);
						}
					}
					}
					break;
			
			case DONNA:		/* == ALFIERE+TORRE */

			case ALFIERE:
				for (t = CasaCorrente - 9; t >= 0 && COLONNA (t) != 7; t -= 9)
				{			/* diagonale verso su sinistra */
					if (case_x_colore(head,t) == VUOTO){
						SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 				}
		 			if (case_x_colore(head,t) == avv){  /* cattura in diagonale */
		 				SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 				}
		      			}
	    			for (t = CasaCorrente - 7; t >= 0 && COLONNA (t) != 0; t -= 7)
	      			{			/* diagonale verso su destra */
					if (case_x_colore(head,t) == VUOTO){
						SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 				}
		 			if (case_x_colore(head,t) == avv){
		 			SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 			}
	      			}
	    			for (t = CasaCorrente + 9; t < 64 && COLONNA (t) != 0; t += 9)
	      			{		/* destra giu */
					if (case_x_colore(head,t) == VUOTO){
		 				SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 				}
		 			if (case_x_colore(head,t) == avv){
		 			SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 			}
	      			}
	    			for (t = CasaCorrente + 7; t < 64 && COLONNA (t) != 7; t += 7)
	      			{		/* sinistra giu */
					if (case_x_colore(head,t) == VUOTO){
		 				SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 				}
		 				if (case_x_colore(head,t) == avv){
		 				SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 				}
	      			}
	    			if (case_x_pezzo(head,CasaCorrente) == ALFIERE)	
	      			break;

	  		case TORRE:
	    			col = COLONNA (CasaCorrente);
	    			for (r = CasaCorrente - col, t = CasaCorrente - 1; t >= r; t--)
	      {			/* orizontale verso sinistra */
		if (case_x_colore(head,t) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 }
		 if (case_x_colore(head,t) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 }
	      }
	    for (r = CasaCorrente - col + 7, t = CasaCorrente + 1; t <= r; t++)
	      {			/* verso destra */
		if (case_x_colore(head,t) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 }
		 if (case_x_colore(head,t) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 }
	      }
	    for (t = CasaCorrente - 8; t >= 0; t -= 8)
	      {			/* verticale verso sopra */
		if (case_x_colore(head,t) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 }
		 if (case_x_colore(head,t) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 }
	      }
	    for (t = CasaCorrente + 8; t < 64; t += 8)
	      {			/* verso giu */
		if (case_x_colore(head,t) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 }
		 if (case_x_colore(head,t) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 }
	      }
	    break;

	  case CAVALLO:
	    col = COLONNA (CasaCorrente);
	    t=CasaCorrente - 6;
	    if (t >= 0 && col < 6 && case_x_colore(head,t) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 }
	   if (t >= 0 && col < 6 && case_x_colore(head,t) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 }
	   t=CasaCorrente - 10;
	    if (t >= 0 && col > 1 && case_x_colore(head,t) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 }
	   if (t >= 0 && col > 1 && case_x_colore(head,t) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 }
	    t=CasaCorrente - 15;
	    if (t >= 0 && col < 7 && case_x_colore(head,t) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 }
	   if (t >= 0 && col < 7 && case_x_colore(head,t) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 }
	    t=CasaCorrente - 17;
	    if (t >= 0 && col > 0 && case_x_colore(head,t) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 }
	    if (t >= 0 && col > 0 && case_x_colore(head,t) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 }
	    t=CasaCorrente + 6;
	    if (t < 64 && col > 1 && case_x_colore(head,t) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 }
	    if (t < 64 && col > 1 && case_x_colore(head,t) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 }
	    t=CasaCorrente + 10;
	    if (t < 64 && col < 6 && case_x_colore(head,t) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 }
            if (t < 64 && col < 6 && case_x_colore(head,t) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 }
	    t=CasaCorrente + 15;
	    if (t < 64 && col > 0 && case_x_colore(head,t) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 }
	    if (t < 64 && col > 0 && case_x_colore(head,t) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 }
	   t=CasaCorrente + 17;
	    if (t < 64 && col < 7 && case_x_colore(head,t) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),1);
		 }
	    if (t < 64 && col < 7 && case_x_colore(head,t) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (t),0);
		 }
	    break;

	  case RE:
	    /* il re si puo muovere di una casella in tutte le direzioi */
	    col = COLONNA (CasaCorrente);
	    if (col && case_x_colore(head,CasaCorrente - 1) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente-1),1);
		 }	/* sinistra */
	    if (col && case_x_colore(head,CasaCorrente - 1) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente-1),0);
		 }
	    if (col < 7 && case_x_colore(head,CasaCorrente + 1) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente+1),1);
		 }	/* destra */
		 if (col < 7 && case_x_colore(head,CasaCorrente + 1) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente+1),0);
		 }
	    if (CasaCorrente > 7 && case_x_colore(head,CasaCorrente - 8) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente-8),1);
		 }	/* sopra */
		 if (CasaCorrente > 7 && case_x_colore(head,CasaCorrente - 8) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente-8),0);
		 }
	    if (CasaCorrente < 56 && case_x_colore(head,CasaCorrente + 8) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente+8),1);
		 }	/* sotto */
		 if (CasaCorrente < 56 && case_x_colore(head,CasaCorrente + 8) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente+8),0);
		 }
	    if (col && CasaCorrente > 7 && case_x_colore(head,CasaCorrente - 9) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente-9),1);
		 }	/* diagonale verso sopra */
		 if (col && CasaCorrente > 7 && case_x_colore(head,CasaCorrente - 9) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente-9),0);
		 }
	    if (col < 7 && CasaCorrente > 7 && case_x_colore(head,CasaCorrente - 7) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente-7),1);
		 }	/* diagonale verso sopra */
		 if (col < 7 && CasaCorrente > 7 && case_x_colore(head,CasaCorrente - 7) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente-7),0);
		 }
	    if (col && CasaCorrente < 56 && case_x_colore(head,CasaCorrente + 7) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente+7),1);
		 }	/* diagonale verso sotto */
		 if (col && CasaCorrente < 56 && case_x_colore(head,CasaCorrente + 7) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente+7),0);
		 }
	    if (col < 7 && CasaCorrente < 56 && case_x_colore(head,CasaCorrente + 9) == VUOTO){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente+9),1);
		 }	/* diagonale verso sotto */
		 if (col < 7 && CasaCorrente < 56 && case_x_colore(head,CasaCorrente + 9) == avv){
		 SalvaMossa(&head_mossa,CasaCorrente, (CasaCorrente+9),0);
		 }

		}
	}
	}
return head_mossa;
}



/* Inizializza() inizializza la scacchiera */
struct nodo_Casella* Inizializza(struct nodo_Casella* head) {
	int i;
	
	struct nodo_Casella* casa=head;
	
	for(i=0;i < NUMEROCASE; i++){
	struct nodo_Casella* p; 
    	//alloco il nuovo nodo
    	struct nodo_Casella* new_node;
    	new_node = (struct nodo_Casella*)malloc(sizeof(struct nodo_Casella));
    
    	//popolo il nuovo nodo con i dati
    	(new_node->dato).Colore=ColoriIniziali[i];
    	(new_node->dato).Pezzo=PezziIniziali[i];
    	//lui sara' l'ultimo quindi deve puntare a NULL
    	new_node->next = NULL;
    	//se la lista e' vuota lui sara' anche il primo quindi la testa deve puntare a lui
    	if(casa==NULL){  
        casa=new_node;
    	}
    	else{
   	//inizio a scorrere partendo dalla testa
    	p = casa;
    	//scorro tutta la lista
    	while (p->next != NULL)
        	p = p->next;
    	//inserisco in coda
    	p->next = new_node;
    	}
    	
    }
head=casa;

return head;
}



/* ritorna FALSE se la casella e attaccata altrimenti TRUE */

int CasaAttaccata ( struct nodo_Casella* head,int r, int ChiMuove)
{

  int h;
  int t;
  int riga;			
  int col;			
  int latoatt;
  
  if(ChiMuove==BIANCO)
  latoatt = NERO;
  else	latoatt = BIANCO;    /* lato di chi attacca */

  
  riga = RIGA (r);  /* riga e colonna nelle quali si trova il re */
  col = COLONNA (r);

  /* controllo se il cavallo attacca */
  if (col > 0 && riga > 1 && case_x_colore(head,r-17) == latoatt && case_x_pezzo(head,r-17) == CAVALLO)
    return FALSE;
  if (col < 7 && riga > 1 && case_x_colore(head,r-15) == latoatt && case_x_pezzo(head,r-15) == CAVALLO)
    return FALSE;
  if (col > 1 && riga > 0 && case_x_colore(head,r-10) == latoatt && case_x_pezzo(head,r-10) == CAVALLO)
    return FALSE;
  if (col < 6 && riga > 0 && case_x_colore(head,r-6) == latoatt && case_x_pezzo(head,r-6) == CAVALLO)
    return FALSE;
  if (col > 1 && riga < 7 && case_x_colore(head,r+6) == latoatt && case_x_pezzo(head,r+6) == CAVALLO)
    return FALSE;
  if (col < 6 && riga < 7 && case_x_colore(head,r+10) == latoatt && case_x_pezzo(head,r+10) == CAVALLO)
    return FALSE;
  if (col > 0 && riga < 6 && case_x_colore(head,r+15) == latoatt && case_x_pezzo(head,r+15) == CAVALLO)
    return FALSE;
  if (col < 7 && riga < 6 && case_x_colore(head,r+17) == latoatt && case_x_pezzo(head,r+17) == CAVALLO)
    return FALSE;

  /* controllo delle linee orizontali e verticali DONNA, TORRE, RE */
  /* giu */
  t = r + 8;
  if (t < 64)
    {
      if (case_x_colore(head,t) == latoatt && (case_x_pezzo(head,t) == RE || case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == TORRE))
	return FALSE;
      if (case_x_pezzo(head,t) == VUOTO)
	for (t += 8; t < 64; t += 8)
	  {
	    if (case_x_colore(head,t) == latoatt && (case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == TORRE))
	      return FALSE;
	    if (case_x_pezzo(head,t) != VUOTO)
	      break;
	  }
    }
  /* sinistra */
  t = r - 1;
  h = r - col;
  if (t >= h)
    {
      if (case_x_colore(head,t) == latoatt && (case_x_pezzo(head,t) == RE || case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == TORRE))
	return FALSE;
      if (case_x_pezzo(head,t) == VUOTO)
	for (t--; t >= h; t--)
	  {
	    if (case_x_colore(head,t) == latoatt && (case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == TORRE))
	      return FALSE;
	    if (case_x_pezzo(head,t) != VUOTO)
	      break;
	  }
    }
  /* destra */
  t = r + 1;
  h = r - col + 7;
  if (t <= h)
    {
      if (case_x_colore(head,t) == latoatt && (case_x_pezzo(head,t) == RE || case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == TORRE))
	return FALSE;
      if (case_x_pezzo(head,t) == VUOTO)
	for (t++; t <= h; t++)
	  {
	    if (case_x_colore(head,t) == latoatt && (case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == TORRE))
	      return FALSE;
	    if (case_x_pezzo(head,t) != VUOTO)
	      break;
	  }
    }
  /* su */
  t = r - 8;
  if (t >= 0)
    {
      if (case_x_colore(head,t) == latoatt && (case_x_pezzo(head,t) == RE || case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == TORRE))
	return FALSE;
      if (case_x_pezzo(head,t) == VUOTO)
	for (t -= 8; t >= 0; t -= 8)
	  {
	    if (case_x_colore(head,t) == latoatt && (case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == TORRE))
	      return FALSE;
	    if (case_x_pezzo(head,t) != VUOTO)
	      break;
	  }
    }
  /* controllo di eventuali attacchi dalle diagonali  DONNA, ALFIERE, RE, PEDONE */
  /* diagonale destra giu */
  t = r + 9;
  if (t < 64 && COLONNA (t) != 0)
    {
      if (case_x_colore(head,t) == latoatt)
	{
	  if (case_x_pezzo(head,t) == RE || case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == ALFIERE)
	    return FALSE;
	  if (ChiMuove == NERO && case_x_pezzo(head,t) == PEDONE)
	    return FALSE;
	}
      if (case_x_pezzo(head,t) == VUOTO)
	for (t += 9; t < 64 && COLONNA (t) != 0; t += 9)
	  {
	    if (case_x_colore(head,t) == latoatt && (case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == ALFIERE))
	      return FALSE;
	    if (case_x_pezzo(head,t) != VUOTO)
	      break;
	  }
    }
  /* diagonale sinistra giu */
  t = r + 7;
  if (t < 64 && COLONNA (t) != 7)
    {
      if (case_x_colore(head,t) == latoatt)
	{
	  if (case_x_pezzo(head,t) == RE || case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == ALFIERE)
	    return FALSE;
	  if (ChiMuove == NERO && case_x_pezzo(head,t) == PEDONE)
	    return FALSE;
	}
      if (case_x_pezzo(head,t) == VUOTO)
	for (t += 7; t < 64 && COLONNA (t) != 7; t += 7)
	  {
	    if (case_x_colore(head,t)== latoatt && (case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == ALFIERE))
	      return FALSE;
	    if (case_x_pezzo(head,t) != VUOTO)
	      break;

	  }
    }
  /* diagonale sinistra su */
  t = r - 9;
  if (t >= 0 && COLONNA (t) != 7)
    {
      if (case_x_colore(head,t) == latoatt)
	{
	  if (case_x_pezzo(head,t) == RE || case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == ALFIERE)
	    return FALSE;
	  if (ChiMuove == BIANCO && case_x_pezzo(head,t) == PEDONE)
	    return FALSE;
	}
      if (case_x_pezzo(head,t) == VUOTO)
	for (t -= 9; t >= 0 && COLONNA (t) != 7; t -= 9)
	  {
	    if (case_x_colore(head,t) == latoatt && (case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == ALFIERE))
	      return FALSE;
	    if (case_x_pezzo(head,t) != VUOTO )
	      break;

	  }
    }
  /* diagonale destra su */
  t = r - 7;
  if (t >= 0 && COLONNA(t) != 0)
    {
      if (case_x_colore(head,t) == latoatt)
	{
	  if (case_x_pezzo(head,t) == RE || case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == ALFIERE)
	    return FALSE;
	  if (ChiMuove == BIANCO && case_x_pezzo(head,t) == PEDONE)
	    return FALSE;
	}
      if (case_x_pezzo(head,t) == VUOTO)
	for (t -= 7; t >= 0 && COLONNA (t) != 0; t -= 7)
	  {
	    if (case_x_colore(head,t) == latoatt && (case_x_pezzo(head,t) == DONNA || case_x_pezzo(head,t) == ALFIERE))
	      return FALSE;
	    if (case_x_pezzo(head,t) != VUOTO)
	      break;
	  }
    }
  return TRUE;
}

int posRe;
int InScacco(struct nodo_Casella* head, int ChiMuove) {

	
    int j;
	
	/* verifica che il re non sia in una casa destinazione */
	for (j = 0; j < NUMEROCASE; j++) 
		if (case_x_pezzo(head,j) == RE && case_x_colore(head,j) == ChiMuove) 
			 break;
posRe=j;
	 return CasaAttaccata (head,j,ChiMuove);
}

BOOL MATTO;
void Matto(struct nodo_Casella* head, int ChiMuove) {
   
    	int avv;
	if(ChiMuove==BIANCO)
 	avv = NERO;
  	else	avv = BIANCO;    /* lato di chi attacca */
 	BOOL matto;
   
    	MATTO = TRUE; //c'e scacco
    	int i,r=0;
    	int posRe_nuovo;
    	struct nodo_Mossa *head_mossa=GeneraMosse(head,ChiMuove);
    	struct nodo_Mossa *t=head_mossa;
    	while(head_mossa != NULL) {
    				posRe_nuovo=posRe;
    				trova_e_sostituisci_casella (head, case_x_colore (head, head_mossa->dato.Da),case_x_pezzo (head, head_mossa->dato.Da), head_mossa->dato.A);
    				if( head_mossa->dato.Da==posRe) 
    				posRe_nuovo=head_mossa->dato.A;
				trova_e_sostituisci_casella (head, VUOTO,VUOTO, head_mossa->dato.Da);
				if(CasaAttaccata(head,posRe_nuovo,ChiMuove) == FALSE){
				trova_e_sostituisci_casella (head, case_x_colore (head, head_mossa->dato.A),case_x_pezzo (head, head_mossa->dato.A),head_mossa->dato.Da);
				MATTO=TRUE;
				}
				else {
				struct nodo_Mossa *mosse_prova=GeneraMosse(head,avv);
				struct nodo_Mossa *w=mosse_prova;
				while(mosse_prova != NULL) {
				if(mosse_prova->dato.A == posRe && mosse_prova->dato.A != posRe_nuovo){
				MATTO=FALSE;
				}
				mosse_prova=mosse_prova->next;
				}
				svuota_lista_mosse(&w);
				}
				head_mossa=head_mossa->next;
				}
   //printf(" esito=%d",MATTO);
   svuota_lista_mosse(&t);
   
}


char lettera;
char cattura;
char l_prom;
/* Input riporta una mossa sulla scacchiera */
int Input(struct nodo_Casella* head, char * Input, int ChiMuove) {
	int i, Da, A, ch;
	Mossa M;
	BOOL Trovato = FALSE;
	/* genera le mosse pseudolegali per la posizione corrente */
	struct nodo_Mossa *head_mossa=NULL;
	head_mossa=GeneraMosse(head,ChiMuove);
	struct nodo_Mossa *t=head_mossa;

	/* calcola dai primi due caratteri l'indice della casa di partenza */
	M.Da = (Input[0] - 'a') + 8 * (8 - (Input[1] - '0'));
	/* calcola dai caratteri rimanenti l'indice della casa di destinazione */
	M.A  = (Input[2] - 'a') + 8 * (8 - (Input[3] - '0'));

	/* controlla che la mossa sia ammissibile cercandola nello stack */
	while(head_mossa != NULL) {
		if ((head_mossa->dato.Da == M.Da) && (head_mossa->dato.A  == M.A)) {
			Trovato = TRUE;
			cattura=head_mossa->dato.cat ;
			if(case_x_pezzo(head,M.Da)==RE) lettera='R';
			if(case_x_pezzo(head,M.Da)==DONNA) lettera='D';
			if(case_x_pezzo(head,M.Da)==ALFIERE) lettera='A';
			if(case_x_pezzo(head,M.Da)==CAVALLO) lettera='C';
			if(case_x_pezzo(head,M.Da)==TORRE) lettera='T';
			if(case_x_pezzo(head,M.Da)==PEDONE) lettera=' ';
			
			if ((case_x_pezzo (head, M.Da) == PEDONE) && ((RIGA(M.A) == 0) || (RIGA(M.A) == 7))) {
			printf("\nInserisci il nemero del pezzo che vuoi come promozione. (i pezzi che si possono prendere sono i seguenti, es. DONNA(4), TORRE(3), ALFIERE(2), CAVALLO(1))\n");
				scanf(" %d",&ch);
				if(ch>0 && ch<5){
				trova_e_sostituisci_casella (head, case_x_colore (head, M.Da),ch, M.A);
				trova_e_sostituisci_casella (head, VUOTO,VUOTO, M.Da);
				if(ch==1) l_prom='C';
				if(ch==2) l_prom='A';
				if(ch==3) l_prom='T';
				if(ch==4) l_prom='D';
				}
				else{ 
				Trovato=FALSE;
				l_prom=' ';}
				}
			else{
			trova_e_sostituisci_casella (head, case_x_colore (head, M.Da),case_x_pezzo (head, M.Da), M.A);
			trova_e_sostituisci_casella (head, VUOTO,VUOTO, M.Da);}

			if(InScacco(head,ChiMuove) == FALSE){
				printf(" Il re e' sotto scacco ");
				Trovato=InScacco(head,ChiMuove);
				trova_e_sostituisci_casella (head, case_x_colore (head, M.A),case_x_pezzo (head, M.A), M.Da);
				Matto(head,ChiMuove);
				}
			
		break;
		}
		else head_mossa=head_mossa->next;
		}
	svuota_lista_mosse(&t);			
	/* controlla se si tratta di un arrocco */
	if (ChiMuove == BIANCO) {
	if (case_x_colore(head,E1) == ChiMuove) {
			if (case_x_pezzo(head,E1) == RE) {
		if (!strcmp(Input, "e1g1")) {
		if(case_x_pezzo(head,H1) == TORRE){
		if((case_x_colore(head,F1) == VUOTO) && (case_x_colore(head,G1) == VUOTO)){
			if((CasaAttaccata(head,60,ChiMuove) == FALSE)||(CasaAttaccata(head,61,ChiMuove) == FALSE)||(CasaAttaccata(head,62,ChiMuove) == FALSE)||(CasaAttaccata(head,63,ChiMuove) == FALSE)){
				printf(" l'arrocco non e' possibile ");
				Trovato=FALSE;
				}
			else{
				M.Da = H1; M.A = F1;
				trova_e_sostituisci_casella (head, case_x_colore (head, M.Da),case_x_pezzo (head, M.Da), M.A);
				trova_e_sostituisci_casella (head, VUOTO,VUOTO, M.Da);
				M.Da = E1; M.A = G1;
				trova_e_sostituisci_casella (head, case_x_colore (head, M.Da),case_x_pezzo (head, M.Da), M.A);
				trova_e_sostituisci_casella (head, VUOTO,VUOTO, M.Da);
				lettera='R';
				Trovato = TRUE;
			}
		}}}
		if (!strcmp(Input, "e1c1")) {
		if(case_x_pezzo(head,A1) == TORRE){
		if((case_x_colore(head,D1) == VUOTO) && (case_x_colore(head,C1) == VUOTO) && (case_x_colore(head,B1) == VUOTO)){
			if((CasaAttaccata(head,60,ChiMuove) == FALSE)||(CasaAttaccata(head,59,ChiMuove) == FALSE)||(CasaAttaccata(head,58,ChiMuove) == FALSE)||(CasaAttaccata(head,57,ChiMuove) == FALSE)||(CasaAttaccata(head,56,ChiMuove) == FALSE)){
				printf(" L'arrocco non e' possibile ");
				Trovato=FALSE;
				}
			else{
				M.Da = A1; M.A = D1;
				trova_e_sostituisci_casella (head, case_x_colore (head, M.Da),case_x_pezzo (head, M.Da), M.A);
				trova_e_sostituisci_casella (head, VUOTO,VUOTO, M.Da);
				M.Da = E1; M.A = C1;
				trova_e_sostituisci_casella (head, case_x_colore (head, M.Da),case_x_pezzo (head, M.Da), M.A);
				trova_e_sostituisci_casella (head, VUOTO,VUOTO, M.Da);
				lettera='R';
				Trovato = TRUE;
			}
		}
		}}
		}
	}} else {
	if (case_x_colore(head,E8) == ChiMuove) {
			if (case_x_pezzo(head,E8) == RE) {
		if (!strcmp(Input, "e8g8")) {
		if(case_x_pezzo(head,H8) == TORRE){
		if((case_x_colore(head,F8) == VUOTO) && (case_x_colore(head,G8) == VUOTO)){
			if((CasaAttaccata(head,4,ChiMuove) == FALSE)||(CasaAttaccata(head,5,ChiMuove) == FALSE)||(CasaAttaccata(head,6,ChiMuove) == FALSE)||(CasaAttaccata(head,7,ChiMuove) == FALSE)){
				printf(" L'arrocco non e' possibile ");
				Trovato=FALSE;
				}
			else{
				M.Da = H8; M.A = F8;
				trova_e_sostituisci_casella (head, case_x_colore (head, M.Da),case_x_pezzo (head, M.Da), M.A);
				trova_e_sostituisci_casella (head, VUOTO,VUOTO, M.Da);
				M.Da = E8; M.A = G8;
				trova_e_sostituisci_casella (head, case_x_colore (head, M.Da),case_x_pezzo (head, M.Da), M.A);
				trova_e_sostituisci_casella (head, VUOTO,VUOTO, M.Da);
				lettera='R';
				Trovato = TRUE;
			}
		}}}
		if (!strcmp(Input, "e8c8")) {
		if(case_x_pezzo(head,A8) == TORRE){
		if((case_x_colore(head,D8) == VUOTO) && (case_x_colore(head,C8) == VUOTO) && (case_x_colore(head,B8) == VUOTO)){
			if((CasaAttaccata(head,0,ChiMuove) == FALSE)||(CasaAttaccata(head,1,ChiMuove) == FALSE)||(CasaAttaccata(head,2,ChiMuove) == FALSE)||(CasaAttaccata(head,4,ChiMuove) == FALSE)||(CasaAttaccata(head,4,ChiMuove) == FALSE)){
				printf(" L'arrocco non e' possibile ");
				Trovato=FALSE;
				}
			else{
				M.Da = A8; M.A = D8;
				trova_e_sostituisci_casella (head, case_x_colore (head, M.Da),case_x_pezzo (head, M.Da), M.A);
				trova_e_sostituisci_casella (head, VUOTO,VUOTO, M.Da);
				M.Da = E8; M.A = C8;
				trova_e_sostituisci_casella (head, case_x_colore (head, M.Da),case_x_pezzo (head, M.Da), M.A);
				trova_e_sostituisci_casella (head, VUOTO,VUOTO, M.Da);
				lettera='R';
				Trovato = TRUE;
			}
		}}}
	}
	}
	}
	
	
		
	return Trovato;
}

void titolo(){
printf("\n");
printf("\n");
	printf("    ███████╗ ██████╗ █████═╗ ███████╗ ██████╗██╗  ██╗██╗ \n");
	printf("    ██╔════╝██╔════╝██║  ██║██╔═════╝██╔════╝██║  ██║██║ \n");
	printf("    ███████╗██║     ███████║██║      ██║     ███████║██║ \n");
	printf("    ╚════██║██║     ██╔══██║██║      ██║     ██╔══██║██║ \n");
	printf("    ███████║╚██████╗██║  ██║ ╚██████╗╚██████╗██║  ██║██║ \n");
	printf("    ╚══════╝ ╚═════╝╚═╝  ╚═╝  ╚═════╝ ╚═════╝╚═╝  ╚═╝╚═╝ \n\n\n\n");
}

/* VisualizzaScacchiera visualizza la posizione dei pezzi */
void VisualizzaScacchiera(struct nodo_Casella* head, int ChiMuove) {
	int i;
	/* pezzi neri */
	char *SimboliPezziNero[NUMEROPEZZI] = {"\u2659","\u2658","\u2657","\u2656","\u2655","\u2654"};
	/*  pezzi bianchi  */
	char *SimboliPezziBianco[NUMEROPEZZI] = {"\u265F","\u265E","\u265D","\u265C","\u265B","\u265A"};
	
	titolo();
	printf("\n");
	printf("\n       +----+----+----+----+----+----+----+----+");
	printf("\n    8  |");
	for (i = 0; i < NUMEROCASE; i++) {
	
		switch (case_x_colore(head,i)) {
			case BIANCO:
				printf("  %s |", SimboliPezziBianco[case_x_pezzo(head,i)]);
				break;
			case NERO:
				printf("  %s |", SimboliPezziNero[case_x_pezzo(head,i)]);
				break;
			case VUOTO:
				printf("    |");
				break;
		}
		if ((i + 1) % 8 == 0 && i != (NUMEROCASE - 1)){
			printf("\n       +----+----+----+----+----+----+----+----+\n    %d  |", 7 - RIGA(i));
			
			}
	}
	printf("\n       +----+----+----+----+----+----+----+----+\n         a    b    c    d    e    f    g    h\n");
	
	if (ChiMuove == BIANCO) {
		printf("\n  Turno: BIANCO\n");
	} else {
		printf("\n  Turno: NERO\n");
	}
}

/* main gestisce il interfaccia dei comandi */
int main() {
	
		
	char Command[256];	/* riga di comando */
	char da_c, da_r, a_c, a_r;
	struct nodo_Casella* Scacchiera=NULL; 
	struct nodo_Storia* Crono=NULL;
	struct giocatore *Giocatore=NULL;
	
	int matto=0;
	int patta=0; //patta 1, non patta 0
	int  ChiMuove=BIANCO;			/* colore che deve muovere */
	
	
	int i=0;
	char scelta[20]="";
	char s[20]="";
	char gbianco[20], gnero[20];
	char player1[20]="";
	char player2[20]="";
	
	char risp[5]="";
     
	caricaGiocatori(&Giocatore);
	
	

     for(i=0;i<2;i++)
     {
     titolo();
	struct giocatore *temp=NULL;
        temp=Giocatore;

	printf("                ╔══════════════════════════╗\n");
	printf("                ║                          ║\n");
	printf("                ║          accedi          ║\n");
	printf("                ║                          ║\n");
	printf("                ║        registrati        ║\n");
	printf("                ║                          ║\n");
	printf("                ╚══════════════════════════╝\n\n");
	printf("\n\n Digita 'accedi' se ti sei già registrato o 'registrati' se sei un nuovo utente:\n");
        scanf(" %s",scelta);
        printf(RESET);

        if((strcmp(scelta,"accedi")==0)&&(Giocatore!=NULL))
         {
          if(i==0) strcpy(player1,Login(Giocatore));
          else strcpy(player2,Login(Giocatore));
         
	}
	else
         {
             if(Giocatore==NULL)
             {
                 printf("\n Ancora non si e' registrato nessuno, ti stiamo registrando.\n");
             }

              if(i==0) strcpy(player1,Registrazione(&Giocatore));
             else strcpy(player2,Registrazione(&Giocatore));
             
         }printf(RESET);
	}
	
	stampa_classifica(Giocatore);
	printf(" %s vuoi giocare con il bianco o con il nero? (Digita 'bianco' o 'nero')\n",player1);
	scanf(" %s",scelta);
	if(strcmp(scelta,"bianco")==0){
	strcpy(gbianco,player1);
	strcpy(gnero,player2);}
	else {
	strcpy(gnero,player1);
	strcpy(gbianco,player2);}
	printf("\n");
	printf(RESET);
	
	for(;;) {
	titolo();
	printf("                ╔══════════════════════════╗\n");
	printf("                ║                          ║\n");
	printf("                ║           new            ║\n");
	printf("                ║                          ║\n");
	printf("                ║        classifica        ║\n");
	printf("                ║                          ║\n");
	printf("                ║           help           ║\n");
	printf("                ║                          ║\n");
	printf("                ║           exit           ║\n");
	printf("                ║                          ║\n");
	printf("                ╚══════════════════════════╝\n\n");
	printf("-> ");
	scanf(" %s",s);
	
	
	if(!strcmp(s, "classifica")) {
			stampa_classifica(Giocatore);
			sleep(20);
			printf(RESET);
			continue;
		}
	if (!strcmp(s, "help")) {
			
			printf("\n Libretto Delle Istruzioni\n");
			printf(" Le regole degli scacchi prevedono che ogni singolo pezzo si sposti sulla scacchiera in modo preciso.\n Il giocatore che muove per primo è colui che vede il proprio Re a destra della propria Regina e si chiama Bianco mentre l’avversario è il Nero.\n Apertura, mediogioco e finale sono le tre fasi che compongono ogni partita.\n");
			printf(" Nessun pezzo, secondo il regolamento, può occupare una casella in cui si trova un altro pezzo dello stesso schieramento.\n Al contrario un pezzo può spingersi verso una casa occupata da un pezzo avversario eliminandolo e prendendo il suo posto sulla scacchiera.\n In questo caso si parla di “cattura” e si annota con x, mentre quando un pezzo “attacca” o “minaccia” una casa significa che può muoversi su di essa.\n");
			printf(" Nelle partite ufficiali non si puo' annullare la mossa fatta.\n\n\n");
			printf(" COME SI MUOVONO I PEZZI: \n");
			printf(" ♙  Pedone: la prima mossa gli consente di muoversi di una o due case in avanti a patto che la casella finale e la casa saltata siano libere.\n Nelle mosse successive il pedone può avanzare di una sola casella.\n A differenza degli altri pezzi non può mai muoversi all’indietro e cattura gli avversari spostandosi in senso diagonale e sempre in avanti.\n");
			printf(" ♘  Cavallo: può spostarsi e catturare sia su caselle nere che su caselle bianche e i suoi movimenti formano idealmente una “L”.\n È l’unico dei sedici pezzi a cui è permesso “saltare” sia gli alleati che gli avversari e pur trovandosi dietro i pedoni può essere mosso senza spostarli.\n");
			printf(" ♗  Alfiere: può muoversi diagonalmente in base al numero delle caselle libere disponibili e la sua casa d’arrivo non può essere occupata da un pezzo amico. \n");
			printf(" ♖  Torre: la torre può muoversi sia in senso verticale che orizzontale in base alle caselle libere disponibili davanti e di fianco.\n Se non ostacolata da uno qualunque dei pezzi, a prescindere dalla sua posizione, la torre controlla sempre 14 case.\n È protagonista, insieme al re, di una mossa particolare conosciuta con il nome di arrocco, l’unica che permette di spostare contemporaneamente due pezzi e che consente al Re di muoversi di due caselle, l'arrocco può essere corto O-O o lungo O-O-O, nel primo caso il re si muove alla destra del giocatore, nel secondo caso alla sinistra.\n");
			printf(" ♕  Regina: o donna (D), è il pezzo più forte e conosciuto in passato con il nome di “generale”, “stratega” o “visir”.\n Può spostarsi verticalmente, orizzontalmente o diagonalmente in base alle caselle libere disponibili e a ogni mossa può anche scegliere se muoversi come un alfiere o una torre.\n A differenza della torre non può effettuare la mossa dell’arrocco.\n");
			printf(" ♔  Re: tra i sedici pezzi è il più importante e può muoversi in qualsiasi direzione ma una casa alla volta e a patto che la casella di destinazione non sia minacciata da un pezzo avversario.\n Al re è permesso catturare pezzi avversari muovendosi sulla casa occupata da questi ultimi e insieme alla torre può eseguire la mossa dell’arrocco.\n Quando il re è minacciato si trova sotto scacco.\n Lo scacco matto (o più semplicemente, matto) è invece una situazione che si verifica quando non più possibile per il giocatore difendere il re con altri pezzi o spostarlo in una casella libera che lo salverebbe dall’avversario.\n Il giocatore che utilizza questa mossa chiude la partita e si aggiudica la vittoria.\n");
			sleep(25);
			printf(RESET);
			continue;
		}
	if (!strcmp(s, "new")) {
	printf(RESET);
	printf("                ╔══════════════════════════╗\n");
	printf("                ║                          ║\n");
	printf("                ║          comandi         ║\n");
	printf("                ║                          ║\n");
	printf("                ║        classifica        ║\n");
	printf("                ║                          ║\n");
	printf("                ║        cronologia        ║\n");
	printf("                ║                          ║\n");
	printf("                ║          patta           ║\n");
	printf("                ║                          ║\n");
	printf("                ║         abbandono        ║\n");
	printf("                ║                          ║\n");
	printf("                ╚══════════════════════════╝\n\n");
	
	
	Scacchiera=Inizializza(Scacchiera);
	VisualizzaScacchiera(Scacchiera, ChiMuove);
	
	

	for(;;) {
	
	

		/* prende l'input da consolle */
		printf("-> ");
		
		
		
		if (scanf("%s", Command) == EOF)
			return 0;

		if (!strcmp(Command, "comandi")) {
			printf("\npatta - la partita finisce in pareggio.\n");
			printf("abbandono - mi arrendo (perdo la partita).\n");
			printf("classifica - visualizza la classifica dei giocatori\n");
			printf("cronologia - visualizza l'annotazione delle mosse giocate\n");
			printf("\nes. mosse: inserisci le coordinate di partenza e arrivo: e2e4, e1g1 per arrocco\n\n");
			sleep(7);
			printf(RESET);
			VisualizzaScacchiera(Scacchiera,ChiMuove);
			continue;
		}

		if(!strcmp(Command, "classifica")) {
		
			stampa_classifica(Giocatore);
			sleep(20);
			printf(RESET);
			VisualizzaScacchiera(Scacchiera,ChiMuove);
			continue;
		}
		
		if(!strcmp(Command, "cronologia")) {
		
			stampa(&Crono,matto); //per una vista veloce
			sleep(10);
			printf(RESET);
			VisualizzaScacchiera(Scacchiera,ChiMuove);
			continue;
		}

		if (!strcmp(Command, "abbandono")) {
			
						
			if (ChiMuove == BIANCO) {
			trova_e_sostituisci(&Giocatore, gnero,gbianco,patta);
			printf("\n 0-1 {%s vince a tavolino}\n",gnero);
			
			} else {
			trova_e_sostituisci(&Giocatore, gbianco,gnero,patta);
			printf("\n 1-0 {%s vince a tavolino}\n",gbianco);
			}
			sleep(5);
			printf(RESET);
			stampa(&Crono,matto);
			printf("\n\n-> Ciao!\n");
			sleep(15);
			printf(RESET);
			stampa_classifica(Giocatore);
	    		sleep(5);
	    		salvaUtenti(Giocatore);
	    		salvaAnnotazione(&Crono,matto,gbianco,gnero);
			svuota_lista_giocatori(&Giocatore);
			svuota_lista_casella(&Scacchiera);
			svuota_lista_crono(&Crono);
			printf(RESET);
			return 0;
		}
		
		if(!strcmp(Command, "patta")) {
		if (ChiMuove == BIANCO) {
			printf("\n %s vuoi accettare la patta? (si/no)  \n",gnero);
			scanf("%s",risp);
			} else {
			printf("\n %s vuoi accettare la patta? (si/no)  \n",gbianco);
			scanf("%s",risp);
			}
			if(!strcmp(risp, "si")){
			printf("\n 0.5-0.5 {patta}\n\n\n");
			sleep(5);
			printf(RESET);
			stampa(&Crono,matto);
			printf("\n\n-> Ciao!\n");
			sleep(15);
			patta=1;
	    		trova_e_sostituisci(&Giocatore, gnero,gbianco,patta);
	    		patta=0;
	    		printf(RESET);	
	    		stampa_classifica(Giocatore);
	    		sleep(5);
	    		salvaUtenti(Giocatore);
	    		salvaAnnotazione(&Crono,matto,gbianco,gnero);
			svuota_lista_giocatori(&Giocatore);
			svuota_lista_casella(&Scacchiera);		
			svuota_lista_crono(&Crono);
			printf(RESET);
			return 0;}
			else {
			printf(RESET);
			VisualizzaScacchiera(Scacchiera,ChiMuove);
			continue;}
		}
	
		
		if (Input(Scacchiera,Command,ChiMuove) == TRUE) {
			da_c = Command[0];
			da_r= Command[1];
			a_c = Command[2];
			a_r =Command[3] ;
			inserisci_storia(&Crono,&da_c,&da_r,&a_c,&a_r,cattura,&lettera,&l_prom);
			l_prom=' ';
			printf(RESET);
			/* cambia colore */
			ChiMuove ^= 1;
			/* mossa accettata, mostra il risultato */
			VisualizzaScacchiera(Scacchiera,ChiMuove);
			
			
		} 
		
		else {
		
			/* Comando non valido */
			printf(" Comando non valido\n");
		}
		

		if (MATTO == TRUE){
			if (ChiMuove == BIANCO) {
			trova_e_sostituisci(&Giocatore, gnero,gbianco,patta);
			printf("\n 0-1 {%s vince per scacco matto}\n",gnero);
			} else {
			trova_e_sostituisci(&Giocatore, gbianco,gnero,patta);
			printf("\n 1-0 {%s vince per scacco matto}\n",gbianco);
			}
			matto=1;
			sleep(5);
			printf(RESET);
			stampa(&Crono,matto);
			printf("\n\n-> Ciao!\n");
			sleep(15);	
			printf(RESET);	
	    		stampa_classifica(Giocatore);
	    		sleep(5);
	    		salvaUtenti(Giocatore);
	    		salvaAnnotazione(&Crono,matto,gbianco,gnero);
			svuota_lista_giocatori(&Giocatore);
			svuota_lista_casella(&Scacchiera);
			svuota_lista_crono(&Crono);
			printf(RESET);
			return 0;
		}
	
	}
}
if(strcmp(s,"exit")==0){
	printf("\n Ciao!\n");
	salvaUtenti(Giocatore);
	svuota_lista_giocatori(&Giocatore);
	sleep(5);
	printf(RESET);
	return 0;
		}
}
}

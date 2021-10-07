#include "giocatore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

#define BOOL			int
#define TRUE			1
#define FALSE			0


void inserisciGiocatore(struct giocatore **head,char username[],char password[],int ELO, float Pt, int partite)
{

     
     //Creazione nodo dell'giocatore
     struct giocatore *p=NULL;
     p=(struct giocatore *)malloc(sizeof(struct giocatore));

     //Copia delle credenziali all'interno del nodo giocatore
     strcpy(p->username,username);
     strcpy(p->password,password);
     p->elo=ELO;
     p->punteggio=Pt;
     p->partite=partite;
     
    
    struct giocatore* r=*head;
    struct giocatore* q=*head;
    
    while((q!=NULL)&&(q->punteggio >= p->punteggio)){
    r=q;
    q=q->next;
    }
    if(q==*head){
    p->next=(*head);
    (*head)=p;
    }
    else{
    r->next=p;
    p->next=q;}
    

}

void caricaGiocatori(struct giocatore **head)
{
    
    //Dichiarazione ed apertura del file in lettura per andare a leggere gli utenti
     FILE *f;
     f=NULL;
     f=fopen("utenti.txt","r");

     //Dichiarazione variabili utenti
     char username[20]="";
     char password[20]="";
     int elo=0;
     float punteggio=0;
     int partite=0;

     //Se il file viene aperto correttamente
     if(f!=NULL)
     {
         //Ciclo che scorre il file fino a quando non si raggiunge la fine di esso, ed inserimento di ciascun utente nella lista utenti
        while(fscanf(f,"%s %s %d %f %d\n",(username),(password),(&elo),(&punteggio),(&partite))!=EOF)
         {
             inserisciGiocatore(head,username,password,elo,punteggio,partite);
         }
     }
     else
     {
         printf("\nErrore apertura file.\n");
         return;
     }
     
     fclose(f);
   
}

void cancellaGiocatore(struct giocatore** head, char* nome){
    
    struct giocatore* r = *head; //precedente 
    struct giocatore* q = *head; //successivo 
    
    while ((q != NULL) && (strcmp(q->username,nome)!=0)){
        //r raggiunge q
        r = q;
        //q va avanti di uno
        q = q->next;
    }
    
    //se sono ancora in testa alla lista devo spostare la testa
    if (q==*head)
        (*head) = (*head)->next;
    else
        r->next = q->next;
    //dealloco l'elemento
    free(q);
   
     
}

void trova_e_sostituisci (struct giocatore** head, char* nomevinc, char* nomeperd, int patta){
    struct giocatore* q = *head;
    while ((q != NULL) && (strcmp(q->username,nomevinc)!=0)){
        //scorro la lista
        q = q->next;
    }
    char nickname[20]="";
    char password[20]="";
       int elo=0;
    float punteggio=0;
    int partite=0;
    strcpy(nickname,q->username);
    strcpy(password,q->password);
        if(patta==1){
    elo=q->elo+5;
    punteggio=q->punteggio+0.5;
    }
    else{
    elo=q->elo+10;
    punteggio=q->punteggio+1;
    }
    partite=q->partite+1;
    cancellaGiocatore(head, nomevinc);
    
    inserisciGiocatore(head,nickname,password,elo,punteggio,partite);
    struct giocatore* s = *head;
    while ((s != NULL) && (strcmp(s->username,nomeperd)!=0)){
        //scorro la lista
        s = s->next;
    }
    
    strcpy(nickname,s->username);
    strcpy(password,s->password);
        if(patta==1){
    elo=s->elo+5;
    punteggio=s->punteggio+0.5;
    }
    else{
        elo=s->elo-5;
    punteggio=s->punteggio;
    }
    partite=s->partite+1;
    cancellaGiocatore(head, nomeperd);
    
    inserisciGiocatore(head,nickname,password,elo,punteggio,partite);
}
char n[20];

char* Registrazione(struct giocatore **head)
{
     char nickname[20]="";
     char password[20]="";
     int elo=0;
     float punteggio=0;
     int partite=0;
     
     struct giocatore *temp=NULL;

     temp=(*head);

     
     BOOL user_usato=FALSE;
     BOOL a=FALSE;
     
	printf("\n\n REGISTRAZIONE \n\n");

     //Se c'è almeno un giocatore già registrato
     if(temp!=NULL)
     {
         
     
         while(user_usato==FALSE)
         {
             
             printf("\n Inserisci username:  ");
             fflush(stdin);
             scanf("%s",nickname);
       
             a=FALSE;
             temp=(*head);
       
             
             while((temp!=NULL)&&(a==0))
             {
                 if(strcmp(temp->username,nickname)==0)
                 {
                     a=TRUE;
                 }
                 else
                 { 
                     temp=temp->next;
                 }
             }

             
             if(a==TRUE)
             {
                 printf("\n Username non disponibile. Inserisci un altro username.\n");
             }
             else
             {
                 
                 user_usato=TRUE;
             }
        
        }
     }
     else
     {
         
         printf("\n Inserisci username:  ");
         fflush(stdin);
         scanf("%s",nickname);  
     }

     
     fflush(stdin);
     printf("\n Inserisci password (P.S. ricordala per poter accede altre volte):   ");
     fflush(stdin);
     scanf("%s",password);
     fflush(stdin);
     printf("\n Inserisci il tuo ELO (se non hai mai giocato a scacchi e' 1440):  ");
     scanf("%d",&elo);
     fflush(stdin);
     
     strcpy(n,nickname);
 
     inserisciGiocatore(head,nickname,password,elo,punteggio,partite);
     printf("\nBenvenuto %s \n",nickname);
     return n;
}


char* Login(struct giocatore *head)
{
    //Dichiarazione variabili
     BOOL trovato=FALSE;
     BOOL a=FALSE;
     char nickname[20]="";
     char password[20]="";
 
     
     struct giocatore *temp=NULL;
     printf("\n\n LOGIN\n");
     //faccio un username finchè non trovo lo stesso username
     do
     {
         printf("\n Inserisci username:");
         fflush(stdin);
         scanf("%s",nickname);
       
         temp=head;

         while((temp!=NULL)&&(a==FALSE))
         {
             if(strcmp(temp->username,nickname)==0)
             {
                 a=TRUE;
             }
             else
             {
                 temp=temp->next;
             }
         }

         //Uscita dal ciclo while.Verifica se è stato trovato un username uguale a quello dell'giocatore 
         if(a==TRUE)
         {
             //La variabile di verifica diventa 1 e dunque l'giocatore è stato trovato nella lista
             trovato=TRUE;
         }
         else
         {
             printf("\n Username errato. \n");
         }

     }while(trovato==FALSE);

     trovato=FALSE;

     do
     {    
         //Inserimento della password 
          printf("\n Inserisci password:  ");
          fflush(stdin);
          scanf("%s",password);

          //Se la password inserita e corretta trovato diventa true e si esce dal ciclo while
         if(strcmp(temp->password,password)==0)
         {
             printf("\n Password corretta. Accesso in corso.\n");
             trovato=TRUE;
         }
         else
         {
             printf("\n Password errata. Riprova.\n");
         }

     }while(trovato==FALSE);
     
     
     
 printf("\nBenvenuto %s \n",temp->username);

 return temp->username;
}

void salvaUtenti(struct giocatore* head){
    FILE *f;
    FILE *c;
    c=NULL;
    f=NULL;
    f=fopen("utenti.txt","w");
    c=fopen("classifica.txt","w");
    int i=1;

    //Se il file viene aperto correttamente
    if(f!=NULL)
    {fprintf(c,"\n\n CLASSIFICA \n\n");
         //Ciclo che scorre la lista degli utenti e scrive ciascun utente nel file fino a quando non si raggiunge la fine della lista
         while(head!=NULL)
         {
              fprintf(f,"%s %s %d %.1f %d \n",head->username,head->password,head->elo,head->punteggio,head->partite);
              fprintf(c,"%d - %s pt=%.1f partite=%d elo=%d\n",i,head->username,head->punteggio,head->partite,head->elo);
              head=head->next;
              i++;
         }
    }
    else
    {
         //Se il file non è aperto correttamente
         printf("Errore scrittura file.\n");
         return;
    }
    //Chiusura del file
    fclose(f);
    fclose(c);
}

void stampa_classifica(struct giocatore* head){
    //posso usare head per scorrere la lista perche' e' una copia
    //scorro fino alla fine e stampo
    int i=1;
    printf("\n\nCLASSIFICA \n\n");
    while ((head != NULL)){
        printf("\n%d - %s pt=%.1f partite=%d elo=%d", i,head->username,head->punteggio,head->partite,head->elo);
        head = head->next;
        i++;
    }
    printf("\n\n\n");
}

void svuota_lista_giocatori (struct giocatore** head){
struct giocatore* t = *head; //precedente 
struct giocatore* w = *head; //successivo 
while(t!=NULL){
w=t->next;
free(t);
t=w;}
t=NULL;
}


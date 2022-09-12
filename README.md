# SCACCHI 2 players

# scacchi 2.0

Comandi per compilare ed eseguire il gioco su terminale linux o wsl (su windows):     
./make.sh    
./scacchi


DOCUMENTO DI PROGETTO

L’obiettivo di questo programma è di avvicinare chiunque al gioco degli scacchi; pertanto, si tratterà di un software didattico che permetterà a qualsiasi utente di imparare quali sono le mosse e le regole basilari, come per esempio quella di portare al termine la partita una volta iniziata, a meno che non si chieda: patta o abbandono. 

Questo gioco permetterà all’utente di registrarsi e/o accedere con l’immissione di nickname e password, verrà chiesto ad uno dei due utenti con il quale vuole giocare, dopo sarà possibile scegliere un’operazione dal menu principale tra: 
-	New - per iniziare una nuova partita;
-	Classifica - per stampare a schermo il punteggio dei giocatori registrati, le partite giocate e l’elo (punteggio che rappresenta l’abilita e il livello del giocatore);
-	Help - per leggere le istruzioni del gioco;
-	Exit - per uscire dal gioco.

Selezionando New oltre a poter iniziare la partita si accede ad un menu secondario che permetterà:
-	Comandi – per visualizzare i comandi possibili;
-	Classifica - per stampare a schermo il punteggio dei giocatori;
-	Cronologia - annotazione delle mosse appena giocate;
-	Abbandono - per abbandonare la partita, in questo caso la partita è persa;
-	Patta - per finire la partita in pareggio.

Appena finisco una partita stampa le mosse giocate con l’annotazione specifica degli scacchi, e la classifica con i dati aggiornati.
Nello stampare la scacchiera si utilizzerà la codifica Unicode al fine di visualizzare i caratteri speciali che rappresentato i pezzi con i rispettivi colori.
*PS. è consigliabile per stampare correttamente i pezzi l'utilizzo di linux.

# INTERFACCIA UTENTE

![1](https://user-images.githubusercontent.com/83581773/189664917-466846cd-6e8a-41b0-b25e-43deb01bb95a.png)

![2](https://user-images.githubusercontent.com/83581773/189666136-9594ea91-28b3-4b84-91fb-9eaa97912618.png)

![3](https://user-images.githubusercontent.com/83581773/189666186-eaccb595-ff4a-4db0-9ed7-ef4ca3c97dbf.png)

![4](https://user-images.githubusercontent.com/83581773/189666267-29b914c8-503d-4e36-9e48-b238c70776e6.png)

![5](https://user-images.githubusercontent.com/83581773/189666305-49716d56-2631-461c-8a9f-805ee74dbf7c.png)

![6](https://user-images.githubusercontent.com/83581773/189666310-924d16d4-c54f-4c50-a11f-f0572d84bbba.png)

![7](https://user-images.githubusercontent.com/83581773/189666313-f5954416-dddc-4bbc-a482-6ad8474ae260.png)

![8](https://user-images.githubusercontent.com/83581773/189666316-68323222-a913-4ffe-b743-c021adf57408.png)


LA SCACCHIERA E' STATA COSTRUITA RISPETTANDO QUESTO SCHEMA

![rappresentazione_scacchiera](https://user-images.githubusercontent.com/83581773/189666734-a6d6eb95-f1be-4432-a0f2-775c073c5bfc.png)

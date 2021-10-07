#!/bin/bash


gcc -g -c scacchi.c
gcc -g -c giocatore.c

gcc scacchi.o giocatore.o -o scacchi

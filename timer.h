/**
 * @file timer.h
 * @brief Dans ce fichier on déclare les fonctions pour la gestion du timer
 */
#ifndef TIMER_H
#define TIMER_H
#include <stdio.h>
#include <time.h>
#include <donnees_borne.h>
#include <memoire_borne.h>
#include <unistd.h>  // j'ai ajouté cette ligne pour la suppression de l'avertissement lié à sleep
void timer_raz();
int timer_valeur();

int timer_count_sec();


#endif 

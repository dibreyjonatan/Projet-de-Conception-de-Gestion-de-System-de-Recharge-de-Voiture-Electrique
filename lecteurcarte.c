/**
*@file lecteurcarte.c
*@brief Dans ce fichier on fait la gestion du lecteur de carte
*/
#include <unistd.h>
#include "lecteurcarte.h"

/** 
*@brief Cette fonction permet d'initialiser le port sur le lecteur 
*@details
* Cette fonction est déjà écrite par le prof elle est juste implementer 
*@return void 
*/
void lecteurcarte_initialiser_lecteur()
{
         initialisations_ports();
}

/**

*@brief Cette fonction initialise le lecteur de carte
*@details 
* Cette fonction affiche un message demandant a l'utilisateur d'insérer une carte
*@return void 

*/
void lecteurcarte_initialiser()
{
 printf("veuillez inserer votre de carte: \n") ; 
}

/**

*@brief Cette fonction lit la valeur du numéro passé en paramètre 
*@details 
* Cette fonction affiche un message demandant a l'utilisateur d'insérer une carte
*@par variable internes utilisées
* int num : le numero de la carte insérée
*@return int le numero de la carte inserée
*@retval num le numero de la carte inserée

*/
int lecteurcarte_lire_carte()
{ int num ;
 
   attente_insertion_carte();
   num = lecture_numero_carte();
   printf("Numéro lu:%d\n", num);
   attente_retrait_carte();

   return num ; 
}


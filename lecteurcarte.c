/**
*@file lecteurcarte.c
*@brief Dans ce fichier on fait la gestion du lecteur de carte
*/
#include <unistd.h>
#include "lecteurcarte.h"

/**

*@brief Cette fonction initialise le lecteur de carte
*@details 
* Cette fonction affiche un message demandant a l'utilisateur d'insérer une carte
*@return void 

*/
void lecteurcarte_initialiser()
{
 printf("veuillez inserer un numero de carte: \n") ; 
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
 scanf("%d",&num) ;
  return num ; 
}


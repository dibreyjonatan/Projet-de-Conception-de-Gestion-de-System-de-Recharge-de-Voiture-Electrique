/**
*@file generateur_save.c
*@brief Dans ce fichier on fait la gestion du contacteur et du generateur save 
*/
#include "generateur_save.h"


entrees *io_g ;
int shmid_g ;


void generateur_save_fermer_contacteur() {
    io_g=acces_memoire(&shmid_g) ;
  io_g->contacteur_AC=1; 
}

void generateur_save_ouvrir_contacteur() {
 io_g=acces_memoire(&shmid_g) ;
  io_g->contacteur_AC=0; 
}

/**

*@brief Cette fonction on consulte la valeur de la tension DC et on la renvoie
*@details 
* On reserve la mémoire partagé shmid et ensuite on lit dans cette mémoire et on retourne la valeur lu.
* les valeurs de tension possible générer par le générateur save est 12,9,6,0.
*@return int 
*@retval 12
*@retval 9
*@retval 6
*@retval 0

*/
int generateur_save_tension_DC() {
    io_g=acces_memoire(&shmid_g) ;
  return (io_g->gene_u); 
   
}

/**

*@brief Cette fonction on consulte la valeur de la tension DC et on la renvoie
*@details 
* On reserve la mémoire partagé shmid et ensuite on écrit dans cette mémoire.
*@return void

*/
void generateur_save_generer_pwm(pwm signal) {
     io_g=acces_memoire(&shmid_g) ;
  io_g->gene_pwm=signal; 
 
}
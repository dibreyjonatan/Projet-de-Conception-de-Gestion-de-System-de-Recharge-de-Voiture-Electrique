/**
*@file generateur_save.c
*@brief Dans ce fichier on fait la gestion du contacteur et du generateur save 
*/
#include "generateur_save.h"


/**
 * @brief variable globale de generateur_save.c pour la gestion mémoire partagée
 * - *io_g: pointeur vers la structure des entrées de la mémoire partagée
 */
entrees *io_g ;

/**
 * @brief variable globale de generateur_save.c 
 * - *shmid_g : entier qui servira d'entrée pour la mémoire partagée
 */
int shmid_g ;


/**

*@brief Cette fonction ferme le contacteur
*@details 
* accède la mémoire partagée  partagé shmid et on ferme le contacteur en mettant la valeur à 1
*@return void

*/
void generateur_save_fermer_contacteur() {
    io_g=acces_memoire(&shmid_g) ;
  io_g->contacteur_AC=1; 
}

/**

*@brief Cette fonction  ouvre le contacteur
*@details 
* accède la mémoire partagée  partagé shmid et ensuite on écrit dans cette mémoire pour ouvrir le contacteur en mettant la valeur à 0
*@return void

*/
void generateur_save_ouvrir_contacteur() {
 io_g=acces_memoire(&shmid_g) ;
  io_g->contacteur_AC=0; 
}

/**

*@brief Cette fonction consulte la valeur de la tension DC et la renvoie
*@details 
* accède la mémoire partagée  partagé shmid et ensuite on lit dans cette mémoire et on retourne la valeur lu.
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

*@brief Cette fonction  génère le signal PWM
*@details 
* accède la mémoire partagée  partagé shmid et ensuite on écrit dans cette mémoire.
*@return void

*/
void generateur_save_generer_pwm(pwm signal) {
     io_g=acces_memoire(&shmid_g) ;
  io_g->gene_pwm=signal; 
 
}
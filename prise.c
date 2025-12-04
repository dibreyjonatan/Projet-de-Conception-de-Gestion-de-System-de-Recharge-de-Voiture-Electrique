/**
*@file prise.c
*@brief Dans ce fichier on fait la gestion de la prise
*/
#include "prise.h"

/**
 * @brief variable globale de prise.c pour la gestion mémoire partagée
 * - *io_p: pointeur vers la structure des entrées de la mémoire partagée
 */
entrees *io_p ;

/**
 * @brief variable globale de prise.c
 * - *shmid_p : entier qui servira d'entrée pour la mémoire partagée
 */
int shmid_p ;


/**
*@brief Cette fonction verouille la trappe
*@details 
* Dans cette fonction on accède la mémoire partagée et on vérouille la trappe en mettant la led à OFF
*@return void 
*/
void prise_verrouille_trappe() {

    io_p=acces_memoire(&shmid_p) ;
  io_p->led_trappe=OFF; 
}
/**

*@brief Cette fonction verouille la trappe
*@details 
* Dans cette fonction on accède la mémoire partagée et on dévérouille la trappe en mettant la led à VERT
*@return void 
*/
void prise_deverrouille_trappe() {

    io_p=acces_memoire(&shmid_p) ;
  io_p->led_trappe=VERT; 
}

/**

*@brief Cette fonction met la led à VERT ou ROUGE ou l'éteint
*@details 
* Dans cette fonction on accède la mémoire partagée et on met la led à l'état souhaité
*@param[in] prise l'état souhaité de la led
*@return void 
*/
void prise_set_prise(led prise) {

    io_p=acces_memoire(&shmid_p) ;
  io_p->led_prise=prise; 
}
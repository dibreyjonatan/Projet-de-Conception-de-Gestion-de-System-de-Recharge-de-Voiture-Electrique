/**
*@file voyant.c
*@brief Dans ce fichier on fait la gestion des voyants
*/

#include "voyant.h"
/**
 * @brief variable globale de voyant.c pour la gestion mémoire partagée
 * - *io : pointeur vers la structure des entrées de la mémoire partagée
 */
entrees *io ;
/**
 * @brief variable globale de voyant.c 
 * - *shmid : entier qui servira d'entrée pour la mémoire partagée 
 */
int shmid ;
/**
 *@brief Cette fonction met la led de disponibilité à VERT ou ROUGE ou l'éteint
 *@details 
 * Dans cette fonction on accède la mémoire partagée et on met la led à l'état souhaité
 *@param[in] dispo l'état souhaité de la led
 *@return void
 */
void voyant_set_dispo(led dispo){
  io=acces_memoire(&shmid) ;
  io->led_dispo=dispo; 
}
/**
 *@brief Cette fonction met la led de charge à VERT ou ROUGE ou l'éteint
 *@details
 * Dans cette fonction on accède la mémoire partagée et on met la led à l'état souhaité
 *@param[in] charge l'état souhaité de la led
 *@return void
 */
void voyant_set_charge(led charge){
io=acces_memoire(&shmid) ;
  io->led_charge=charge; 

}
/**
 *@brief Cette fonction fait clignoter la led de charge
*@details
 * Dans cette fonction on fait clignoter la led de charge en VERT 4 fois avec un intervalle d'une seconde
 * Ce qui donne 8 secondes de clignotement. 1 seconde allumée et 1 seconde éteinte.
 *@return void
 */
void voyant_blink_charge(){
  for(int i=0; i<4; i++){
   voyant_set_charge(VERT);
   sleep(1);
   voyant_set_charge(OFF);
   sleep(1);
}
}
/**
 *@brief Cette fonction met la led de défaut à VERT ou ROUGE ou l'éteint
 *@details 
 * Dans cette fonction on accède la mémoire partagée et on met la led à l'état souhaité
 *@param[in] defaut l'état souhaité de la led
 *@return void
 */
void voyant_set_defaut(led defaut){
 io=acces_memoire(&shmid) ;
  io->led_defaut=defaut; 
}
/**
  *@brief Cette fonction fait clignoter la led de défaut
  *@details
 * Dans cette fonction on fait clignoter la led de défaut en ROUGE 4 fois avec un intervalle d'une seconde
 *  Ce qui donne 8 secondes de clignotement. 1 seconde allumée et 1 seconde éteinte.
 * *@return void
 */
void voyant_blink_defaut(){
    for(int i=0; i<4; i++){
   voyant_set_defaut(ROUGE);
   sleep(1);
   voyant_set_defaut(OFF);
   sleep(1);
}
}
/**
 *@brief Cette fonction met la led de prise à VERT ou ROUGE ou l'éteint
 *@details
 * Dans cette fonction on accède la mémoire partagée et on met la led à l'état souhaité
 *@param[in] prise l'état souhaité de la led
 *@return void
 */
void voyant_set_prise(led prise) {
 io=acces_memoire(&shmid) ;
  io->led_prise=prise; 
}
/**
 *@brief Cette fonction met la led de trappe à VERT ou ROUGE ou l'éteint
 *@details
 * Dans cette fonction on accède la mémoire partagée et on met la led à l'état souhaité
 *@param[in] trappe l'état souhaité de la led
 *@return void
 */
void voyant_set_trappe(led trappe){
io=acces_memoire(&shmid) ;
  io->led_trappe=trappe; 

}
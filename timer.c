/**
*@file timer.c
*@brief Dans ce fichier on fait la gestion du timer
*/

#include "timer.h"
/**
 * @brief variable globale de timer.c 
 * - *io_t: pointeur vers la structure des entrées de la mémoire partagée
 */
entrees *io_t ;
/**
 * @brief variable globale de timer.c
 * - *shmid_t : entier qui servira d'entrée pour la mémoire partagée
 */
int shmid_t ;
/**
 * @brief variable globale de timer.c 
 * - depart_timer : variable de type time_t pour stocker le temps de départ du timer
 */
time_t depart_timer ;

/**
 *@brief Cette fonction remet le timer à zéro    
 *@details
 * Dans cette fonction on accède la mémoire partagée et on remet le timer à zéro
 * La variable depart_timer est initialisée avec le temps actuel
 * @return void 
 */
void timer_raz(){
    io_t=acces_memoire(&shmid_t) ;
     io_t->timer_sec=0;
    depart_timer=time(NULL) ;
     
}
 // calcul la difference de temps après 1s
 /**
  *@brief Cette fonction calcule la différence de temps depuis le dernier appel    
  * @details
  *  Dans cette fonction on dort pendant 1 seconde puis on calcule la différence entre le temps actuel et le temps de départ
  *  La variable depart_timer est mise à jour avec le temps actuel pour le prochain appel.
  *  @par Variable interne utilisée :
  * - now : variable de type time_t pour stocker le temps actuel
  * - diff : variable de type int pour stocker la différence de temps en secondes
  * @return int  
  * @retval int la différence de temps en secondes
  * 
  */
int timer_valeur(){
      sleep(1) ;
    time_t now=time(NULL) ;
     
    int diff = now - depart_timer ; 
     // printf("la difference est : %d\n",diff) ;
    depart_timer=now ;
  return diff ;
}

/**

*@brief Cette fonction met à jour le timer chaque seconde
*@details 
* Dans cette fonction on accède la mémoire partagée et on met à jour le timer
* La valeur du timer est incrémentée chaque seconde
* La fonction timer_valeur() est appelée pour obtenir la seconde écoulée depuis le dernier appel.
* @par Variable interne utilisée :
* - timer_value : variable de type int pour stocker la valeur du timer en secondes
*@return int
*@retval int le nombre de secondes écoulées
*/

int timer_count_sec(){
        
        int timer_value = timer_valeur () ;
     io_t=acces_memoire(&shmid_t) ;
//printf("timer system avant 1s: %d\n",io_t->timer_sec) ;
     io_t->timer_sec+= timer_value;
  //  printf("timer system apres 1s : %d\n",io_t->timer_sec) ;
           return (timer_value) ;
}


// brouillon 
/*
int timer_renvoie_sec(){
     timer_count_sec() ;
    
    io_t=acces_memoire(&shmid_t) ;
    printf("secondes retourner : %d\n",io_t->timer_sec) ;
     return (io_t->timer_sec); 

}*/
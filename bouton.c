/**
*@file bouton.c 
*@brief Ici on a les fonctions qui permettent de lire la valeur du bouton et de mettre les états à 0.
*/
#include "bouton.h"

entrees *io_b ;
int shmid_b ;
/**

*@brief Cette fonction retourne la valeur du bouton charge 
*@details 
* On reserve la mémoire partagé shmid et ensuite on lit dans cette mémoire la valeur de la charge 
*@return int 
*@retval 1 si le client a appuyer
*@retval 0 si le client n'a pas appuyer 

*/
int bouton_appuie_button_charge(){
    io_b= acces_memoire(&shmid_b) ;
    return ( io_b -> bouton_charge ) ;
}
/**

*@brief Cette fonction retourne la valeur du bouton stop
*@details 
* On reserve la mémoire partagé shmid et ensuite on lit dans cette mémoire la valeur du bouton stop
*@return int 
*@retval 1 si le client a appuyer
*@retval 0 si le client n'a pas appuyer 

*/
int bouton_apppuie_button_stop() {

    io_b= acces_memoire(&shmid_b) ;
    return ( io_b -> bouton_stop ) ;
}

/**

*@brief Cette fonction met l'état du bouton charge à 0
*@details 
* On reserve la mémoire partagé shmid et on écrit 0 sur cette mémoire
*@return void 


*/
void bouton_set_bouton_charge(){
  io_b= acces_memoire(&shmid_b) ;
    io_b -> bouton_charge=0 ;
}

/**

*@brief Cette fonction met l'état du bouton stop à 0
*@details 
* On reserve la mémoire partagé shmid et on écrit 0 sur cette mémoire
*@return void 


*/
void bouton_set_bouton_stop(){
io_b= acces_memoire(&shmid_b) ;
    io_b -> bouton_stop=0 ;
    }



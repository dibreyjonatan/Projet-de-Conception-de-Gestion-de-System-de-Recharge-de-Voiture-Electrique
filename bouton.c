#include "bouton.h"

entrees *io_b ;
int shmid_b ;

int button_appuie_button_charge(){
    io_b= acces_memoire(&shmid_b) ;
    return ( io_b -> bouton_charge ) ;
}

int button_apppuie_button_stop() {

    io_b= acces_memoire(&shmid_b) ;
    return ( io_b -> bouton_stop ) ;
}


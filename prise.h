#ifndef PRISE_H
#define PRISE_H

#include <donnees_borne.h>
#include <memoire_borne.h>
#include <unistd.h>

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std ;
#endif

class Prise{
    Private :
       entrees* io_p ;
    Public :
        Prise();
        void prise_verrouille_trappe() ;
        void prise_deverrouille_trappe() ;
        void prise_set_prise(led prise) ;

};

#endif 
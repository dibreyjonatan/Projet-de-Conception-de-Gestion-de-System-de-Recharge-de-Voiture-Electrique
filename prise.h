#ifndef PRISE_H
#define PRISE_H

#include <stdio.h>
#include <donnees_borne.h>
#include <memoire_borne.h>
#include <unistd.h>

void prise_verrouille_trappe() ;
void prise_deverrouille_trappe() ;

void prise_set_prise(led prise) ;

#endif 

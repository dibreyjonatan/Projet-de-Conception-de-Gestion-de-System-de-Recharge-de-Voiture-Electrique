#ifndef VOYANT_H
#define VOYANT_H

#include <stdio.h>
#include <donnees_borne.h>
#include <memoire_borne.h>
#include <unistd.h>

void voyant_set_dispo(led dispo);
void voyant_set_charge(led charge);
void voyant_blink_charge() ;
void voyant_set_defaut(led defaut);
void voyant_blink_defaut();
void voyant_set_prise(led prise) ;
void voyant_set_trappe(led trappe);



#endif 

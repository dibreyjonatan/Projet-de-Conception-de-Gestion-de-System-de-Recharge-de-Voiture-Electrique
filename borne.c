#include <memoire_borne.h>
#include <donnees_borne.h>

#include "lecteurcarte.h"
#include "baseclient.h"
#include "voyant.h"
#include "timer.h"
#include "bouton.h"
#include "prise.h"
#include "generateur_save.h"


int main()
{

    

    while (1)
    { 
      
        lecteurcarte_initialiser();
        int numero=lecteurcarte_lire_carte();
        printf("numero lu est : %d\n",numero); 
        int found=baseclient_authentifier(numero);
        printf("resultat authentification : %d\n",found) ;
        // echec d'authentification
        if(found==0) {
        // clignoter voyant defaut 8s en cas d'erreur
          voyant_blink_defaut(); 
         break ;
          }
        // clignoter voyant charge 8s en cas de success
        voyant_blink_charge();
       
         //consigne client 
         printf("vous disposez de 1 minute pour appuyer sur charge\n") ;
        // temporisation de 1 minutes 
          int timer_secs=0 ;
          // initialisation de temporisation 
          timer_raz();
          // detection apuie button en 60s
            int butt_apuie=1 ;
          while(timer_secs<60 && butt_apuie==1){
           printf("secondes recu du timer :%d",timer_secs) ; 
           timer_secs+=timer_count_sec() ;
             int sortie=button_appuie_button_charge();
              if(sortie==1 ) {
                butt_apuie=0 ;
		printf("le bouton a ete appuyer\n");
                 break;
            }
           }

          if(butt_apuie==1) printf("le button n'a pas été appuyer durant les 1mins") ;
         /* else{
                 voyant_set_dispo(OFF);
            } */       
        //si le button a été appuyer durant les 1 mins, on demarre le cycle de charge
        voyant_set_charge(ROUGE) ;
	generateur_save_generer_pwm(DC) ;
 	prise_deverrouille_trappe();
	
          
           int d= 0 ;
             // Atente de branchement de la prise par l'utilisateur 
           while(1){
            d=generateur_save_tension_DC();
            sleep(10);
           printf("tension: %d\n",d);
            if(d==9) break ; 
            }
            prise_set_prise(VERT) ;
    }

}

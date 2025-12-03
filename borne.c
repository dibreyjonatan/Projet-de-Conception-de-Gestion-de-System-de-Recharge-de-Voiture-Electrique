#include <memoire_borne.h>
#include <donnees_borne.h>

#include "lecteurcarte.h"
#include "baseclient.h"
#include "voyant.h"
#include "timer.h"
#include "bouton.h"
#include "prise.h"
#include "generateur_save.h"

typedef enum { etat0, etat1, etat2, etat3, etat4, etat5, etat6,etat255 } etatsystem ;

void administration_operateur(); 

int main()
{

     etatsystem etat_present, etat_suivant ;
      etat_present=etat_suivant=etat0 ;
    
    while (1)
    { 
      switch(etat_present){
      case etat0 :
        lecteurcarte_initialiser();
        int numero=lecteurcarte_lire_carte();
        printf("numero lu est : %d\n",numero); 

        if(numero==255){
         
         etat_suivant=etat255 ;
         break ; 
        }
        
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
             int sortie=bouton_appuie_button_charge();
              if(sortie==1 ) {
                butt_apuie=0 ;
		printf("le bouton a ete appuyer\n");
		        etat_suivant=etat1 ;
                 break;
            }
           }

          if(butt_apuie==1) {
          printf("le button n'a pas été appuyer durant les 1mins\n") ;
          etat_suivant=etat0 ;
           break ;
          }
         /* else{
                 voyant_set_dispo(OFF);
            } */       
        //si le button a été appuyer durant les 1 mins, on demarre le cycle de charge
      case etat1 :  
           bouton_set_bouton_charge(); //mise à zéro logiciellement
           voyant_set_charge(ROUGE) ;
           voyant_set_dispo(OFF); 
	       generateur_save_generer_pwm(DC) ;
 	       prise_deverrouille_trappe();
	       
          if(generateur_save_tension_DC()==9)
             etat_suivant=etat2 ;
          else etat_suivant=etat1 ; 
          sleep(2) ;
          
           break ; 
      case etat2 :
             prise_set_prise(VERT) ;
             generateur_save_generer_pwm(AC_1K) ;
             generateur_save_ouvrir_contacteur();
             if(generateur_save_tension_DC()==6)
               etat_suivant=etat3 ;
             else etat_suivant=etat2 ; 
               sleep(2) ;
              break ;
              //charge du vehicule
       case etat3 :
             generateur_save_generer_pwm(AC_CL) ;
             generateur_save_fermer_contacteur();
             
              if(generateur_save_tension_DC()==9)
               etat_suivant=etat4;
             else etat_suivant=etat3; 
               sleep(2) ;
             break ;
             //fin de la recharge et reprise du véhicule selon le usecase 1
       case etat4 : 
             printf(" fin de la recharge \n") ; 
             voyant_set_charge(VERT) ;
             generateur_save_ouvrir_contacteur();
	         generateur_save_generer_pwm(DC) ;
 	         
              if(generateur_save_tension_DC()==12)
               etat_suivant=etat5;
             else etat_suivant=etat4; 
               sleep(2) ;
             break ; 
       case etat5 :
            voyant_set_charge(OFF) ;
            generateur_save_generer_pwm(DC) ;
            prise_set_prise(OFF) ;
            prise_verrouille_trappe();
            if(generateur_save_tension_DC()==12)
               etat_suivant=etat6;
            else etat_suivant=etat5;    
            sleep(2) ;
            break ;
       case etat6 :
            voyant_set_dispo(VERT); 
            etat_suivant=etat0 ; 
            break ; 
                                         
      // cas operateur :
      case etat255 :
            administration_operateur();
            etat_suivant=etat0 ;
            break ;

            }
            
            etat_present=etat_suivant ;
    }

}

void administration_operateur(){

  int c ;
  printf("Bienvenue Operateur : \n");
  printf(" Tapez 1, si vous voulez ajouter un nouveau client \n");
  printf("Tapez 2, si vous voulez supprimer un client \n");
  printf("Entrez votre choix : ");
  scanf("%d",&c);

  if(c==1){
    printf("\n Veuillez entrer le numero du client à ajouter\n");
    scanf("%d",&c);
    baseclient_ajoutclient(c);
  }
  if(c==2){
    printf("\n Veuillez entrer le numero du client à supprimer\n");
    scanf("%d",&c);
    baseclient_supprimeclient(c);
  }
}

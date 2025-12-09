/**
*@file borne.c
*@brief borne.c est le fichier principale du système 
*@author KAMDA TEZEBO DIBREY JONATAN
*@author Nassime BOUANANI
*@version 1.0
*@date 03/12/2025
*/

#include <memoire_borne.h>
#include <donnees_borne.h>

#include "lecteurcarte.h"
#include "baseclient.h"
#include "voyant.h"
#include "timer.h"
#include "bouton.h"
#include "prise.h"
#include "generateur_save.h"

/**
 * @brief Définition des états du système de recharge
 *  On déclare une énumération pour représenter les différents états du système de recharge de véhicule électrique.
 */
typedef enum { etat0, etat1, etat2, etat3, etat4, etat5, etat6,etat255 } etatsystem ;
/**
*@brief Fonction charger de gérer l'administration des clients une fois que l'operateur met son id 
*@note Cette fonction est utilisé pour le cas etat255 
*@details 
* Cette fonction demande à l'opérateur de saisir le choix de l'opération qu'il aimerait faire soit ajout ou suppression de client
* par la suite, l'opérateur fait entrer le numéro du client correspondant 
* ensuite elle fait appels aux fonctions de baseclient_ajoutclient(int) ou baseclient_supprimeclient(int) selon le besoin.
*@par Variables internes utilisées :
*@li c : variable de type int pour stocker le choix de l'opération et le numéro du client
*@return void  
*/
void administration_operateur(); 
/**
*@brief la fonction principale de tout le système, c'est ici qu'est implementée la machine à état decrivant le système
*@note dans le main, une boucle infinie est faite !
*@details 
* Dans cette fonction on implemente la machine à état en utilisant switch case et etatsystem.
* Les cas sont extraites de la machine à état.
* dans la fonction on definit deux états, état_present et état_suivant 
* Pour évoluer d'un état à un autre, état_present prend la valeur de état_suivant
* état0 on demande à l'utilisateur d'entrer son numéro et s'assure qu'il clique sur button charge en moins de 1 min
* état1 on attent que l'utilisateur branche la prise 
* état2 c'est la prise branché 
* état3 c'est la charge du véhicule 
* état4 c'est la fin de charge 
* état5 c'est la récupération du véhicule 
* état6 c'est la mise en place du système comme s'il était à l'état initiale 
* état255 c'est l'état de gestion de l'administration par l'opérateur
*@warning l'état initiale dont fait référence l'état6 est différent de état0 car c'est 
* l'état de démarrage du système avant l'attente des identifiants du client.
*@par Variables internes utilisées :
*@li etat_present : variable de type etatsystem qui stocke l'état actuel du système
*@li etat_suivant : variable de type etatsystem qui stocke l'état suivant du système
*@li id : variable de type int utilisée pour la gestion de l'authentification lors de la reprise du véhicule
*@li data : variable entière int pour stocker le numéro entré par l'utilisateur lors de l'authentification
*@li numero : variable entière int pour stocker le numéro lu par le lecteur de carte
*@li found : variable entière int pour stocker le résultat de l'authentification du client
*@li timer_secs : variable entière int pour stocker le temps écoulé en secondes
*@li butt_apuie : variable entière int pour vérifier si le bouton charge a été appuyé

*@return int 
*@retval 0 en cas de succès d'execution 

*/
int main()
{

     etatsystem etat_present, etat_suivant ;
      etat_present=etat_suivant=etat0 ;
      int id=0,data ; //pour la reprise vehicule //id pour le nombre de fois qu'il va interroger le client  // data pour la lecture 
     bouton_set_bouton_stop();
     lecteurcarte_initialiser_lecteur(); // initialisation du port de lecture 
    while (1)
    { 
      switch(etat_present){
      case etat0 :
        lecteurcarte_initialiser();
        int numero=lecteurcarte_lire_carte();
        //printf("numero lu est : %d\n",numero); 
        if(numero==3456){  // pour la gestion avec les cartes, le N° 3456 est l'administrateur 
         
         etat_suivant=etat255 ;
         break ; 
        }
        
        int found=baseclient_authentifier(numero);
        printf("resultat authentification : %d\n",found) ;
        // echec d'authentification
        if(found==0) {
        // clignoter voyant defaut 8s en cas d'erreur
          printf(" Client inconnue \n"); 
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
           printf("secondes recu du timer :%d\n",timer_secs) ; 
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
              
        //si le button a été appuyer durant les 1 mins, on demarre le cycle de charge
      case etat1 :  
           bouton_set_bouton_charge(); //mise à zéro logiciellement
           voyant_set_charge(ROUGE) ;
           voyant_set_dispo(OFF); 
	       generateur_save_generer_pwm(DC) ;
 	       prise_deverrouille_trappe();
	       
          if(generateur_save_tension_DC()==9)
           {
              prise_set_prise(VERT) ;
             etat_suivant=etat2 ;
           }
          else etat_suivant=etat1 ; 
          sleep(2) ;
          
           break ; 
      case etat2 : //prise branché 
             if(bouton_apppuie_button_stop()){
               printf("stop appuyer\n");
                bouton_set_bouton_stop();
               etat_suivant=etat5 ;
               generateur_save_generer_pwm(STOP) ;
               id=0 ;
               break ; 
             }
             
             //prise_set_prise(VERT) ;
             prise_verrouille_trappe();
             generateur_save_generer_pwm(AC_1K) ;
             generateur_save_ouvrir_contacteur();
             
             if(generateur_save_tension_DC()==6)
               etat_suivant=etat3 ;
             else etat_suivant=etat2 ; 
               sleep(2) ;
              break ;
              
       case etat3 : //charge du vehicule
               if(bouton_apppuie_button_stop()){
               printf("stop appuyer\n");
                bouton_set_bouton_stop();
               etat_suivant=etat5 ;
                generateur_save_generer_pwm(STOP) ;
               id=0 ;
               break ;
             }
            
             generateur_save_generer_pwm(AC_CL) ;
             generateur_save_fermer_contacteur();
             
              if(generateur_save_tension_DC()==9)
               {etat_suivant=etat4; id=0; }
             else etat_suivant=etat3; 
               sleep(2) ;
             break ;
             //fin de la recharge et reprise du véhicule selon le usecase 1
       case etat4 : 
            
            if(id==0){

            printf("fin de la recharge\n") ; 
            printf("veuillez vous authentifier à nouveau pour récupérer votre véhicule \n");
            data=lecteurcarte_lire_carte();
            if(numero!= data) {
            printf(" Veuillez inserer la bonne carte s'il vous plait\n");  
            etat_suivant=etat4;   
              break ; 
            }
            else id=1 ;
            }
              if(bouton_apppuie_button_stop()){
               printf("stop appuyer\n");
                bouton_set_bouton_stop();
               etat_suivant=etat5 ;
                id=0 ;
               break ;
             }
             prise_deverrouille_trappe();
             voyant_set_charge(VERT) ;
             generateur_save_ouvrir_contacteur();
	         generateur_save_generer_pwm(DC) ;
 	         
              if(generateur_save_tension_DC()==12)
               { etat_suivant=etat5; }
             else etat_suivant=etat4; 
               sleep(2) ;
             break ; 
       case etat5 : //reprise vehicule 
            if(id==0){

            printf("veuillez vous authentifier à nouveau pour récupérer votre véhicule \n");
            data=lecteurcarte_lire_carte();
            if(numero!= data) {
            printf(" Veuillez inserer la bonne carte s'il vous plait\n"); 
            etat_suivant=etat5;   
              break ;
            }
            else id=1 ;
            }
            prise_deverrouille_trappe();
            voyant_set_charge(OFF) ;
            generateur_save_generer_pwm(DC) ;
            prise_set_prise(OFF) ;
             //prise_verrouille_trappe();
            if(generateur_save_tension_DC()==12)
               etat_suivant=etat6;
            else etat_suivant=etat5;    
            sleep(2) ;
            break ;
       case etat6 :
           
            prise_verrouille_trappe();
            voyant_set_dispo(VERT); 
            generateur_save_ouvrir_contacteur(); //on ferme le contacteur AC, cette ligne a été ajouter pour une gestion efficace de UC3
            generateur_save_generer_pwm(STOP) ; //on éteint l'alimentation et on attend le prochain client
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
    return 0 ;
}

void administration_operateur(){

  int c ; int num ; 
  printf("Bienvenue Operateur : \n");
  printf(" Tapez 1, si vous voulez ajouter un nouveau client \n");
  printf("Tapez 2, si vous voulez supprimer un client \n");
  printf("Entrez votre choix : ");
  scanf("%d",&c);
  
  switch (c){

    case 1 : 
     printf("\n Veuillez inserer la carte à enregistrer \n");
     num=lecteurcarte_lire_carte();
    
     baseclient_ajoutclient(num);
    break ;
    case 2 : 
    printf("\n Veuillez saisir le numero du client à supprimer\n");
    scanf("%d",&num);
    baseclient_supprimeclient(num);
     break ;
    default : break ; 
  }
 
}

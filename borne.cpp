/**
 * 
 * @file    borne.cpp
 * @brief   Fichier principal du système de borne de recharge
 *  Ce fichier contient les objets de chaque classe.
 *
 * @authors
 *   - KAMDA TEZEBO DIBREY JONATAN
 *   - Nassime BOUANANI
 *
 * @version 1.0
 * @date    30/03/2026
 * 
 */
#include "lecteurcarte.h"
#include "voyant.h"
#include "baseclient.h"
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
 * @brief Fonction chargée de gérer l'administration des clients 
 *        une fois que l'opérateur met son id.
 * @note Cette fonction est utilisée pour le cas etat255.
 * @details
 * Cette fonction affiche un menu à l'opérateur avec les opérations disponibles :
 * - Ajouter un client (choix 1) : demande le numéro de carte et le nom du client,
 *   puis appelle baseclient_ajoutclient(int, string).
 * - Supprimer un client (choix 2) : demande le numéro de carte du client,
 *   puis appelle baseclient_supprimeclient(int).
 * - Afficher tous les clients (choix 3) : appelle baseclient_afficher_clients().
 * - Modifier un client (choix 4) : demande le numéro de carte du client à modifier,
 *   puis appelle baseclient_modifierclient(int). L'opérateur peut modifier
 *   le nom, le numéro de carte, ou les deux.
 * @par Variables internes utilisées :
 * @li c   : variable de type int pour stocker le choix de l'opération.
 * @li num : variable de type int pour stocker le numéro de carte du client.
 * @li nom : variable de type std::string pour stocker le nom du client (ajout uniquement).
 * @return void
 */
void administration_operateur(); 

// Creation des Objets globales qui seront vu par la fonction administration_operateur
Baseclient baseclient;
LecteurCarte lecteurcarte;
/**
 * @brief Fonction principale du système — implémentation de la machine à états
 *        décrivant le comportement complet de la borne de recharge.
 *
 * @note Une boucle infinie est volontairement maintenue : la borne doit rester
 *       opérationnelle en permanence, sans interruption entre deux sessions de charge.
 *
 * @details
 * Le main orchestre l'ensemble du cycle de vie de la borne en s'appuyant sur
 * une machine à états finie (FSM), implémentée via un switch-case sur le type
 * énuméré @c etatsystem. À chaque tour de boucle, @c etat_present prend la
 * valeur de @c etat_suivant, ce qui provoque la transition vers l'état suivant.
 *
 * Les objets instanciés en début de fonction représentent chacun un composant
 * physique de la borne :
 * @li **LecteurCarte lecteurcarte** : lecture des cartes RFID — point d'entrée
 *     obligatoire du cycle, aucune action n'est possible sans carte valide.
 * @li **Voyant voyant** : pilotage des voyants lumineux informant l'utilisateur
 *     de l'état courant (disponible, en charge, défaut).
 * @li **Prise prise** : contrôle de la trappe et de la prise de charge,
 *     verrouillée durant la charge pour des raisons de sécurité.
 * @li **Timer timer** : mesure du temps écoulé en secondes, utilisé pour
 *     la temporisation de 60 secondes en attente d'appui bouton.
 * @li **Generateur_Save generateur_save** : pilotage du signal PWM et du
 *     contacteur AC selon la phase de charge (DC, AC_1K, AC_CL, STOP).
 * @li **Bouton bouton** : lecture des boutons physiques CHARGE et STOP,
 *     ce dernier pouvant interrompre la charge depuis les états 2, 3 et 4.
 *
 * Les états de la machine sont les suivants :
 * @li **etat0**   : lecture de la carte RFID et authentification du client.
 *                   Si le client est reconnu, il dispose de 60 secondes pour
 *                   appuyer sur le bouton CHARGE afin de démarrer le cycle.
 * @li **etat1**   : initialisation du cycle de charge — déverrouillage de la
 *                   trappe et attente du branchement de la prise par le client.
 * @li **etat2**   : prise branchée — verrouillage de la trappe, passage en
 *                   mode AC et ouverture du contacteur.
 * @li **etat3**   : charge active du véhicule — fermeture du contacteur et
 *                   surveillance de la tension DC jusqu'en fin de charge.
 * @li **etat4**   : fin de charge — re-authentification du client requise
 *                   avant de pouvoir récupérer son véhicule.
 * @li **etat5**   : récupération du véhicule — déverrouillage de la trappe,
 *                   extinction de la prise et attente du débranchement.
 * @li **etat6**   : remise en disponibilité de la borne pour le client suivant.
 * @li **etat255** : mode administration — réservé à l'opérateur pour la
 *                   gestion de la base clients (ajout, suppression, modification).
 *
 * @warning L'état initial auquel fait référence @c etat6 est différent de
 *          @c etat0 dans le sens où il s'agit d'un état de réinitialisation
 *          matérielle de la borne (contacteur ouvert, PWM stoppé, voyant
 *          disponible allumé), tandis que @c etat0 représente l'attente active
 *          d'un client avec lecture de carte.
 *
 * @par Variables internes utilisées :
 * @li **etat_present** : de type @c etatsystem, stocke l'état courant du système.
 * @li **etat_suivant** : de type @c etatsystem, stocke l'état vers lequel transiter.
 * @li **numero**       : entier int stockant le numéro lu par le lecteur de carte RFID.
 * @li **found**        : entier int stockant le résultat de l'authentification
 *                        (1 = client reconnu, 0 = échec).
 * @li **timer_secs**   : entier int comptabilisant les secondes écoulées
 *                        durant la temporisation de 60 secondes.
 * @li **butt_apuie**   : entier int indiquant si le bouton CHARGE a été appuyé
 *                        (0 = appuyé, 1 = non appuyé).
 * @li **id**           : entier int servant d'indicateur pour la re-authentification
 *                        lors de la reprise du véhicule (0 = non authentifié, 1 = ok).
 * @li **data**         : entier int stockant le numéro saisi par le client
 *                        lors de la re-authentification en etat4 et etat5.
 *
 * @return int
 * @retval 0 en cas de succès d'exécution (jamais atteint en pratique,
 *           la boucle infinie étant intentionnelle).
 */
int main (){
       // creation des objets en local
       Voyant voyant ;
       Prise prise ;
       Timer timer;
       Generateur_Save generateur_save ;
       Bouton bouton ;
       //initialiser 
       etatsystem etat_present, etat_suivant ;
       etat_present=etat_suivant=etat0 ;
       int id=0,data ; //pour la reprise vehicule //id pour le nombre de fois qu'il va interroger le client  // data pour la lecture 
       int numero, found,  timer_secs=0 , butt_apuie=1 ;  
       bouton.bouton_set_bouton_stop();
       
        // initialisation du port de lecteur de carte
        lecteurcarte.lecteurcarte_initialiser_lecteur();
       while (1)
    {
       switch(etat_present){
      case etat0 :
        lecteurcarte.lecteurcarte_initialiser();
        numero=lecteurcarte.lecteurcarte_lire_carte();
        cout<<"numero lu est :"<<numero<<"\n"; 

        if(numero==3456){
         
         etat_suivant=etat255 ;
         break ; 
        }
        
        found=baseclient.baseclient_authentifier(numero);
        cout<<"resultat authentification : "<<found<<"\n" ;
        // echec d'authentification
        if(found==0) {
        // clignoter voyant defaut 8s en cas d'erreur
          voyant.voyant_blink_defaut(); 
         break ;
          }
        // clignoter voyant charge 8s en cas de success
        voyant.voyant_blink_charge();
       
         //consigne client 
         cout<<"vous disposez de 1 minute pour appuyer sur charge\n";
        // temporisation de 1 minutes 
          timer_secs=0 ;
          // initialisation de temporisation 
          timer.timer_raz();
          // detection apuie button en 60s
            butt_apuie=1 ;
          while(timer_secs<60 && butt_apuie==1){
           cout<<"secondes recu du timer : "<<timer_secs<<"\n" ; 
           timer_secs+=timer.timer_count_sec() ;
             int sortie=bouton.bouton_appuie_button_charge();
              if(sortie==1 ) {
                butt_apuie=0 ;
		            cout<<"le bouton a ete appuyer\n";
		        etat_suivant=etat1 ;
                 break;
            }
           }

          if(butt_apuie==1) {
          cout<<"le button n'a pas été appuyer durant les 1mins\n" ;
          etat_suivant=etat0 ;
           break ;
          }
              
        //si le button a été appuyer durant les 1 mins, on demarre le cycle de charge
      case etat1 :  
           bouton.bouton_set_bouton_charge(); //mise à zéro logiciellement
           voyant.voyant_set_charge(ROUGE) ;
           voyant.voyant_set_dispo(OFF); 
	         generateur_save.generateur_save_generer_pwm(DC) ;
 	         prise.prise_deverrouille_trappe();
	       
          if(generateur_save.generateur_save_tension_DC()==9)
           {
              prise.prise_set_prise(VERT) ;
             etat_suivant=etat2 ;
           }
          else etat_suivant=etat1 ; 
          sleep(2) ;
          
           break ; 
      case etat2 : //prise branché 
             if(bouton.bouton_apppuie_button_stop()){
                cout<<"stop appuyer\n";
                bouton.bouton_set_bouton_stop();
               etat_suivant=etat5 ;
               generateur_save.generateur_save_generer_pwm(STOP) ;
               id=0 ;
               break ; 
             }
             
             //prise_set_prise(VERT) ;
             prise.prise_verrouille_trappe();
             generateur_save.generateur_save_generer_pwm(AC_1K) ;
             generateur_save.generateur_save_ouvrir_contacteur();
             
             if(generateur_save.generateur_save_tension_DC()==6)
               etat_suivant=etat3 ;
             else etat_suivant=etat2 ; 
               sleep(2) ;
              break ;
              
       case etat3 : //charge du vehicule
               if(bouton.bouton_apppuie_button_stop()){
                 cout<<"stop appuyer\n";
                 bouton.bouton_set_bouton_stop();
               etat_suivant=etat5 ;
                generateur_save.generateur_save_generer_pwm(STOP) ;
               id=0 ;
               break ;
             }
            
             generateur_save.generateur_save_generer_pwm(AC_CL) ;
             generateur_save.generateur_save_fermer_contacteur();
             
              if(generateur_save.generateur_save_tension_DC()==9)
               {etat_suivant=etat4; id=0; }
             else etat_suivant=etat3; 
               sleep(2) ;
             break ;
             //fin de la recharge et reprise du véhicule selon le usecase 1
       case etat4 : 
             cout<<" fin de la recharge \n" ; 
                if(id==0){

             cout<<"veuillez vous authentifier à nouveau pour récupérer votre véhicule \n";
             data=lecteurcarte.lecteurcarte_lire_carte();
            if(numero!= data) {
            etat_suivant=etat4;   
              break ; 
            }
            else id=1 ;
            }
              if(bouton.bouton_apppuie_button_stop()){
               cout<<"stop appuyer\n";
               bouton.bouton_set_bouton_stop();
               etat_suivant=etat5 ;
                id=0 ;
               break ;
             }
             prise.prise_deverrouille_trappe();
             voyant.voyant_set_charge(VERT) ;
             generateur_save.generateur_save_ouvrir_contacteur();
	           generateur_save.generateur_save_generer_pwm(DC) ;
 	         
              if(generateur_save.generateur_save_tension_DC()==12)
               { etat_suivant=etat5; }
             else etat_suivant=etat4; 
               sleep(2) ;
             break ; 
       case etat5 : //reprise vehicule 
            if(id==0){

            cout<<"veuillez vous authentifier à nouveau pour récupérer votre véhicule \n";
            data=lecteurcarte.lecteurcarte_lire_carte();
            if(numero!= data) {
            etat_suivant=etat5;   
              break ;
            }
            else id=1 ;
            }
            prise.prise_deverrouille_trappe();
            voyant.voyant_set_charge(OFF) ;
            generateur_save.generateur_save_generer_pwm(DC) ;
            prise.prise_set_prise(OFF) ;
             //prise_verrouille_trappe();
            if(generateur_save.generateur_save_tension_DC()==12)
               etat_suivant=etat6;
            else etat_suivant=etat5;    
            sleep(2) ;
            break ;
       case etat6 :
           
            prise.prise_verrouille_trappe();
            voyant.voyant_set_dispo(VERT); 
            generateur_save.generateur_save_ouvrir_contacteur(); //on ferme le contacteur AC, cette ligne a été ajouter pour une gestion efficace de UC3
            generateur_save.generateur_save_generer_pwm(STOP) ; //on éteint l'alimentation et on attend le prochain client
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
void administration_operateur() {
    int c;
    string nom;
    cout << "Bienvenue Operateur\n";
    cout << "1 - Ajouter un client\n";
    cout << "2 - Supprimer un client\n";
    cout << "3 - Afficher tous les clients\n";
    cout << "4 - Modifier un client\n";        
    cin >> c;

    if (c == 1) {
        cout << "Numéro de carte à insérer : \n";
        int num=lecteurcarte.lecteurcarte_lire_carte();
        cout << "Nom du client : ";
        cin.ignore();
        getline(std::cin, nom);
        baseclient.baseclient_ajoutclient(num, nom);
    }
    if (c == 2) {
        cout << "Veuillez saisir le numéro de la carte à supprimer : ";
        int num; cin >> num;
        baseclient.baseclient_supprimeclient(num);
    }
    if (c == 3) {
        baseclient.baseclient_afficher_clients();
    }
    if (c == 4) {                                   
        cout << "Veuillez saisir le numéro de la carte du client à modifier : ";
        int num; cin >> num;
        baseclient.baseclient_modifierclient(num);
    }
}

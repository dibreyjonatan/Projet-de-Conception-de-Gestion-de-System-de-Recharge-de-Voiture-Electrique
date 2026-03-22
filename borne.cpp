#include "lecteurcarte.h"
#include "voyant.h"
#include "baseclient.h"
#include "timer.h"
#include "bouton.h"
#include "prise.h"
#include "generateur_save.h"

typedef enum { etat0, etat1, etat2, etat3, etat4, etat5, etat6,etat255 } etatsystem ;

void administration_operateur(); 
int main (){
       // creation des objets 
       LecteurCarte lecteurcarte;
       Voyant voyant ;
       Baseclient baseclient;
       Prise prise ;
       Timer timer;
       Generateur_Save generateur_save ;
       Bouton bouton ;
      while(1){
       voyant.voyant_blink_defaut();
      }
    return 0 ;
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


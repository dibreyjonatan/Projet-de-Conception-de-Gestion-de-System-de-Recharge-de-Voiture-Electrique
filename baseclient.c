#include <baseclient.h>

int baseclient_authentifier(int num){

  FILE* fich = fopen("database.txt","r") ;
 if(fich==NULL) printf("erreur d'ouverture du fichier\n");
  else{
   int numero_r;
   while(fscanf(fich,"%d\n",&numero_r)==1)
{
   if(numero_r==num) {
    printf("found here\n") ;
    fclose(fich) ;
    return 1 ;
}

}
  
}
  fclose(fich);
  return 0 ;

}

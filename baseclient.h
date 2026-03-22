#ifndef BASECLIENT_H
#define BASECLIENT_H

#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <algorithm>   
#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std ;
#endif

struct Client {
    int numero;
    string nom;
};

class Baseclient {
private:
    vector<Client> clients;
    const string fichier = "database.txt";

    void charger();
    void sauvegarder();

public:
    Baseclient();
    int  baseclient_authentifier(int num);
    void baseclient_ajoutclient(int num, string nom);
    void baseclient_supprimeclient(int num);
    void baseclient_afficher_clients();   
    void baseclient_modifierclient(int num);
};

#endif 




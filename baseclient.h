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

class Baseclient{
    private :
    std::vector<int> clients;        // cache mémoire
    const std::string fichier = "database.txt";

    void charger();                  // lit le fichier → vector
    void sauvegarder();              // écrit le vector → fichier
    public :
        Baseclient();       
        int baseclient_authentifier(int);
        void baseclient_ajoutclient(int);
        void baseclient_supprimeclient(int);
};
#endif 
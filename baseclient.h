#ifndef BASECLIENT_H
#define BASECLIENT_H

#include <fstream>
#include <string>
#include <cstdio>

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std ;
#endif

class Baseclient{
    public :
        int baseclient_authentifier(int);
        void baseclient_ajoutclient(int);
        void baseclient_supprimeclient(int);
};
#endif 
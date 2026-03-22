
/**
 * @file baseclient.h
 * @brief Déclaration de la structure Client et de la classe Baseclient.
 *
 * @details
 * Ce fichier définit la structure de données représentant un client
 * ainsi que la classe Baseclient qui gère l'ensemble des opérations
 * sur la base de données clients (authentification, ajout, suppression,
 * modification et affichage).
 *
 * La base de données est persistée dans un fichier texte @b database.txt,
 * chargé en mémoire au démarrage dans un vecteur de clients, ce qui évite
 * des accès disque répétés lors des opérations courantes.
 */

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

/**
 * @struct Client
 * @brief Représente un client enregistré dans la base de données.
 *
 * @details
 * Chaque client est identifié par un numéro de carte et
 * associé à un nom. Ces deux champs sont stockés sur deux lignes
 * consécutives dans le fichier @c database.txt.
 *
 * @var Client::numero
 *      Numéro de carte du client. Doit être unique et différent
 *      de celui réservé à l'accès opérateur.
 *
 * @var Client::nom
 *      Nom  du client
 *
 **/
struct Client {
    int numero;
    string nom;
};

/**
 * @class Baseclient
 * @brief Classe de gestion de la base de données clients de la borne.
 
 **/
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




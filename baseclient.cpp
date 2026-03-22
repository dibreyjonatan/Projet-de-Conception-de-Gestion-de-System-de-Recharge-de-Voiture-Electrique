#include "baseclient.h"

Baseclient::Baseclient() {
    charger();
}

void Baseclient::charger() {
    clients.clear();
    std::ifstream fich(fichier);
    if (!fich.is_open()) {
        std::cerr << "Erreur d'ouverture du fichier\n";
        return;
    }
    int val;
    while (fich >> val) {
        clients.push_back(val);   // on remplit le vector
    }
    fich.close();
}


void Baseclient::sauvegarder() {
    std::ofstream fich(fichier);
    if (!fich.is_open()) {
        std::cerr << "Erreur d'ouverture du fichier\n";
        return;
    }
    for (int c : clients) {
        fich << c << "\n";        // on réécrit tout le vector
    }
    fich.close();
}

int Baseclient::baseclient_authentifier(int num) {
    auto it = std::find(clients.begin(), clients.end(), num);
    return (it != clients.end()) ? 1 : 0;
}


void Baseclient::baseclient_ajoutclient(int k) {
    if (k == 255) {
        std::cout << "Vous ne pouvez pas ajouter ce numéro\n";
        return;
    }
    if (baseclient_authentifier(k)) {
        std::cout << "Le client figure déjà dans la liste\n";
        return;
    }
    clients.push_back(k);   // ajout dans le vector
    sauvegarder();           // puis on met à jour le fichier
    std::cout << "Client " << k << " ajouté avec succès\n";
}


void Baseclient::baseclient_supprimeclient(int k) {
    auto it = std::find(clients.begin(), clients.end(), k);
    if (it == clients.end()) {
        std::cout << "Le client ne figure pas dans la base de données\n";
        return;
    }
    clients.erase(it);   // suppression dans le vector
    sauvegarder();        // puis on met à jour le fichier
    std::cout << "Suppression avec succès\n";
}
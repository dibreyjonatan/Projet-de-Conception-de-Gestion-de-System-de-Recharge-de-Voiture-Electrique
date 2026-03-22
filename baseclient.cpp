#include "baseclient.h"

Baseclient::Baseclient() {
    charger();
}

void Baseclient::charger() {
    clients.clear();
    ifstream fich(fichier);
    if (!fich.is_open()) {
        cerr << "Erreur d'ouverture du fichier\n";
        return;
    }

    Client c;
    string ligne;
    while (getline(fich, ligne)) {
        c.numero = stoi(ligne);          // 1ère ligne : numéro
        if (getline(fich, c.nom)) {      // 2ème ligne : nom
            clients.push_back(c);
        }
    }
    fich.close();
}

void Baseclient::sauvegarder() {
    ofstream fich(fichier);
    if (!fich.is_open()) {
        cerr << "Erreur d'ouverture du fichier\n";
        return;
    }
    for (const Client& c : clients) {
        fich << c.numero << "\n";
        fich << c.nom    << "\n";
    }
    fich.close();
}

int Baseclient::baseclient_authentifier(int num) {
    for (const Client& c : clients) {
        if (c.numero == num) {
            cout << "Bonjour " << c.nom << " !\n";
            return 1;
        }
    }
    return 0;
}

void Baseclient::baseclient_ajoutclient(int num, string nom) {
    if (num == 255) {
        cout << "Vous ne pouvez pas ajouter ce numéro\n";
        return;
    }
    if (baseclient_authentifier(num)) {
        cout << "Ce client figure déjà dans la liste\n";
        return;
    }
    clients.push_back({num, nom});
    sauvegarder();
    cout << "Client " << nom << " ajouté avec succès\n";
}

void Baseclient::baseclient_supprimeclient(int num) {
    auto it = find_if(clients.begin(), clients.end(),
                [num](const Client& c){ return c.numero == num; });

    if (it == clients.end()) {
        cout << "Client introuvable dans la base de données\n";
        return;
    }
    cout << "Client " << it->nom << " supprimé avec succès\n";
    clients.erase(it);
    sauvegarder();
}

void Baseclient::baseclient_afficher_clients() {
    if (clients.empty()) {
        cout << "Aucun client enregistré\n";
        return;
    }
    cout << "\nListe des clients \n";
    for (const Client& c : clients) {
        cout << "  Carte n°" << c.numero 
                  << "  →  " << c.nom << "\n";
    }
   


void Baseclient::baseclient_modifierclient(int num) {
    auto it = find_if(clients.begin(), clients.end(),
                [num](const Client& c){ return c.numero == num; });

    if (it == clients.end()) {
        cout << "Client introuvable dans la base de données\n";
        return;
    }

    cout << "\nClient trouvé : Carte n°" << it->numero 
              << "  →  " << it->nom << "\n";
    cout << "Que voulez-vous modifier ?\n";
    cout << "  1 - Le nom    (actuel : " << it->nom     << ")\n";
    cout << "  2 - Le numéro (actuel : " << it->numero  << ")\n";
    cout << "  3 - Les deux\n";
    cout << "Votre choix : ";

    int choix;
    cin >> choix;

    if (choix == 1 || choix == 3) {
        string nouveau_nom;
        cout << "Nouveau nom : ";
        cin.ignore();
        getline(cin, nouveau_nom);
        it->nom = nouveau_nom;
    }

    if (choix == 2 || choix == 3) {
        int nouveau_num;
        cout << "Nouveau numéro de carte : ";
        cin >> nouveau_num;

        // vérifier que le nouveau numéro n'est pas déjà pris
        if (nouveau_num == 255) {
            cout << "Ce numéro est réservé, modification annulée\n";
            return;
        }
        auto doublon = find_if(clients.begin(), clients.end(),
                        [nouveau_num](const Client& c){ return c.numero == nouveau_num; });
        if (doublon != clients.end()) {
            cout << "Ce numéro est déjà attribué à " 
                     << doublon->nom << ", modification annulée\n";
            return;
        }
        it->numero = nouveau_num;
    }

    sauvegarder();
    cout << "Client modifié avec succès → Carte n°" 
              << it->numero << "  →  " << it->nom << "\n";
}
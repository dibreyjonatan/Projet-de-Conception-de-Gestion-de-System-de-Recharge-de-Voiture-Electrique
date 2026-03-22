#include "baseclient.h"

int Baseclient::baseclient_authentifier(int){
 std::ifstream fich("database.txt");
    if (!fich.is_open()) {
        std::cerr << "Erreur d'ouverture du fichier\n";
        return 0;
    }

    int numero_r;
    while (fich >> numero_r) {
        if (numero_r == num) {
            fich.close();
            return 1;
        }
    }
    fich.close();
    return 0;
}
void Baseclient::baseclient_ajoutclient(int){
 if (k == 255) {
        std::cout << "Vous ne pouvez pas ajouter ce numéro\n";
        return;
    }

    if (baseclient_authentifier(k)) {
        std::cout << "\nLe client que vous essayez d'ajouter figure déjà dans la liste\n";
        return;
    }

    std::ofstream fich("database.txt", std::ios::app);
    if (!fich.is_open()) {
        std::cerr << "Erreur d'ouverture du fichier\n";
        return;
    }
    fich << k << "\n";
    fich.close();

}
void Baseclient::baseclient_supprimeclient(int){
     std::ifstream fich("database.txt");
    std::ofstream tmp_data("temp.txt");

    if (!fich.is_open() || !tmp_data.is_open()) {
        std::cerr << "Erreur d'ouverture de la base de données ou du fichier temporaire\n";
        return;
    }

    int val;
    bool found = false;

    while (fich >> val) {
        if (val == k) {
            found = true;
        } else {
            tmp_data << val << "\n";
        }
    }

    fich.close();
    tmp_data.close();

    std::remove("database.txt");
    std::rename("temp.txt", "database.txt");

    if (found) {
        std::cout << "Suppression avec succès\n";
    } else {
        std::cout << "Le client que vous voulez supprimer ne figure pas dans la base de données\n";
    }
}
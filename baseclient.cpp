/**
 * @file baseclient.cpp
 * @brief Implémentation des méthodes de la classe Baseclient.
 *
 * @details
 * Ce fichier contient l'implémentation complète de la gestion de la base
 * de données clients. Toutes les opérations (lecture, écriture, recherche)
 * transitent par le vecteur @c clients maintenu en mémoire, le fichier
 * @c database.txt n'étant accédé qu'au chargement initial et après
 * chaque modification.
 */
#include "baseclient.h"

/**
 * @brief Constructeur de la classe Baseclient.
 *
 * @details
 * À l'instanciation de l'objet, le constructeur déclenche immédiatement
 * le chargement du fichier @c database.txt en mémoire via @c charger().
 * Ainsi, dès que l'objet est prêt à l'emploi, le vecteur @c clients
 * est déjà rempli et toutes les opérations peuvent s'effectuer sans
 * nouvel accès disque.
 */

 /**
 * @brief Charge le contenu du fichier @c database.txt dans le vecteur @c clients.
 *
 * @details
 * Cette méthode privée lit le fichier texte ligne par ligne en supposant
 * un format strict de deux lignes par client :
 * @li ligne impaire : numéro de carte (entier)
 * @li ligne paire   : nom du client (chaîne, peut contenir des espaces)
 *
 * Le vecteur @c clients est vidé avant chaque chargement pour éviter
 * les doublons en cas d'appel multiple. Si le fichier est introuvable
 * ou illisible, un message d'erreur est affiché et la méthode retourne
 * sans modifier le vecteur.
 *
 * @note Cette méthode est exclusivement appelée par le constructeur.
 *       Elle n'est pas destinée à être réutilisée en cours d'exécution.
 *
 * @par Variables internes utilisées :
 * @li **fich**  : flux de lecture associé au fichier @c database.txt.
 * @li **c**     : structure Client temporaire utilisée pour construire
 *                 chaque entrée avant insertion dans le vecteur.
 * @li **ligne** : chaîne de caractères lue ligne par ligne depuis le fichier.
 */
Baseclient::Baseclient() {
    charger();
}

/**
 * @brief Charge le contenu du fichier @c database.txt dans le vecteur @c clients.
 *
 * @details
 * Cette méthode privée lit le fichier texte ligne par ligne en supposant
 * un format strict de deux lignes par client :
 * @li ligne impaire : numéro de carte (entier)
 * @li ligne paire   : nom du client (chaîne, peut contenir des espaces)
 *
 * Le vecteur @c clients est vidé avant chaque chargement pour éviter
 * les doublons en cas d'appel multiple. Si le fichier est introuvable
 * ou illisible, un message d'erreur est affiché et la méthode retourne
 * sans modifier le vecteur.
 *
 * @note Cette méthode est exclusivement appelée par le constructeur.
 *       Elle n'est pas destinée à être réutilisée en cours d'exécution.
 *
 * @par Variables internes utilisées :
 * @li **fich**  : flux de lecture associé au fichier @c database.txt.
 * @li **c**     : structure Client temporaire utilisée pour construire
 *                 chaque entrée avant insertion dans le vecteur.
 * @li **ligne** : chaîne de caractères lue ligne par ligne depuis le fichier.
 */

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
/**
*
 * @brief Sauvegarde le contenu du vecteur @c clients dans le fichier @c database.txt.
 *
 * @details
 * Cette méthode privée réécrit intégralement le fichier à partir du vecteur
 * @c clients. Le fichier est ouvert en mode écrasement — son contenu précédent
 * est donc effacé à chaque appel. Chaque client est écrit sur deux lignes
 * consécutives : le numéro de carte, puis le nom.
 *
 * Elle est systématiquement appelée après toute opération modifiant le vecteur
 * (ajout, suppression, modification) afin de garantir la cohérence entre
 * la mémoire et le fichier de persistance.
 *
 * @warning En cas d'échec d'ouverture du fichier, la modification reste
 *          présente en mémoire dans le vecteur mais n'est pas persistée sur disque.
 *
 * @par Variables internes utilisées :
 * @li **fich** : flux d'écriture associé au fichier @c database.txt.
 * @li **c**    : référence constante sur chaque Client du vecteur lors du parcours.
  */
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
/**
 * @brief Authentifie un client à partir de son numéro de carte.
 *
 * @details
 * Parcourt le vecteur @c clients à la recherche d'un client dont le numéro
 * correspond au paramètre @p num. Si le client est trouvé, son nom est affiché
 * en guise d'accueil avant de retourner 1. Si aucun client ne correspond,
 * la fonction retourne 0 sans afficher de message.
 *
 * @param num Numéro de carte lu par le lecteur, à vérifier dans la base.
 *
 * @return int
 * @retval 1 si le client est reconnu dans la base de données.
 * @retval 0 si le numéro de carte est inconnu.
 */

int Baseclient::baseclient_authentifier(int num) {
    for (const Client& c : clients) {
        if (c.numero == num) {
            cout << "Bonjour " << c.nom << " !\n";
            return 1;
        }
    }
    return 0;
}
/**
 * @brief Ajoute un nouveau client dans la base de données.
 *
 * @details
 * Avant tout ajout, deux vérifications sont effectuées :
 * @li le numéro 255 est refusé car il est réservé à l'accès opérateur.
 * @li si le numéro est déjà présent dans le vecteur, l'ajout est annulé
 *     pour éviter les doublons.
 *
 * Si les vérifications sont passées, le client est ajouté en fin de vecteur
 * puis le fichier est mis à jour via @c sauvegarder().
 *
 * @param num Numéro de carte du nouveau client. Doit être unique et différent de 255.
 * @param nom Nom complet du nouveau client. Peut contenir des espaces.
 *
 * @return void
 */
void Baseclient::baseclient_ajoutclient(int num, string nom) {
    if (num == get_numAdmin()) {
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
/**
 * @brief Supprime un client de la base de données à partir de son numéro de carte.
 *
 * @details
 * Recherche dans le vecteur @c clients le premier élément dont le numéro
 * correspond à @p num via @c find_if. Si le client est trouvé, il est retiré
 * du vecteur avec @c erase() et le fichier est mis à jour via @c sauvegarder().
 * Si aucun client ne correspond, un message d'erreur est affiché et la
 * fonction retourne sans modifier la base.
 *
 * @param num Numéro de carte du client à supprimer.
 *
 * @par Variables internes utilisées :
 * @li **it** : itérateur pointant sur le client trouvé dans le vecteur,
 *              ou sur @c clients.end() si introuvable.
 *
 * @return void
 */

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
/**
 * @brief Affiche la liste complète des clients enregistrés dans la base.
 *
 * @details
 * Parcourt le vecteur @c clients et affiche pour chaque entrée le numéro
 * de carte et le nom du client. Si le vecteur est vide, un message
 * l'indique et la fonction retourne immédiatement.
 *
 * @note Cette fonction est destinée à un usage opérateur uniquement,
 *       appelée depuis @c administration_operateur() via le choix 3 du menu.
 *
 * @return void
 */
void Baseclient::baseclient_afficher_clients() {
    if (clients.empty()) {
        cout << "Aucun client enregistré\n";
        return;
    }
    cout << "\nListe des clients \n";
    for (const Client& c : clients) {
        cout << "  Carte n°" << c.numero 
                  << "  " << c.nom << "\n";
    }
   
}
/**
 * @brief Modifie le nom, le numéro de carte, ou les deux d'un client existant.
 *
 * @details
 * Recherche d'abord le client via son numéro @p num. Si trouvé, un sous-menu
 * est proposé à l'opérateur avec trois options :
 * @li **choix 1** : modification du nom uniquement.
 * @li **choix 2** : modification du numéro de carte uniquement.
 * @li **choix 3** : modification des deux champs.
 *
 * Avant toute modification du numéro, deux garde-fous sont vérifiés :
 * @li le numéro 255 est refusé car réservé à l'opérateur.
 * @li le nouveau numéro ne doit pas être déjà attribué à un autre client.
 *
 * Si l'une de ces conditions est violée, la modification est intégralement
 * annulée, y compris si le nom avait déjà été modifié (choix 3).
 * En cas de succès, le vecteur est mis à jour et persisté via @c sauvegarder().
 *
 * @param num Numéro de carte du client à modifier.
 *
 * @par Variables internes utilisées :
 * @li **it**          : itérateur pointant sur le client à modifier.
 * @li **choix**       : entier stockant le choix de modification saisi par l'opérateur.
 * @li **nouveau_nom** : chaîne stockant le nouveau nom saisi (choix 1 et 3).
 * @li **nouveau_num** : entier stockant le nouveau numéro saisi (choix 2 et 3).
 * @li **doublon**     : itérateur utilisé pour vérifier l'unicité du nouveau numéro.
 *
 * @warning Si le choix 3 est sélectionné et que le nouveau numéro est invalide,
 *          la modification du nom déjà effectuée en mémoire est perdue
 *          car @c sauvegarder() n'est pas appelée.
 *
 * @return void
 */
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
        if (nouveau_num == get_numAdmin()) {
            cout << "Impossible de l'ajouter \n";
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
    cout << "Client modifié avec succès Carte n°" 
              << it->numero << "  " << it->nom << "\n";
}

int get_numAdmin(){
    return numAdmin;
}

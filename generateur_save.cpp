/**
 * @file generateur_save.cpp
 * @brief Implémentation des méthodes de la classe Generateur_Save.
 *
 * @details
 * Ce fichier contient l'implémentation des méthodes de pilotage du
 * générateur PWM, du contacteur AC et de la lecture de tension DC.
 * Chaque méthode agit directement sur les registres matériels de la
 * borne via le pointeur @c io_g, initialisé à la construction de l'objet
 * par accès au segment de mémoire partagée.
 */

#include "generateur_save.h"

/**
 * @details
 * Récupère l'adresse du segment de mémoire partagée via @c acces_memoire()
 * et l'affecte au pointeur @c io_g. À partir de ce moment, toutes les
 * interactions avec le matériel transitent par ce pointeur sans nouvel
 * accès au mécanisme de mémoire partagée.
 *
 * @par Variables internes utilisées :
 * @li **shmid_g** : identifiant du segment de mémoire partagée, passé
 *                   par adresse à @c acces_memoire() pour être initialisé.
 */
Generateur_Save::Generateur_Save() {
    int shmid_g ;
    io_g = acces_memoire(&shmid_g) ;
}

/**
 * @details
 * Force le champ @c contacteur_AC à 1 dans le registre matériel via @c io_g,
 * ce qui provoque la fermeture physique du contacteur électromécanique
 * et établit la connexion AC vers le véhicule.
 *
 * @note Appelée exclusivement en @c etat3, après génération du signal
 *       @c AC_CL et verrouillage de la trappe.
 */
void Generateur_Save::generateur_save_fermer_contacteur() {
    io_g->contacteur_AC = 1;
}

/**
 * @details
 * Force le champ @c contacteur_AC à 0 dans le registre matériel via @c io_g,
 * ce qui provoque l'ouverture physique du contacteur électromécanique
 * et coupe la connexion AC vers le véhicule.
 *
 * @note Appelée dans plusieurs états du cycle :
 * @li en @c etat2 : ouverture avant le passage en charge active.
 * @li en @c etat4 : ouverture en fin de charge lors de la récupération.
 * @li en @c etat6 : ouverture finale avant remise en disponibilité de la borne.
 */
void Generateur_Save::generateur_save_ouvrir_contacteur() {
    io_g->contacteur_AC = 0;
}

/**
 * @details
 * Retourne directement la valeur du champ @c gene_u depuis le registre
 * matériel via @c io_g. Ce champ représente la tension DC mesurée côté
 * véhicule, mise à jour en temps réel par le matériel.
 *
 * Les valeurs attendues et leurs significations dans le cycle de charge :
 * @li **9**  → prise détectée par le véhicule        → transition @c etat1 vers @c etat2.
 * @li **6**  → véhicule prêt à recevoir la charge     → transition @c etat2 vers @c etat3.
 * @li **9**  → charge terminée                        → transition @c etat3 vers @c etat4.
 * @li **12** → véhicule débranché                     → transition @c etat5 vers @c etat6.
 *
 * @note Toute valeur différente de celles listées ci-dessus maintient
 *       le système dans son état courant jusqu'à la prochaine lecture.
 */
int Generateur_Save::generateur_save_tension_DC() {
    return (io_g->gene_u);
}

/**
 * @details
 * Affecte directement la valeur de @p signal au champ @c gene_pwm du
 * registre matériel via @c io_g. Le matériel prend en charge la génération
 * effective du signal PWM correspondant sans traitement supplémentaire
 * côté logiciel.
 *
 * Correspondance entre les valeurs du type @c pwm et leur usage :
 * @li @c DC    : détection de présence véhicule — @c etat1, @c etat4, @c etat5.
 * @li @c AC_1K : pré-charge à 1kHz               — @c etat2.
 * @li @c AC_CL : charge active                   — @c etat3.
 * @li @c STOP  : arrêt du générateur             — @c etat6 et appui bouton STOP.
 *
 * @warning Changer le signal PWM sans ouvrir le contacteur au préalable
 *          peut provoquer des transitoires de tension dangereuses pour
 *          le chargeur embarqué du véhicule.
 */
void Generateur_Save::generateur_save_generer_pwm(pwm signal) {
    io_g->gene_pwm = signal;
}
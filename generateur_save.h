/**
 * @file generateur_save.h
 * @brief Déclaration de la classe Generateur_Save — pilotage du générateur
 *        de signal PWM et du contacteur AC de la borne de recharge.
 *
 * @details
 * Ce fichier déclare la classe Generateur_Save qui assure l'interface entre
 * le logiciel et les composants de puissance de la borne :
 * @li le **générateur PWM** : produit les signaux nécessaires aux différentes
 *     phases de charge (DC, AC_1K, AC_CL, STOP).
 * @li le **contacteur AC**  : interrupteur électromécanique permettant d'ouvrir
 *     ou de fermer le circuit d'alimentation du véhicule.
 * @li la **tension DC**     : mesure de la tension côté véhicule, utilisée
 *     pour surveiller la progression et la fin de charge.
 *
 * La classe s'appuie sur la structure @c entrees définie dans @c donnees_borne.h,
 * accessible via un segment de mémoire partagée fourni par @c memoire_borne.h.
 */

#ifndef GENERATEUR_SAVE_H
#define GENERATEUR_SAVE_H

#include <donnees_borne.h>
#include <memoire_borne.h>

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std ;
#endif

/**
 * @class Generateur_Save
 * @brief Pilote le générateur PWM, le contacteur AC et la lecture
 *        de tension DC de la borne de recharge.
 *
 * @details
 * La classe Generateur_Save encapsule l'ensemble des interactions avec
 * les composants de puissance de la borne. Elle est utilisée dans la
 * machine à états de @c borne.cpp pour piloter les transitions entre
 * les différentes phases du cycle de charge :
 *
 * | Phase       | Signal PWM | Contacteur | État associé |
 * |-------------|------------|------------|--------------|
 * | Attente     | STOP       | Ouvert     | etat0, etat6 |
 * | Détection   | DC         | Ouvert     | etat1, etat4 |
 * | Pré-charge  | AC_1K      | Ouvert     | etat2        |
 * | Charge      | AC_CL      | Fermé      | etat3        |
 * | Récupération| DC         | Ouvert     | etat5        |
 *
 * En interne, elle s'appuie sur un pointeur @c io_g vers la structure
 * @c entrees mappée en mémoire partagée, donnant accès direct aux
 * registres matériels de la borne.
 */
class Generateur_Save {
    private :

        /**
         * @brief Pointeur vers la structure des entrées matérielles de la borne.
         *
         * @details
         * Initialisé dans le constructeur via @c acces_memoire(), ce pointeur
         * donne accès en lecture et en écriture aux registres matériels,
         * notamment la tension DC mesurée et les commandes PWM et contacteur.
         */
        entrees *io_g ;

    public :

        /**
         * @brief Constructeur de la classe Generateur_Save.
         *
         * @details
         * Initialise le pointeur @c io_g vers le segment de mémoire partagée
         * contenant les registres matériels de la borne, via @c acces_memoire().
         */
        Generateur_Save();

        /**
         * @brief Ferme le contacteur AC pour alimenter le véhicule.
         *
         * @details
         * Active le contacteur électromécanique afin d'établir la connexion
         * entre le réseau AC et le chargeur embarqué du véhicule. Cette action
         * est effectuée en @c etat3, une fois le signal PWM @c AC_CL généré
         * et la prise correctement verrouillée.
         *
         * @warning Ne pas fermer le contacteur sans avoir préalablement
         *          généré le signal PWM approprié, au risque d'endommager
         *          le chargeur embarqué du véhicule.
         *
         * @return void
         */
        void generateur_save_fermer_contacteur();

        /**
         * @brief Ouvre le contacteur AC pour couper l'alimentation du véhicule.
         *
         * @details
         * Désactive le contacteur électromécanique afin d'interrompre la
         * connexion entre le réseau AC et le véhicule. Appelée lors de
         * toute transition nécessitant une coupure sécurisée du circuit,
         * notamment en @c etat2, @c etat4 et @c etat6.
         *
         * @return void
         */
        void generateur_save_ouvrir_contacteur();

        /**
         * @brief Lit la tension DC mesurée côté véhicule.
         *
         * @details
         * Retourne la valeur entière de la tension DC lue depuis le registre
         * matériel via @c io_g. Cette mesure est utilisée à chaque état du
         * cycle de charge pour décider des transitions :
         *
         * @li retourne **9**  → prise détectée, transition vers @c etat2.
         * @li retourne **6**  → véhicule prêt à charger, transition vers @c etat3.
         * @li retourne **9**  → charge terminée, transition vers @c etat4.
         * @li retourne **12** → véhicule débranché, transition vers @c etat6.
         *
         * @return int  Valeur entière de la tension DC mesurée.
         */
        int generateur_save_tension_DC();

        /**
         * @brief Génère un signal PWM selon la phase de charge demandée.
         *
         * @details
         * Configure et active le générateur de signal PWM en fonction du
         * paramètre @p signal. Chaque valeur correspond à une phase précise
         * du cycle de charge :
         *
         * @li @c DC    : signal de détection, utilisé en @c etat1, @c etat4 et @c etat5.
         * @li @c AC_1K : signal de pré-charge à 1kHz, utilisé en @c etat2.
         * @li @c AC_CL : signal de charge active, utilisé en @c etat3.
         * @li @c STOP  : arrêt du générateur, utilisé en @c etat6 et lors
         *               d'un appui sur le bouton STOP.
         *
         * @param signal  Valeur de type @c pwm indiquant la phase à générer.
         *
         * @return void
         */
        void generateur_save_generer_pwm(pwm signal);
};

#endif
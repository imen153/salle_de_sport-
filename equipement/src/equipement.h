#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include <gtk/gtk.h>

// Structure pour Équipement
typedef struct {
    int id_equipement;
    char nom_equipement[300];
    char categorie[100];
    char centre[200];
    int quantite;
    char etat[20];  // "reserve", "disponible", "maintenance"
} equipement;

// Structure pour Réservation
typedef struct {
    int id_reservation;
    char nom_equipement[300];
    int quantite_reservee;
    char id_entraineur[50];
    char nom_entraineur[200];
    char nom_cours[200];
    int jour;
    int mois;
    int annee;
    int duree_heures;
    int duree_minutes;
    int horaire;
    char nom_centre[200];
    int numero_salle;
} reservation;

// ==== PROTOTYPES - EQUIPEMENT CRUD ====
void ajouter_equipement(equipement eq);
int equipement_existe(int id_equipement);
void supprimer_equipement(int id_equipement);
void modifier_equipement(equipement eq);
equipement trouver_equipement(int id_equipement);
int get_next_equipement_id();

// ==== PROTOTYPES - AFFICHAGE & RECHERCHE EQUIPEMENT ====
void afficher_equipements(GtkWidget *treeview);
int rechercher_equipements(const char *critere, GtkWidget *treeview);
int rechercher_equipements_avec_filtre(const char *critere,
                                       GtkWidget *treeview,
                                       gboolean filtre_disponible,
                                       gboolean filtre_reserve,
                                       gboolean filtre_maintenance);

// ==== PROTOTYPES - RESERVATION CRUD ====
void ajouter_reservation(reservation res);
void supprimer_reservation(int id_reservation);
void modifier_reservation(reservation res);
reservation trouver_reservation(int id_reservation);
int get_next_reservation_id();

// ==== PROTOTYPES - AFFICHAGE RESERVATIONS ====
void afficher_reservations(GtkWidget *treeview);
int rechercher_reservations(const char *critere, GtkWidget *treeview);

#endif // EQUIPEMENT_H

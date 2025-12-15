#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <ctype.h>
#include "equipement.h"

/* ============================================= */
/*          EQUIPEMENT CRUD FUNCTIONS            */
/* ============================================= */

int get_next_equipement_id() {
    equipement eq;
    int max_id = 0;
    FILE *f = fopen("equipement.txt", "r");
    if (f) {
        while (fscanf(f, "%d\t%299[^\t]\t%99[^\t]\t%199[^\t]\t%d\t%19[^\n]\n",
                      &eq.id_equipement, eq.nom_equipement, eq.categorie, 
                      eq.centre, &eq.quantite, eq.etat) != EOF) {
            if (eq.id_equipement > max_id) {
                max_id = eq.id_equipement;
            }
        }
        fclose(f);
    }
    return max_id + 1;
}

void ajouter_equipement(equipement eq) {
    FILE *f = fopen("equipement.txt", "a");
    if (!f) return;
    
    eq.id_equipement = get_next_equipement_id();
    
    fprintf(f, "%d\t%s\t%s\t%s\t%d\t%s\n",
            eq.id_equipement,
            eq.nom_equipement,
            eq.categorie,
            eq.centre,
            eq.quantite,
            eq.etat);
    
    fflush(f);
    fclose(f);
}

int equipement_existe(int id_equipement) {
    equipement eq;
    FILE *f = fopen("equipement.txt", "r");
    if (!f) return 0;
    
    while (fscanf(f, "%d\t%299[^\t]\t%99[^\t]\t%199[^\t]\t%d\t%19[^\n]\n",
                  &eq.id_equipement, eq.nom_equipement, eq.categorie, 
                  eq.centre, &eq.quantite, eq.etat) != EOF) {
        if (eq.id_equipement == id_equipement) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void supprimer_equipement(int id_equipement) {
    equipement eq;
    FILE *f = fopen("equipement.txt", "r");
    FILE *temp = fopen("temp_eq.txt", "w");
    
    if (!f || !temp) {
        if(f) fclose(f);
        if(temp) fclose(temp);
        return;
    }

    while (fscanf(f, "%d\t%299[^\t]\t%99[^\t]\t%199[^\t]\t%d\t%19[^\n]\n",
                  &eq.id_equipement, eq.nom_equipement, eq.categorie, 
                  eq.centre, &eq.quantite, eq.etat) != EOF) {
        if (eq.id_equipement != id_equipement) {
            fprintf(temp, "%d\t%s\t%s\t%s\t%d\t%s\n",
                    eq.id_equipement, eq.nom_equipement, eq.categorie,
                    eq.centre, eq.quantite, eq.etat);
        }
    }
    
    fclose(f);
    fclose(temp);
    remove("equipement.txt");
    rename("temp_eq.txt", "equipement.txt");
}

void modifier_equipement(equipement eq) {
    equipement tmp;
    FILE *f = fopen("equipement.txt", "r");
    FILE *temp = fopen("temp_eq.txt", "w");
    
    if (!f || !temp) {
        if(f) fclose(f);
        if(temp) fclose(temp);
        return;
    }

    while (fscanf(f, "%d\t%299[^\t]\t%99[^\t]\t%199[^\t]\t%d\t%19[^\n]\n",
                  &tmp.id_equipement, tmp.nom_equipement, tmp.categorie, 
                  tmp.centre, &tmp.quantite, tmp.etat) != EOF) {
        if (tmp.id_equipement == eq.id_equipement) {
            fprintf(temp, "%d\t%s\t%s\t%s\t%d\t%s\n",
                    eq.id_equipement, eq.nom_equipement, eq.categorie,
                    eq.centre, eq.quantite, eq.etat);
        } else {
            fprintf(temp, "%d\t%s\t%s\t%s\t%d\t%s\n",
                    tmp.id_equipement, tmp.nom_equipement, tmp.categorie,
                    tmp.centre, tmp.quantite, tmp.etat);
        }
    }
    
    fclose(f);
    fclose(temp);
    remove("equipement.txt");
    rename("temp_eq.txt", "equipement.txt");
}

equipement trouver_equipement(int id_equipement) {
    equipement eq = {0};
    FILE *f = fopen("equipement.txt", "r");
    if (!f) return eq;

    while (fscanf(f, "%d\t%299[^\t]\t%99[^\t]\t%199[^\t]\t%d\t%19[^\n]\n",
                  &eq.id_equipement, eq.nom_equipement, eq.categorie, 
                  eq.centre, &eq.quantite, eq.etat) != EOF) {
        if (eq.id_equipement == id_equipement) {
            fclose(f);
            return eq;
        }
    }
    
    fclose(f);
    eq.id_equipement = 0;
    return eq;
}

/* ============================================= */
/*            DISPLAY & SEARCH FUNCTIONS         */
/* ============================================= */

enum {
    COL_ID_EQ,
    COL_NOM_EQ,
    COL_CATEGORIE_EQ,
    COL_CENTRE_EQ,
    COL_QUANTITE_EQ,
    COL_ETAT_EQ,
    COL_COUNT_EQ
};

void afficher_equipements(GtkWidget *treeview) {
    GtkListStore *store;
    GtkTreeIter iter;
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    
    if (!store) {
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "ID", renderer, "text", COL_ID_EQ, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "Nom", renderer, "text", COL_NOM_EQ, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "Catégorie", renderer, "text", COL_CATEGORIE_EQ, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "Centre", renderer, "text", COL_CENTRE_EQ, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "Quantité", renderer, "text", COL_QUANTITE_EQ, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "État", renderer, "text", COL_ETAT_EQ, NULL);

        store = gtk_list_store_new(COL_COUNT_EQ,
                                   G_TYPE_INT,    // ID
                                   G_TYPE_STRING, // Nom
                                   G_TYPE_STRING, // Categorie
                                   G_TYPE_STRING, // Centre
                                   G_TYPE_INT,    // Quantite
                                   G_TYPE_STRING);// Etat
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
    }
    
    gtk_list_store_clear(store);

    FILE *f = fopen("equipement.txt", "r");
    if (!f) return;

    equipement eq;
    while (fscanf(f, "%d\t%299[^\t]\t%99[^\t]\t%199[^\t]\t%d\t%19[^\n]\n",
                  &eq.id_equipement, eq.nom_equipement, eq.categorie, 
                  eq.centre, &eq.quantite, eq.etat) != EOF) {
        
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           COL_ID_EQ, eq.id_equipement,
                           COL_NOM_EQ, eq.nom_equipement,
                           COL_CATEGORIE_EQ, eq.categorie,
                           COL_CENTRE_EQ, eq.centre,
                           COL_QUANTITE_EQ, eq.quantite,
                           COL_ETAT_EQ, eq.etat,
                           -1);
    }
    
    fclose(f);
}

int rechercher_equipements(const char *critere, GtkWidget *treeview) {
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    if (!store) return 0;
    
    gtk_list_store_clear(store);

    char lower_critere[600];
    strcpy(lower_critere, critere);
    for(size_t i = 0; lower_critere[i]; i++) 
        lower_critere[i] = tolower(lower_critere[i]);

    FILE *f = fopen("equipement.txt", "r");
    if (!f) return 0;

    int found = 0;
    equipement eq;

    while (fscanf(f, "%d\t%299[^\t]\t%99[^\t]\t%199[^\t]\t%d\t%19[^\n]\n",
                  &eq.id_equipement, eq.nom_equipement, eq.categorie, 
                  eq.centre, &eq.quantite, eq.etat) != EOF) {

        char temp[1000];
        snprintf(temp, sizeof(temp), "%s %s %s %s", 
                 eq.nom_equipement, eq.categorie, eq.centre, eq.etat);
        
        for(size_t i = 0; temp[i]; i++) 
            temp[i] = tolower(temp[i]);

        if (strstr(temp, lower_critere)) {
            found++;
            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               COL_ID_EQ, eq.id_equipement,
                               COL_NOM_EQ, eq.nom_equipement,
                               COL_CATEGORIE_EQ, eq.categorie,
                               COL_CENTRE_EQ, eq.centre,
                               COL_QUANTITE_EQ, eq.quantite,
                               COL_ETAT_EQ, eq.etat,
                               -1);
        }
    }
    
    fclose(f);
    return found;
}


/***********************************************/
/* Nouvelle fonction : recherche avec filtres d'état */
int rechercher_equipements_avec_filtre(const char *critere,
                                       GtkWidget *treeview,
                                       gboolean filtre_disponible,
                                       gboolean filtre_reserve,
                                       gboolean filtre_maintenance)
{
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    if (!store) return 0;

    gtk_list_store_clear(store);

    // Préparer critère texte
    char lower_critere[600] = {0};
    if (critere && strlen(critere) > 0) {
        strcpy(lower_critere, critere);
        for (size_t i = 0; lower_critere[i]; i++)
            lower_critere[i] = tolower(lower_critere[i]);
    }
    gboolean has_text_search = (strlen(lower_critere) > 0);

    // Vérifier si filtre état actif
    gboolean has_etat_filter = filtre_disponible || filtre_reserve || filtre_maintenance;

    FILE *f = fopen("equipement.txt", "r");
    if (!f) return 0;

    int found = 0;
    equipement eq;

    while (fscanf(f, "%d\t%299[^\t]\t%99[^\t]\t%199[^\t]\t%d\t%19[^\n]\n",
                  &eq.id_equipement, eq.nom_equipement, eq.categorie,
                  eq.centre, &eq.quantite, eq.etat) != EOF) {

        gboolean match_etat = TRUE;
        if (has_etat_filter) {
            match_etat = FALSE;
            if (filtre_disponible && strcmp(eq.etat, "disponible") == 0) match_etat = TRUE;
            if (filtre_reserve     && strcmp(eq.etat, "reserve") == 0)     match_etat = TRUE;
            if (filtre_maintenance && strcmp(eq.etat, "maintenance") == 0) match_etat = TRUE;
        }

        if (!match_etat) continue;

        // Filtre texte
        if (has_text_search) {
            char temp[1000];
            snprintf(temp, sizeof(temp), "%d %s %s %s %s",
                     eq.id_equipement, eq.nom_equipement, eq.categorie,
                     eq.centre, eq.etat);
            for (size_t i = 0; temp[i]; i++) temp[i] = tolower(temp[i]);
            if (!strstr(temp, lower_critere)) continue;
        }

        // Ajouter au TreeView
        found++;
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           COL_ID_EQ, eq.id_equipement,
                           COL_NOM_EQ, eq.nom_equipement,
                           COL_CATEGORIE_EQ, eq.categorie,
                           COL_CENTRE_EQ, eq.centre,
                           COL_QUANTITE_EQ, eq.quantite,
                           COL_ETAT_EQ, eq.etat,
                           -1);
    }

    fclose(f);
    return found;
}

// Add these functions to your equipement.c file

/* ============================================= */
/*          RESERVATION CRUD FUNCTIONS           */
/* ============================================= */

int get_next_reservation_id() {
    reservation res;
    int max_id = 0;
    FILE *f = fopen("reservations.txt", "r");
    if (f) {
        while (fscanf(f, "%d\t%299[^\t]\t%d\t%49[^\t]\t%199[^\t]\t%199[^\t]\t%d\t%d\t%d\t%d\t%d\t%d\t%199[^\t]\t%d\n",
                      &res.id_reservation, res.nom_equipement, &res.quantite_reservee,
                      res.id_entraineur, res.nom_entraineur, res.nom_cours,
                      &res.jour, &res.mois, &res.annee, 
                      &res.duree_heures, &res.duree_minutes, &res.horaire,
                      res.nom_centre, &res.numero_salle) != EOF) {
            if (res.id_reservation > max_id) {
                max_id = res.id_reservation;
            }
        }
        fclose(f);
    }
    return max_id + 1;
}

void ajouter_reservation(reservation res) {
    FILE *f = fopen("reservations.txt", "a");
    if (!f) return;
    
    res.id_reservation = get_next_reservation_id();
    
    fprintf(f, "%d\t%s\t%d\t%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%d\n",
            res.id_reservation,
            res.nom_equipement,
            res.quantite_reservee,
            res.id_entraineur,
            res.nom_entraineur,
            res.nom_cours,
            res.jour,
            res.mois,
            res.annee,
            res.duree_heures,
            res.duree_minutes,
            res.horaire,
            res.nom_centre,
            res.numero_salle);
    
    fflush(f);
    fclose(f);
}

void supprimer_reservation(int id_reservation) {
    reservation res;
    FILE *f = fopen("reservations.txt", "r");
    FILE *temp = fopen("temp_res.txt", "w");
    
    if (!f || !temp) {
        if(f) fclose(f);
        if(temp) fclose(temp);
        return;
    }

    while (fscanf(f, "%d\t%299[^\t]\t%d\t%49[^\t]\t%199[^\t]\t%199[^\t]\t%d\t%d\t%d\t%d\t%d\t%d\t%199[^\t]\t%d\n",
                  &res.id_reservation, res.nom_equipement, &res.quantite_reservee,
                  res.id_entraineur, res.nom_entraineur, res.nom_cours,
                  &res.jour, &res.mois, &res.annee, 
                  &res.duree_heures, &res.duree_minutes, &res.horaire,
                  res.nom_centre, &res.numero_salle) != EOF) {
        if (res.id_reservation != id_reservation) {
            fprintf(temp, "%d\t%s\t%d\t%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%d\n",
                    res.id_reservation, res.nom_equipement, res.quantite_reservee,
                    res.id_entraineur, res.nom_entraineur, res.nom_cours,
                    res.jour, res.mois, res.annee, 
                    res.duree_heures, res.duree_minutes, res.horaire,
                    res.nom_centre, res.numero_salle);
        }
    }
    
    fclose(f);
    fclose(temp);
    remove("reservations.txt");
    rename("temp_res.txt", "reservations.txt");
}

void modifier_reservation(reservation res) {
    reservation tmp;
    FILE *f = fopen("reservations.txt", "r");
    FILE *temp = fopen("temp_res.txt", "w");
    
    if (!f || !temp) {
        if(f) fclose(f);
        if(temp) fclose(temp);
        return;
    }

    while (fscanf(f, "%d\t%299[^\t]\t%d\t%49[^\t]\t%199[^\t]\t%199[^\t]\t%d\t%d\t%d\t%d\t%d\t%d\t%199[^\t]\t%d\n",
                  &tmp.id_reservation, tmp.nom_equipement, &tmp.quantite_reservee,
                  tmp.id_entraineur, tmp.nom_entraineur, tmp.nom_cours,
                  &tmp.jour, &tmp.mois, &tmp.annee, 
                  &tmp.duree_heures, &tmp.duree_minutes, &tmp.horaire,
                  tmp.nom_centre, &tmp.numero_salle) != EOF) {
        if (tmp.id_reservation == res.id_reservation) {
            fprintf(temp, "%d\t%s\t%d\t%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%d\n",
                    res.id_reservation, res.nom_equipement, res.quantite_reservee,
                    res.id_entraineur, res.nom_entraineur, res.nom_cours,
                    res.jour, res.mois, res.annee, 
                    res.duree_heures, res.duree_minutes, res.horaire,
                    res.nom_centre, res.numero_salle);
        } else {
            fprintf(temp, "%d\t%s\t%d\t%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%d\n",
                    tmp.id_reservation, tmp.nom_equipement, tmp.quantite_reservee,
                    tmp.id_entraineur, tmp.nom_entraineur, tmp.nom_cours,
                    tmp.jour, tmp.mois, tmp.annee, 
                    tmp.duree_heures, tmp.duree_minutes, tmp.horaire,
                    tmp.nom_centre, tmp.numero_salle);
        }
    }
    
    fclose(f);
    fclose(temp);
    remove("reservations.txt");
    rename("temp_res.txt", "reservations.txt");
}

reservation trouver_reservation(int id_reservation) {
    reservation res = {0};
    FILE *f = fopen("reservations.txt", "r");
    if (!f) return res;

    while (fscanf(f, "%d\t%299[^\t]\t%d\t%49[^\t]\t%199[^\t]\t%199[^\t]\t%d\t%d\t%d\t%d\t%d\t%d\t%199[^\t]\t%d\n",
                  &res.id_reservation, res.nom_equipement, &res.quantite_reservee,
                  res.id_entraineur, res.nom_entraineur, res.nom_cours,
                  &res.jour, &res.mois, &res.annee, 
                  &res.duree_heures, &res.duree_minutes, &res.horaire,
                  res.nom_centre, &res.numero_salle) != EOF) {
        if (res.id_reservation == id_reservation) {
            fclose(f);
            return res;
        }
    }
    
    fclose(f);
    res.id_reservation = 0;
    return res;
}

/* ============================================= */
/*       RESERVATION DISPLAY FUNCTIONS           */
/* ============================================= */

enum {
    COL_ID_RES,
    COL_EQUIPEMENT_RES,
    COL_QTE_RES,
    COL_ENTRAINEUR_RES,
    COL_COURS_RES,
    COL_DATE_RES,
    COL_HORAIRE_RES,
    COL_CENTRE_RES,
    COL_SALLE_RES,
    COL_COUNT_RES
};

void afficher_reservations(GtkWidget *treeview) {
    GtkListStore *store;
    GtkTreeIter iter;
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    
    if (!store) {
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "ID", renderer, "text", COL_ID_RES, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "Équipement", renderer, "text", COL_EQUIPEMENT_RES, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "Qté", renderer, "text", COL_QTE_RES, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "Entraineur", renderer, "text", COL_ENTRAINEUR_RES, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "Cours", renderer, "text", COL_COURS_RES, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "Date", renderer, "text", COL_DATE_RES, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "Horaire", renderer, "text", COL_HORAIRE_RES, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "Centre", renderer, "text", COL_CENTRE_RES, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, 
            "Salle", renderer, "text", COL_SALLE_RES, NULL);

        store = gtk_list_store_new(COL_COUNT_RES,
                                   G_TYPE_INT,    // ID
                                   G_TYPE_STRING, // Equipement
                                   G_TYPE_INT,    // Quantite
                                   G_TYPE_STRING, // Entraineur
                                   G_TYPE_STRING, // Cours
                                   G_TYPE_STRING, // Date
                                   G_TYPE_STRING, // Horaire
                                   G_TYPE_STRING, // Centre
                                   G_TYPE_INT);   // Salle
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
    }
    
    gtk_list_store_clear(store);

    FILE *f = fopen("reservations.txt", "r");
    if (!f) return;

    reservation res;
    while (fscanf(f, "%d\t%299[^\t]\t%d\t%49[^\t]\t%199[^\t]\t%199[^\t]\t%d\t%d\t%d\t%d\t%d\t%d\t%199[^\t]\t%d\n",
                  &res.id_reservation, res.nom_equipement, &res.quantite_reservee,
                  res.id_entraineur, res.nom_entraineur, res.nom_cours,
                  &res.jour, &res.mois, &res.annee, 
                  &res.duree_heures, &res.duree_minutes, &res.horaire,
                  res.nom_centre, &res.numero_salle) != EOF) {
        
        char date_str[50];
        sprintf(date_str, "%02d/%02d/%04d", res.jour, res.mois, res.annee);
        
        char horaire_str[50];
        sprintf(horaire_str, "%02d:00 (%dh%02dm)", res.horaire, res.duree_heures, res.duree_minutes);
        
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           COL_ID_RES, res.id_reservation,
                           COL_EQUIPEMENT_RES, res.nom_equipement,
                           COL_QTE_RES, res.quantite_reservee,
                           COL_ENTRAINEUR_RES, res.nom_entraineur,
                           COL_COURS_RES, res.nom_cours,
                           COL_DATE_RES, date_str,
                           COL_HORAIRE_RES, horaire_str,
                           COL_CENTRE_RES, res.nom_centre,
                           COL_SALLE_RES, res.numero_salle,
                           -1);
    }
    
    fclose(f);
}

int rechercher_reservations(const char *critere, GtkWidget *treeview) {
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    if (!store) return 0;
    
    gtk_list_store_clear(store);

    char lower_critere[600];
    strcpy(lower_critere, critere);
    for(size_t i = 0; lower_critere[i]; i++) 
        lower_critere[i] = tolower(lower_critere[i]);

    FILE *f = fopen("reservations.txt", "r");
    if (!f) return 0;

    int found = 0;
    reservation res;

    while (fscanf(f, "%d\t%299[^\t]\t%d\t%49[^\t]\t%199[^\t]\t%199[^\t]\t%d\t%d\t%d\t%d\t%d\t%d\t%199[^\t]\t%d\n",
                  &res.id_reservation, res.nom_equipement, &res.quantite_reservee,
                  res.id_entraineur, res.nom_entraineur, res.nom_cours,
                  &res.jour, &res.mois, &res.annee, 
                  &res.duree_heures, &res.duree_minutes, &res.horaire,
                  res.nom_centre, &res.numero_salle) != EOF) {

        char temp[1000];
        snprintf(temp, sizeof(temp), "%s %s %s %s", 
                 res.nom_equipement, res.nom_entraineur, res.nom_cours, res.nom_centre);
        
        for(size_t i = 0; temp[i]; i++) 
            temp[i] = tolower(temp[i]);

        if (strstr(temp, lower_critere)) {
            found++;
            
            char date_str[50];
            sprintf(date_str, "%02d/%02d/%04d", res.jour, res.mois, res.annee);
            
            char horaire_str[50];
            sprintf(horaire_str, "%02d:00 (%dh%02dm)", res.horaire, res.duree_heures, res.duree_minutes);
            
            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               COL_ID_RES, res.id_reservation,
                               COL_EQUIPEMENT_RES, res.nom_equipement,
                               COL_QTE_RES, res.quantite_reservee,
                               COL_ENTRAINEUR_RES, res.nom_entraineur,
                               COL_COURS_RES, res.nom_cours,
                               COL_DATE_RES, date_str,
                               COL_HORAIRE_RES, horaire_str,
                               COL_CENTRE_RES, res.nom_centre,
                               COL_SALLE_RES, res.numero_salle,
                               -1);
        }
    }
    
    fclose(f);
    return found;
}


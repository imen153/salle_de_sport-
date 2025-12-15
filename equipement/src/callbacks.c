#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "equipement.h"

static int etat_ajout = 2;      
static int etat_modifier = 2;   
static int supprimer_confirmed = 0; 


static gboolean filtre_disponible = FALSE;
static gboolean filtre_reserve     = FALSE;
static gboolean filtre_maintenance = FALSE;


gchar* get_combo_active_text(GtkComboBox *combo) {
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *text = NULL;
    
    if (gtk_combo_box_get_active_iter(combo, &iter)) {
        model = gtk_combo_box_get_model(combo);
        gtk_tree_model_get(model, &iter, 0, &text, -1);
    }
    return text;
}

const gchar* get_combo_entry_text(GtkWidget *combo) {
    GtkWidget *entry = gtk_bin_get_child(GTK_BIN(combo));
    if (entry && GTK_IS_ENTRY(entry)) {
        return gtk_entry_get_text(GTK_ENTRY(entry));
    }
    return "";
}


/* WINDOW ADMINISTRATEUR - MENU PRINCIPAL */

void on_ajouter_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window_admin = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *window_ajout = create_window_ajouter();
    
    g_object_set_data(G_OBJECT(window_ajout), "parent_window", window_admin);
    gtk_widget_show(window_ajout);
    gtk_widget_destroy(window_admin);

}

void on_modifier_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window_admin = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *window_recherche = create_window_rechercher();
    
    g_object_set_data(G_OBJECT(window_recherche), "parent_window", window_admin);
    gtk_widget_show(window_recherche);
    gtk_widget_destroy(window_admin);
}

void on_supprimer_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window_admin = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *window_supp = create_window_supprimer();
    
    g_object_set_data(G_OBJECT(window_supp), "parent_window", window_admin);
    gtk_widget_show(window_supp);
    gtk_widget_destroy(window_admin);
}

void on_rechercher_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window_admin = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *window_recherche = create_window_rechercher();
    
    g_object_set_data(G_OBJECT(window_recherche), "parent_window", window_admin);
    gtk_widget_show(window_recherche);
    gtk_widget_destroy(window_admin);
}

void on_button_retour_espace_admin_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(window);
}


/* WINDOW RECHERCHER */

void on_window_rechercher_show(GtkWidget *widget, gpointer user_data) {
    GtkWidget *treeview = lookup_widget(widget, "treeview_equipement_imen");
    if (treeview) {
        afficher_equipements(treeview);
    }
}

void on_button_rechercher_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *entry = lookup_widget(window, "entry_recherche_imen");
    GtkWidget *treeview = lookup_widget(window, "treeview_equipement_imen");

    if (!entry || !treeview) return;

    const gchar *critere = gtk_entry_get_text(GTK_ENTRY(entry));

    int found;

    if (strlen(critere) == 0) {
        
        if (filtre_disponible || filtre_reserve || filtre_maintenance) {
found = rechercher_equipements_avec_filtre(critere, treeview,
                                           filtre_disponible,
                                           filtre_reserve,
                                           filtre_maintenance);
        } else {
            afficher_equipements(treeview);
            return;
        }
    } else {
        
found = rechercher_equipements_avec_filtre(critere, treeview,
                                           filtre_disponible,
                                           filtre_reserve,
                                           filtre_maintenance);
    }

    if (found == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
            "Aucun équipement trouvé selon les critères.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_recherche_modifier_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *treeview = lookup_widget(window, "treeview_equipement_imen");
    
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Veuillez sélectionner un équipement à modifier.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    gint id;
    gtk_tree_model_get(model, &iter, 0, &id, -1);
    
    equipement eq = trouver_equipement(id);
    if (eq.id_equipement == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
            "Équipement introuvable !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    GtkWidget *win_mod = create_window_modifier();
    
    
    GtkWidget *entry_nom = lookup_widget(win_mod, "entry_modifier_Nom");
    GtkWidget *entry = gtk_bin_get_child(GTK_BIN(entry_nom));
    if (entry && GTK_IS_ENTRY(entry)) {
        gtk_entry_set_text(GTK_ENTRY(entry), eq.nom_equipement);
    }
    
    GtkWidget *entry_cat = lookup_widget(win_mod, "entry_modifier_Categorie");
    entry = gtk_bin_get_child(GTK_BIN(entry_cat));
    if (entry && GTK_IS_ENTRY(entry)) {
        gtk_entry_set_text(GTK_ENTRY(entry), eq.categorie);
    }
    
    GtkWidget *entry_centre = lookup_widget(win_mod, "entry_modifier_centre");
    entry = gtk_bin_get_child(GTK_BIN(entry_centre));
    if (entry && GTK_IS_ENTRY(entry)) {
        gtk_entry_set_text(GTK_ENTRY(entry), eq.centre);
    }
    
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(win_mod, "spinbutton_modifier_Quantite")), eq.quantite);
    
    
    if (strcmp(eq.etat, "reserve") == 0) {
        etat_modifier = 1;
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(win_mod, "radio_reserve")), TRUE);
    } else if (strcmp(eq.etat, "disponible") == 0) {
        etat_modifier = 2;
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(win_mod, "radio_disponible")), TRUE);
    } else {
        etat_modifier = 3;
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(win_mod, "radio_maintenance")), TRUE);
    }
    
    g_object_set_data(G_OBJECT(win_mod), "equipement_id", GINT_TO_POINTER(eq.id_equipement));
    g_object_set_data(G_OBJECT(win_mod), "parent_window", window);
    gtk_widget_show(win_mod);
}

void on_button_annuler_clicked(GtkButton *button, gpointer user_data) {
    gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));
}

void on_treeview_equipement_imen_cursor_changed(GtkTreeView *treeview, gpointer user_data) {
    // Optional: Show tooltip or preview
}

/* WINDOW MODIFIER */

void on_radio_reserve_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) etat_modifier = 1;
}

void on_radio_disponible_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) etat_modifier = 2;
}

void on_radio_maintenance_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) etat_modifier = 3;
}

void on_button_enregistrer_modifier_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *win = gtk_widget_get_toplevel(GTK_WIDGET(button));
    int id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(win), "equipement_id"));
    
    if (id <= 0) return;
    
    const gchar *nom = get_combo_entry_text(lookup_widget(win, "entry_modifier_Nom"));
    const gchar *cat = get_combo_entry_text(lookup_widget(win, "entry_modifier_Categorie"));
    const gchar *centre = get_combo_entry_text(lookup_widget(win, "entry_modifier_centre"));
    int quantite = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(lookup_widget(win, "spinbutton_modifier_Quantite")));
    
    if (strlen(nom) < 1) {
        GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(win), GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Nom requis !");
        gtk_dialog_run(GTK_DIALOG(d));
        gtk_widget_destroy(d);
        return;
    }
    
    equipement eq = {0};
    eq.id_equipement = id;
    strcpy(eq.nom_equipement, nom);
    strcpy(eq.categorie, cat);
    strcpy(eq.centre, centre);
    eq.quantite = quantite;
    
    if (etat_modifier == 1) strcpy(eq.etat, "reserve");
    else if (etat_modifier == 2) strcpy(eq.etat, "disponible");
    else strcpy(eq.etat, "maintenance");
    
    modifier_equipement(eq);
    
    GtkWidget *ok = gtk_message_dialog_new(GTK_WINDOW(win), GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Équipement modifié !");
    gtk_dialog_run(GTK_DIALOG(ok));
    gtk_widget_destroy(ok);
    
    
    GtkWidget *parent = g_object_get_data(G_OBJECT(win), "parent_window");
    if (parent) {
        GtkWidget *tv = lookup_widget(parent, "treeview_equipement_imen");
        if (tv) afficher_equipements(tv);
    }
    
    gtk_widget_destroy(win);
    etat_modifier = 2;
}

void on_button_annuler_modifier_clicked(GtkButton *button, gpointer user_data) {
    gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));
    etat_modifier = 2;
}


/* WINDOW SUPPRIMER */

void on_check_attention_supprimer_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    supprimer_confirmed = gtk_toggle_button_get_active(togglebutton);
}

void on_button_supprimer_imen_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    if (!supprimer_confirmed) {
        GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, 
            "Veuillez cocher la case de confirmation !");
        gtk_dialog_run(GTK_DIALOG(d));
        gtk_widget_destroy(d);
        return;
    }
    
    GtkWidget *entry = lookup_widget(window, "entryID_a_supp");
    const gchar *id_str = gtk_entry_get_text(GTK_ENTRY(entry));
    int id = atoi(id_str);
    
    if (id <= 0) {
        GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "ID invalide !");
        gtk_dialog_run(GTK_DIALOG(d));
        gtk_widget_destroy(d);
        return;
    }
    
    if (!equipement_existe(id)) {
        GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Équipement introuvable !");
        gtk_dialog_run(GTK_DIALOG(d));
        gtk_widget_destroy(d);
        return;
    }
    
    supprimer_equipement(id);
    
    GtkWidget *ok = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Équipement supprimé !");
    gtk_dialog_run(GTK_DIALOG(ok));
    gtk_widget_destroy(ok);
    
    gtk_widget_destroy(window);
    supprimer_confirmed = 0;
}

void on_button_supprimer_retour_clicked(GtkButton *button, gpointer user_data) {
    gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));
    supprimer_confirmed = 0;
}


/* WINDOW AJOUTER */

void on_radiobutton_reserver_equi_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) etat_ajout = 1;
}

void on_radio_disponible_equip_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) etat_ajout = 2;
}

void on_radiobutton_maintenance_equi_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) etat_ajout = 3;
}

void on_button_ajouter_enregistrer_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *win = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    gchar *nom = get_combo_active_text(GTK_COMBO_BOX(lookup_widget(win, "entry_ajouter_nom")));
    gchar *cat = get_combo_active_text(GTK_COMBO_BOX(lookup_widget(win, "entry_ajouter_categorie")));
    gchar *centre = get_combo_active_text(GTK_COMBO_BOX(lookup_widget(win, "combobox_centre")));
    int quantite = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(lookup_widget(win, "spinbutton_ajouter_quantite")));
    
    if (!nom || strlen(nom) < 1) {
        GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(win), GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Nom requis !");
        gtk_dialog_run(GTK_DIALOG(d));
        gtk_widget_destroy(d);
        g_free(nom); g_free(cat); g_free(centre);
        return;
    }
    
    equipement eq = {0};
    strcpy(eq.nom_equipement, nom ? nom : "");
    strcpy(eq.categorie, cat ? cat : "");
    strcpy(eq.centre, centre ? centre : "");
    eq.quantite = quantite;
    
    if (etat_ajout == 1) strcpy(eq.etat, "reserve");
    else if (etat_ajout == 2) strcpy(eq.etat, "disponible");
    else strcpy(eq.etat, "maintenance");
    
    ajouter_equipement(eq);
    
    GtkWidget *ok = gtk_message_dialog_new(GTK_WINDOW(win), GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Équipement ajouté !");
    gtk_dialog_run(GTK_DIALOG(ok));
    gtk_widget_destroy(ok);
    
    gtk_widget_destroy(win);
    g_free(nom); g_free(cat); g_free(centre);
    etat_ajout = 2;
}

void on_bouton_ajouter_retour_clicked(GtkButton *button, gpointer user_data) {
    gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));
    etat_ajout = 2;
}



/* WINDOW DASHBOARD */

void on_retour_clicked(GtkButton *button, gpointer user_data) {
    gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));
}

void on_administrateur_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window_admin = create_window_administrateur();
    gtk_widget_show(window_admin);

}

void on_entraineur_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window_reserver = create_window_reserver();
    gtk_widget_show(window_reserver);
}

void
on_checkbutton_disponible_toggled (GtkToggleButton *togglebutton,
                                   gpointer         user_data)
{
    filtre_disponible = gtk_toggle_button_get_active(togglebutton);
}

void
on_checkbutton_reserver_equi_toggled (GtkToggleButton *togglebutton,
                                      gpointer         user_data)
{
    filtre_reserve = gtk_toggle_button_get_active(togglebutton);
}

void
on_checkbutton_maintenance_toggled (GtkToggleButton *togglebutton,
                                    gpointer         user_data)
{
    filtre_maintenance = gtk_toggle_button_get_active(togglebutton);
}



/* WINDOW RESERVER */

void on_window_reserver_state_changed(GtkWidget *widget, gpointer user_data) {
    
    GtkWidget *treeview = lookup_widget(widget, "treeview_reservation_equip");
    if (treeview) {
        afficher_reservations(treeview);
    }
    
    
    GtkWidget *nom_equip = lookup_widget(widget, "nom_equip_res");
    if (nom_equip) {
        
        GtkWidget *entry = gtk_bin_get_child(GTK_BIN(nom_equip));
        
        
        FILE *f = fopen("equipement.txt", "r");
        if (f) {
            equipement eq;
            char equipments_added[1000][300] = {0}; 
            int count = 0;
            
            while (fscanf(f, "%d\t%299[^\t]\t%99[^\t]\t%199[^\t]\t%d\t%19[^\n]\n",
                          &eq.id_equipement, eq.nom_equipement, eq.categorie, 
                          eq.centre, &eq.quantite, eq.etat) != EOF) {
                
                
                if (strcmp(eq.etat, "disponible") == 0) {
                    
                    int already_added = 0;
                    for (int i = 0; i < count; i++) {
                        if (strcmp(equipments_added[i], eq.nom_equipement) == 0) {
                            already_added = 1;
                            break;
                        }
                    }
                    
                    if (!already_added) {
                        gtk_combo_box_append_text(GTK_COMBO_BOX(nom_equip), eq.nom_equipement);
                        strcpy(equipments_added[count], eq.nom_equipement);
                        count++;
                    }
                }
            }
            fclose(f);
        }
    }
    
    
    GtkWidget *nom_centre = lookup_widget(widget, "nom_centre_res");
    if (nom_centre) {
        gtk_combo_box_append_text(GTK_COMBO_BOX(nom_centre), "energy fit");
        gtk_combo_box_append_text(GTK_COMBO_BOX(nom_centre), "energy gym");
    }
}
void on_reserver_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    
    GtkWidget *nom_equip = lookup_widget(window, "nom_equip_res");
    GtkWidget *quantite = lookup_widget(window, "quantite");
    GtkWidget *id_entraineur = lookup_widget(window, "id_entrain_res");
    GtkWidget *nom_entraineur = lookup_widget(window, "nom_entraineur_res");
    GtkWidget *nom_cours = lookup_widget(window, "nom_cours_res");
    GtkWidget *jour = lookup_widget(window, "jour_res");
    GtkWidget *mois = lookup_widget(window, "mois_res");
    GtkWidget *annee = lookup_widget(window, "annee_res");
    GtkWidget *heure = lookup_widget(window, "heure_res");
    GtkWidget *min = lookup_widget(window, "min_res");
    GtkWidget *horaire = lookup_widget(window, "horaire_res");
    GtkWidget *nom_centre = lookup_widget(window, "nom_centre_res");
    GtkWidget *salle = lookup_widget(window, "nb_salle_res");
    
    
    const gchar *equip_text = get_combo_entry_text(nom_equip);
    const gchar *id_entr_text = gtk_entry_get_text(GTK_ENTRY(id_entraineur));
    const gchar *entr_text = get_combo_entry_text(nom_entraineur);
    const gchar *cours_text = get_combo_entry_text(nom_cours);
    const gchar *centre_text = get_combo_entry_text(nom_centre);
    
    if (strlen(equip_text) < 1 || strlen(id_entr_text) < 1 || 
        strlen(entr_text) < 1 || strlen(cours_text) < 1 || strlen(centre_text) < 1) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Veuillez remplir tous les champs obligatoires !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
   
    reservation res = {0};
    strcpy(res.nom_equipement, equip_text);
    res.quantite_reservee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(quantite));
    strcpy(res.id_entraineur, id_entr_text);
    strcpy(res.nom_entraineur, entr_text);
    strcpy(res.nom_cours, cours_text);
    res.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(jour));
    res.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(mois));
    res.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(annee));
    res.duree_heures = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(heure));
    res.duree_minutes = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(min));
    res.horaire = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(horaire));
    strcpy(res.nom_centre, centre_text);
    res.numero_salle = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(salle));
    
    
    ajouter_reservation(res);
    
    
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
        GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
        "Réservation ajoutée avec succès !");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    
    GtkWidget *treeview = lookup_widget(window, "treeview_reservation_equip");
    if (treeview) {
        afficher_reservations(treeview);
    }
    
    
    on_reset_clicked(button, user_data);
}

void on_reset_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    
    GtkWidget *nom_equip = lookup_widget(window, "nom_equip_res");
    GtkWidget *entry = gtk_bin_get_child(GTK_BIN(nom_equip));
    if (entry && GTK_IS_ENTRY(entry)) {
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    }
    
    GtkWidget *nom_entraineur = lookup_widget(window, "nom_entraineur_res");
    entry = gtk_bin_get_child(GTK_BIN(nom_entraineur));
    if (entry && GTK_IS_ENTRY(entry)) {
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    }
    
    GtkWidget *nom_cours = lookup_widget(window, "nom_cours_res");
    entry = gtk_bin_get_child(GTK_BIN(nom_cours));
    if (entry && GTK_IS_ENTRY(entry)) {
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    }
    
    GtkWidget *nom_centre = lookup_widget(window, "nom_centre_res");
    entry = gtk_bin_get_child(GTK_BIN(nom_centre));
    if (entry && GTK_IS_ENTRY(entry)) {
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    }
    
    
    GtkWidget *id_entraineur = lookup_widget(window, "id_entrain_res");
    gtk_entry_set_text(GTK_ENTRY(id_entraineur), "");
    
    
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(window, "quantite")), 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(window, "jour_res")), 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(window, "mois_res")), 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(window, "annee_res")), 2024);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(window, "heure_res")), 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(window, "min_res")), 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(window, "horaire_res")), 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(window, "nb_salle_res")), 1);
}

void on_retour_reserver_clicked(GtkButton *button, gpointer user_data) {
    gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));
}

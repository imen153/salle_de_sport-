#include <gtk/gtk.h>


void
on_ajouter_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_modifier_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_supprimer_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_rechercher_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_retour_espace_admin_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_window_rechercher_show              (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_treeview_equipement_imen_cursor_changed
                                        (GtkTreeView     *treeview,
                                        gpointer         user_data);

void
on_button_rechercher_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_recherche_modifier_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_annuler_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_radio_reserve_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radio_disponible_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radio_maintenance_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button_enregistrer_modifier_clicked (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_annuler_modifier_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_check_attention_supprimer_toggled   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button_supprimer_retour_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_supprimer_imen_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_radiobutton_reserver_equi_toggled   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radio_disponible_equip_toggled      (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton_maintenance_equi_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button_ajouter_enregistrer_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_bouton_ajouter_retour_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_window_reserver_state_changed       (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_reserver_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_reset_clicked                       (GtkButton       *button,
                                        gpointer         user_data);

void
on_retour_reserver_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_retour_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

void
on_administrateur_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_entraineur_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_checkbutton_disponible_toggled      (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_reserver_equi_toggled   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_maintenance_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

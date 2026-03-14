#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "adherant.h"
#include "structs.h"
#include "styleOutput.h"

#ifndef EMPRUNT_H
#define EMPRUNT_H

void EmprunterLivre(char *idAdherant);
void ValiderDemandesEmprunt();
void AfficherMesDemandesEnAttente(char *idAdherant);
void RestituerLivre(char *idAdherant);
int listEmpruntParAdherant(char *idAdherant);
int listEmpruntParLivre();
void affichageEmprunt();
int recherchehistorique();
int historiqueEmpruntparAdherant(char *idAdherant);
int historiqueEmpruntparLivre();
void Affichagehistorique();
int rechercheEmprunthistorique();
void historiqueEmpruntparAdherantEmpruntsOnly(char *idAdherant);
void historiqueRestitutionparAdherant(char *idAdherant);
void historiqueEmpruntparLivreOnly();
void historiqueRestitutionparLivre();
void listerTousLesLivres();
void listerTousLesLivresSansPause();
// ========== FONCTION UTILITAIRE: Obtenir la date d'aujourd'hui ==========
void getToday(int *y, int *m, int *d) {
  time_t now = time(NULL);
  struct tm *T = localtime(&now);
  *y = T->tm_year + 1900;
  *m = T->tm_mon + 1;
  *d = T->tm_mday;
}

// ========== FONCTION UTILITAIRE: Lister tous les livres disponibles ==========
// Version SANS pause - pour afficher avant une autre action
void listerTousLesLivres() {
   FILE *f = fopen("biblioteque.txt", "r");
  if (!f) return;
  
  setColor(14);
  printf("\n  LIVRES DISPONIBLES:\n");
  setColor(7);
  printDivider();
  
  char ligne[256];
  while (fgets(ligne, sizeof(ligne), f)) {
    if (strstr(ligne, "ISBN") != NULL) continue;
    char titre[53];
    strncpy(titre, ligne + 26, 52);
    titre[53] = '\0';
    printf("  > %s\n", titre);
  }
  fclose(f);
  printf("\n");
}

// ========== FONCTION UTILITAIRE: Lister tous les adhérants ==========
void listerTousLesAdherants() {
  FILE *f = fopen("adherantlist.txt", "r");
  if (!f) return;
  
  setColor(14);
  printf("\n  ADHERANTS DISPONIBLES:\n");
  setColor(7);
  printDivider();
  
  char ligne[256];
  while (fgets(ligne, sizeof(ligne), f)) {
    if (strstr(ligne, "ID") != NULL) continue;
    
    char id[11];
    strncpy(id, ligne + 2, 10);
    id[10] = '\0';
    
    // Retirer les espaces de fin
    int len = strlen(id) - 1;
    while (len >= 0 && id[len] == ' ') {
      id[len] = '\0';
      len--;
    }
    printf("  > %s\n", id);
  }
  fclose(f);
  printf("\n");
}

// ========== 1. EMPRUNTER UN LIVRE ==========
void EmprunterLivre(char *idAdherant) {
  clearScreen();
  printHeader("EMPRUNTER UN LIVRE");

  if (idAdherant == NULL) {
    printError("Erreur: ID adherant est NULL");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  FILE *f1 = fopen("biblioteque.txt", "r");
  FILE *f_requests = fopen("demandes_emprunt.txt", "a");

  if (!f1 || !f_requests) {
    printError("Erreur d'ouverture de fichier");
    if (f1) fclose(f1);
    if (f_requests) fclose(f_requests);
    return;
  }

  // Afficher les livres disponibles
  listerTousLesLivres();

  Emprunt Em;
  char title1[53];
  int trouve = 0;

  printDivider();
  printSectionTitle("Informations d'emprunt");

  viderBuffer();
  printInputPrompt("Titre du livre a emprunter : ");
  fgets(title1, sizeof(title1), stdin);
  title1[strcspn(title1, "\n")] = 0;
  printf("\n");

  // Vérifier que le livre existe et a du stock
  char line[256];
  const int STOCK_OFFSET = 135;
  int stock;

  printf("\n");
  printInfo("Verification du stock...");

  while (fgets(line, sizeof(line), f1)) {
    if (strstr(line, title1) == NULL) {
      continue;
    }
    trouve = 1;

    sscanf(line + STOCK_OFFSET, "%d", &stock);

    if (stock <= 0) {
      printError("Aucun exemplaire disponible");
      fclose(f1);
      fclose(f_requests);
      printf("\n  Appuyez sur Entree pour continuer...");
      getchar();
      return;
    }
    break;
  }

  if (!trouve) {
    printf("\n");
    printError("Livre non trouve");
    fclose(f1);
    fclose(f_requests);
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  printf("\n");
  printSectionTitle("Date de retour souhaitee");

  printInputPrompt("Annee : ");
  scanf("%d", &Em.dateRetour.annee);
  printf("\n");

  printInputPrompt("Mois : ");
  scanf("%d", &Em.dateRetour.mois);
  printf("\n");

  printInputPrompt("Jour : ");
  scanf("%d", &Em.dateRetour.jour);

  // Date automatique pour la demande
  time_t now = time(NULL);
  struct tm *T = localtime(&now);
  Em.dateEmprunt.annee = T->tm_year + 1900;
  Em.dateEmprunt.mois = T->tm_mon + 1;
  Em.dateEmprunt.jour = T->tm_mday;

  // Sauvegarder la demande avec statut "EN_ATTENTE"
  fprintf(f_requests, "| %-52s | %10s | %d-%02d-%02d | %d-%02d-%02d | %-12s |\n", 
          title1,
          idAdherant, 
          Em.dateEmprunt.annee, Em.dateEmprunt.mois, Em.dateEmprunt.jour, 
          Em.dateRetour.annee, Em.dateRetour.mois, Em.dateRetour.jour,
          "EN_ATTENTE");

  fclose(f1);
  fclose(f_requests);

  printf("\n");
  printDivider();
  printSuccess("Demande d'emprunt envoyee avec succes!");
  setColor(14);
  printf("  Livre : %s\n", title1);
  printf("  Statut : En attente de validation par le responsable\n");
  printf("  Date de retour souhaitee : %02d/%02d/%d\n", 
         Em.dateRetour.jour, Em.dateRetour.mois, Em.dateRetour.annee);
  setColor(7);
  
  printDivider();
  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
  getchar();
}

// ========== 2. AFFICHER MES DEMANDES EN ATTENTE ==========
void AfficherMesDemandesEnAttente(char *idAdherant) {
  clearScreen();
  printHeader("MES DEMANDES EN ATTENTE");

  FILE *f = fopen("demandes_emprunt.txt", "r");
  if (!f) {
    printInfo("Aucune demande en attente");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  char line[256];
  int count = 0;

  printDivider();
  setColor(11);
  printf("  VOS DEMANDES D'EMPRUNT EN ATTENTE DE VALIDATION:\n");
  setColor(7);
  printDivider();
  printf("\n");

  while (fgets(line, sizeof(line), f)) {
    if (strstr(line, idAdherant) && strstr(line, "EN_ATTENTE")) {
      printf("%s", line);
      count++;
    }
  }

  fclose(f);

  if (count == 0) {
    printInfo("Vous n'avez aucune demande en attente");
  } else {
    printf("\n");
    setColor(14);
    printf("  Total : %d demande(s) en attente\n", count);
    setColor(7);
  }

  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
}

// ========== 3. VALIDER LES DEMANDES D'EMPRUNT ==========
void ValiderDemandesEmprunt() {
  clearScreen();
  printHeader("VALIDER LES DEMANDES D'EMPRUNT");

  FILE *f_requests = fopen("demandes_emprunt.txt", "r");
  if (!f_requests) {
    printError("Impossible d'ouvrir le fichier des demandes");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  // Afficher toutes les demandes en attente
  char line[256];
  int count = 0;
  
  printDivider();
  setColor(11);
  printf("  DEMANDES EN ATTENTE DE VALIDATION:\n");
  setColor(7);
  printDivider();
  printf("\n");
  
  while (fgets(line, sizeof(line), f_requests)) {
    if (strstr(line, "EN_ATTENTE") != NULL) {
      printf("%s", line);
      count++;
    }
  }
  
  if (count == 0) {
    printInfo("Aucune demande en attente");
    fclose(f_requests);
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  rewind(f_requests);

  printf("\n");
  printDivider();
  
  // Lister les livres et adhérants disponibles
  listerTousLesLivres();
  listerTousLesAdherants();
  
  char titre[53], idAdh[11];
  
  viderBuffer();
  printInputPrompt("Titre du livre a valider : ");
  fgets(titre, sizeof(titre), stdin);
  titre[strcspn(titre, "\n")] = 0;

  printf("\n");
  printInputPrompt("ID de l'adherant : ");
  scanf("%10s", idAdh);

  // Traiter la validation
  FILE *f_temp = fopen("temp_demandes.txt", "w");
  FILE *f_biblio = fopen("biblioteque.txt", "r");
  FILE *f_biblio_temp = fopen("temp_biblio.txt", "w");
  FILE *f_emprunts = fopen("emprunts.txt", "a");
  FILE *f_historique = fopen("historique.txt", "a");

  if (!f_temp || !f_biblio || !f_biblio_temp || !f_emprunts || !f_historique) {
    printError("Erreur d'ouverture des fichiers");
    if (f_temp) fclose(f_temp);
    if (f_biblio) fclose(f_biblio);
    if (f_biblio_temp) fclose(f_biblio_temp);
    if (f_emprunts) fclose(f_emprunts);
    if (f_historique) fclose(f_historique);
    fclose(f_requests);
    return;
  }

  int found = 0;
  Emprunt Em;
  
  // Trouver et valider la demande
  while (fgets(line, sizeof(line), f_requests)) {
    if (strstr(line, titre) && strstr(line, idAdh) && strstr(line, "EN_ATTENTE")) {
      found = 1;
      
      // Parser les dates de la demande
      char *datePos = strstr(line, idAdh) + strlen(idAdh) + 3;
      sscanf(datePos, "%d-%d-%d | %d-%d-%d", 
             &Em.dateEmprunt.annee, &Em.dateEmprunt.mois, &Em.dateEmprunt.jour,
             &Em.dateRetour.annee, &Em.dateRetour.mois, &Em.dateRetour.jour);

      // Ajouter aux emprunts
      fprintf(f_emprunts, "| %-52s | %10s | %d-%02d-%02d | %d-%02d-%02d |\n", 
              titre, idAdh,
              Em.dateEmprunt.annee, Em.dateEmprunt.mois, Em.dateEmprunt.jour,
              Em.dateRetour.annee, Em.dateRetour.mois, Em.dateRetour.jour);

      // Ajouter à l'historique
      fprintf(f_historique, "| %-52s | %10s | %d-%02d-%02d | %d-%02d-%02d | %-10s |\n", 
              titre, idAdh,
              Em.dateEmprunt.annee, Em.dateEmprunt.mois, Em.dateEmprunt.jour,
              Em.dateRetour.annee, Em.dateRetour.mois, Em.dateRetour.jour,
              "EMPRUNT");

      // Ne pas copier dans temp (retirer des demandes en attente)
      continue;
    }
    fputs(line, f_temp);
  }

  if (found) {
    // Mettre à jour le stock dans biblioteque
    char biblio_line[256];
    const int STOCK_OFFSET = 135;
    int stock, i;

    while (fgets(biblio_line, sizeof(biblio_line), f_biblio)) {
      if (strstr(biblio_line, titre) == NULL) {
        fputs(biblio_line, f_biblio_temp);
        continue;
      }

      for (i = 0; i < STOCK_OFFSET; i++) {
        fputc(biblio_line[i], f_biblio_temp);
      }

      sscanf(biblio_line + STOCK_OFFSET, "%d", &stock);
      stock--;
      fprintf(f_biblio_temp, " %-10d |\n", stock);
    }

    printf("\n");
    printDivider();
    printSuccess("Demande validee avec succes!");
    setColor(10);
    printf("  Livre : %s\n", titre);
    printf("  Adherant : %s\n", idAdh);
    printf("  Date de retour : %02d/%02d/%d\n", 
           Em.dateRetour.jour, Em.dateRetour.mois, Em.dateRetour.annee);
    setColor(7);
  } else {
    printError("Demande non trouvee ou deja traitee");
  }

  fclose(f_requests);
  fclose(f_temp);
  fclose(f_biblio);
  fclose(f_biblio_temp);
  fclose(f_emprunts);
  fclose(f_historique);

  remove("demandes_emprunt.txt");
  rename("temp_demandes.txt", "demandes_emprunt.txt");
  
  if (found) {
    remove("biblioteque.txt");
    rename("temp_biblio.txt", "biblioteque.txt");
  } else {
    remove("temp_biblio.txt");
  }

  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
  getchar();
}

// ========== 4. RESTITUER UN LIVRE ==========
void RestituerLivre(char *idAdherant) {
  clearScreen();
  printHeader("RESTITUER UN LIVRE");

  if (idAdherant == NULL) {
    printError("Erreur: ID adherant est NULL");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  FILE *f1 = fopen("biblioteque.txt", "r");
  FILE *f2 = fopen("temp.txt", "w");
  FILE *f3 = fopen("emprunts.txt", "r");
  FILE *f4 = fopen("tempp.txt", "w");
  FILE *f5 = fopen("historique.txt", "a");

  if (!f1 || !f2 || !f3 || !f4 || !f5) {
    printError("Erreur d'ouverture de fichier");
    if (f1) fclose(f1);
    if (f2) fclose(f2);
    if (f3) fclose(f3);
    if (f4) fclose(f4);
    if (f5) fclose(f5);
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }
  
  // Afficher les livres disponibles
  listerTousLesLivres();
  
  Emprunt Em;
  char title1[53];
  int trouve = 0;
  int found = 0, i;

  printDivider();
  viderBuffer();
  printInputPrompt("Titre du livre a restituer : ");
  fgets(title1, sizeof(title1), stdin);
  title1[strcspn(title1, "\n")] = 0;

  printf("\n");
  printInfo("Traitement en cours...");

  char line[256];
  const int STOCK_OFFSET = 135;
  int stock;

  // Mettre à jour le stock
  while (fgets(line, sizeof(line), f1)) {
    if (strstr(line, title1) == NULL) {
      fputs(line, f2);
      continue;
    }
    trouve = 1;

    for (i = 0; i < STOCK_OFFSET; i++) {
      fputc(line[i], f2);
    }

    sscanf(line + STOCK_OFFSET, "%d", &stock);
    stock++;
    fprintf(f2, " %-10d |\n", stock);
  }

  printf("\n");
  printSectionTitle("Date d'emprunt");

  printInputPrompt("Annee : ");
  scanf("%d", &Em.dateEmprunt.annee);
  printf("\n");

  printInputPrompt("Mois : ");
  scanf("%d", &Em.dateEmprunt.mois);
  printf("\n");

  printInputPrompt("Jour : ");
  scanf("%d", &Em.dateEmprunt.jour);
  
  // Retirer des emprunts et ajouter à l'historique
  while (fgets(line, sizeof(line), f3)) {
    if (strstr(line, title1) && strstr(line, idAdherant)) {
      found = 1;

      // Obtenir la date d'aujourd'hui
      time_t now = time(NULL);
      struct tm *T = localtime(&now);
      Em.dateRetour.annee = T->tm_year + 1900;
      Em.dateRetour.mois = T->tm_mon + 1;
      Em.dateRetour.jour = T->tm_mday;
      
      printf("  Date : %02d/%02d/%d\n", Em.dateRetour.jour, Em.dateRetour.mois,
             Em.dateRetour.annee);

      // Ajouter à l'historique avec le bon format
      fprintf(f5, "| %-52s | %10s | %d-%02d-%02d | %d-%02d-%02d | %-10s |\n", 
              title1, 
              idAdherant,
              Em.dateEmprunt.annee, Em.dateEmprunt.mois, Em.dateEmprunt.jour,
              Em.dateRetour.annee, Em.dateRetour.mois, Em.dateRetour.jour,
              "RESTITUE");

    } else {
      fputs(line, f4);
    }
  }

  fclose(f1);
  fclose(f2);
  fclose(f3);
  fclose(f4);
  fclose(f5);

  remove("biblioteque.txt");
  rename("temp.txt", "biblioteque.txt");
  remove("emprunts.txt");
  rename("tempp.txt", "emprunts.txt");

  printf("\n");
  printDivider();

  if (!trouve) {
    printError("Livre non trouve dans la bibliotheque");
  } else if (!found) {
    printError("Cet emprunt n'existe pas pour cet adherant");
  } else {
    printSuccess("Livre restitue avec succes!");
    setColor(10);
    printf("  Livre : %s\n", title1);
    setColor(7);
  }

  printDivider();
  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
  getchar();
}

// ========== 5. LISTE DES EMPRUNTS PAR ADHERANT ==========
int listEmpruntParAdherant(char *idAdherant) {
  clearScreen();
  printHeader("EMPRUNTS PAR ADHERANT");

  FILE *f1 = fopen("emprunts.txt", "r");
  if (f1 == NULL) {
    printError("Impossible d'ouvrir le fichier");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return 0;
  }

  int found = 0;

  printDivider();
  printf("\n");
  printInfo("Recherche des emprunts...");
  printf("\n");
  printDivider();

  setColor(11);
  printf("\n  %-52s %-14s %-14s\n", "TITRE", "DATE EMPRUNT", "DATE RETOUR");
  setColor(7);
  printDivider();

  char line[256];
  while (fgets(line, sizeof(line), f1)) {
    if (strstr(line, "TITRE") != NULL)
      continue;
    if (strstr(line, idAdherant) != NULL) {
      found = 1;
      char titre[53], dateE[11], dateR[11];
      strncpy(titre, line + 2, 52);
      titre[52] = '\0';
      strncpy(dateE, line + 70, 10);
      dateE[10] = '\0';
      strncpy(dateR, line + 83, 10);
      dateR[10] = '\0';
      printf("  %-52s %-14s %-14s\n", titre, dateE, dateR);
    }
  }

  fclose(f1);

  printDivider();

  if (!found) {
    printf("\n");
    printError("Aucun emprunt trouve pour cet adherant");
  } else {
    printf("\n");
    printSuccess("Recherche terminee");
  }

  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
  return found;
}

// ========== 6. LISTE DES EMPRUNTS PAR LIVRE ==========
int listEmpruntParLivre() {
  clearScreen();
  printHeader("EMPRUNTS PAR LIVRE");

  FILE *f1 = fopen("emprunts.txt", "r");
  if (f1 == NULL) {
    printError("Impossible d'ouvrir le fichier");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return 0;
  }

   FILE *f = fopen("biblioteque.txt", "r");
  if (!f) exit(0);
  
  setColor(14);
  printf("\n  LIVRES DISPONIBLES:\n");
  setColor(7);
  printDivider();
  
  char ligne[256];
  while (fgets(ligne, sizeof(ligne), f)) {
    if (strstr(ligne, "ISBN") != NULL) continue;
    char titre[53];
    strncpy(titre, ligne + 26, 52);
    titre[53] = '\0';
    printf("  > %s\n", titre);
  }
  fclose(f);
  printf("\n");

  int found = 0;
  char title[53];

  printDivider();
  viderBuffer();
  printInputPrompt("Titre du livre : ");
  fgets(title, sizeof(title), stdin);
  title[strcspn(title, "\n")] = 0;

  printf("\n");
  printInfo("Recherche des emprunts...");
  printf("\n");
  printDivider();

  setColor(11);
  printf("\n  %-14s %-14s %-14s\n", "ID ADHERANT", "DATE EMPRUNT",
         "DATE RETOUR");
  setColor(7);
  printDivider();

  char line[256];
  while (fgets(line, sizeof(line), f1)) {
    if (strstr(line, "TITRE") != NULL)
      continue;
    if (strstr(line, title) != NULL) {
      found = 1;
      char id[11], dateE[11], dateR[11];
      strncpy(id, line + 57, 10);
      id[10] = '\0';
      strncpy(dateE, line + 70, 10);
      dateE[10] = '\0';
      strncpy(dateR, line + 83, 10);
      dateR[10] = '\0';
      printf("  %-14s %-14s %-14s\n", id, dateE, dateR);
    }
  }

  fclose(f1);
  printDivider();

  if (!found) {
    printf("\n");
    printError("Aucun emprunt trouve pour ce livre");
  } else {
    printf("\n");
    printSuccess("Recherche terminee");
  }
  
  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
  return found;
}

// ========== 7. AFFICHAGE DE TOUS LES EMPRUNTS ==========
void affichageEmprunt() {
  clearScreen();
  printHeader("LISTE DES EMPRUNTS EN COURS");

  FILE *f = fopen("emprunts.txt", "r");
  if (!f) {
    printError("Impossible d'ouvrir le fichier");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  printDivider();
  char line[256];
  int count = 0;

  while (fgets(line, sizeof(line), f) != NULL) {
    printf("  %s", line);
    if (strstr(line, "TITRE") == NULL)
      count++;
  }

  fclose(f);

  printf("\n");
  printDivider();
  setColor(11);
  printf("  Total : %d emprunt(s) en cours\n", count);
  setColor(7);
  printDivider();
  
  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
}

// ========== 8. AFFICHAGE HISTORIQUE COMPLET ==========
void Affichagehistorique() {
  clearScreen();
  printHeader("HISTORIQUE COMPLET DES ACTIONS");

  FILE *f = fopen("historique.txt", "r");
  if (!f) {
    printError("Impossible d'ouvrir le fichier historique");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  char line[256];
  int count = 0;

  printDivider();
  setColor(11);
  printf("  HISTORIQUE DE TOUTES LES ACTIONS:\n");
  setColor(7);
  printDivider();
  printf("\n");

  // En-tête
  setColor(14);
  printf("  %-52s | %-10s | %-10s | %-10s | %-10s\n", 
         "TITRE", "ID ADHERANT", "DATE EMPR.", "DATE RET.", "ACTION");
  setColor(8);
  printf("  ");
  int i;
  for (i = 0; i < 100; i++) printf("-");
  printf("\n");
  setColor(7);

  while (fgets(line, sizeof(line), f)) {
    if (strstr(line, "TITRE") != NULL) continue;
    
    printf("%s", line);
    count++;
  }

  fclose(f);

  printf("\n");
  printDivider();
  setColor(10);
  printf("  Total : %d action(s) dans l'historique\n", count);
  setColor(7);

  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
}

// ========== 9. HISTORIQUE PAR ADHERANT (TOUS) ==========
int historiqueEmpruntparAdherant(char *idAdherant) {
  clearScreen();
  printHeader("HISTORIQUE PAR ADHERANT");

  if (idAdherant == NULL || strlen(idAdherant) == 0) {
    printError("ID adherant invalide");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return 0;
  }

  FILE *f = fopen("historique.txt", "r");
  if (!f) {
    printError("Impossible d'ouvrir le fichier historique");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return 0;
  }

  char line[256];
  int count = 0;

  printDivider();
  setColor(11);
  printf("  HISTORIQUE POUR L'ADHERANT: %s\n", idAdherant);
  setColor(7);
  printDivider();
  printf("\n");

  // En-tête
  setColor(14);
  printf("  %-52s | %-10s | %-10s | %-10s\n", 
         "TITRE", "DATE EMPR.", "DATE RET.", "ACTION");
  setColor(8);
  printf("  ");
  int i;
  for (i = 0; i < 90; i++) printf("-");
  printf("\n");
  setColor(7);

  
  while (fgets(line, sizeof(line), f)) {
    if (strstr(line, "TITRE") != NULL)
      continue;
    if (strstr(line, idAdherant) != NULL) {
      count++;
      char titre[53], dateE[11], dateR[11],action[11];
      strncpy(titre, line + 2, 52);
      titre[52] = '\0';
      strncpy(dateE, line + 70, 10);
      dateE[10] = '\0';
      strncpy(dateR, line + 83, 10);
      dateR[10] = '\0';
      strncpy(action,line+95,10);
      printf("  %-52s %-14s %-14s %-14s \n", titre, dateE, dateR, action);
    }
  }

  fclose(f);

  if (count == 0) {
    printInfo("Aucun historique trouve pour cet adherant");
  } else {
    printf("\n");
    printDivider();
    setColor(10);
    printf("  Total : %d action(s) pour cet adherant\n", count);
    setColor(7);
  }

  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
  return count > 0 ? 1 : 0;
}

// ========== 10. HISTORIQUE EMPRUNTS PAR ADHERANT ==========
void historiqueEmpruntparAdherantEmpruntsOnly(char *idAdherant) {
  clearScreen();
  printHeader("HISTORIQUE EMPRUNTS PAR ADHERANT");

  if (idAdherant == NULL || strlen(idAdherant) == 0) {
    printError("ID adherant invalide");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  FILE *f = fopen("historique.txt", "r");
  if (!f) {
    printError("Impossible d'ouvrir le fichier historique");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  char line[256];
  int count = 0;

  printDivider();
  setColor(11);
  printf("  HISTORIQUE DES EMPRUNTS POUR: %s\n", idAdherant);
  setColor(7);
  printDivider();
  printf("\n");

  setColor(14);
  printf("  %-52s | %-10s | %-10s\n", 
         "TITRE", "DATE EMPR.", "DATE RET.");
  setColor(8);
  printf("  ");
  int i;
  for (i = 0; i < 80; i++) printf("-");
  printf("\n");
  setColor(7);

  while (fgets(line, sizeof(line), f)) {
    if (strstr(line, "TITRE") != NULL) continue;
    
    if (strstr(line, idAdherant) != NULL && strstr(line, "EMPRUNT") != NULL) {
    if (strstr(line, "TITRE") != NULL)
      continue;
    if (strstr(line, idAdherant) != NULL) {
      char titre[53], dateE[11], dateR[11];
      strncpy(titre, line + 2, 52);
      titre[52] = '\0';
      strncpy(dateE, line + 70, 10);
      dateE[10] = '\0';
      strncpy(dateR, line + 83, 10);
      dateR[10] = '\0';
      printf("  %-52s %-14s %-14s\n", titre, dateE, dateR);
      count++;
    }
  }
    }


  fclose(f);

  if (count == 0) {
    printInfo("Aucun emprunt trouve pour cet adherant");
  } else {
    printf("\n");
    printDivider();
    setColor(10);
    printf("  Total : %d emprunt(s)\n", count);
    setColor(7);
  }

  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
}

// ========== 11. HISTORIQUE RESTITUTIONS PAR ADHERANT ==========
void historiqueRestitutionparAdherant(char *idAdherant) {
  clearScreen();
  printHeader("HISTORIQUE RESTITUTIONS PAR ADHERANT");

  if (idAdherant == NULL || strlen(idAdherant) == 0) {
    printError("ID adherant invalide");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  FILE *f = fopen("historique.txt", "r");
  if (!f) {
    printError("Impossible d'ouvrir le fichier historique");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  char line[256];
  int count = 0;

  printDivider();
  setColor(11);
  printf("  HISTORIQUE DES RESTITUTIONS POUR: %s\n", idAdherant);
  setColor(7);
  printDivider();
  printf("\n");

  setColor(14);
  printf("  %-52s | %-10s | %-10s\n", 
         "TITRE", "DATE EMPR.", "DATE RESTIT.");
  setColor(8);
  printf("  ");
  int i;
  for (i = 0; i < 80; i++) printf("-");
  printf("\n");
  setColor(7);

  while (fgets(line, sizeof(line), f)) {
    if (strstr(line, "TITRE") != NULL) continue;
    
    if (strstr(line, idAdherant) != NULL && strstr(line, "RESTITUE") != NULL) {
      char titre[53], dateE[11], dateR[11];
      strncpy(titre, line + 2, 52);
      titre[52] = '\0';
      strncpy(dateE, line + 70, 10);
      dateE[10] = '\0';
      strncpy(dateR, line + 83, 10);
      dateR[10] = '\0';
      printf("  %-52s %-14s %-14s \n", titre, dateE, dateR);
      count++;
    }
  }
  

  fclose(f);

  if (count == 0) {
    printInfo("Aucune restitution trouvee pour cet adherant");
  } else {
    printf("\n");
    printDivider();
    setColor(10);
    printf("  Total : %d restitution(s)\n", count);
    setColor(7);
  }

  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
}

// ========== 12. HISTORIQUE PAR LIVRE (TOUS) ==========
int historiqueEmpruntparLivre() {
  clearScreen();
  printHeader("HISTORIQUE PAR LIVRE");

  FILE *f = fopen("historique.txt", "r");
  if (!f) {
    printError("Impossible d'ouvrir le fichier historique");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return 0;
  }

   FILE *f1 = fopen("biblioteque.txt", "r");
  if (!f1) exit(0);
  
  setColor(14);
  printf("\n  LIVRES DISPONIBLES:\n");
  setColor(7);
  printDivider();
  
  char ligne[256];
  while (fgets(ligne, sizeof(ligne), f1)) {
    if (strstr(ligne, "ISBN") != NULL) continue;
    char titre[53];
    strncpy(titre, ligne + 26, 52);
    titre[53] = '\0';
    printf("  > %s\n", titre);
  }
  fclose(f1);
  printf("\n");
  
  char titre[53];
  printDivider();
  
  viderBuffer();
  printInputPrompt("Titre du livre : ");
  fgets(titre, sizeof(titre), stdin);
  titre[strcspn(titre, "\n")] = 0;

  printf("\n");
  setColor(11);
  printf("  HISTORIQUE POUR LE LIVRE: %s\n", titre);
  setColor(7);
  printDivider();
  printf("\n");

  char line[256];
  int count = 0;

  setColor(14);
  printf("  %-10s | %-10s | %-10s | %-10s\n", 
         "ID ADHERANT", "DATE EMPR.", "DATE RET.", "ACTION");
  setColor(8);
  printf("  ");
  int i;
  for (i = 0; i < 60; i++) printf("-");
  printf("\n");
  setColor(7);
  while (fgets(line, sizeof(line), f)) {
    if (strstr(line, "TITRE") != NULL)
      continue;
    if (strstr(line, titre) != NULL) {
      count++;
      char id[11], dateE[11], dateR[11],action[11];
      strncpy(id, line + 57, 10);
      id[10] = '\0';
      strncpy(dateE, line + 70, 10);
      dateE[10] = '\0';
      strncpy(dateR, line + 83, 10);
      dateR[10] = '\0';
      strncpy(action,line+95,10);
      action[10]='\0';
      printf("  %-14s %-14s %-14s %-14s\n", id, dateE, dateR,action);
    }
  }

  fclose(f);

  if (count == 0) {
    printInfo("Aucun historique trouve pour ce livre");
  } else {
    printf("\n");
    printDivider();
    setColor(10);
    printf("  Total : %d action(s) pour ce livre\n", count);
    setColor(7);
  }

  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
  return count > 0 ? 1 : 0;
}

// ========== 13. HISTORIQUE EMPRUNTS PAR LIVRE ==========
void historiqueEmpruntparLivreOnly() {
  clearScreen();
  printHeader("HISTORIQUE EMPRUNTS PAR LIVRE");

  FILE *f = fopen("historique.txt", "r");
  if (!f) {
    printError("Impossible d'ouvrir le fichier historique");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

   FILE *f1 = fopen("biblioteque.txt", "r");
  if (!f1) return;
  
  setColor(14);
  printf("\n  LIVRES DISPONIBLES:\n");
  setColor(7);
  printDivider();
  
  char ligne[256];
  while (fgets(ligne, sizeof(ligne), f1)) {
    if (strstr(ligne, "ISBN") != NULL) continue;
    char titre[53];
    strncpy(titre, ligne + 26, 52);
    titre[53] = '\0';
    printf("  > %s\n", titre);
  }
  fclose(f1);
  printf("\n");
  
  char titre[53];
  printDivider();
  
  viderBuffer();
  printInputPrompt("Titre du livre : ");
  fgets(titre, sizeof(titre), stdin);
  titre[strcspn(titre, "\n")] = 0;

  printf("\n");
  setColor(11);
  printf("  HISTORIQUE DES EMPRUNTS POUR: %s\n", titre);
  setColor(7);
  printDivider();
  printf("\n");

  char line[256];
  int count = 0;

  setColor(14);
  printf("  %-10s | %-10s | %-10s\n", 
         "ID ADHERANT", "DATE EMPR.", "DATE RET.");
  setColor(8);
  printf("  ");
  int i;
  for (i = 0; i < 50; i++) printf("-");
  printf("\n");
  setColor(7);

    while (fgets(line, sizeof(line), f)) {
    if (strstr(line, "TITRE") != NULL)
      continue;
    if (strstr(line, titre) != NULL&&strstr(line,"EMPRUNT")) {
      count++;
      char id[11], dateE[11], dateR[11];
      strncpy(id, line + 57, 10);
      id[10] = '\0';
      strncpy(dateE, line + 70, 10);
      dateE[10] = '\0';
      strncpy(dateR, line + 83, 10);
      dateR[10] = '\0';
      printf("  %-14s %-14s %-14s \n", id, dateE, dateR);
    }
  }

  fclose(f);

  if (count == 0) {
    printInfo("Aucun emprunt trouve pour ce livre");
  } else {
    printf("\n");
    printDivider();
    setColor(10);
    printf("  Total : %d emprunt(s) pour ce livre\n", count);
    setColor(7);
  }

  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
}

// ========== 14. HISTORIQUE RESTITUTIONS PAR LIVRE ==========
void historiqueRestitutionparLivre() {
  clearScreen();
  printHeader("HISTORIQUE RESTITUTIONS PAR LIVRE");

  FILE *f = fopen("historique.txt", "r");
  if (!f) {
    printError("Impossible d'ouvrir le fichier historique");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

   FILE *f1 = fopen("biblioteque.txt", "r");
  if (!f1) return;
  
  setColor(14);
  printf("\n  LIVRES DISPONIBLES:\n");
  setColor(7);
  printDivider();
  
  char ligne[256];
  while (fgets(ligne, sizeof(ligne), f1)) {
    if (strstr(ligne, "ISBN") != NULL) continue;
    char titre[53];
    strncpy(titre, ligne + 26, 52);
    titre[53] = '\0';
    printf("  > %s\n", titre);
  }
  fclose(f1);
  printf("\n");

  char titre[53];
  printDivider();
  
  viderBuffer();
  printInputPrompt("Titre du livre : ");
  fgets(titre, sizeof(titre), stdin);
  titre[strcspn(titre, "\n")] = 0;

  printf("\n");
  setColor(11);
  printf("  HISTORIQUE DES RESTITUTIONS POUR: %s\n", titre);
  setColor(7);
  printDivider();
  printf("\n");

  char line[256];
  int count = 0;

  setColor(14);
  printf("  %-10s | %-10s | %-10s\n", 
         "ID ADHERANT", "DATE EMPR.", "DATE RESTIT.");
  setColor(8);
  printf("  ");
  int i;
  for (i = 0; i < 50; i++) printf("-");
  printf("\n");
  setColor(7);

    while (fgets(line, sizeof(line), f)) {
    if (strstr(line, "TITRE") != NULL)
      continue;
    if (strstr(line, titre) != NULL&&strstr(line,"RESTITUE")) {
      count++;
      char id[11], dateE[11], dateR[11],action[11];
      strncpy(id, line + 57, 10);
      id[10] = '\0';
      strncpy(dateE, line + 70, 10);
      dateE[10] = '\0';
      strncpy(dateR, line + 83, 10);
      dateR[10] = '\0';
      printf("  %-14s %-14s %-14s  \n", titre, dateE, dateR);
    }
  }

  fclose(f);

  if (count == 0) {
    printInfo("Aucune restitution trouvee pour ce livre");
  } else {
    printf("\n");
    printDivider();
    setColor(10);
    printf("  Total : %d restitution(s) pour ce livre\n", count);
    setColor(7);
  }

  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
}

// ========== 15. RECHERCHE DANS L'HISTORIQUE ==========
int recherchehistorique() {
  clearScreen();
  printHeader("RECHERCHE DANS L'HISTORIQUE");

  FILE *f = fopen("historique.txt", "r");
  if (!f) {
    printError("Impossible d'ouvrir le fichier historique");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return 0;
  }

  printDivider();
  setColor(11);
  printf("  RECHERCHE PAR:\n");
  setColor(7);
  printf("  1. Titre du livre\n");
  printf("  2. ID Adherant\n");
  printf("  3. Type d'action (EMPRUNT/RESTITUE)\n");
  printf("  4. Date\n");
  printDivider();
  
  int choix;
  printInputPrompt("Votre choix : ");
  scanf("%d", &choix);
  getchar();

  char recherche[100];
  printf("\n");
  printInputPrompt("Terme de recherche : ");
  fgets(recherche, sizeof(recherche), stdin);
  recherche[strcspn(recherche, "\n")] = 0;

  printf("\n");
  setColor(11);
  printf("  RESULTATS DE LA RECHERCHE:\n");
  setColor(7);
  printDivider();
  printf("\n");

  char line[256];
  int count = 0;

  while (fgets(line, sizeof(line), f)) {
    if (strstr(line, "TITRE") != NULL) continue;
    
    if (strstr(line, recherche) != NULL) {
      count++;
      char id[11], dateE[11], dateR[11],titre[53];
      strncpy(id, line + 57, 10);
      id[10]='\0';
      strncpy(titre,line+2,52);
      titre[52] = '\0';
      strncpy(dateE, line + 70, 10);
      dateE[10] = '\0';
      strncpy(dateR, line + 83, 10);
      dateR[10] = '\0';
      printf("  %-54s %-14s %-14s %-14s \n", titre,id, dateE, dateR);
    }
  }
  fclose(f);

  if (count == 0) {
    printInfo("Aucun resultat trouve");
  } else {
    printf("\n");
    printDivider();
    setColor(10);
    printf("  Total : %d resultat(s) trouve(s)\n", count);
    setColor(7);
  }

  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
  return count > 0 ? 1 : 0;
}

// ========== 16. RECHERCHE EMPRUNT DANS L'HISTORIQUE ==========
int rechercheEmprunthistorique() {
    clearScreen();
    printHeader("RECHERCHE EMPRUNT DANS L'HISTORIQUE");

    FILE *f = fopen("historique.txt", "r");
    if (!f) {
        printError("Impossible d'ouvrir le fichier");
        printf("\n  Appuyez sur Entree pour continuer...");
        getchar();
        return 0;
    }

    // Afficher les livres et adhérants disponibles
     FILE *f1 = fopen("biblioteque.txt", "r");
  if (!f1) exit(1);
  
  setColor(14);
  printf("\n  LIVRES DISPONIBLES:\n");
  setColor(7);
  printDivider();
  
  char ligne[256];
  while (fgets(ligne, sizeof(ligne), f1)) {
    if (strstr(ligne, "ISBN") != NULL) continue;
    char titre[53];
    strncpy(titre, ligne + 26, 52);
    titre[53] = '\0';
    printf("  > %s\n", titre);
  }
  fclose(f1);
  printf("\n");
    listerTousLesAdherants();

    char title[60];
    char idadh[11];
    int found = 0;

    printDivider();
    printInputPrompt("Titre du livre : ");
    viderBuffer();
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;
    printf("\n");

    printInputPrompt("ID d'adherant : ");
    scanf("%10s", idadh);

    printf("\n");
    printInfo("Recherche dans l'historique...");
    printf("\n");
    printDivider();

    setColor(11);
    printf("\n  %-14s %-14s\n", "DATE EMPRUNT", "DATE RETOUR");
    setColor(7);
    printDivider();

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "TITRE"))
            continue;
        if (strstr(line, idadh) && strstr(line, title) && strstr(line, "EMPRUNT")) {
            found = 1;
            char dateE[11], dateR[11];
            strncpy(dateE, line + 70, 10);
            dateE[10] = '\0';
            strncpy(dateR, line + 83, 10);
            dateR[10] = '\0';
            printf("  %-14s %-14s\n", dateE, dateR);
        }
    }

    fclose(f);
    printDivider();

    if (!found) {
        printf("\n");
        printError("Aucun emprunt trouve dans l'historique");
    } else {
        printf("\n");
        printSuccess("Recherche terminee");
    }

    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    getchar();
    return found;
}
int rechercheRestitutionhistorique() {
    clearScreen();
    printHeader("RECHERCHE RESTITUTION DANS L'HISTORIQUE");

    FILE *f = fopen("historique.txt", "r");
    if (!f) {
        printError("Impossible d'ouvrir le fichier");
        printf("\n  Appuyez sur Entree pour continuer...");
        getchar();
        return 0;
    }

    // Afficher les livres et adhérants disponibles
    listerTousLesLivres();
    listerTousLesAdherants();

    char title[60];
    char idadh[11];
    int found = 0;

    printDivider();
    printInputPrompt("Titre du livre : ");
    viderBuffer();
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;
    printf("\n");

    printInputPrompt("ID d'adherant : ");
    scanf("%10s", idadh);

    printf("\n");
    printInfo("Recherche dans l'historique...");
    printf("\n");
    printDivider();

    setColor(11);
    printf("\n  %-14s %-14s\n", "DATE EMPRUNT", "DATE RESTITUTION");
    setColor(7);
    printDivider();

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "TITRE"))
            continue;
        if (strstr(line, idadh) && strstr(line, title) && strstr(line, "RESTITUE")) {
            found = 1;
            char dateE[11], dateR[11];
            strncpy(dateE, line + 70, 10);
            dateE[10] = '\0';
            strncpy(dateR, line + 83, 10);
            dateR[10] = '\0';
            printf("  %-14s %-14s\n", dateE, dateR);
        }
    }

    fclose(f);
    printDivider();

    if (!found) {
        printf("\n");
        printError("Aucune restitution trouvee dans l'historique");
    } else {
        printf("\n");
        printSuccess("Recherche terminee");
    }

    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    getchar();
    return found;
}
#endif
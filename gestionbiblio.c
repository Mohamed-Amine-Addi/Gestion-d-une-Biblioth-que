#include "adherant.h"
#include "emprunt.h"
#include "livre.h"
#include "structs.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
int loginRespo();
void printLine(char c, int length);
void printDoubleLine(int length);
void getCurrentTime(char *buffer);
void printBanner();
void printheader(const char *title, int color);
void printMenuItem(int number, const char *text, const char *description);
void printCard(const char *title, const char *content[], int lines, int color);
void waitForKey();
void showNotification(const char *type, const char *message);
void printSectionHeader(const char *title,int color);
void printFooter();
void displayMainMenu();
void displayResponsableMenu();
void displayAdherantAuthMenu();
void displayAdherantMenu();
int main() {
  int choixPrincipal;

  SetConsoleOutputCP(CP_UTF8);
  system("cls");

  do {
    displayMainMenu();
    scanf("%d", &choixPrincipal);
    getchar();
    system("cls");

    /* ============ RESPONSABLE ============ */
    if (choixPrincipal == 1) {
      printheader("AUTHENTIFICATION RESPONSABLE", 11);

      const char *loginInfo[] = {
          "Veuillez entrer vos identifiants de responsable",
          "pour acceder au tableau de bord administratif."};
      printCard("CONNEXION SECURISEE", loginInfo, 2, 11);

      printf("\n");
      
      if (loginRespo()) {
        showNotification(
            "success",
            "Authentification reussie ! Bienvenue dans l'espace responsable.");
        waitForKey();

        int choixRespo;
        char idAdherant[11];
        
        do {
          displayResponsableMenu();
          scanf("%d", &choixRespo);
          getchar();
          system("cls");

          switch (choixRespo) {
          case 1:
            printheader("AJOUTER UN NOUVEAU LIVRE", 10);
            ajouterlivre();
            waitForKey();
            break;
            
          case 2:
            printheader("RECHERCHER UN LIVRE", 10);
            rechercherpartitre();
            waitForKey();
            break;
            
          case 3:
            printheader("CATALOGUE COMPLET DES LIVRES", 10);
            ListeLivres();
            waitForKey();
            break;
            
          case 4:
            printheader("SUPPRIMER UN LIVRE", 10);
            SupprimerLivre();
            waitForKey();
            break;
            
          case 5:
            printheader("VALIDER DEMANDES D'EMPRUNT", 12);
            ValiderDemandesEmprunt();
            waitForKey();
            break;
            
          case 6:
            printheader("EMPRUNTS EN COURS", 12);
            affichageEmprunt();
            waitForKey();
            break;
            
          case 7:
            printheader("EMPRUNTS PAR ADHERENT", 12);
            listerTousLesAdherants();
            viderBuffer();
            printInputPrompt("ID de l'adherant : ");
            fgets(idAdherant, sizeof(idAdherant), stdin);
            idAdherant[strcspn(idAdherant, "\n")] = 0;
            listEmpruntParAdherant(idAdherant);
            waitForKey();
            break;
            
          case 8:
            printheader("EMPRUNTS PAR LIVRE", 12);
            listEmpruntParLivre();
            waitForKey();
            break;
            
          case 9:
            printheader("HISTORIQUE COMPLET", 13);
            Affichagehistorique();
            waitForKey();
            break;
            
          case 10:
            printheader("HISTORIQUE PAR ADHERENT (TOUS)", 13);
            listerTousLesAdherants();
            viderBuffer();
            printInputPrompt("ID de l'adherant : ");
            fgets(idAdherant, sizeof(idAdherant), stdin);
            idAdherant[strcspn(idAdherant, "\n")] = 0;
            historiqueEmpruntparAdherant(idAdherant);
            waitForKey();
            break;
            
          case 11:
            printheader("HISTORIQUE EMPRUNTS PAR ADHERENT", 13);
            listerTousLesAdherants();
            viderBuffer();
            printInputPrompt("ID de l'adherant : ");
            fgets(idAdherant, sizeof(idAdherant), stdin);
            idAdherant[strcspn(idAdherant, "\n")] = 0;
            historiqueEmpruntparAdherantEmpruntsOnly(idAdherant);
            waitForKey();
            break;
            
          case 12:
            printheader("HISTORIQUE RESTITUTIONS PAR ADHERENT", 13);
            listerTousLesAdherants();
            viderBuffer();
            printInputPrompt("ID de l'adherant : ");
            fgets(idAdherant, sizeof(idAdherant), stdin);
            idAdherant[strcspn(idAdherant, "\n")] = 0;
            historiqueRestitutionparAdherant(idAdherant);
            waitForKey();
            break;
          case 13:
           printheader("RECHERCHE DANS L'HISTORIQUE",13);
           recherchehistorique();
           waitForKey();
           break;
          case 14:
            printheader("HISTORIQUE PAR LIVRE (TOUS)", 13);
            historiqueEmpruntparLivre();
            waitForKey();
            break;
            
          case 15:
            printheader("HISTORIQUE EMPRUNTS PAR LIVRE", 13);
            historiqueEmpruntparLivreOnly();
            waitForKey();
            break;
            
          case 16:
            printheader("HISTORIQUE RESTITUTIONS PAR LIVRE", 13);
            historiqueRestitutionparLivre();
            waitForKey();
            break;
            
          case 17:
            printheader("RECHERCHE DANS L'HISTORIQUE", 13);
            recherchehistorique();
            waitForKey();
            break;
            
          case 18:
            printheader("AJOUTER UN ADHERENT", 14);
            ajouterAdh();
            waitForKey();
            break;
            
          case 19:
            printheader("RECHERCHER UN ADHERENT", 14);
            rechercheAdh();
            waitForKey();
            break;
            
          case 20:
            printheader("LISTE DES ADHERENTS", 14);
            ListAdh();
            waitForKey();
            break;
            
          case 21:
            printheader("SUPPRIMER UN ADHERENT", 14);
            suppressionAdh();
            waitForKey();
            break;
            
          case 0:
            showNotification("info", "Deconnexion en cours...");
            Sleep(800);
            system("cls");
            break;
            
          default:
            showNotification(
                "error",
                "Choix invalide ! Veuillez selectionner une option valide.");
            Sleep(2000);
            system("cls");
          }

        } while (choixRespo != 0);
        
      } else {
        showNotification(
            "error", "Echec de l'authentification ! Identifiants incorrects.");
        Sleep(2000);
        system("cls");
      }
    }
    
    /* ============ ADHERANT ============ */
    else if (choixPrincipal == 2) {
      int choixAdh;
      char *idAdherant = NULL;
      
      do {
        displayAdherantAuthMenu();
        scanf("%d", &choixAdh);
        getchar();
        system("cls");

        if (choixAdh == 1) {
          printheader("CONNEXION ADHERENT", 14);
          const char *loginMsg[] = {
              "Entrez vos identifiants pour acceder a votre espace personnel"};
          printCard("AUTHENTIFICATION", loginMsg, 1, 14);
          printf("\n");
          
          idAdherant = loginAdh();
          
          if (idAdherant != NULL) {
            showNotification(
                "success",
                "Connexion reussie ! Bienvenue dans votre espace adherent.");
          } else {
            showNotification("error",
                             "Identifiants incorrects. Veuillez reessayer.");
          }
          waitForKey();
          
        } else if (choixAdh == 2) {
          printheader("INSCRIPTION NOUVEAU MEMBRE", 14);
          const char *inscMsg[] = {
              "Remplissez le formulaire ci-dessous pour creer votre compte adherent"};
          printCard("NOUVELLE INSCRIPTION", inscMsg, 1, 14);
          printf("\n");
          
          idAdherant = inscriptionAdh();
          waitForKey();
        }
        
      } while (idAdherant == NULL && choixAdh != 0);

      if (idAdherant != NULL) {
        int menuAdh;
        
        do {
          displayAdherantMenu();
          scanf("%d", &menuAdh);
          getchar();
          system("cls");

          switch (menuAdh) {
          case 1:
            printheader("EMPRUNTER UN LIVRE", 14);
            EmprunterLivre(idAdherant);
            waitForKey();
            break;
            
          case 2:
            printheader("MES DEMANDES EN ATTENTE", 14);
            AfficherMesDemandesEnAttente(idAdherant);
            waitForKey();
            break;
            
          case 3:
            printheader("RESTITUER UN LIVRE", 14);
            RestituerLivre(idAdherant);
            waitForKey();
            break;
            
          case 4:
            printheader("MES EMPRUNTS EN COURS", 14);
            listEmpruntParAdherant(idAdherant);
            waitForKey();
            break;
            
          case 5:
            printheader("MON HISTORIQUE COMPLET", 14);
            historiqueEmpruntparAdherant(idAdherant);
            waitForKey();
            break;
            
          case 6:
            printheader("MON HISTORIQUE D'EMPRUNTS", 14);
            historiqueEmpruntparAdherantEmpruntsOnly(idAdherant);
            waitForKey();
            break;
            
          case 7:
            printheader("MON HISTORIQUE DE RESTITUTIONS", 14);
            historiqueRestitutionparAdherant(idAdherant);
            waitForKey();
            break;
            
          case 8:
            printheader("RECHERCHER UN LIVRE", 13);
            rechercherpartitre();
            waitForKey();
            break;
            
          case 9:
            printheader("CATALOGUE DE LA BIBLIOTHEQUE", 13);
            ListeLivres();
            waitForKey();
            break;
            
          case 0:
            showNotification("info", "Deconnexion de votre compte...");
            free(idAdherant);
            idAdherant = NULL;
            Sleep(800);
            system("cls");
            break;
            
          default:
            showNotification(
                "error",
                "Choix invalide ! Veuillez selectionner une option valide.");
            Sleep(2000);
            system("cls");
          }
          
        } while (menuAdh != 0);
      }
    }
    
    /* ============ QUITTER ============ */
    else if (choixPrincipal == 0) {
      system("cls");
      printBanner();
      setColor(11);
      printf("\n");
      printf("  +======================================================================"
             "==========+\n");
      printf("  |                                                                      "
             "          |\n");
      printf("  |             ");
      setColor(15);
      printf("Merci d'avoir utilise notre systeme de gestion !");
      setColor(11);
      printf("                        |\n");
      printf("  |                                                                      "
             "          |\n");
      printf("  |                      ");
      setColor(14);
      printf("A bientot a la Bibliotheque ENSAH");
      setColor(11);
      printf("                              |\n");
      printf("  |                                                                      "
             "          |\n");
      printf("  +======================================================================"
             "==========+\n");
      setColor(7);
      printf("\n\n");
      
    } else {
      showNotification("error", "Choix invalide ! Veuillez entrer 1, 2 ou 0.");
      Sleep(2000);
      system("cls");
    }

  } while (choixPrincipal != 0);

  setColor(7);
  return 0;
}
int loginRespo() {
  clearScreen();
  printheader("CONNEXION RESPONSABLE", 11);

  FILE *f = fopen("respo.txt", "r");
  if (!f) {
    printError("Impossible d'ouvrir le fichier");
    printf("\n  Appuyez sur Entree pour retourner au menu...");
    getchar();
    return 0;
  }

  char email[100], pass[50], line[256];
  int found1 = 0, found2 = 0;

  printDivider();
  printInputPrompt("Email : ");
  scanf("%s", email);

  rewind(f);
  while (fgets(line, sizeof(line), f)) {
    if (strstr(line, email) != NULL) {
      found1 = 1;
      break;
    }
  }

  if (!found1) {
    printError("Email non trouve dans le systeme");
    printf("  Appuyez sur Entree pour retourner au menu...");
    getchar();
    getchar();
    fclose(f);
    return 0;
  }

  int attempts = 0;
  do {
    printf("\n");
    printInputPrompt("Mot de passe : ");
    scanf("%s", pass);

    rewind(f);
    found2 = 0;
    while (fgets(line, sizeof(line), f)) {
      if (strstr(line, pass) != NULL) {
        found2 = 1;
        break;
      }
    }

    if (!found2) {
      attempts++;
      printError("Mot de passe incorrect");
      if (attempts < 3) {
        setColor(11);
        printf("  Tentative %d/3\n", attempts);
        setColor(7);
      } else {
        printError("Trop de tentatives echouees");
        fclose(f);
        return 0;
      }
    }
  } while (!found2 && attempts < 3);

  fclose(f);
  return 1;
}
void printLine(char c, int length) {
  int i;
  for (i = 0; i < length; i++)
    printf("%c", c);
  printf("\n");
}

void printDoubleLine(int length) {
  int i;
  for (i = 0; i < length; i++)
    printf("=");
  printf("\n");
}

void getCurrentTime(char *buffer) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(buffer, 50, "%d/%m/%Y %H:%M", t);
}

void printBanner() {
  setColor(11);
  printf("\n");
  printDoubleLine(80);
  printf("||");
  setColor(15);
  printf("                                                                     "
         "         ");
  setColor(11);
  printf("||\n");

  printf("||");
  setColor(15);
  printf("                        +===========================+                "
         "        ");
  setColor(11);
  printf("||\n");

  printf("||");
  setColor(15);
  printf("                        |  BIBLIOTHEQUE DE L'ENSAH |                 "
         "       ");
  setColor(11);
  printf("||\n");

  printf("||");
  setColor(15);
  printf("                        |   Systeme de Gestion     |                 "
         "       ");
  setColor(11);
  printf("||\n");

  printf("||");
  setColor(15);
  printf("                        +===========================+                "
         "        ");
  setColor(11);
  printf("||\n");

  printf("||");
  setColor(15);
  printf("                                                                     "
         "         ");
  setColor(11);
  printf("||\n");
  printDoubleLine(80);
  setColor(7);
}

void printMenuItem(int number, const char *text, const char *description) {
  setColor(14);
  printf("    > ");
  setColor(15);
  printf("[%2d] ", number);
  setColor(11);
  printf("%-35s", text);
  setColor(8);
  if (description != NULL) {
    printf(" - %s", description);
  }
  setColor(7);
  printf("\n");
}

void printSimpleMenuItem(int number, const char *text) {
  setColor(14);
  printf("    > ");
  setColor(15);
  printf("[%2d] ", number);
  setColor(11);
  printf("%s", text);
  setColor(7);
  printf("\n");
}

void printCard(const char *title, const char *content[], int lines, int color) {
  setColor(color);
  printf("\n  +");
  int i;
  for (i = 0; i < 76; i++)
    printf("=");
  printf("+\n");

  printf("  |  ");
  setColor(15);
  printf("%-72s", title);
  setColor(color);
  printf("|\n");

  printf("  +");
  for (i = 0; i < 76; i++)
    printf("=");
  printf("+\n");

  for (i = 0; i < lines; i++) {
    printf("  |  ");
    setColor(7);
    printf("%-72s", content[i]);
    setColor(color);
    printf("|\n");
  }

  printf("  +");
  for (i = 0; i < 76; i++)
    printf("=");
  printf("+\n");
  setColor(7);
}

void waitForKey() {
  printf("\n");
  setColor(0);
  printf("                                                                     "
         "           ");
  printf("\r");
  setColor(8);
  printf("  "
         "+--------------------------------------------------------------------"
         "------+\n");
  printf("  |          ");
  setColor(11);
  printf("Appuyez sur n'importe quelle touche pour continuer...");
  setColor(8);
  printf("                 |\n");
  printf("  "
         "+--------------------------------------------------------------------"
         "------+");
  setColor(7);
  getch();
  system("cls");
}

void showNotification(const char *type, const char *message) {
  int i;
  if (strcmp(type, "success") == 0) {
    setColor(10);
    printf("\n  +");
    for (i = 0; i < 76; i++)
      printf("=");
    printf("+\n");
    printf("  |  [SUCCES] ");
    setColor(15);
    printf("%-63s", message);
    setColor(10);
    printf("|\n");
    printf("  +");
    for (i = 0; i < 76; i++)
      printf("=");
    printf("+\n");
  } else if (strcmp(type, "error") == 0) {
    setColor(12);
    printf("\n  +");
    for (i = 0; i < 76; i++)
      printf("=");
    printf("+\n");
    printf("  |  [ERREUR] ");
    setColor(15);
    printf("%-63s", message);
    setColor(12);
    printf("|\n");
    printf("  +");
    for (i = 0; i < 76; i++)
      printf("=");
    printf("+\n");
  } else if (strcmp(type, "info") == 0) {
    setColor(14);
    printf("\n  +");
    for (i = 0; i < 76; i++)
      printf("=");
    printf("+\n");
    printf("  |  [INFO] ");
    setColor(15);
    printf("%-65s", message);
    setColor(14);
    printf("|\n");
    printf("  +");
    for (i = 0; i < 76; i++)
      printf("=");
    printf("+\n");
  }
  setColor(7);
}

void printFooter() {
  setColor(8);
  printf("\n");
  printLine('-', 80);
  printf("  � 2025 Bibliotheque de l'ENSAH | Systeme de Gestion Integre\n");
  printLine('-', 80);
  setColor(7);
}

/* ===================== MENU DISPLAYS ===================== */

void displayMainMenu() {
  printBanner();

  setColor(11);
  printf("\n");
  printf("  "
         "+===================================================================="
         "========+\n");
  printf("  |                          PORTAIL D'ACCES PRINCIPAL               "
         "          |\n");
  printf("  "
         "+===================================================================="
         "========+\n");
  setColor(7);

  printSectionHeader("SELECTIONNEZ VOTRE ESPACE", 14);

  printf("\n");
  printSimpleMenuItem(
      1, "Espace Responsable - Gestion complete de la bibliotheque");
  printf("\n");
  printSimpleMenuItem(2, "Espace Adherent - Consultation et emprunts");
  printf("\n");
  setColor(8);
  printf("    "
         "---------------------------------------------------------------------"
         "----\n");
  setColor(7);
  printf("\n");
  printSimpleMenuItem(0, "Quitter l'application");

  printf("\n");
  setColor(11);
  printf("  "
         "+===================================================================="
         "========+\n");
  printf("  |  ");
  setColor(15);
  printf("Entrez votre choix : ");
  setColor(11);
  printf("                                                      |\n");
  printf("  "
         "+===================================================================="
         "========+\n");
  printf("  >> ");
  setColor(7);
}

void displayResponsableMenu() {
  printheader("ESPACE RESPONSABLE - TABLEAU DE BORD", 10);

  printSectionHeader("GESTION DES LIVRES", 10);
  printMenuItem(1, "Ajouter un nouveau livre", "Enrichir le catalogue");
  printMenuItem(2, "Rechercher un livre", "Par titre, auteur ou ISBN");
  printMenuItem(3, "Catalogue complet", "Voir tous les livres disponibles");
  printMenuItem(4, "Supprimer un livre", "Retirer du catalogue");

  printSectionHeader("GESTION DES EMPRUNTS", 12);
  printMenuItem(5, "Valider demandes d'emprunt", "Approuver les demandes adherents");
  printMenuItem(6, "Emprunts en cours", "Vue d'ensemble des emprunts actifs");
  printMenuItem(7, "Emprunts par adherent", "Filtrer par membre");
  printMenuItem(8, "Emprunts par livre", "Historique d'un livre specifique");

  printSectionHeader("HISTORIQUE & STATISTIQUES", 13);
  printMenuItem(9, "Historique complet", "Tous les emprunts et restitutions");
  printMenuItem(10, "Historique par adherent", "Toutes les actions d'un membre");
  printMenuItem(11, "Emprunts par adherent", "Uniquement les emprunts");
  printMenuItem(12, "Restitutions par adherent", "Uniquement les restitutions");
  printMenuItem(13, "recherche historique", "par adherant/par livre");
  printMenuItem(14, "Historique par livre", "Toutes les actions sur un livre");
  printMenuItem(15, "Emprunts par livre", "Uniquement les emprunts");
  printMenuItem(16, "Restitutions par livre", "Uniquement les restitutions");
  printMenuItem(17, "Recherche dans historique", "Recherche avancee");

  printSectionHeader("GESTION DES ADHERENTS", 14);
  printMenuItem(18, "Ajouter un adherent", "Nouvelle inscription");
  printMenuItem(19, "Rechercher un adherent", "Par nom, email ou ID");
  printMenuItem(20, "Liste des adherents", "Repertoire complet");
  printMenuItem(21, "Supprimer un adherent", "Desactivation de compte");

  printf("\n");
  setColor(8);
  printf("  ======================================================================"
         "========\n");
  setColor(7);
  printSimpleMenuItem(0, "Deconnexion et retour au menu principal");

  printFooter();
  printf("\n  >> ");
}


void displayAdherantAuthMenu() {
  printheader("ESPACE ADHERENT - AUTHENTIFICATION", 14);

  const char *welcomeContent[] = {
      "Bienvenue sur le portail adherent de la bibliotheque ENSAH !",
      "",
      "Connectez-vous pour acceder a :",
      "  - Emprunter et restituer des livres",
      "  - Consulter votre historique d'emprunts",
      "  - Explorer notre catalogue complet",
      "  - Gerer vos demandes d'emprunt"};
  printCard("BIENVENUE", welcomeContent, 7, 14);

  printf("\n");
  printSimpleMenuItem(1, "Se connecter - J'ai deja un compte");
  printf("\n");
  printSimpleMenuItem(2, "S'inscrire - Creer un nouveau compte");
  printf("\n");
  setColor(8);
  printf("  ----------------------------------------------------------------------"
         "---------\n");
  setColor(7);
  printf("\n");
  printSimpleMenuItem(0, "Retour au menu principal");

  printFooter();
  printf("\n  >> ");
}

// ========== MENU ADHERANT ==========
void displayAdherantMenu() {
  printheader("ESPACE ADHERENT - MON COMPTE", 14);

  printSectionHeader("MES EMPRUNTS", 11);
  printMenuItem(1, "Emprunter un livre", "Faire une demande d'emprunt");
  printMenuItem(2, "Mes demandes en attente", "Voir le statut de mes demandes");
  printMenuItem(3, "Restituer un livre", "Retourner un livre emprunte");
  printMenuItem(4, "Mes emprunts en cours", "Voir mes prets actifs");

  printSectionHeader("MON HISTORIQUE", 13);
  printMenuItem(5, "Historique complet", "Tous mes emprunts et restitutions");
  printMenuItem(6, "Mes emprunts", "Uniquement mes emprunts");
  printMenuItem(7, "Mes restitutions", "Uniquement mes restitutions");

  printSectionHeader("EXPLORER LE CATALOGUE", 12);
  printMenuItem(8, "Rechercher un livre", "Par titre, auteur, genre...");
  printMenuItem(9, "Parcourir le catalogue", "Tous les livres disponibles");

  printf("\n");
  setColor(8);
  printf("  ======================================================================"
         "========\n");
  setColor(7);
  printSimpleMenuItem(0, "Se deconnecter");

  printFooter();
  printf("\n  >> ");
}
  void printheader(const char *title, int color) {
  char timeStr[50];
  getCurrentTime(timeStr);

  setColor(color);
  printf("\n");
  printDoubleLine(80);
  printf("|| ");
  setColor(15);
  printf("%-60s", title);
  setColor(8);
  printf("%16s ", timeStr);
  setColor(color);
  printf("||\n");
  printDoubleLine(80);
  setColor(7);
  printf("\n");
}

void printSectionHeader(const char *title, int color) {
  setColor(color);
  printf("\n  +");
  int i;
  for (i = 0; i < 76; i++) {
    printf("-");
  }
  printf("+\n");
  
  printf("  |  ");
  setColor(15);
  printf("%-72s", title);
  setColor(color);
  printf("  |\n");
  
  printf("  +");
  for (i = 0; i < 76; i++) {
    printf("-");
  }
  printf("+\n");
  setColor(7);
}


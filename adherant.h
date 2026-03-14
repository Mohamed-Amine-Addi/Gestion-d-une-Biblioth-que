#include "structs.h"
#include "styleOutput.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef adherant_h
#define adherant_h

char *IdAdh(char *Id);
char *loginAdh();
char *inscriptionAdh();
void rechercheAdh();
void suppressionAdh();
void ListAdh();
void ajouterAdh();
void viderBuffer();
char *my_strdup(const char *str);


// Generates a random ID (8 digits + 2 letters)
char *IdAdh(char *Id) {
  const char *nums = "0123456789";
  const char *letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char id1[11];
  int k = 0;

  srand(time(NULL));

  // 8 random digits
  int i;
  for (i = 0; i < 8; i++)
    id1[k++] = nums[rand() % 10];

  // 2 random letters
  for (i = 0; i < 2; i++)
    id1[k++] = letters[rand() % 26];

  // shuffle the array
  for (i = 0; i < 10; i++) {
    int j = rand() % 10;
    char tmp = id1[i];
    id1[i] = id1[j];
    id1[j] = tmp;
  }

  id1[10] = '\0';
  strcpy(Id, id1);
  return Id;
}

// Helper function to replace strdup with better error checking
char *my_strdup(const char *str) {
  if (str == NULL) {
    printf("ERROR: my_strdup received NULL pointer\n");
    return NULL;
  }
  
  size_t len = strlen(str);
  if(len==0) return NULL;
  len++; // +1 for null terminator
  char *copy = (char *)malloc(len * sizeof(char));
  
  strcpy(copy, str);
  return copy;
}

char *loginAdh() {
  clearScreen();
  printHeader("CONNEXION ADHERENT");

  FILE *fadh = fopen("adherantlist.txt", "rt");
  if (fadh == NULL) {
    printError("Erreur lors de l'ouverture du fichier adherents");
    return NULL;
  }

  FILE *fpass = fopen("passwords.txt", "rt");
  if (fpass == NULL) {
    printError("Erreur lors de l'ouverture du fichier mots de passe");
    fclose(fadh);
    return NULL;
  }

  char pass[11], id[11], line[256];
  int result1 = 0, result2 = 0;
  
  // Clear the arrays
  memset(id, 0, sizeof(id));
  memset(pass, 0, sizeof(pass));

  printDivider();
  printInputPrompt("Identifiant : ");
  
  // Read ID with size limit
  if (scanf("%10s", id) != 1) {
    printError("Erreur de lecture de l'identifiant");
    fclose(fadh);
    fclose(fpass);
    return NULL;
  }
  
  // Clear input buffer
  int c;
  while ((c = getchar()) != '\n' && c != EOF);


  rewind(fadh);
  while (fgets(line, sizeof(line), fadh)) {
    if (strstr(line, id) != NULL) {
      result1 = 1;
      break;
    }
  }

  if (result1 == 0) {
    printError("Vous n'etes pas membre");
    printInfo("Veuillez vous inscrire d'abord");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    fclose(fpass);
    fclose(fadh);
    return NULL;
  }

  printf("\n");
  printInputPrompt("Mot de passe : ");
  
  if (scanf("%10s", pass) != 1) {
    printError("Erreur de lecture du mot de passe");
    fclose(fadh);
    fclose(fpass);
    return NULL;
  }
  
  // Clear input buffer
  while ((c = getchar()) != '\n' && c != EOF);

  rewind(fpass);
  while (fgets(line, sizeof(line), fpass)) {
    if (strstr(line, pass) != NULL && strstr(line, id) != NULL) {
      result2 = 1;
      break;
    }
  }

  if (result1 && result2) {
    printSuccess("Connexion reussie! Bienvenue");
  } else if (!result2) {
    printError("Mot de passe incorrect");
    printf("\n");
    printInputPrompt("Reessayez : ");
    
    if (scanf("%10s", pass) != 1) {
      printError("Erreur de lecture");
      fclose(fadh);
      fclose(fpass);
      return NULL;
    }
    
    while ((c = getchar()) != '\n' && c != EOF);

    rewind(fpass);
    while (fgets(line, sizeof(line), fpass)) {
      if (strstr(line, pass) != NULL && strstr(line, id) != NULL) {
        printSuccess("Connexion reussie!");
        result2 = 1;
        break;
      }
    }
    if (!result2) {
      printError("Echec de connexion");
      fclose(fadh);
      fclose(fpass);
      return NULL;
    }
  }

  fclose(fadh);
  fclose(fpass);

  char *result = my_strdup(id);
  return result;
}

char *inscriptionAdh() {
  clearScreen();
  printHeader("INSCRIPTION ADHERENT");

  adherant ad;
  FILE *fadh = fopen("adherantlist.txt", "at");
  FILE *fpass = fopen("passwords.txt", "at");

  if (!fadh || !fpass) {
    printError("Erreur lors de l'ouverture des fichiers");
    if (fadh) fclose(fadh);
    if (fpass) fclose(fpass);
    return NULL;
  }

  printDivider();
  setColor(11);
  printf("  Veuillez remplir vos informations\n");
  setColor(7);
  printDivider();
  printf("\n");

  printInputPrompt("CNE : ");
  scanf("%s", ad.CNE);
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
  printf("\n");

  printInputPrompt("Nom : ");
  scanf("%s", ad.nom);
  while ((c = getchar()) != '\n' && c != EOF);
  printf("\n");

  printInputPrompt("Prenom : ");
  scanf("%s", ad.prenom);
  while ((c = getchar()) != '\n' && c != EOF);
  printf("\n");

  printInputPrompt("Ville : ");
  scanf("%s", ad.ville);
  while ((c = getchar()) != '\n' && c != EOF);
  printf("\n");

  printInputPrompt("Telephone : ");
  scanf("%s", ad.num);
  while ((c = getchar()) != '\n' && c != EOF);

  // Generate unique ID
  char ID[11], line[256];
  int unique = 0;
  memset(ID, 0, sizeof(ID));
  
  printf("\n\n");
  setColor(14);
  printf("  Generation de votre identifiant unique...\n");
  setColor(7);

  FILE *fcheck = fopen("adherantlist.txt", "r");
  if (fcheck) {
    while (!unique) {
      IdAdh(ID);
      unique = 1;
      rewind(fcheck);
      while (fgets(line, sizeof(line), fcheck)) {
        if (strstr(line, ID) != NULL) {
          unique = 0;
          break;
        }
      }
    }
    fclose(fcheck);
  } else {
    // First user
    IdAdh(ID);
  }

  strcpy(ad.id, ID);


  // Get current date
  time_t now = time(NULL);
  struct tm *T = localtime(&now);
  ad.t.annee = T->tm_year + 1900;
  ad.t.mois = T->tm_mon + 1;
  ad.t.jour = T->tm_mday;

  fprintf(fadh, "| %s | %s | %-12s | %-12s | %-12s | %s | %d-%02d-%02d |\n",
          ad.id, ad.CNE, ad.nom, ad.prenom, ad.ville, ad.num, ad.t.annee,
          ad.t.mois, ad.t.jour);
  fflush(fadh);

  printf("\n");
  printInputPrompt("Mot de passe (max 10 caracteres) : ");
  scanf("%10s", ad.password);
  while ((c = getchar()) != '\n' && c != EOF);

  fprintf(fpass, "| %s | %s |\n", ad.id, ad.password);
  fflush(fpass);

  fclose(fadh);
  fclose(fpass);

  printf("\n");
  printDivider();
  printSuccess("Inscription reussie!");
  setColor(10);
  printf("  Votre identifiant : %s\n", ad.id);
  setColor(7);
  printf("  Date d'inscription : %02d/%02d/%d\n", ad.t.jour, ad.t.mois,
         ad.t.annee);
  printDivider();

  printf("  Appuyez sur une touche pour acceder a votre espace...");
  getchar();
  
  char *result = my_strdup(ad.id);
  return result;
}
void ListAdh() {
  clearScreen();
  printHeader("LISTE DES ADHERANTS");

  FILE *fadh = fopen("adherantlist.txt", "r");
  if (fadh == NULL) {
    printError("Impossible d'ouvrir le fichier");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  printDivider();
  char line[256];
  int count = 0;

  while (fgets(line, sizeof(line), fadh) != NULL) {
    printf("  %s", line);
    count++;
  }

  fclose(fadh);

  printf("\n");
  printDivider();
  setColor(11);
  printf("  Total : %d adherant(s)\n", count);
  setColor(7);
  printDivider();
  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
}
void rechercheAdh() {
  clearScreen();
  printHeader("RECHERCHE D'ADHERANT");

  char line[256];
  char recherche[20];

  FILE *fadh = fopen("adherantlist.txt", "r");
  if (fadh == NULL) {
    printError("Impossible d'ouvrir le fichier");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  printDivider();
  printInputPrompt("Information du membre (nom/prenom/ID/CNE) : ");
  scanf("%s", recherche);

  printf("\n");
  printInfo("Recherche en cours...");
  printf("\n");
  printDivider();

  int found = 0;
  while (fgets(line, sizeof(line), fadh)) {
    if (strstr(line, recherche) != NULL) {
      printSuccess("Membre trouve!");
      setColor(10);
      printf("\n  %s\n", line);
      setColor(7);
      found = 1;
      break;
    }
  }

  fclose(fadh);

  if (!found) {
    printError("Aucun membre trouve avec cette information");
  }

  printDivider();
  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
  getchar();
}
void suppressionAdh() {
  clearScreen();
  printHeader("SUPPRESSION D'ADHERANT");

  char line[256];
  FILE *fadh = fopen("adherantlist.txt", "r");
  FILE *ftmp = fopen("tmp.txt", "w");
  FILE *fpass = fopen("passwords.txt", "r");
  FILE *ftmpp = fopen("tempp.txt", "w");

  if (!fadh || !ftmp || !ftmpp || !fpass) {
    printError("Erreur lors de l'ouverture des fichiers");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  char supp[50];
  int found1 = 0, found2 = 0;

  printDivider();
  setColor(12);
  printf("  [ATTENTION] Cette action est irreversible!\n");
  setColor(7);
  printDivider();
  printf("\n");

  printInputPrompt("Information du membre a supprimer : ");
  scanf("%s", supp);

  printf("\n");
  printInfo("Traitement en cours...");

  while (fgets(line, sizeof(line), fadh)) {
    if (strstr(line, supp)) {
      found1 = 1;
      continue;
    }
    fputs(line, ftmp);
  }

  while (fgets(line, sizeof(line), fpass)) {
    if (strstr(line, supp)) {
      found2 = 1;
      continue;
    }
    fputs(line, ftmpp);
  }

  fclose(fadh);
  fclose(ftmp);
  fclose(fpass);
  fclose(ftmpp);

  remove("adherantlist.txt");
  rename("tmp.txt", "adherantlist.txt");
  remove("passwords.txt");
  rename("tempp.txt", "passwords.txt");

  printf("\n");
  printDivider();

  if (found1 && found2) {
    printSuccess("Suppression reussie!");
  } else {
    printError("Aucun membre trouve avec cette information");
  }

  printDivider();
  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
  getchar();
}
void ajouterAdh() {
  clearScreen();
  printHeader("AJOUT D'ADHERANT");

  adherant ad;
  FILE *f = fopen("adherantlist.txt", "at");
  FILE *F = fopen("passwords.txt", "at");

  if (f == NULL || F == NULL) {
    printError("Impossible d'ouvrir le fichier");
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
    return;
  }

  printDivider();
  printSectionTitle("Informations de l'adherant");

  printInputPrompt("ID : ");
  scanf("%s", ad.id);
  printf("\n");

  printInputPrompt("CNE : ");
  scanf("%s", ad.CNE);
  printf("\n");

  printInputPrompt("Nom : ");
  scanf("%s", ad.nom);
  printf("\n");

  printInputPrompt("Prenom : ");
  scanf("%s", ad.prenom);
  printf("\n");

  printInputPrompt("Ville : ");
  scanf("%s", ad.ville);
  printf("\n");

  printInputPrompt("Telephone : ");
  scanf("%s", ad.num);
  printf("\n");

  time_t now = time(NULL);
  struct tm *T = localtime(&now);
  ad.t.annee = T->tm_year + 1900;
  ad.t.mois = T->tm_mon + 1;
  ad.t.jour = T->tm_mday;

  printInputPrompt("Mot de passe : ");
  scanf("%s", ad.password);

  fprintf(f, "| %s | %s | %-12s | %-12s | %-12s | %s | %d-%02d-%02d |\n", ad.id,
          ad.CNE, ad.nom, ad.prenom, ad.ville, ad.num, ad.t.annee, ad.t.mois,
          ad.t.jour);
  fprintf(F, "| %s | %s |\n", ad.CNE, ad.password);
printSectionTitle("Date d'inscription");
printf("%-2d/%-2d/%d\n",ad.t.jour,ad.t.mois,ad.t.annee);
  fclose(f);
  fclose(F);

  printf("\n");
  printDivider();
  printSuccess("Adherant ajoute avec succes!");
  setColor(10);
  printf("  ID : %s\n", ad.id);
  printf("  Nom complet : %s %s\n", ad.nom, ad.prenom);
  setColor(7);
  printDivider();
  printf("\n  Appuyez sur Entree pour continuer...");
  getchar();
  getchar();
}

#endif

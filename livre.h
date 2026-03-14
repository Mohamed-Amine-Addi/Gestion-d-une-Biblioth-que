#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include"styleOutput.h"
#include "emprunt.h"
#ifndef LIVRE_H
#define LIVRE_H
void ajouterlivre();
void rechercherpartitre();
void ListeLivres();
void SupprimerLivre();
void ajouterlivre() {
    clearScreen();
    printHeader("AJOUTER UN LIVRE");
    
    livre L;
    int tr = 0;
    char idL[50];
    
    FILE *f1 = fopen("biblioteque.txt", "a+");
    if (!f1) {
        printError("Impossible d'ouvrir le fichier");
        printf("\n  Appuyez sur Entree pour continuer...");
        getchar();
        return;
    }
    
    printDivider();
    printSectionTitle("Informations du livre");
    
    printInputPrompt("ISBN du livre : ");
    scanf("%s", idL);
    
    printf("\n");
    printInfo("Verification de l'ISBN...");
    
    rewind(f1);
    char line[256];
    while (fgets(line, sizeof(line), f1)) {
        if (strstr(line, idL)) {
            tr = 1;
            break;
        }
    }
    
    if (tr) {
        printf("\n");
        printError("Ce livre existe deja dans la bibliotheque");
        fclose(f1);
        printDivider();
        getchar();
        getchar();
        return;
    }
    
    strcpy(L.id, idL);
    
    printf("\n");
    printInputPrompt("Titre du livre : ");
    getchar();
    fgets(L.titre, 50, stdin);
    L.titre[strcspn(L.titre, "\n")] = 0;
    printf("\n");
    
    printInputPrompt("Auteur : ");
    fgets(L.Auteur, 50, stdin);
    L.Auteur[strcspn(L.Auteur, "\n")] = 0;
    printf("\n");
    
    printInputPrompt("Annee de publication : ");
    scanf("%d", &L.ann_pub);
    printf("\n");
    
    printInputPrompt("Edition : ");
    scanf("%d", &L.edition);
    printf("\n");
    
    printInputPrompt("Nombre d'exemplaires : ");
    scanf("%d", &L.stock);
    
    fprintf(f1, "| %-21s | %-52s | %-32s | %d | %-10d | %-10d |\n",
            L.id, L.titre, L.Auteur, L.ann_pub, L.edition, L.stock);
    
    fclose(f1);
    
    printf("\n");
    printDivider();
    printSuccess("Livre ajoute avec succes!");
    setColor(10);
    printf("  ISBN : %s\n", L.id);
    printf("  Titre : %s\n", L.titre);
    printf("  Auteur : %s\n", L.Auteur);
    printf("  Stock : %d exemplaire(s)\n", L.stock);
    setColor(7);
    printDivider();
    getchar();
    getchar();
}
void rechercherpartitre() {
    clearScreen();
    printHeader("RECHERCHER UN LIVRE");
    
    FILE *f1 = fopen("biblioteque.txt", "r");
    if (!f1) {
        printError("Impossible d'ouvrir le fichier");
        printf("\n  Appuyez sur Entree pour continuer...");
        getchar();
        return;
    }
    
    char titre2[53];
    int trouver = 0;
    
    printDivider();
    printInputPrompt("Titre du livre a rechercher : ");
    
    // NE PAS appeler viderBuffer() ICI car vous utilisez fgets() après
    // viderBuffer(); // <-- RETIREZ CETTE LIGNE
    
    fgets(titre2, sizeof(titre2), stdin);
    titre2[strcspn(titre2, "\n")] = 0;
    
    // AJOUTEZ CETTE VERIFICATION
    if (strlen(titre2) == 0) {
        printError("Veuillez entrer un titre valide");
        fclose(f1);
        printf("\n  Appuyez sur Entree pour continuer...");
        getchar();
        return;
    }
    
    
    printf("\n");
    printInfo("Recherche en cours...");
    printf("\n");
    printDivider();
    
    char line[256];
    while (fgets(line, sizeof(line), f1)) {
        if (strstr(line, titre2) != NULL) {
            printf("%s", line);
            trouver = 1;
        }
    }
    
    printf("\n");
    printDivider();
    
    if (!trouver) {
        printError("Aucun livre trouve avec ce titre");
    } else {
        printSuccess("Recherche terminee");
    }
    
    fclose(f1);
    
    printDivider();
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
}

void ListeLivres() {
    
    FILE *f1 = fopen("biblioteque.txt", "r");
    if (!f1) {
        printError("Impossible d'ouvrir le fichier");
        printf("\n  Appuyez sur Entree pour continuer...");
        getchar();
        return;
    }
    clearScreen();
    printHeader("LISTE DES LIVRES");
    printDivider();
    char line[256];
    int count = 0;
    
    while (fgets(line, sizeof(line), f1) != NULL) {
        puts(line);
        if (strstr(line, "ISBN") == NULL && strlen(line) > 5)
            count++;
    }
    
    fclose(f1);
    
    printf("\n");
    printDivider();
    setColor(11);
    printf("  Total : %d livre(s) disponible(s)\n", count);
    setColor(7);
    printDivider();
    getchar();
}

void SupprimerLivre() {
    clearScreen();
    printHeader("SUPPRIMER UN LIVRE");
    
    // PREMIER: Afficher les livres disponibles
    FILE *f1 = fopen("biblioteque.txt", "r");
    if (!f1) {
        printError("Erreur d'ouverture de fichier");
        printf("\n  Appuyez sur Entree pour continuer...");
        getchar();
        return;
    }
    
    setColor(14);
    printf("\n  LIVRES DISPONIBLES:\n");
    setColor(7);
    printDivider();
    
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f1)) {
        if (strstr(ligne, "ISBN") != NULL) continue;
        char titre[53];
        strncpy(titre, ligne + 26, 52);
        titre[52] = '\0';  // <- Correction: index 52, pas 53
        printf("  > %s\n", titre);
    }
    fclose(f1);  // FERMER ici
    printf("\n");
    
    // DEMANDER le titre à supprimer
    char title1[53];
    int trouve = 0;
    
    printDivider();
    setColor(12);
    printf("  [ATTENTION] Cette action est irreversible!\n");
    setColor(7);
    printDivider();
    printf("\n");
    
    printInputPrompt("Titre du livre a supprimer : ");
    viderBuffer();
    fgets(title1, sizeof(title1), stdin);
    title1[strcspn(title1, "\n")] = 0;
    
    printf("\n");
    printInfo("Traitement en cours...");
    
    // ROUVRIR les fichiers pour la suppression
    f1 = fopen("biblioteque.txt", "r");  // <- ROUVRIR ici
    FILE *f2 = fopen("temp.txt", "w");
    
    if (!f1 || !f2) {
        printError("Erreur d'ouverture de fichier");
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        printf("\n  Appuyez sur Entree pour continuer...");
        getchar();
        return;
    }
    
    // COPIER toutes les lignes sauf celle à supprimer
    char line[256];
    while (fgets(line, sizeof(line), f1)) {
        if (strstr(line, title1) == NULL) {  // <- Si PAS trouvé, garder la ligne
            fputs(line, f2);
        } else {
            trouve = 1;  // Ligne trouvée, on la saute
        }
    }
    
    fclose(f1);
    fclose(f2);
    
    // REMPLACER le fichier
    remove("biblioteque.txt");
    rename("temp.txt", "biblioteque.txt");
    
    printf("\n");
    printDivider();
    
    if (trouve) {
        printSuccess("Livre supprime avec succes!");
        setColor(10);
        printf("  Livre : %s\n", title1);
        setColor(7);
    } else {
        printError("Livre non trouve");
    }
    
    printDivider();
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
}
#endif

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#ifndef STRUCTS_H
#define STRUCTS_H
typedef struct {
        int jour;
        int mois;
        int annee;
}date;
typedef struct{
    char id[11];//identifiant d'adherant dans la biblioteque
    char CNE[11];//numero de massar d'adherant
    char nom[50];//nom d'adherant
    char prenom[50];//prenom d'adherant
    char ville[20];//adresse d'adherant
    char num[14];//numero du telephone
    char password[11];//mot de passe d'adherant
    date t;//date d'inscription d'adherant
}adherant;
typedef struct {
    char id[60];//ISBN du livre
    char titre[50];//titre du livre
    int ann_pub;//annee publication du livre
    int edition;//numero d'edition
    int dispo; // 1 = disponible, 0 = emprunté
    char Auteur[50];//nom d'auteur
    int stock;//nombre de livres dans le stock du biblioteque
}livre;
typedef struct {
	char titrelivre[50];//titre du livre a emprunter
	char  idAdherant[11];// id d'adherant
	date dateEmprunt;//date emprunt livre
	date dateRetour;//date retour livre
}Emprunt;
typedef struct {
    char nom[50];//nom du responsable
    char prenom[50];//prenom du responsable
    char email[50];//email du responsable
    char mdpass[20];// mot de pass
}responsable;
//pour coloration
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
//pour vider buffer
void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
#endif
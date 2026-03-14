#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include "structs.h"
#ifndef style_h
#define style_h
void clearScreen() {
    system("cls");
}

void printHeader(const char* title) {
    printf("\n");
    printf("===============================================================\n");
    printf("                                                               \n");
    printf("                  %-40s\n", title);
    printf("                                                               \n");
    printf("===============================================================\n");
    printf("\n");
}

void printBox(const char* message) {
    int len = strlen(message),i;
    printf("+");
    for( i = 0; i < len + 2; i++) printf("-");
    printf("+\n");
    printf("| %s |\n", message);
    printf("+");
    for(i = 0; i < len + 2; i++) printf("-");
    printf("+\n");
}

void printInputPrompt(const char* prompt) {
    setColor(14); // Yellow
    printf("  > %s", prompt);
    setColor(7);  // White
}

void printSuccess(const char* message) {
    printf("\n");
    setColor(10); // Green
    printf("  [SUCCESS] %s\n", message);
    setColor(7);
    printf("\n");
}

void printError(const char* message) {
    printf("\n");
    setColor(12); // Red
    printf("  [ERREUR] %s\n", message);
    setColor(7);
    printf("\n");
}

void printInfo(const char* message) {
    setColor(11); // Cyan
    printf("  [INFO] %s\n", message);
    setColor(7);
}

void printDivider() {
    printf("  -----------------------------------------------------------\n");
}
void printSectionTitle(const char* title) {
    printf("\n");
    setColor(11);
    printf("  [ %s ]\n", title);
    setColor(7);
    printf("\n");
}
#endif

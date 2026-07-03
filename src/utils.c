#include "utils.h"

void trim(char *str) {
    int i = 0;
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') i++;
    int j = 0;
    while (str[i]) str[j++] = str[i++];
    str[j] = '\0';
    i = strlen(str) - 1;
    while (i >= 0 && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')) {
        str[i--] = '\0';
    }
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[2J\033[H");
    #endif
}

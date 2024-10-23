#include "manip.h"

//-1 si croix blanche non faite et 0, 1, 2 ou 3 selon le nombre de mouvements D à faire pour aligner avec les centres
int WC_done(rubik* cube) {
    
    int list_areteW[4] = {1, 9, 11, 6};

    for(int i = 0; i < 4; i++) {
        if (cube->flipped[list_areteW[i]] == 1) {
            return -1;
        }
    }

    if (cube->edges[1] == 1 && cube->edges[9] == 9 && cube->edges[11] == 11 && cube->edges[6] == 6) return 0;
    if (cube->edges[1] == 6 && cube->edges[9] == 1 && cube->edges[11] == 9 && cube->edges[6] == 11) return 1;
    if (cube->edges[1] == 11 && cube->edges[9] == 6 && cube->edges[11] == 1 && cube->edges[6] == 9) return 2;
    if (cube->edges[1] == 9 && cube->edges[9] == 11 && cube->edges[11] == 6 && cube->edges[6] == 1) return 3;
    
    return -1;
}

//renvoie une liste des arêtes blanches insérables en un mouvement
int* WC_inserable1(rubik* cube) {
    char* cube_str = cube_to_chaine(cube);
    int* list_inserable = malloc(sizeof(int)*12);
    assert(list_inserable != NULL);
    for(int i = 0; i < 12; i++) {
        list_inserable[i] = -1;
    }
    int j = 0;
    if (cube_str[1] == 'D') {list_inserable[j] = 1; j++; }
    if (cube_str[3] == 'D') {list_inserable[j] = 3; j++; }
    if (cube_str[5] == 'D') {list_inserable[j] = 5; j++; }
    if (cube_str[7] == 'D') {list_inserable[j] = 7; j++; }
    if (cube_str[12] == 'D') {list_inserable[j] = 12; j++;}
    if (cube_str[14] == 'D') {list_inserable[j] = 14; j++; }
    if (cube_str[21] == 'D') {list_inserable[j] = 21; j++; }
    if (cube_str[23] == 'D') {list_inserable[j] = 23; j++; }
    if (cube_str[30] == 'D') {list_inserable[j] = 30; j++; }
    if (cube_str[32] == 'D') {list_inserable[j] = 32; j++; }
    if (cube_str[39] == 'D') {list_inserable[j] = 39; j++; }
    if (cube_str[41] == 'D') {list_inserable[j] = 41; j++;}
    free(cube_str);
    return list_inserable;
}
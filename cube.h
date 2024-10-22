#include "algorithme.h"

typedef struct {
    int coins[8];
    int edges[12];
    int twisted[8];
    int flipped[12];
} rubik;


/*

couche 1 :
            coins 7     | arete 10  | coins 4
          arete 7     | centre U  | arete 4
        coins 3     | arete 3   | coins 0

couche 2 :
            arete 8     | centre B  | arete 5
          centre L    | x         | centre R
        arete 2     | centre F  | arete 0

couche 3 :
            coins 6     | arete 11  | coins 5
          arete 9     | centre D  | arete 6
        coins 2     | arete 1   | coins 1

*/

rubik* init_rubik() {
    rubik* cube = malloc(sizeof(rubik));
    assert(cube != NULL);
    for (int i = 0; i < 8; i++) {
        cube->coins[i] = i;
        cube->twisted[i] = 0;
    }
    for (int i = 0; i < 12; i++) {
        cube->edges[i] = i;
        cube->flipped[i] = 0;
    }
    return cube;
}

int sign(int* permutation, int n) {
    int signature = 1;
    int i, j;
    
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (permutation[i] > permutation[j]) {
                signature = -signature;
            }
        }
    }
    return signature;
}

bool is_solvable(rubik* cube) {
    int nb_twisted = 0;
    int nb_flipped = 0;
    for (int i = 0; i < 12; i++) {
        nb_flipped += cube->flipped[i];
    }
    for(int i = 0; i < 8; i++) {
        nb_twisted += cube->twisted[i];
    }
    for(int i = 0; i < 8; i++) {
        if (cube->coins[i] < 0 || cube->coins[i] > 7) return false;
    }
    for(int i = 0; i < 12; i++) {
        if (cube->edges[i] < 0 || cube->edges[i] > 11) return false;
    }
    for(int i = 0; i < 8; i++) {
        if (cube->twisted[i] != 0 && cube->twisted[i] != 1 && cube->twisted[i] != 2) return false;
    }
    for(int i = 0; i < 12; i++) {
        if (cube->flipped[i] != 0 && cube->flipped[i] != 1) return false;
    }
    if ((nb_twisted % 3 != 0 || nb_flipped % 2 != 0 )|| (sign(cube->coins,8) != sign(cube->edges,12))) return false;
    return true;
}
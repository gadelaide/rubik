#include "algorithme.h"
#include "cube.h"

int increm_mod(int i, int incr, int n) {
    return (i + incr) % n;
}

void permutation(int* tab, int i1, int i2, int i3, int i4) {
    int tmp = tab[i4];
    tab[i4] = tab[i3];
    tab[i3] = tab[i2];
    tab[i2] = tab[i1];
    tab[i1] = tmp;
}

void rot_U(rubik* cube) {
    permutation(cube->corners, 0, 3, 7, 4);
    permutation(cube->edges, 3, 7, 10, 4);
    permutation(cube->twisted, 0, 3, 7, 4);
    permutation(cube->flipped, 3, 7, 10, 4);
}

void rot_D(rubik* cube) {
    permutation(cube->corners, 1, 5, 6, 2);
    permutation(cube->edges, 1, 6, 11, 9);
    permutation(cube->twisted, 1, 5, 6, 2);
    permutation(cube->flipped, 1, 6, 11, 9);
}

void rot_R(rubik* cube) {
    permutation(cube->corners, 0, 4, 5, 1);
    permutation(cube->edges, 0, 4, 5, 6);
    permutation(cube->twisted, 0, 4, 5, 1);
    permutation(cube->flipped, 0, 4, 5, 6);
    int twist_R[8] = {1,2,0,0,2,1,0,0};
    int flip_R[12] = {1,0,0,0,1,1,1,0,0,0,0,0};
    for(int i = 0; i < 8; i++) {
        cube->twisted[i] = increm_mod(cube->twisted[i], twist_R[i], 3);
    }
    for(int i = 0; i < 12; i++) {
        cube->flipped[i] = increm_mod(cube->flipped[i], flip_R[i], 2);
    }
    
}

void rot_L(rubik* cube) {
    permutation(cube->corners, 2, 6, 7, 3);
    permutation(cube->edges, 2, 9, 8, 7);
    permutation(cube->twisted, 2, 6, 7, 3);
    permutation(cube->flipped, 2, 9, 8, 7);
    int twist_L[8] = {0,0,1,2,0,0,2,1};
    int flip_L[12] = {0,0,1,0,0,0,0,1,1,1,0,0};
    for(int i = 0; i < 8; i++) {
        cube->twisted[i] = increm_mod(cube->twisted[i], twist_L[i], 3);
    }
    for(int i = 0; i < 12; i++) {
        cube->flipped[i] = increm_mod(cube->flipped[i], flip_L[i], 2);
    }
}

void rot_F(rubik* cube) {
    permutation(cube->corners, 0, 1, 2, 3);
    permutation(cube->edges, 0, 1, 2, 3);
    permutation(cube->twisted, 0, 1, 2, 3);
    permutation(cube->flipped, 0, 1, 2, 3);
    int twist_F[8] = {2,1,2,1,0,0,0,0};
    for (int i = 0; i < 8; i++) {
        cube->twisted[i] = increm_mod(cube->twisted[i], twist_F[i], 3);
    } 
}

void rot_B(rubik* cube) {
    permutation(cube->corners, 4, 7, 6, 5);
    permutation(cube->edges, 10, 8, 11, 5);
    permutation(cube->twisted, 4, 7, 6, 5);
    permutation(cube->flipped, 10, 8, 11, 5);
    int twist_B[8] = {0,0,0,0,1,2,1,2};
    for (int i = 0; i < 8; i++) {
        cube->twisted[i] = increm_mod(cube->twisted[i], twist_B[i], 3);
    }
}

void rotation(rubik* cube, mvnt mouvement, int rotation) {
    int nb_rot = rotation % 4;
    if (nb_rot == 0) return;
    if (nb_rot == 1) {
        switch (mouvement) {
            case U: rot_U(cube); break;
            case D: rot_D(cube); break;
            case R: rot_R(cube); break;
            case L: rot_L(cube); break;
            case F: rot_F(cube); break;
            case B: rot_B(cube); break;
            case I: break;
        }
    }
    if (nb_rot == 2) {
        switch (mouvement) {
            case U: rot_U(cube); rot_U(cube); break;
            case D: rot_D(cube); rot_D(cube); break;
            case R: rot_R(cube); rot_R(cube); break;
            case L: rot_L(cube); rot_L(cube); break;
            case F: rot_F(cube); rot_F(cube); break;
            case B: rot_B(cube); rot_B(cube); break;
            case I: break;
        }
    }
    if (nb_rot == 3) {
        switch (mouvement) {
            case U: rot_U(cube); rot_U(cube); rot_U(cube); break;
            case D: rot_D(cube); rot_D(cube); rot_D(cube); break;
            case R: rot_R(cube); rot_R(cube); rot_R(cube); break;
            case L: rot_L(cube); rot_L(cube); rot_L(cube); break;
            case F: rot_F(cube); rot_F(cube); rot_F(cube); break;
            case B: rot_B(cube); rot_B(cube); rot_B(cube); break;
            case I: break;
        }
    }
}

void ex_algorithme(rubik* cube, algo* seq) {
    algo* tmp = seq;
    while (tmp != NULL) {
        rotation(cube, tmp->mouvement, tmp->nb);
        tmp = tmp->next;
    }
}